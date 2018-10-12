
char *index_html = "<html>\n<head>\n<meta charset=\"utf-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n<title>Esp32 web server</title>\n<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n</head>\n\n<body>\n<div id=\"main\">\n<h1>Esp8266 web server</h1>\n<p>\nThis page has been loaded <b>%counter%</b> times.\n<ul>\n\t<li><a href=\"flash.html\">OTA flashing</a></ul>\n\t<li><a href=\"switch.tpl\">GPIO ovládání</a></li>\n\t<li><a href=\"dht12.tpl\">DHT12 senzor</a></li>\n\t<li><a href=\"moisture.tpl\">Vlkhost půdy</a></li>\n\t<li><a href=\"/wifi\">Wi-fi scan a připojení</a></li>\n</ul>\n</p>\n</div>\n</body></html>\n";

char *style_css = "/*\nbody {\n  font: 23px/30px 'Lucida Grande', Tahoma, Verdana, sans-serif;\n  color: white;\n  background: #3b3f46;\n}\n*/\n\nbody {\n\tbackground-color: #404040;\n\tfont-family: sans-serif;\n}\n\n#main {\n\tbackground-color: #d0d0FF;\n\t-moz-border-radius: 5px;\n\t-webkit-border-radius: 5px;\n\tborder-radius: 5px;\n\tborder: 2px solid #000000;\n\twidth: 800px;\n\tmargin: 0 auto;\n\tpadding: 20px\n}\n\n#progressbar {\n\tmargin: 10px;\n\tpadding: 0;\n\tborder: 1px solid #000000;\n\theight: 20px;\n\twidth: 200px;\n\tbackground-color: #808080;\n}\n\n#progressbarinner {\n\twidth: 10px;\n\theight: 20px;\n\tborder: none;\n\tbackground-color: #00ff00;\n}\n\n\n.button { \n\tbackground-color: #195B6A; \n\tborder: none; \n\tcolor: white; \n\tpadding: 16px 40px;\n\ttext-decoration: none; \n\tfont-size: 30px; \n\tmargin: 2px; \n\tcursor: pointer;\n}\n\n.button2 {\n\tbackground-color: #77878A;\n}\n\n\n.container {\n  margin: 0 auto;\n  padding: 90px 0;\n  width: 400px;\n  background-image: -webkit-radial-gradient(center, farthest-side, rgba(255, 255, 255, 0.1), rgba(255, 255, 255, 0));\n  background-image: -moz-radial-gradient(center, farthest-side, rgba(255, 255, 255, 0.1), rgba(255, 255, 255, 0));\n  background-image: -o-radial-gradient(center, farthest-side, rgba(255, 255, 255, 0.1), rgba(255, 255, 255, 0));\n  background-image: radial-gradient(center, farthest-side, rgba(255, 255, 255, 0.1), rgba(255, 255, 255, 0));\n}\n\n.switch {\n  position: relative;\n  margin: 20px auto;\n  height: 46px;\n  width: 240px;\n  background: rgba(0, 0, 0, 0.25);\n  border-radius: 3px;\n  -webkit-box-shadow: inset 0 1px 3px rgba(0, 0, 0, 0.3), 0 1px rgba(255, 255, 255, 0.1);\n  box-shadow: inset 0 1px 3px rgba(0, 0, 0, 0.3), 0 1px rgba(255, 255, 255, 0.1);\n}\n\n.switch-label {\n  position: relative;\n  z-index: 2;\n  float: left;\n  width: 118px;\n  line-height: 45px;\n  font-size: 26px;\n  color: rgba(255, 255, 255, 0.35);\n  text-align: center;\n  text-shadow: 0 1px 1px rgba(0, 0, 0, 0.45);\n  cursor: pointer;\n}\n.switch-label:active {\n  font-weight: bold;\n}\n\n.switch-label-off {\n  padding-left: 2px;\n}\n\n.switch-label-on {\n  padding-right: 2px;\n}\n\n.switch-input {\n  display: none;\n}\n.switch-input:checked + .switch-label {\n  font-weight: bold;\n  color: rgba(0, 0, 0, 0.65);\n  text-shadow: 0 1px rgba(255, 255, 255, 0.25);\n  -webkit-transition: 0.15s ease-out;\n  -moz-transition: 0.15s ease-out;\n  -o-transition: 0.15s ease-out;\n  transition: 0.15s ease-out;\n}\n.switch-input:checked + .switch-label-on ~ .switch-selection {\n  left: 120px;\n  /* Note: left: 50% doesn't transition in WebKit */\n}\n\n.switch-selection {\n  display: block;\n  position: absolute;\n  z-index: 1;\n  top: 2px;\n  left: 2px;\n  width: 118px;\n  height: 42px;\n  background: #65bd63;\n  border-radius: 3px;\n  background-image: -webkit-linear-gradient(top, #9dd993, #65bd63);\n  background-image: -moz-linear-gradient(top, #9dd993, #65bd63);\n  background-image: -o-linear-gradient(top, #9dd993, #65bd63);\n  background-image: linear-gradient(to bottom, #9dd993, #65bd63);\n  -webkit-box-shadow: inset 0 1px rgba(255, 255, 255, 0.5), 0 0 2px rgba(0, 0, 0, 0.2);\n  box-shadow: inset 0 1px rgba(255, 255, 255, 0.5), 0 0 2px rgba(0, 0, 0, 0.2);\n  -webkit-transition: left 0.15s ease-out;\n  -moz-transition: left 0.15s ease-out;\n  -o-transition: left 0.15s ease-out;\n  transition: left 0.15s ease-out;\n}\n.switch-blue .switch-selection {\n  background: #3aa2d0;\n  background-image: -webkit-linear-gradient(top, #4fc9ee, #3aa2d0);\n  background-image: -moz-linear-gradient(top, #4fc9ee, #3aa2d0);\n  background-image: -o-linear-gradient(top, #4fc9ee, #3aa2d0);\n  background-image: linear-gradient(to bottom, #4fc9ee, #3aa2d0);\n}\n.switch-yellow .switch-selection {\n  background: #c4bb61;\n  background-image: -webkit-linear-gradient(top, #e0dd94, #c4bb61);\n  background-image: -moz-linear-gradient(top, #e0dd94, #c4bb61);\n  background-image: -o-linear-gradient(top, #e0dd94, #c4bb61);\n  background-image: linear-gradient(to bottom, #e0dd94, #c4bb61);\n}\n\n.mainDiv {\n\ttext-align: center; \n\twidth: 100%;\n}\n";

char *flash_html = "<html>\n<head><title>Upgrade firmware</title>\n<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n<script type=\"text/javascript\" src=\"140medley.min.js\"></script>\n<script type=\"text/javascript\">\n\nvar xhr=j();\n\nfunction doReboot() {\n\txhr.open(\"GET\", \"reboot\");\n\txhr.onreadystatechange=function() {\n\t\tif (xhr.readyState==4 && xhr.status>=200 && xhr.status<300) {\n\t\t\twindow.setTimeout(function() {\n\t\t\t\tlocation.reload(true);\n\t\t\t}, 3000);\n\t\t}\n\t}\n\t//ToDo: set timer to \n\txhr.send();\n}\n\nfunction setProgress(amt) {\n\t$(\"#progressbarinner\").style.width=String(amt*200)+\"px\";\n}\n\nfunction doUpgrade() {\n\tvar f=$(\"#file\").files[0];\n\tif (typeof f=='undefined') {\n\t\t$(\"#remark\").innerHTML=\"Can't read file!\";\n\t\treturn\n\t}\n\txhr.open(\"POST\", \"upload\");\n\txhr.onreadystatechange=function() {\n\t\tif (xhr.readyState==4 && xhr.status>=200 && xhr.status<300) {\n\t\t\tsetProgress(1);\n\t\t\tif (xhr.responseText!=\"\") {\n\t\t\t\t$(\"#remark\").innerHTML=\"Error: \"+xhr.responseText;\n\t\t\t} else {\n\t\t\t\t$(\"#remark\").innerHTML=\"Uploading done. Rebooting.\";\n\t\t\t\tdoReboot();\n\t\t\t}\n\t\t}\n\t}\n\tif (typeof xhr.upload.onprogress != 'undefined') {\n\t\txhr.upload.onprogress=function(e) {\n\t\t\tsetProgress(e.loaded / e.total);\n\t\t}\n\t}\n\txhr.send(f);\n\treturn false;\n}\n\n\nwindow.onload=function(e) {\n\txhr.open(\"GET\", \"next\");\n\txhr.onreadystatechange=function() {\n\t\tif (xhr.readyState==4 && xhr.status>=200 && xhr.status<300) {\n\t\t\tvar txt=\"Please upload \"+xhr.responseText+\" or ota file.\";\n\t\t\t$(\"#remark\").innerHTML=txt;\n\t\t\tsetProgress(0);\n\t\t}\n\t}\n\txhr.send();\n}\n\n</script>\n</head>\n<body>\n<div id=\"main\">\n<h1>Update firmware</h1>\n<div id=\"remark\">Loading...</div>\n<input type=\"file\" id=\"file\" />\n<input type=\"submit\" value=\"Upgrade!\" onclick=\"doUpgrade()\" />\n<div id=\"progressbar\"><div id=\"progressbarinner\"></div></div>\n</body>";
