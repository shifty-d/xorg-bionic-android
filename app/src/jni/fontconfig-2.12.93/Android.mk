LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
# ./configure --build=x86_64-pc-linux-gnu --host=arm-linux-eabi
LOCAL_MODULE    := libfontconfig

 fontconfig_flags := \
 -DFONTCONFIG_PATH=\"/sdcard/.fcconfig\" \
 -DFC_CACHEDIR=\"/sdcard/.fccache\" \
 -DFC_DEFAULT_FONTS=\"/system/fonts\" \
 -DHAVE_RANDOM=0 \
 -DSIZEOF_VOID_P=4 \
 -DFLEXIBLE_ARRAY_MEMBER \
 -DHAVE_MKSTEMP \
 -DHAVE_FCNTL_H=1 \
 -DHAVE_RAND=1 \
 -DFC_TEMPLATEDIR=\"$(X4DROID_DATADIR)/use/share/fontconfig/conf.avail\" \
 -DFC_GPERF_SIZE_T=size_t \
 -DALIGNOF_VOID_P=8


ifeq ($(TARGET_ARCH),arm)
 fontconfig_flags += -DALIGNOF_DOUBLE=8
else
 fontconfig_flags += -DALIGNOF_DOUBLE=4
endif

# fontconfig-2.11.1/config.h: comment out
# fontconfig-2.12.93/config.h: comment out
# HAVE_RANDOM_R
# HAVE_SYS_STATVFS_H
# uses uuid which is not available


LOCAL_CFLAGS    := \
 $(fontconfig_flags)

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/include
LOCAL_SRC_FILES := \
    src/fcatomic.c \
    src/fccache.c \
    src/fccfg.c \
    src/fccharset.c \
    src/fccompat.c \
    src/fcdbg.c \
    src/fcdefault.c \
    src/fcdir.c \
    src/fcformat.c \
    src/fcfreetype.c \
    src/fcfs.c \
    src/fcptrlist.c \
    src/fchash.c \
    src/fcinit.c \
    src/fclang.c \
    src/fclist.c \
    src/fcmatch.c \
    src/fcmatrix.c \
    src/fcname.c \
    src/fcobjs.c \
    src/fcpat.c \
    src/fcrange.c \
    src/fcserialize.c \
    src/fcstat.c \
    src/fcstr.c \
    src/fcweight.c \
    src/fcxml.c \
    src/ftglue.c
LOCAL_STATIC_LIBRARIES := libfreetype2 libexpat libuuid
include $(BUILD_STATIC_LIBRARY)
