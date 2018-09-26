LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libXServer-composite 
LOCAL_C_INCLUDES := $(XSERVER_INCLUDES)
LOCAL_CFLAGS    := -DHAVE_DIX_CONFIG_H -DHAVE_XORG_CONFIG_H $(X4DROID_CFLAGS)
LOCAL_SRC_FILES := 	\
    compalloc.c   \
    compext.c   \
    compinit.c   \
    compoverlay.c   \
    compwindow.c

LOCAL_SHARED_LIBRARIES := libpixman_full
include $(BUILD_STATIC_LIBRARY)
