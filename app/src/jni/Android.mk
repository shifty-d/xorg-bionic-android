LOCAL_PATH := $(call my-dir)

GLOBAL_X11_INCLUDES := \
    $(LOCAL_PATH)/xorgproto-2018.4/include \
    $(LOCAL_PATH)/xtrans-1.3.5 \
    $(LOCAL_PATH)/pixman/pixman

include $(call all-subdir-makefiles)
