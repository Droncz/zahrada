#ifndef __MY_HTTPD_H__
#define __MY_HTTPD_H__

httpd_handle_t  start_webserver(void);
void            stop_webserver(httpd_handle_t webserver);
void            register_handlers(httpd_handle_t webserver);

// Examples of definition of callback functions:
// typedef void (*transaction_cb_t)(spi_transaction_t *trans);
// typedef int (* cgiRecvHandler)(HttpdConnData *connData, char *data, int len);
// typedef void (* TplCallback)(HttpdConnData *connData, char *token, void **arg);

typedef int (*changeToken_cb_t)(httpd_req_t *req, char *token);

void processTemplate(httpd_req_t *req, char *buff, changeToken_cb_t changeToken_cb);

#endif // __MY_TESTS_H__
