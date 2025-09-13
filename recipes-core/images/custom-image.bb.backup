SUMMARY = "My custom linux image for the BBBI"
DESCRIPTION = "Custom BeagleBone Black image"

require recipes-core/images/core-image-minimal.bb

IMAGE_INSTALL += " \
    vim \
"

IMAGE_LINGUAS = " "
LICENSE = "MIT"
inherit extrausers

# Set rootfs to 200MiB
IMAGE_OVERHEAD_FACTOR ?= "1.0"
IMAGE_ROOTFS_SIZE ?= "204800"

IMAGE_FEATURES += "ssh-server-openssh"

# Correct way to set root password - use lowercase -p with encrypted password
# This creates password 'root' (you can change it)
EXTRA_USERS_PARAMS = "usermod -p '\$6\$rounds=656000\$YQKJEhCc7aNvOzNW\$K5QG8FeL5H.XIwOzN/XM0wOZLPr6D8bFh6C6nCJFXmXj5/5Dh8B6h8VnGsEWqm/kM2ooRKKKkMJ1qGkFqM' root;"
