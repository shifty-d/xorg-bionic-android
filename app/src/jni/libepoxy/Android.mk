LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libepoxy_droid

LOCAL_CFLAGS    := 

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/include $(LOCAL_PATH)/src
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/include
LOCAL_SRC_FILES :=		\
    src/dispatch_common.c \
    src/dispatch_egl.c \
    src/gl_generated_dispatch.c \
    src/egl_generated_dispatch.c

LOCAL_LDLIBS := -ldl
LOCAL_SHARED_LIBRARIES := 

include $(BUILD_SHARED_LIBRARY)
