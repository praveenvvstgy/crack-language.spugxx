# configuration for the spug++ library
# Michael Muller 2007-01-24
# created from review of many other autoconf macro files, all of which seem to
# be derivatives of scripts by Manish Singh & Owen Taylor

dnl AM_PATH_SPUGPP([MINIMUM-VERSION [, ACTION-IF-FOUND
dnl                [,ACTION-IF-NOT-FOUND]]])
dnl
AC_DEFUN([AM_PATH_SPUGPP],
[AC_ARG_WITH(spugpp-prefix,
   [  --with-spugpp-prefix=PFX  Prefix where the spug++ package is installed],
   spugpp_prefix="$withval", spugpp_prefix="")
AC_ARG_WITH(spugpp-exec-prefix,
   [  --with-spugpp-exec-prefix=PFX  Prefix where the spug++ libraries are installed],
   spugpp_exec_prefix="$withval", spugpp_exec_prefix="")
AC_ARG_WITH(spugpp-libraries,
   [  --with-spugpp-libraries=DIR  Directory where spug++ libraries are installed],
   spugpp_libraries="$withval", spugpp_libraries="")
AC_ARG_WITH(spugpp-includes,
   [  --with-spugpp-includes=DIR  Directory where spug++ header files are installed],
   spugpp_includes="$withval", spugpp_includes="")

   if test "x$spugpp_libraries" != "x" ; then
     SPUGPP_LIBS="-L$spugpp_libraries"
   elif test "x$spugpp_exec_prefix" != "x" ; then
     SPUGPP_LIBS="-L$spugpp_exec_prefix/lib"
   elif test "x$spugpp_prefix" != "x" ; then
     SPUGPP_LIBS="-L$spugpp_prefix/lib"
   elif test "x$prefix" != "xNONE" ; then
     SPUGPP_LIBS="-L$prefix/lib"
   fi

   SPUGPP_LIBS="$SPUGPP_LIBS -lspug++"

   if test "x$spugpp_includes" != "x" ; then
     SPUGPP_CXXFLAGS="-I$spugpp_includes"
   elif test "x$spugpp_prefix" != "x" ; then
     SPUGPP_CXXFLAGS="-I$spugpp_prefix/include"
   elif test "x$prefix" != "xNONE" ; then
     SPUGPP_CXXFLAGS="-I$prefix/include"
   fi

   AC_MSG_CHECKING(for spug++)
   

   ac_save_CXXFLAGS="$CXXFLAGS"
   ac_save_LIBS="$LIBS"
   CXXFLAGS="$CXXFLAGS $SPUGPP_CXXFLAGS"
   LIBS="$LIBS $SPUGPP_LIBS"
   ac_save_link="$ac_link"
   ac_link='$CXX -o conftest$ac_exeext $CXXFLAGS $CPPFLAGS $LDFLAGS conftest.$ac_ext $LIBS >&5'
   AC_TRY_RUN([
#include <spug/Tracer.h>

main() {
   spug::Tracer::get().getLevel("test");
}
],, no_spugpp=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
   CXXFLAGS="$ac_save_CXXFLAGS"
   LIBS="$ac_save_LIBS"


   if test "x$no_spugpp" = "x" ; then
      AC_MSG_RESULT(yes)
      ifelse([$2], , :, [$2])
   else
      AC_MSG_RESULT(no)
      SPUGPP_CXXFLAGS=""
      SPUGPP_LIBS=""
      ifelse([$3], , :, [$3])
   fi
   ac_link="$ac_save_link"
   AC_SUBST(SPUGPP_CXXFLAGS)
   AC_SUBST(SPUGPP_LIBS)
])
