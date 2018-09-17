LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libxshmfence

LOCAL_CFLAGS    := -DHAVE_CONFIG_H

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/src $(LOCAL_PATH)/X11 $(GLOBAL_X11_INCLUDES)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SRC_FILES := \
    src/xshmfence_alloc.c  \
    src/xshmfence_pthread.c


LOCAL_STATIC_LIBRARIES :=
include $(BUILD_STATIC_LIBRARY)
