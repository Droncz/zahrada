#ifndef __MY_URLS_H__
#define __MY_URLS_H__

// #include "../html/html_code.h"

char index_html[] = "<html>\n<head>\n<meta charset=\"utf-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n<title>Esp32 web server</title>\n</head>\n\n<body>\n<div class=\"main\">\n<h1>Esp32 web server</h1>\n<p>\nSystém byl restartován již <b>%reboots%</b>-krát.\n<p>\nOd posledního restartu byla tato stránka zobrazena již <b>%counter%</b>-krát.\n<ul>\n\t<li><a href=\"flash.html\">OTA flashing</a></li>\n\t<li><a href=\"switch.cgi\">GPIO ovládání</a></li>\n\t<li><a href=\"dht12.html\">DHT12 senzor</a></li>\n\t<li><a href=\"moisture.html\">Vlkhost půdy</a></li>\n</ul>\n</p>\n</div>\n</body></html>\n";
char style_css[] = "body, html {\n  /* background-color: #efefef; */\n  /* background-color: #404040; */\n  display: flex;\n  justify-content: center;\n  align-items: center;\n  /* min-height: 100%; */\n  z-index: -1;\n  font-family: sans-serif;\n}\n\n#progressbar {\n  margin: 10px;\n  padding: 0;\n  border: 1px solid #000000;\n  height: 20px;\n  width: 200px;\n  background-color: #808080;\n}\n\n#progressbarinner {\n  width: 10px;\n  height: 20px;\n  border: none;\n  background-color: #00ff00;\n}\n\n.main {\n  background-color: #d0d0FF;\n  /* width: 800px; */\n  border-radius: 5px;\n  border: 2px solid #000000;\n  margin: 0 auto;\n  padding: 10px;\n}\n\n.btn {\n  border: 3px solid #1a1a1a;\n  display: inline-block;\n  padding: 10px;\n  position: relative;\n  text-align: center;\n  transition: background 600ms ease, color 600ms ease;\n}\n\ntable {\n    border:none;\n    border-collapse: collapse;\n}\nth {\n    padding: 6px;\n    padding-bottom: 12px;\n}\nth, td {\n  border-bottom: 2px solid #eee;\n  border-right: 2px solid #eee;\n  text-align: center;\n}\ntr:hover {background-color: #f5f5f5;}\n\ninput[type=\"radio\"].toggle {\n  display: none;\n}\ninput[type=\"radio\"].toggle + label {\n  cursor: pointer;\n  min-width: 10px;\n}\ninput[type=\"radio\"].toggle + label:hover {\n  background: none;\n  color: #1a1a;\n}\ninput[type=\"radio\"].toggle + label:after {\n  background: #a1a;\n  content: \"\";\n  height: 100%;\n  position: absolute;\n  top: 0;\n  transition: left 200ms cubic-bezier(0.77, 0, 0.175, 1);\n  width: 100%;\n  /* z-index: -1; */\n}\ninput[type=\"radio\"].toggle.toggle-left + label {\n  border-right: 0;\n}\ninput[type=\"radio\"].toggle.toggle-left + label:after {\n  left: 100%;\n}\ninput[type=\"radio\"].toggle.toggle-right + label {\n  margin-left: -5px;\n}\ninput[type=\"radio\"].toggle.toggle-right + label:after {\n  left: -100%;\n}\ninput[type=\"radio\"].toggle:checked + label {\n  cursor: default;\n  color: #fff;\n  transition: color 200ms;\n}\ninput[type=\"radio\"].toggle:checked + label:after {\n  left: 0;\n}\n";
char flash_html[] = "<html>\n<head><title>Upgrade firmware</title>\n<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n<script type=\"text/javascript\" src=\"140medley.min.js\"></script>\n<script type=\"text/javascript\">\n\nvar xhr=j();\n\nfunction doReboot() {\n\txhr.open(\"GET\", \"reboot\");\n\txhr.onreadystatechange=function() {\n\t\tif (xhr.readyState==4 && xhr.status>=200 && xhr.status<300) {\n\t\t\twindow.setTimeout(function() {\n\t\t\t\tlocation.reload(true);\n\t\t\t}, 3000);\n\t\t}\n\t}\n\t//ToDo: set timer to \n\txhr.send();\n}\n\nfunction setProgress(amt) {\n\t$(\"#progressbarinner\").style.width=String(amt*200)+\"px\";\n}\n\nfunction doUpgrade() {\n\tvar f=$(\"#file\").files[0];\n\tif (typeof f=='undefined') {\n\t\t$(\"#remark\").innerHTML=\"Can't read file!\";\n\t\treturn\n\t}\n\txhr.open(\"POST\", \"upload\");\n\txhr.onreadystatechange=function() {\n\t\tif (xhr.readyState==4 && xhr.status>=200 && xhr.status<300) {\n\t\t\tsetProgress(1);\n\t\t\tif (xhr.responseText!=\"\") {\n\t\t\t\t$(\"#remark\").innerHTML=\"Error: \"+xhr.responseText;\n\t\t\t} else {\n\t\t\t\t$(\"#remark\").innerHTML=\"Uploading done. Rebooting.\";\n\t\t\t\tdoReboot();\n\t\t\t}\n\t\t}\n\t}\n\tif (typeof xhr.upload.onprogress != 'undefined') {\n\t\txhr.upload.onprogress=function(e) {\n\t\t\tsetProgress(e.loaded / e.total);\n\t\t}\n\t}\n\txhr.send(f);\n\treturn false;\n}\n\n\nwindow.onload=function(e) {\n\txhr.open(\"GET\", \"next\");\n\txhr.onreadystatechange=function() {\n\t\tif (xhr.readyState==4 && xhr.status>=200 && xhr.status<300) {\n\t\t\tvar txt=\"Please upload \"+xhr.responseText+\" or ota file.\";\n\t\t\t$(\"#remark\").innerHTML=txt;\n\t\t\tsetProgress(0);\n\t\t}\n\t}\n\txhr.send();\n}\n\n</script>\n</head>\n<body>\n<div id=\"main\">\n<h1>Update firmware</h1>\n<div id=\"remark\">Loading...</div>\n<input type=\"file\" id=\"file\" />\n<input type=\"submit\" value=\"Upgrade!\" onclick=\"doUpgrade()\" />\n<div id=\"progressbar\"><div id=\"progressbarinner\"></div></div>\n</body>";
char medley_min_js[] = "var t=function(a,b){return function(c,d){return a.replace(/#{([^}]*)}/g,function(a,f){return Function(\"x\",\"with(x)return \"+f).call(c,d||b||{})})}},s=function(a,b){return b?{get:function(c){return a[c]&&b.parse(a[c])},set:function(c,d){a[c]=b.stringify(d)}}:{}}(this.localStorage||{},JSON),p=function(a,b,c,d){c=c||document;d=c[b=\"on\"+b];a=c[b]=function(e){d=d&&d(e=e||c.event);return(a=a&&b(e))?b:d};c=this},m=function(a,b,c){b=document;c=b.createElement(\"p\");c.innerHTML=a;for(a=b.createDocumentFragment();b=c.firstChild;)a.appendChild(b);return a},$=function(a,b){a=a.match(/^(\\W)?(.*)/);return(b||document)[\"getElement\"+(a[1]?a[1]==\"#\"?\"ById\":\"sByClassName\":\"sByTagName\")](a[2])},j=function(a){for(a=0;a<4;a++)try{return a?new ActiveXObject([,\"Msxml2\",\"Msxml3\",\"Microsoft\"][a]+\".XMLHTTP\"):new XMLHttpRequest}catch(b){}};";
char switch_cgi[] = "<html>\n<head>\n<meta charset=\"utf-8\">\n<meta name=\"viewport\" content=\"width=device-width; initial-scale=1; shrink-to-fit=yes; maximum-scale=2.0;\">\n<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n<title>Řízení GPIO</title>\n</head>\n\n<body>\n<div class=\"main\">\n<h1>Switch</h1>\n<table>\n  <tr>\n    <th>Relé</th>\n    <th>GPIO</th>\n    <th>Nyní</th> \n    <th>Přepnout</th> \n    <th>Povoleno/Zakázáno</th>\n  </tr>\n  <tr>\n    <td>%RELAY_1_0-name%</td>\n    <td>%RELAY_1_0-gpio%</td>\n    <td>%RELAY_1_0-state%</td>\n    <td>\n      <form>\n        <button type=\"submit\" name=\"RELAY_1_0-state\" class=\"btn\" value=\"%RELAY_1_0-newstate%\">%RELAY_1_0-newstate%</button>\n      </form>\n    </td>\n    <td>\n      <form>\n        <input id=\"RELAY_1_0-on\" class=\"toggle toggle-left\" name=\"RELAY_1_0-enabled\" value=\"1\" type=\"radio\" onchange='this.form.submit();' %RELAY_1_0-checkedon%>\n        <label for=\"RELAY_1_0-on\" class=\"btn\">ZAP</label>\n        <input id=\"RELAY_1_0-off\" class=\"toggle toggle-right\" name=\"RELAY_1_0-enabled\" value=\"0\" type=\"radio\" onchange='this.form.submit();' %RELAY_1_0-checkedoff%>\n        <label for=\"RELAY_1_0-off\" class=\"btn\">VYP</label>\n      </form>\n    </td>\n  </tr>\n  <tr>\n    <td>%RELAY_1_1-name%</td>\n    <td>%RELAY_1_1-gpio%</td>\n    <td>%RELAY_1_1-state%</td>\n    <td>\n      <form>\n        <button type=\"submit\" name=\"RELAY_1_1-state\" class=\"btn\" value=\"%RELAY_1_1-newstate%\">%RELAY_1_1-newstate%</button>\n      </form>\n    </td>\n    <td>\n      <form>\n        <input id=\"RELAY_1_1-on\" class=\"toggle toggle-left\" name=\"RELAY_1_1-enabled\" value=\"1\" type=\"radio\" onchange='this.form.submit();' %RELAY_1_1-checkedon%>\n        <label for=\"RELAY_1_1-on\" class=\"btn\">ZAP</label>\n        <input id=\"RELAY_1_1-off\" class=\"toggle toggle-right\" name=\"RELAY_1_1-enabled\" value=\"0\" type=\"radio\" onchange='this.form.submit();' %RELAY_1_1-checkedoff%>\n        <label for=\"RELAY_1_1-off\" class=\"btn\">VYP</label>\n      </form>\n    </td>\n  </tr>\n</table>\n<p>\n<a href=\"/\">HOME</a>\n</div>\n</body>\n</html>";

esp_err_t   redirect_handler(httpd_req_t *req);
esp_err_t   upload_firmware_handler(httpd_req_t *req);
esp_err_t   reboot_handler(httpd_req_t *req);
esp_err_t   html_get_handler(httpd_req_t *req);
esp_err_t   html_post_handler(httpd_req_t *req);

httpd_uri_t handlers[] = {
    { .uri      = "/style.css",
      .method   = HTTP_GET,
      .handler  = html_get_handler,
      .user_ctx = style_css,
    },
    { .uri      = "/",
      .method   = HTTP_GET,
      .handler  = redirect_handler,
      .user_ctx = "/index.html",
    },
    { .uri      = "/index.html",
      .method   = HTTP_GET,
      .handler  = html_get_handler,
      .user_ctx = index_html,
    },
    { .uri      = "/flash.html",
      .method   = HTTP_GET,
      .handler  = html_get_handler,
      .user_ctx = flash_html,
    },
    { .uri      = "/switch.cgi",
      .method   = HTTP_GET,
      .handler  = html_get_handler,
      .user_ctx = switch_cgi,
    },
    { .uri      = "/140medley.min.js",
      .method   = HTTP_GET,
      .handler  = html_get_handler,
      .user_ctx = medley_min_js,
    },
    { .uri      = "/upload",
      .method   = HTTP_POST,
      .handler  = upload_firmware_handler,
      .user_ctx = NULL,
    },
    { .uri      = "/reboot",
      .method   = HTTP_GET,
      .handler  = reboot_handler,
      .user_ctx = NULL,
    }
};


#endif // __MY_URLS_H__
