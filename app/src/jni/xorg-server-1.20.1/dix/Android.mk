LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(XSERVER_INCLUDES)
LOCAL_MODULE    := libXServer-dix
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_CFLAGS    := -DHAVE_DIX_CONFIG_H -DHAVE_XORG_CONFIG_H $(X4DROID_CFLAGS)
LOCAL_LDFLAGS   := 
LOCAL_SRC_FILES := 			\
    atom.c   \
    colormap.c   \
    cursor.c   \
    devices.c   \
    dispatch.c   \
    dixfonts.c   \
    dixutils.c   \
    enterleave.c   \
    eventconvert.c   \
    events.c   \
    extension.c   \
    gc.c   \
    getevents.c   \
    globals.c   \
    glyphcurs.c   \
    grabs.c   \
    initatoms.c   \
    inpututils.c   \
    main.c   \
    pixmap.c   \
    privates.c   \
    property.c   \
    ptrveloc.c   \
    region.c   \
    registry.c   \
    resource.c   \
    selection.c   \
    swaprep.c   \
    swapreq.c   \
    tables.c   \
    touch.c   \
    window.c	\

LOCAL_SHARED_LIBRARIES := libpixman_full
LOCAL_STATIC_LIBRARIES := libXfont
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := Xorg
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_SRC_FILES := stubmain.c
LOCAL_SHARED_LIBRARIES := libXFree86
include $(BUILD_EXECUTABLE)
