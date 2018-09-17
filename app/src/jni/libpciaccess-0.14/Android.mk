LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libpciaccess

LOCAL_SRC_FILES := \
    src/common_bridge.c \
    src/common_capability.c \
    src/common_device_name.c \
    src/common_init.c \
    src/common_interface.c \
    src/common_io.c \
    src/common_iterator.c \
    src/common_map.c \
    src/common_vgaarb.c \
    src/linux_devmem.c \
    src/linux_sysfs.c \

LOCAL_CFLAGS := \
    -DHAVE_ZLIB \
    -DHAVE_STDINT_H \
    -DHAVE_STRING_H \
    -DPCIIDS_PATH=\"/etc/hwdata\" \
    -Wno-unused-parameter \

LOCAL_EXPORT_C_INCLUDES := \
    $(LOCAL_PATH)/include \

LOCAL_C_INCLUDES := \
    $(LOCAL_EXPORT_C_INCLUDES) \

include $(BUILD_STATIC_LIBRARY)
