BOOT_SIGNER := $(HOST_OUT_EXECUTABLES)/boot_signer
BOOT_IMAGE_SIGNING_KEY := device/oneplus/msm8998-common/verity

recoveryimage-deps += $(BOOT_SIGNER)

# $1: boot image target
define build_boot_supports_boot_signer
  $(MKBOOTIMG) --kernel $(call bootimage-to-kernel,$(1)) $(INTERNAL_BOOTIMAGE_ARGS) $(INTERNAL_MKBOOTIMG_VERSION_ARGS) $(BOARD_MKBOOTIMG_ARGS) --output $(1)
  $(BOOT_SIGNER) /boot $@ $(BOOT_IMAGE_SIGNING_KEY).pk8 $(BOOT_IMAGE_SIGNING_KEY).x509.pem $(1)
  $(call assert-max-image-size,$(1),$(call get-bootimage-partition-size,$(1),boot))
endef

$(INSTALLED_BOOTIMAGE_TARGET): $(MKBOOTIMG) $(INTERNAL_BOOTIMAGE_FILES) $(BOOT_SIGNER)
	$(call pretty,"Target boot image: $@")
	$(call build_boot_supports_boot_signer,$@)

$(call declare-1p-container,$(INSTALLED_BOOTIMAGE_TARGET),)
$(call declare-container-license-deps,$(INSTALLED_BOOTIMAGE_TARGET),$(INTERNAL_BOOTIMAGE_FILES),$(PRODUCT_OUT)/:/)

UNMOUNTED_NOTICE_DEPS += $(INSTALLED_BOOTIMAGE_TARGET)

.PHONY: bootimage-nodeps
bootimage-nodeps: $(MKBOOTIMG) $(BOOT_SIGNER)
	@echo "make $@: ignoring dependencies"
	$(foreach b,$(INSTALLED_BOOTIMAGE_TARGET),$(call build_boot_supports_boot_signer,$(b)))

# $(1): output file
define build-recoveryimage-target
  $(MKBOOTIMG) $(INTERNAL_RECOVERYIMAGE_ARGS) $(INTERNAL_MKBOOTIMG_VERSION_ARGS) $(BOARD_RECOVERY_MKBOOTIMG_ARGS) --output $(1)
  $(BOOT_SIGNER) /recovery $(1) $(BOOT_IMAGE_SIGNING_KEY).pk8 $(BOOT_IMAGE_SIGNING_KEY).x509.pem $(1)
  $(call assert-max-image-size,$(1),$(call get-hash-image-max-size,$(BOARD_RECOVERYIMAGE_PARTITION_SIZE)))
endef

$(INSTALLED_RECOVERYIMAGE_TARGET): $(recoveryimage-deps)
	$(call build-recoveryimage-target, $@)

