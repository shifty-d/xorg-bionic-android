lib_LTLIBRARIES = \
	libhybris-eglplatformcommon.la

libhybris_eglplatformcommon_la_SOURCES = \
	native_handle.c \
	nativewindowbase.cpp \
	eglplatformcommon.cpp \
	windowbuffer.cpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = hybris-egl-platform.pc

libhybris_eglplatformcommon_la_CFLAGS = -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS) -I$(top_srcdir)/egl -I$(top_srcdir)/common/

if WANT_MESA
libhybris_eglplatformcommon_la_CFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif
if WANT_TRACE
libhybris_eglplatformcommon_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libhybris_eglplatformcommon_la_CFLAGS += -ggdb -O0
endif


libhybris_eglplatformcommon_la_CXXFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/egl $(ANDROID_HEADERS_CFLAGS) -I$(top_srcdir)/common/
if WANT_MESA
libhybris_eglplatformcommon_la_CXXFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif

if WANT_TRACE
libhybris_eglplatformcommon_la_CXXFLAGS += -DDEBUG
endif
if WANT_DEBUG
libhybris_eglplatformcommon_la_CXXFLAGS += -ggdb -O0
endif
libhybris_eglplatformcommon_la_LDFLAGS = \
	-version-info "1":"0":"0"

eglplatformcommondir = $(includedir)/hybris/eglplatformcommon
eglplatformcommon_HEADERS = \
	support.h \
	nativewindowbase.h \
	eglplatformcommon.h \
	hybris_nativebufferext.h
