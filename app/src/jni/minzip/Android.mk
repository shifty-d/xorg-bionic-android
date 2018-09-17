LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	Hash.c \
	SysUtil.c \
	DirUtil.c \
	Inlines.c \
	Zip.c

LOCAL_MODULE := libminzip

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CLANG := true

LOCAL_CFLAGS += -Werror -Wall

include $(BUILD_STATIC_LIBRARY)
