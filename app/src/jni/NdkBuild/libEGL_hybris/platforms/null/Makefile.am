pkglib_LTLIBRARIES = eglplatform_null.la


eglplatform_null_la_SOURCES = eglplatform_null.c
eglplatform_null_la_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/ \
	-I$(top_srcdir)/platforms/common

eglplatform_null_la_LDFLAGS = \
	-avoid-version -module -shared -export-dynamic \
	-lhardware -lsync \
	$(top_builddir)/platforms/common/libhybris-eglplatformcommon.la

if WANT_DEBUG
eglplatform_null_la_CFLAGS += -I$(top_srcdir)/common -ggdb -O0
endif

if WANT_TRACE
eglplatform_null_la_CFLAGS += -DDEBUG
endif
