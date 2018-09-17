LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := libXServer-Xext 
LOCAL_C_INCLUDES := $(XSERVER_INCLUDES)
LOCAL_CFLAGS    := -DHAVE_DIX_CONFIG_H -DHAVE_XORG_CONFIG_H
LOCAL_SRC_FILES := 	\
    dpms.c   \
    geext.c   \
    hashtable.c   \
    shape.c   \
    security.c   \
    sleepuntil.c   \
    sync.c   \
    xace.c   \
    xcmisc.c   \
    xres.c   \
    xtest.c   \
    xvdisp.c   \
    xvmain.c   \
    xvmc.c   \
    bigreq.c

#saver.c   \
#xselinux_ext.c   \
#xselinux_hooks.c   \
#xselinux_label.c   \
#xf86bigfont.c   \
#dpmsstubs.c   \
#panoramiX.c   \
#panoramiXprocs.c   \
#panoramiXSwap.c   \
#shm.c

include $(BUILD_STATIC_LIBRARY)
