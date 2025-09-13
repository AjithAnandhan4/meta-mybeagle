#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "bbb_gpio"
#define CLASS_NAME "gpio_class"

// BeagleBone Black GPIO pins (you can modify these)
#define GPIO_LED_PIN 60     // P9_12 (GPIO1_28 = 32 + 28 = 60)
#define GPIO_BUTTON_PIN 48  // P9_15 (GPIO1_16 = 32 + 16 = 48)

// Function prototypes
static int gpio_driver_open(struct inode *inode, struct file *file);
static int gpio_driver_release(struct inode *inode, struct file *file);
static ssize_t gpio_driver_write(struct file *file, const char __user *buffer, 
                                size_t length, loff_t *offset);
static ssize_t gpio_driver_read(struct file *file, char __user *buffer, 
                               size_t length, loff_t *offset);

#endif
