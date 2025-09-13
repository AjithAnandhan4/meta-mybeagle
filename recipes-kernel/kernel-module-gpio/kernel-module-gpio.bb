SUMMARY = "BeagleBone Black GPIO kernel module"
DESCRIPTION = "Custom GPIO driver for BeagleBone Black with LED and button control"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

inherit module

PV = "1.0"
PR = "r0"

SRC_URI = " \
    file://gpio_driver.c \
    file://gpio_driver.h \
    file://Makefile \
"

S = "${WORKDIR}"

# Ensure proper kernel build environment
DEPENDS = "virtual/kernel"

# Kernel module autoload
KERNEL_MODULE_AUTOLOAD += "gpio_driver"

# Extra build flags for kernel module
EXTRA_OEMAKE += "KERNEL_DIR=${STAGING_KERNEL_DIR}"

# Packaging
FILES:${PN} += " \
    ${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra/gpio_driver.ko \
    ${sysconfdir}/modules-load.d/gpio_driver.conf \
"

# Create module autoload configuration
do_install:append() {
    install -d ${D}${sysconfdir}/modules-load.d
    echo "gpio_driver" > ${D}${sysconfdir}/modules-load.d/gpio_driver.conf
}
