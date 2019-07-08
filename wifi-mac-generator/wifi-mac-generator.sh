#!/vendor/bin/sh
WLAN_MAC_VENDOR_PREFIX="C0EEFB"
WLAN_MAC_PERSIST_PATH="/mnt/vendor/persist/wlan_mac.bin"

function wait_for_file() {
    retries=0

    while [ ! -f "${1}" ]; do
        retries=$((retries + 1))

        if [ "${retries}" -eq 10 ]; then
            return 1
        fi

        sleep 1
    done

    return 0
}

if [[ ! -f "${WLAN_MAC_PERSIST_PATH}" ]] || [[ ! -s "${WLAN_MAC_PERSIST_PATH}" ]]; then
    MAC_0_PATH="/data/vendor/oemnvitems/4678_0"

    if ! wait_for_file "${MAC_0_PATH}"; then
        MAC_0="${WLAN_MAC_VENDOR_PREFIX}`xxd -l 3 -p /dev/urandom | tr '[:lower:]' '[:upper:]'`"
    else
        MAC_0=`xxd -p "${MAC_0_PATH}" | grep -o '..' | tac | tr -d '\n' | tr '[:lower:]' '[:upper:]'`
    fi

    MAC_1_PATH="/data/vendor/oemnvitems/4678_1"

    if ! wait_for_file "${MAC_1_PATH}"; then
        MAC_1="${WLAN_MAC_VENDOR_PREFIX}`xxd -l 3 -p /dev/urandom | tr '[:lower:]' '[:upper:]'`"
    else
        MAC_1=`xxd -p "${MAC_1_PATH}" | grep -o '..' | tac | tr -d '\n' | tr '[:lower:]' '[:upper:]'`
    fi

    echo "Intf0MacAddress=${MAC_0}" > "${WLAN_MAC_PERSIST_PATH}"
    echo "Intf1MacAddress=${MAC_1}" >> "${WLAN_MAC_PERSIST_PATH}"
    echo "Intf2MacAddress=000AF58989FD" >> "${WLAN_MAC_PERSIST_PATH}"
    echo "Intf3MacAddress=000AF58989FC" >> "${WLAN_MAC_PERSIST_PATH}"
    echo "END" >> "${WLAN_MAC_PERSIST_PATH}"
fi
