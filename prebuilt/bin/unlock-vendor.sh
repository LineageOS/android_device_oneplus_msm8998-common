#!/sbin/sh
#
# Unlock vendor partition
#

VENDOR=/dev/block/bootdevice/by-name/vendor
BLOCKDEV=/dev/block/sdf

safeRunCommand() {
   cmnd="$@"
   $cmnd
   ERROR_CODE=$?
   if [ ${ERROR_CODE} != 0 ]; then
      printf "Error when executing command: '${command}'\n"
      exit ${ERROR_CODE}
   fi
}

if [ ! -e "$VENDOR" ]; then
    echo "Vendor partition does not exist"

    # Change typecode to '8300 Linux filesystem'
    command="/tmp/sgdisk-op5 --typecode=6:8300 $BLOCKDEV"
    safeRunCommand $command

    # Change name to 'vendor'
    command="/tmp/sgdisk-op5 --change-name=6:vendor $BLOCKDEV"
    safeRunCommand $command
else
    echo "Found vendor partiton. Good!"
fi

exit 0
