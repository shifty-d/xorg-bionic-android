LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libXServer-miext 
LOCAL_C_INCLUDES := $(XSERVER_INCLUDES)
LOCAL_CFLAGS    := -DHAVE_DIX_CONFIG_H -DHAVE_XORG_CONFIG_H $(X4DROID_CFLAGS)
LOCAL_STATIC_LIBRARIES := libXfont libxshmfence
LOCAL_SHARED_LIBRARIES := libpixman_full
LOCAL_SRC_FILES := 	\
    damage/damage.c   \
    shadow/shadow.c   \
    shadow/shpacked.c   \
    shadow/shplanar.c   \
    shadow/shplanar8.c   \
    shadow/shrot16pack.c   \
    shadow/shrot16pack_180.c   \
    shadow/shrot16pack_270.c   \
    shadow/shrot16pack_270YX.c   \
    shadow/shrot16pack_90.c   \
    shadow/shrot16pack_90YX.c   \
    shadow/shrot32pack.c   \
    shadow/shrot32pack_180.c   \
    shadow/shrot32pack_270.c   \
    shadow/shrot32pack_90.c   \
    shadow/shrot8pack.c   \
    shadow/shrot8pack_180.c   \
    shadow/shrot8pack_270.c   \
    shadow/shrot8pack_90.c   \
    shadow/shrotate.c   \
	sync/misync.c   \
	sync/misyncfd.c   \
	sync/misyncshm.c
    #rootless/rootlessCommon.c   \
    #rootless/rootlessGC.c   \
    #rootless/rootlessScreen.c   \
    #rootless/rootlessValTree.c   \
    #rootless/rootlessWindow.c
include $(BUILD_STATIC_LIBRARY)
