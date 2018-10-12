#ifndef __MY_HTTPD_H__
#define __MY_HTTPD_H__

httpd_handle_t  start_webserver(void);
void            stop_webserver(httpd_handle_t webserver);
void            register_handlers(httpd_handle_t webserver);


#endif // __MY_TESTS_H__
