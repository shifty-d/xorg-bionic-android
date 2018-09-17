LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := libXServer-fb
LOCAL_C_INCLUDES := $(XSERVER_INCLUDES)
LOCAL_STATIC_LIBRARIES := libXfont
LOCAL_CFLAGS    := -DHAVE_DIX_CONFIG_H -DHAVE_XORG_CONFIG_H
LOCAL_SRC_FILES := 	\
	fballpriv.c		\
	fbarc.c			\
	fbbits.c		\
	fbblt.c			\
	fbbltone.c		\
	fbcmap_mi.c		\
	fbcopy.c		\
	fbfill.c		\
	fbfillrect.c	\
	fbfillsp.c		\
    fbgc.c			\
    fbgetsp.c		\
    fbglyph.c		\
    fbimage.c		\
    fbline.c		\
    fbpict.c		\
    fbpixmap.c		\
    fbpoint.c		\
    fbpush.c		\
    fbsolid.c		\
    fbscreen.c		\
    fbseg.c			\
    fbsetsp.c		\
    fbtrap.c		\
    fbutil.c		\
    fbwindow.c

include $(BUILD_STATIC_LIBRARY)
