LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libmtdev
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CFLAGS :=
LOCAL_SRC_FILES :=		\
	src/caps.c			\
	src/core.c			\
	src/iobuf.c			\
	src/match.c			\
	src/match_four.c

include $(BUILD_STATIC_LIBRARY)
