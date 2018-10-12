cat .\style.css | .\bin2h.exe -cz style > html_code.h
cat .\flash.html | .\bin2h.exe -cz flash >> html_code.h
cat .\index.html | .\bin2h.exe -cz flash >> html_code.h