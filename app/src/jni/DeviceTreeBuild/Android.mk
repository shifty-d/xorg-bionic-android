ifeq ($(XORG_NDK_BUILD),false)
    include $(call all-subdir-makefiles)
endif
