LOCAL_PATH := $(call my-dir)
XSERVER_PATH := $(LOCAL_PATH)

XSERVER_INCLUDES :=							\
    $(LOCAL_PATH)/include 					\
    $(LOCAL_PATH)/dri3 						\
    $(LOCAL_PATH)/fb 						\
    $(LOCAL_PATH)/mi 						\
    $(LOCAL_PATH)/Xext 						\
    $(LOCAL_PATH)/Xi 						\
    $(LOCAL_PATH)/composite 				\
    $(LOCAL_PATH)/randr	 					\
    $(LOCAL_PATH)/render	 				\
    $(LOCAL_PATH)/damageext 				\
    $(LOCAL_PATH)/os		 				\
    $(LOCAL_PATH)/present	 				\
    $(LOCAL_PATH)/hw/xfree86/dixmods/extmod	\
    $(LOCAL_PATH)/miext/cw					\
    $(LOCAL_PATH)/miext/damage				\
    $(LOCAL_PATH)/miext/shadow				\
    $(LOCAL_PATH)/miext/sync				\
    $(LOCAL_PATH)/hw/xfree86/common			\
    $(LOCAL_PATH)/hw/xfree86/ddc			\
    $(LOCAL_PATH)/hw/xfree86/i2c			\
    $(LOCAL_PATH)/hw/xfree86/loader			\
    $(LOCAL_PATH)/hw/xfree86/modes			\
    $(LOCAL_PATH)/hw/xfree86/os-support		\
    $(LOCAL_PATH)/hw/xfree86/os-support/bus	\
    $(LOCAL_PATH)/hw/xfree86/parser			\
    $(LOCAL_PATH)/hw/xfree86/ramdac			\
    $(LOCAL_PATH)/xfixes

XSERVER_INCLUDES += $(GLOBAL_X11_INCLUDES)

LOCAL_PATH := $(XSERVER_PATH)

include $(XSERVER_PATH)/android/Android.mk
include $(XSERVER_PATH)/fb/Android.mk
include $(XSERVER_PATH)/mi/Android.mk
include $(XSERVER_PATH)/render/Android.mk
include $(XSERVER_PATH)/os/Android.mk
include $(XSERVER_PATH)/Xext/Android.mk
include $(XSERVER_PATH)/Xi/Android.mk
include $(XSERVER_PATH)/xkb/Android.mk
include $(XSERVER_PATH)/record/Android.mk
include $(XSERVER_PATH)/dbe/Android.mk
include $(XSERVER_PATH)/xfixes/Android.mk
include $(XSERVER_PATH)/randr/Android.mk
include $(XSERVER_PATH)/composite/Android.mk
include $(XSERVER_PATH)/damageext/Android.mk
include $(XSERVER_PATH)/miext/Android.mk
include $(XSERVER_PATH)/hw/Android.mk
include $(XSERVER_PATH)/config/Android.mk
include $(XSERVER_PATH)/present/Android.mk
include $(XSERVER_PATH)/dix/Android.mk
