LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libdlinput_drv

LOCAL_CFLAGS    := -DHAVE_CONFIG_H
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/src
LOCAL_SRC_FILES := src/dummy-input.c
LOCAL_SHARED_LIBRARIES := libXorg
include $(BUILD_SHARED_LIBRARY)
