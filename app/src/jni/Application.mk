APP_PLATFORM := android-19
APP_ABI := armeabi-v7a
APP_PIE := true

APP_MODULES := libXorg libandroid_drv libdlinput_drv xkbcomp

X4DROID_DATADIR := /data/data/org.x.android/files

APP_CFLAGS := -DX4DROID_DATADIR=\"$(X4DROID_DATADIR)\" -DDEBUG
