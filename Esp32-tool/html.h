#ifndef HTML_H
#define HTML_H

const char done_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head></head>
<body>
    <h1>OTA Processed</h1>
    <p>done</p>
    <p><a href='/info'>Information page</a></p>
    <p><a href='/ota'>Over the air update page</a></p>
</body>
</html>
)=====";

const char ota_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head></head>
<body>
    <h1>OTA Page</h1>
    <form method='POST' action='/update' enctype='multipart/form-data'>
        <input type='file' name='update'><input type='submit' value='Update'>
    </form>
    <p><a href='/info'>Information page</a></p>
</body>
</html>
)=====";

const char pre_html[] = " <!DOCTYPE html><html><head></head><body><h1>Information Page</h1>";
const char post_html[] = "<p><a href='/ota'>Over the air update page</a></p></body></html>";

const char errPre_html[] = " <!DOCTYPE html><html><head></head><body><h1>Error Page</h1><p>";
const char errPost_html[] = "</p><p><a href='/ota'>Retry over the air update page</a></p></body></html>";

#endif
