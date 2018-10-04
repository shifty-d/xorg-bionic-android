empty:=
space:= $(empty) $(empty)

version := $(subst .,$(space),$(PLATFORM_VERSION))
ANDROID_VERSION_MAJOR := $(firstword $(version))
ANDROID_VERSION_MINOR := $(wordlist 2, 2, $(version))
ANDROID_VERSION_PATCH := $(lastword $(version))

ifeq ($(XORG_NDK_BUILD),false)
    #include $(call all-subdir-makefiles)
    include $(call my-dir)/xf86-video-droid/Android.mk
endif
