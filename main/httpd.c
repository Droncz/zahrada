
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
// #include <sys/param.h>

#include <http_server.h>

#include "config.h"
#include "httpd.h"
#include "urls.h"

void register_handlers(httpd_handle_t webserver)
{
    int i;

    int handlers_no = sizeof(handlers)/sizeof(httpd_uri_t);
    ESP_LOGI(TAG, "Registering handlers");
    ESP_LOGI(TAG, "No of handlers = %d", handlers_no);
    for (i = 0; i < handlers_no; i++) {
        if (httpd_register_uri_handler(webserver, &handlers[i]) != ESP_OK) {
            ESP_LOGW(TAG, "register uri failed for %d", i);
            return;
        }
    }
    ESP_LOGI(TAG, "Success");
}

httpd_handle_t start_webserver(void)
{
    httpd_handle_t webserver = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting webserver on port: '%d'", config.server_port);
    if (httpd_start(&webserver, &config) == ESP_OK) {
        register_handlers(webserver);
        return webserver;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t webserver)
{
    // Stop the httpd server
    httpd_stop(webserver);
}

void tokenize(httpd_req_t *req, char buff[], changeToken_cb_t changeToken_cb)
{
    // TplData *tpd=connData->cgiData;
	int len, tokenlen, x, sp=0, tokenPos=-1;
	char *e=NULL;
    char token[MAXTOKENSIZE];
	// char buff[MAXHTTPSIZE];
    
	len=strlen(buff);
    ESP_LOGD(TAG, "Buff addr: %p\n", buff);
    ESP_LOGI(TAG, "Length of buff: %d.\n", len);
    ESP_LOGD(TAG, "buff: %s.\n", buff);
	if (len>0) {
		sp=0;
		e=buff;
        // Walk the http page (stored in an char array "buff") byte-by-byte
		for (x=0; x<len; x++) {
			if (tokenPos==-1) {
				//Inside ordinary text.
				if (buff[x]=='%') {
                    ESP_LOGI(TAG, "Found %% at position %d.\n", x);
                    printf("So far we have:\n%.*s\n", sp, e);
                    // Found the first tag, i.e. right on the start of the token
					// Send raw data up to now
					if (sp!=0) httpd_resp_send_chunk(req, e, sp);
                    // httpdSend(connData, e, sp);
					sp=0;
					//Go collect token chars.
					tokenPos=0;
				} else {
                    // Count the length of common page text
					sp++;
				}
			} else {
                // Inside the token
				if (buff[x]=='%') {
                    // Found the second tag, i.e. right on the end of the token
					if (tokenPos==0) {
						// If there was no text between, then it´s just a %% escape string.
						// Send a single % and resume with the normal program flow.
                        httpd_resp_send_chunk(req, "%", 1);
						// httpdSend(connData, "%", 1);
					} else {
						//This was an actual token.
						token[tokenPos++]=0; //zero-terminate token
                        ESP_LOGI(TAG, "The token found: %s.\n", token);
                        tokenlen = ((changeToken_cb_t)changeToken_cb)(req, token);
                        ESP_LOGI(TAG, "The token replaced %s.\n", token);
                        httpd_resp_send_chunk(req, token, tokenlen);
						// ((TplCallback)(connData->cgiArg))(connData, tpd->token, &tpd->tplArg);
					}
					//Go collect normal chars again.
					e=&buff[x+1];
					tokenPos=-1;
				} else {
                    // Filling the "token" variable with the token string
					if (tokenPos<(sizeof(token)-1)) token[tokenPos++]=buff[x];
				}
			}
		}
	}
    //Send remaining bit.
	if (sp!=0) httpd_resp_send_chunk(req, e, sp);;
	// if (sp!=0) httpdSend(connData, e, sp);
}