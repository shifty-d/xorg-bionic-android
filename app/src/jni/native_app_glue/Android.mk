LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= xorg_android_native_app_glue
LOCAL_SRC_FILES:= android_native_app_glue.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
ifeq ($(XORG_NDK_BUILD),true)
LOCAL_EXPORT_LDLIBS := -llog -landroid
endif

ifeq ($(XORG_NDK_BUILD),false)
LOCAL_SHARED_LIBRARIES := liblog libandroid
endif

include $(BUILD_STATIC_LIBRARY)
