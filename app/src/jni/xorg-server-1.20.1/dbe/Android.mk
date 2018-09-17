LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := libXServer-dbe 
LOCAL_C_INCLUDES := $(XSERVER_INCLUDES)
LOCAL_CFLAGS    := -DHAVE_DIX_CONFIG_H -DHAVE_XORG_CONFIG_H
LOCAL_SRC_FILES := 	\
    dbe.c \
    midbe.c

include $(BUILD_STATIC_LIBRARY)
