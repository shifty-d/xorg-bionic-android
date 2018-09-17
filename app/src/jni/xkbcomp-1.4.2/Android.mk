LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := xkbcomp
LOCAL_CFLAGS    := -DHAVE_CONFIG_H -DDFLT_XKB_CONFIG_ROOT=\"$(X4DROID_DATADIR)/usr/share/X11/xkb\"
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(GLOBAL_X11_INCLUDES)
LOCAL_SRC_FILES :=	\
	action.c		\
	alias.c			\
	compat.c		\
	expr.c			\
    geometry.c		\
    indicators.c	\
    keycodes.c		\
    keymap.c		\
    keytypes.c		\
    listing.c		\
    misc.c			\
    parseutils.c	\
    symbols.c		\
    xkbcomp.c		\
    xkbpath.c		\
    xkbparse.c		\
    xkbscan.c		\
    utils.c			\
    vmod.c

LOCAL_SHARED_LIBRARIES := libX11
LOCAL_STATIC_LIBRARIES := libxkbfile
LOCAL_LDLIBS := -llog

include $(BUILD_EXECUTABLE)
