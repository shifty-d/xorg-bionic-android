AC_INIT([libhybris], [0.1.0])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_GNU_SOURCE
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

PKG_PROG_PKG_CONFIG

AC_ARG_ENABLE(debug,
  [  --enable-debug            Enable debug build (default=disabled)],
  [debug=$enableval],
  [debug="no"])
AM_CONDITIONAL( [WANT_DEBUG], [test x"$debug" = x"yes"])

AC_ARG_ENABLE(experimental,
  [  --enable-experimental     Enable experimental features (default=disabled)],
  [experimental=$enableval],
  [experimental="no"])
AM_CONDITIONAL( [WANT_EXPERIMENTAL], [test x"$experimental" = x"yes"])

AC_ARG_ENABLE(trace,
  [  --enable-trace            Enable TRACE statements (default=disabled)],
  [trace=$enableval],
  [trace="no"])
AM_CONDITIONAL( [WANT_TRACE], [test x"$trace" = x"yes"])

AC_ARG_ENABLE(mesa,
  [  --enable-mesa            Enable mesa headers (default=disabled)],
  [mesa=$enableval],
  [mesa="no"])
AM_CONDITIONAL( [WANT_MESA], [test x"$mesa" = x"yes"])

DEFAULT_EGL_PLATFORM="null"
AC_ARG_WITH(default-egl-platform,
  [  --with-default-egl-platform=NAME     Use NAME as default EGL platform if not specified by environment ],
  [ DEFAULT_EGL_PLATFORM="$withval" ],
  [ ])
AC_SUBST(DEFAULT_EGL_PLATFORM)

AC_CONFIG_FILES([
	egl.pc
	Makefile
	platforms/Makefile
	platforms/common/Makefile
	platforms/common/hybris-egl-platform.pc
	platforms/x11/Makefile
	platforms/null/Makefile
	include/Makefile
])
AC_OUTPUT

echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE_NAME $PACKAGE_VERSION"
echo "------------------------------------------------------------------------"
echo
echo "Configuration Options:"
echo
echo "  debug build.............: $debug"
echo
echo "  trace...................: $trace"
echo
echo "  experimental features...: $experimental"
echo
echo "  prefix..................: $prefix"
echo
echo "  arch  ..................: $arch"
echo
echo "  default egl platform ...: $DEFAULT_EGL_PLATFORM"
echo
echo "------------------------------------------------------------------------"
echo
echo "Now type 'make' to compile and 'make install' to install this package."
