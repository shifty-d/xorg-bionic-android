pkglib_LTLIBRARIES = eglplatform_x11.la

eglplatform_x11_la_SOURCES = eglplatform_x11.cpp x11_window.cpp xcb_drihybris.c
eglplatform_x11_la_CXXFLAGS = \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/ \
	-I$(top_srcdir)/platforms/common

if WANT_DEBUG
eglplatform_x11_la_CXXFLAGS += -I$(top_srcdir)/common
endif

if WANT_TRACE
eglplatform_x11_la_CXXFLAGS += -DDEBUG
endif

if WANT_DEBUG
eglplatform_x11_la_CXXFLAGS += -ggdb -O0
endif

eglplatform_x11_la_LDFLAGS = \
	-avoid-version -module -shared -export-dynamic \
	$(top_builddir)/platforms/common/libhybris-eglplatformcommon.la \
	-lhardware -lsync \
	-lXext -lxcb -lX11-xcb -lxcb-present
