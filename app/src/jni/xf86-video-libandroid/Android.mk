LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libandroid_drv

LOCAL_CFLAGS    := -DHAVE_CONFIG_H
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/src
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_SRC_FILES :=			\
    src/display.c			\
    src/driver.c			\
    src/glutils.c			\
    src/nativeactivity.c	\
    src/present.c			\
    src/renderer.c			\
    src/shaders.c

LOCAL_SHARED_LIBRARIES := libXorg libepoxy libdlinput_drv
include $(BUILD_SHARED_LIBRARY)
