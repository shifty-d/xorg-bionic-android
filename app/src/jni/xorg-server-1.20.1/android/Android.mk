LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libXServer-android

LOCAL_CFLAGS := -DTARGET_ARCH_ABI=\"$(TARGET_ARCH_ABI)\" $(X4DROID_CFLAGS)
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/src $(XSERVER_INCLUDES)
LOCAL_SRC_FILES := 		\
    android.c	\
    assets.c
LOCAL_EXPORT_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := 	\
    libminzip_xorg			\
    xorg_android_native_app_glue

include $(BUILD_STATIC_LIBRARY)
