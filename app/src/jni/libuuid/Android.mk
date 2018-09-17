LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libuuid

LOCAL_CFLAGS    := -DHAVE_USLEEP

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/uuid
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_SRC_FILES := \
    uuid/predefined.c \
    uuid/isnull.c \
    uuid/gen_uuid.c \
    uuid/parse.c \
    uuid/unpack.c \
    uuid/uuid_time.c \
    uuid/pack.c \
    uuid/unparse.c \
    uuid/compare.c \
    uuid/copy.c \
    uuid/clear.c

LOCAL_STATIC_LIBRARIES :=
include $(BUILD_STATIC_LIBRARY)
