/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Yasuo Ohgaki <yohgaki@ohgaki.net, yohgaki@php.net>           |
  +----------------------------------------------------------------------+
*/

/* $Id: header 310447 2011-04-23 21:14:10Z bjori $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "zend_hash.h"
#include "ext/standard/info.h"
#include "php_realip.h"

ZEND_DECLARE_MODULE_GLOBALS(realip)


/* {{{ realip_functions[]
 *
 * Every user visible function must have an entry in realip_functions[].
 */
const zend_function_entry realip_functions[] = {
	PHP_FE(realip,	NULL)
	{NULL, NULL, NULL}	/* Must be the last line in realip_functions[] */
};
/* }}} */

/* {{{ realip_module_entry
 */
zend_module_entry realip_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"realip",
	realip_functions,
	PHP_MINIT(realip),
	PHP_MSHUTDOWN(realip),
	PHP_RINIT(realip),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(realip),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(realip),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_REALIP
ZEND_GET_MODULE(realip)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
/* For security reasons, realip is disabled by default */
    STD_PHP_INI_ENTRY("realip.enable",      "0", PHP_INI_PERDIR, OnUpdateLong, enable, zend_realip_globals, realip_globals)
    STD_PHP_INI_ENTRY("realip.realip_header",      "HTTP_X_REAL_IP", PHP_INI_PERDIR, OnUpdateString, realip, zend_realip_globals, realip_globals)
PHP_INI_END()
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(realip)
{
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(realip)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(realip)
{
    zval **srv, **trueip, **realip;

    REALIP_G(trueip) = NULL;
    zend_is_auto_global("_SERVER", sizeof("_SERVER")-1 TSRMLS_CC);
    if (REALIP_G(enable)
        && zend_hash_find(&EG(symbol_table), "_SERVER", sizeof("_SERVER"), (void **)&srv) == SUCCESS
        && zend_hash_find(Z_ARRVAL_PP(srv), "REMOTE_ADDR", sizeof("REMOTE_ADDR"), (void **)&trueip) == SUCCESS
        && zend_hash_find(Z_ARRVAL_PP(srv), REALIP_G(realip), strlen(REALIP_G(realip))+1, (void **)&realip) == SUCCESS) {
        REALIP_G(trueip)= estrndup(Z_STRVAL_PP(trueip), Z_STRLEN_PP(trueip));
        *trueip = *realip;
    } 
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(realip)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(realip)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "RealIP support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */


/* {{{ proto mixed realip(void)
   Return true IP address of the client this request. FALSE for diabled. */
PHP_FUNCTION(realip)
{
	char *arg = NULL;
	int arg_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "", &arg, &arg_len) == FAILURE) {
		return;
	}
    if (!REALIP_G(enable)) {
        RETURN_FALSE;
    }
    if (REALIP_G(trueip)) {
        RETURN_STRING(REALIP_G(trueip), 1);
    }
    /* CLI don't have remote address, so no error. */
	RETURN_NULL();
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
