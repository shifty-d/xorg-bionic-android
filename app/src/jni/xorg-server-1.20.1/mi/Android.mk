LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libXServer-mi
LOCAL_C_INCLUDES := $(XSERVER_INCLUDES)
LOCAL_CFLAGS    := -DHAVE_DIX_CONFIG_H -DHAVE_XORG_CONFIG_H $(X4DROID_CFLAGS)
LOCAL_STATIC_LIBRARIES := libXfont
LOCAL_SRC_FILES := 	\
    miarc.c   \
    mibitblt.c   \
    micmap.c   \
    micopy.c   \
    midash.c   \
    midispcur.c   \
    mieq.c   \
    miexpose.c   \
    mifillarc.c   \
    mifillrct.c   \
    migc.c   \
    miglblt.c   \
    miinitext.c   \
    mioverlay.c   \
    mipointer.c   \
    mipoly.c   \
    mipolypnt.c   \
    mipolyrect.c   \
    mipolyseg.c   \
    mipolytext.c   \
    mipushpxl.c   \
    miscrinit.c   \
    misprite.c   \
    mivaltree.c   \
    miwideline.c   \
    miwindow.c   \
    mizerarc.c   \
    mizerclip.c   \
    mizerline.c	\

include $(BUILD_STATIC_LIBRARY)
