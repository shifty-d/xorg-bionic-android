PROJECT_ROOT_PATH := $(call my-dir)
include $(PROJECT_ROOT_PATH)/definitions.mk

include $(call all-subdir-makefiles)
