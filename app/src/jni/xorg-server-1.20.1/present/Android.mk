LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := libXServer-present
LOCAL_C_INCLUDES := $(XSERVER_INCLUDES)
LOCAL_CFLAGS    := -DHAVE_DIX_CONFIG_H -DHAVE_XORG_CONFIG_H
LOCAL_STATIC_LIBRARIES := 
LOCAL_SRC_FILES := 	\
    present_screen.c \
    present_request.c \
    present_vblank.c \
    present_event.c \
    present.c \
    present_wnmd.c \
    present_fake.c \
    present_scmd.c \
    present_execute.c \
    present_notify.c \
    present_fence.c


include $(BUILD_STATIC_LIBRARY)
