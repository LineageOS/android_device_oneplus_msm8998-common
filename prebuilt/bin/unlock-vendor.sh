#!/sbin/sh
#
# Unlock vendor partition
#

VENDOR=/dev/block/bootdevice/by-name/vendor
BLOCKDEV=/dev/block/sdf

if [ ! -e "$VENDOR" ]; then
    echo "Vendor partition does not exist"
    # Change typecode to '8300 Linux filesystem'
    /tmp/install/bin/sgdisk-op5 --typecode=6:8300 $BLOCKDEV
    if [[ ${PIPESTATUS[0]} != 0 ]]; then
        echo "Failed to change typecode"
        exit 1
    fi

    # Change name to 'vendor'
    /tmp/install/bin/sgdisk-op5 --typecode=6:vendor $BLOCKDEV
    if [[ ${PIPESTATUS[0]} != 0 ]]; then
        echo "Failed to change partition name"
        exit 2
    fi
else 
    echo "Found vendor partiton. Good!"
fi 

exit 0
