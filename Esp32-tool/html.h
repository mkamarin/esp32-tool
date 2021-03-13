
/*  Part of esp32-tool.
 *
 *  Basic tool to get microprocessor info, configuration, partition tables, 
 *  etc., and to perform some operations like OTA, clean SPIFFS, etc.
 *
 *  Copyright (C) 2021 Mike Marin -- All Rights Reserved
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 *  You can contact me at mmarin <at> acm <dot> org
 */

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
