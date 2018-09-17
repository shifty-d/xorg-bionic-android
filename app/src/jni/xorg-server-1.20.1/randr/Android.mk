LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libXServer-randr 
LOCAL_C_INCLUDES := $(XSERVER_INCLUDES)
LOCAL_CFLAGS    := -DHAVE_DIX_CONFIG_H -DHAVE_XORG_CONFIG_H
LOCAL_SHARED_LIBRARIES := libpixman
LOCAL_SRC_FILES := 	\
    randr.c   \
    rrcrtc.c   \
    rrdispatch.c   \
    rrinfo.c   \
    rrmode.c   \
    rrmonitor.c   \
    rrlease.c   \
    rroutput.c   \
    rrpointer.c   \
    rrproperty.c   \
    rrprovider.c \
    rrscreen.c   \
    rrsdispatch.c   \
    rrtransform.c   \
    rrproviderproperty.c \
    rrxinerama.c

include $(BUILD_STATIC_LIBRARY)
