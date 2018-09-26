LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := libXServer-xkb 
LOCAL_C_INCLUDES := $(XSERVER_INCLUDES)
LOCAL_CFLAGS    := -DHAVE_DIX_CONFIG_H -DHAVE_XORG_CONFIG_H $(X4DROID_CFLAGS)
LOCAL_STATIC_LIBRARIES := libxkbfile
LOCAL_SRC_FILES := 	\
    ddxBeep.c   \
    ddxCtrls.c   \
    ddxKillSrv.c   \
    ddxLEDs.c   \
    ddxLoad.c   \
    ddxPrivate.c   \
    ddxVT.c   \
    maprules.c   \
    xkb.c   \
    xkbAccessX.c   \
    xkbActions.c   \
    XKBAlloc.c   \
    xkbEvents.c   \
    xkbfmisc.c   \
    XKBGAlloc.c   \
    xkbInit.c   \
    xkbLEDs.c   \
    XKBMAlloc.c   \
    XKBMisc.c   \
    xkbout.c   \
    xkbPrKeyEv.c   \
    xkbSwap.c   \
    xkbtext.c   \
    xkbUtils.c   \
    xkmread.c

include $(BUILD_STATIC_LIBRARY)
