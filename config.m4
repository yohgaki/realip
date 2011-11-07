dnl $Id$
dnl config.m4 for extension realip

PHP_ARG_WITH(realip, for realip support,
dnl Make sure that the comment is aligned:
[  --with-realip             Include realip support])

if test "$PHP_REALIP" != "no"; then
  PHP_NEW_EXTENSION(realip, realip.c, $ext_shared)
fi
