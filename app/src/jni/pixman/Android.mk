LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# pixman-android-neon.S uses gcc-specific directive.
LOCAL_CLANG_ASFLAGS_arm += -no-integrated-as

LOCAL_SDK_VERSION := 14

LOCAL_C_INCLUDES := $(LOCAL_PATH)/pixman
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/pixman

LOCAL_MODULE := libpixman_full

LOCAL_CFLAGS := -DHAVE_CONFIG_H -fvisibility=hidden

# Core files
LOCAL_SRC_FILES :=						\
	pixman/pixman.c						\
	pixman/pixman-access.c				\
	pixman/pixman-access-accessors.c	\
	pixman/pixman-arm.c					\
	pixman/pixman-bits-image.c			\
	pixman/pixman-combine32.c			\
	pixman/pixman-combine-float.c		\
	pixman/pixman-conical-gradient.c	\
	pixman/pixman-edge.c				\
	pixman/pixman-edge-accessors.c		\
	pixman/pixman-fast-path.c			\
	pixman/pixman-general.c				\
	pixman/pixman-glyph.c				\
	pixman/pixman-gradient-walker.c		\
	pixman/pixman-image.c				\
	pixman/pixman-implementation.c		\
	pixman/pixman-linear-gradient.c		\
	pixman/pixman-matrix.c				\
	pixman/pixman-mips.c				\
	pixman/pixman-noop.c				\
	pixman/pixman-ppc.c					\
	pixman/pixman-radial-gradient.c		\
	pixman/pixman-region16.c			\
	pixman/pixman-region32.c			\
	pixman/pixman-solid-fill.c			\
	pixman/pixman-trap.c				\
	pixman/pixman-utils.c				\
	pixman/pixman-x86.c

# Android additions
LOCAL_SRC_FILES += \
	pixman/pixman-android.c

#ifeq ($(strip $(TARGET_ARCH)),arm)
	# Will only be used if runtime detection reports NEON capabilities
#	LOCAL_CFLAGS += -DUSE_ARM_NEON
#	LOCAL_SRC_FILES += \
#		pixman/pixman-android-neon.S
#endif

LOCAL_STATIC_LIBRARIES := cpufeatures

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/cpufeatures)
