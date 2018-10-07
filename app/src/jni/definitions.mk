empty:=
space:= $(empty) $(empty)

version := $(subst .,$(space),$(PLATFORM_VERSION))
ANDROID_VERSION_MAJOR := $(firstword $(version))
ANDROID_VERSION_MINOR := $(wordlist 2, 2, $(version))
ANDROID_VERSION_PATCH := $(lastword $(version))


GLOBAL_X11_INCLUDES :=

ifeq (local,$(NDK_APPS))
    XORG_NDK_BUILD:=true
	X4DROID_DATADIR := /data/data/org.x.android/files
else
    XORG_NDK_BUILD:=false
	X4DROID_DATADIR := /data/org.x.android/
	GLOBAL_X11_INCLUDES +=				\
		external/zlib					\
		system/core/libsync/include
endif

#Device Tree build needs full include paths
GLOBAL_X11_INCLUDES +=												\
	$(PROJECT_ROOT_PATH)/native_app_glue							\
	$(PROJECT_ROOT_PATH)/libepoxy/include							\
	$(PROJECT_ROOT_PATH)/libpciaccess-0.14/include					\
    $(PROJECT_ROOT_PATH)/libX11-1.6.6/include						\
    $(PROJECT_ROOT_PATH)/libxcb-1.13								\
    $(PROJECT_ROOT_PATH)/libXfont2-2.0.3/include					\
    $(PROJECT_ROOT_PATH)/libxkbfile-1.0.9/include					\
    $(PROJECT_ROOT_PATH)/libxshmfence-1.3							\
	$(PROJECT_ROOT_PATH)/minzip										\
	$(PROJECT_ROOT_PATH)/mtdev-1.1.5/include						\
	$(PROJECT_ROOT_PATH)/pixman/pixman								\
	$(PROJECT_ROOT_PATH)/libXau-1.0.8/include						\
	$(PROJECT_ROOT_PATH)/libXdmcp-1.1.2/include						\
    $(PROJECT_ROOT_PATH)/xtrans-1.3.5								\
    $(PROJECT_ROOT_PATH)/xorgproto-2018.4/include					\
																	\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/fb						\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/mi						\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/miext/damage			\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/hw/xfree86/common		\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/hw/xfree86/ddc			\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/hw/xfree86/i2c			\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/hw/xfree86/modes		\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/hw/xfree86/os-support	\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/hw/xfree86/parser		\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/hw/xfree86/ramdac		\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/include					\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/present					\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/randr					\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/render					\
	$(PROJECT_ROOT_PATH)/xorg-server-1.20.1/Xext					\

X4DROID_CFLAGS := -DX4DROID_DATADIR=\"$(X4DROID_DATADIR)\" -std=gnu99
