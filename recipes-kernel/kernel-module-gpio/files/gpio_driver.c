#include "gpio_driver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("BeagleBone Black GPIO Driver");
MODULE_VERSION("1.0");

// Device variables
static int major_number;
static struct class *gpio_class = NULL;
static struct device *gpio_device = NULL;
static struct cdev gpio_cdev;

// File operations structure
static struct file_operations fops = {
    .open = gpio_driver_open,
    .read = gpio_driver_read,
    .write = gpio_driver_write,
    .release = gpio_driver_release,
};

// Module initialization
static int __init gpio_driver_init(void)
{
    int result;
    dev_t dev_num;
    
    printk(KERN_INFO "BBB GPIO Driver: Initializing\n");
    
    // Allocate major number dynamically
    result = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (result < 0) {
        printk(KERN_ALERT "BBB GPIO Driver: Failed to allocate major number\n");
        return result;
    }
    major_number = MAJOR(dev_num);
    printk(KERN_INFO "BBB GPIO Driver: Assigned major number %d\n", major_number);
    
    // Initialize cdev structure
    cdev_init(&gpio_cdev, &fops);
    gpio_cdev.owner = THIS_MODULE;
    
    // Add cdev to system
    result = cdev_add(&gpio_cdev, dev_num, 1);
    if (result < 0) {
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "BBB GPIO Driver: Failed to add cdev\n");
        return result;
    }
    
    // Create device class - FIXED for newer kernel API
    gpio_class = class_create(CLASS_NAME);
    if (IS_ERR(gpio_class)) {
        cdev_del(&gpio_cdev);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "BBB GPIO Driver: Failed to create class\n");
        return PTR_ERR(gpio_class);
    }
    
    // Create device
    gpio_device = device_create(gpio_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(gpio_device)) {
        class_destroy(gpio_class);
        cdev_del(&gpio_cdev);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "BBB GPIO Driver: Failed to create device\n");
        return PTR_ERR(gpio_device);
    }
    
    // Request GPIO pins
    if (gpio_request(GPIO_LED_PIN, "LED_PIN")) {
        printk(KERN_ALERT "BBB GPIO Driver: Cannot request GPIO %d\n", GPIO_LED_PIN);
        goto cleanup;
    }
    
    if (gpio_request(GPIO_BUTTON_PIN, "BUTTON_PIN")) {
        printk(KERN_ALERT "BBB GPIO Driver: Cannot request GPIO %d\n", GPIO_BUTTON_PIN);
        gpio_free(GPIO_LED_PIN);
        goto cleanup;
    }
    
    // Set GPIO directions
    gpio_direction_output(GPIO_LED_PIN, 0);  // LED as output, initially off
    gpio_direction_input(GPIO_BUTTON_PIN);   // Button as input
    
    printk(KERN_INFO "BBB GPIO Driver: Device created successfully\n");
    return 0;
    
cleanup:
    device_destroy(gpio_class, dev_num);
    class_destroy(gpio_class);
    cdev_del(&gpio_cdev);
    unregister_chrdev_region(dev_num, 1);
    return -1;
}

// Module cleanup
static void __exit gpio_driver_exit(void)
{
    dev_t dev_num = MKDEV(major_number, 0);
    
    // Free GPIOs
    gpio_free(GPIO_LED_PIN);
    gpio_free(GPIO_BUTTON_PIN);
    
    // Remove device and class
    device_destroy(gpio_class, dev_num);
    class_destroy(gpio_class);
    cdev_del(&gpio_cdev);
    unregister_chrdev_region(dev_num, 1);
    
    printk(KERN_INFO "BBB GPIO Driver: Cleanup complete\n");
}

// File operations implementations
static int gpio_driver_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "BBB GPIO Driver: Device opened\n");
    return 0;
}

static int gpio_driver_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "BBB GPIO Driver: Device closed\n");
    return 0;
}

static ssize_t gpio_driver_write(struct file *file, const char __user *buffer, 
                                size_t length, loff_t *offset)
{
    char command;
    
    if (length == 0) return 0;
    
    if (copy_from_user(&command, buffer, 1)) {
        return -EFAULT;
    }
    
    switch (command) {
        case '1':
            gpio_set_value(GPIO_LED_PIN, 1);
            printk(KERN_INFO "BBB GPIO Driver: LED ON\n");
            break;
        case '0':
            gpio_set_value(GPIO_LED_PIN, 0);
            printk(KERN_INFO "BBB GPIO Driver: LED OFF\n");
            break;
        default:
            printk(KERN_WARNING "BBB GPIO Driver: Invalid command '%c'\n", command);
            return -EINVAL;
    }
    
    return length;
}

static ssize_t gpio_driver_read(struct file *file, char __user *buffer, 
                               size_t length, loff_t *offset)
{
    int button_state;
    char state_char;
    
    if (*offset > 0) return 0;  // EOF
    if (length < 1) return -EINVAL;
    
    button_state = gpio_get_value(GPIO_BUTTON_PIN);
    state_char = button_state ? '1' : '0';
    
    if (copy_to_user(buffer, &state_char, 1)) {
        return -EFAULT;
    }
    
    *offset = 1;
    return 1;
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);
