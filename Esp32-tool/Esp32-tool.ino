
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

#define USE_ASYNC

#include <WiFi.h>
#ifdef USE_ASYNC
#include <ESPAsyncWebServer.h> // From: https://github.com/me-no-dev/ESPAsyncWebServer
#else
#include <WebServer.h>
#endif
#include <Update.h>
#include "ESP32-Info.h"

#include "credentials.h"
#include "html.h"

#ifdef USE_ASYNC
AsyncWebServer server(80);
#else
WebServer server(80);
#endif

ESP32Info Info;

int cnt(0);

const char* myType =
#ifdef USE_ASYNC
           "Async ";
#else
           "";
#endif

#ifdef USE_ASYNC
void PrintMsgInfo(AsyncWebServerRequest * request,const char* hdr)
{

    size_t nHdrs = request->headers();  // get header count
    size_t nArg = request->args();      // get arguments count
    Serial.printf("MSG[%s, ver:%d, method: %d, url:'%s', host:'%s', type:'%s', len: %d, multypart: %s, method:'%s', req type:'%s'\nHeaders:\n",
        hdr,
        request->version(),
        request->method(),
        request->url().c_str(),
        request->host().c_str(),
        request->contentType().c_str(),
        request->contentLength(),
       (request->multipart() ? "Yes" : "No"),
        request->methodToString(),
        request->requestedConnTypeToString());

    for(int i=0; i<nHdrs; i++)
    {
        Serial.printf("    %s:%s\n", request->headerName(i).c_str(), request->header(i).c_str());
    }

    if(nArg)
    {
        Serial.println("Arguments:");
        for(int i=0; i<nArg; i++)
        {
            Serial.printf("    %s?%s\n", request->argName(i).c_str(), request->arg(i).c_str());
        }
    }

    Serial.println("]MSG");
}

void getInfo(AsyncWebServerRequest *request)
{
    PrintMsgInfo(request, "GET /info");
    request->send_P(200, "text/html", Info.getHTML(pre_html,post_html)->c_str());
}

void getOTA(AsyncWebServerRequest *request)
{
    PrintMsgInfo(request, "GET /ota");
    request->send_P(200, "text/html", ota_html);
}

void postUpdateProcess(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if(!index)
    {
        Serial.setDebugOutput(true);
        PrintMsgInfo(request,"POST /update");
        Serial.printf("\n[BEGIN] Update: '%s', index %d, len %d, %s\n", filename.c_str(), index, len, (final ? "DONE" : ""));
        cnt = 0;
        if (!Update.begin()) //start with max available size
        {
            Update.printError(Serial);
        }
    }

    if(((++cnt)%50)==0)Serial.printf("(%d, %d)\n", index, len); else Serial.print(".");

    if (Update.write(data, len) != len)
    {
        Update.printError(Serial);
    }

    if(final)
    {
        Serial.printf("(%d, %d)\n[END]\n", index, len);
        if (Update.end(true)) //true to set the size to the current progress
        {
            if(Update.hasError())
            {
                Serial.printf("Update Success: %u\nRebooting...\n", index+len);
            } 
            else 
            {
                Update.printError(Serial);
            }
        }
        Serial.setDebugOutput(false);
    }
}

void postUpdateEnd(AsyncWebServerRequest *request)
{
    Serial.println("done");
    if(Update.hasError())
    {
        String errStr = errPre_html;
        errStr += Update.errorString();
        errStr += errPost_html;
        request->send(500, "text/html", errStr.c_str());
    }
    else
    {
        request->send(200, "text/html", done_html);
      //ESP.restart();
    }
}

#else // Non-Async

void PrintMsgInfo(const char* hdr)
{

    size_t nHdrs = server.headers();  // get header count
    size_t nArg = server.args();      // get arguments count
    Serial.printf("MSG[%s, method: %d, uri:'%s'\nHeaders:\n",
        hdr,
        server.method(),
        server.uri().c_str());

    for(int i=0; i<nHdrs; i++)
    {
        Serial.printf("    %s:%s\n", server.headerName(i).c_str(), server.header(i).c_str());
    }

    if(nArg)
    {
        Serial.println("Arguments:");
        for(int i=0; i<nArg; i++)
        {
            Serial.printf("    %s?%s\n", server.argName(i).c_str(), server.arg(i).c_str());
        }
    }

    Serial.println("]MSG");
}

void getInfo(void)
{
    PrintMsgInfo("GET /info");
    server.sendHeader("Connection", "close");
    server.send_P(200, "text/html", Info.getHTML(pre_html,post_html)->c_str());
}

void getOTA(void)
{
    PrintMsgInfo("GET /ota");
    server.sendHeader("Connection", "close");
    server.send_P(200, "text/html", ota_html);
}

void postUpdateProcess(void)
{
    HTTPUpload& upload = server.upload();
    switch (upload.status) 
    {
        case UPLOAD_FILE_START: 
            PrintMsgInfo("POST /update");
            cnt = 0;
            Serial.printf("[BEGIN] Update: '%s'\n", upload.filename.c_str());
            if (!Update.begin()) //start with max available size
            {
                Update.printError(Serial);
            }
            break;

        case UPLOAD_FILE_WRITE: 
            if(((++cnt)%50)==0)Serial.println();
            Serial.print(".");
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) 
            {
                Update.printError(Serial);
            }
            break;

        case UPLOAD_FILE_END: 
            Serial.printf("\n[END]\n");
            if (Update.end(true)) //true to set the size to the current progress
            {
                Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
            } 
            else 
            {
                Update.printError(Serial);
            }
            Serial.setDebugOutput(false);
            break;

        default:
            Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
    }
}

void postUpdateEnd(void)
{
    Serial.println("done");
    server.sendHeader("Connection", "close");
    if(Update.hasError())
    {
        String errStr = errPre_html;
        errStr += Update.errorString();
        errStr += errPost_html;
        server.send(500, "text/html", errStr.c_str());
    }
    else
    {
        server.send(200, "text/html", done_html);
      //ESP.restart();
    }
}
#endif

void WiFiEvent(WiFiEvent_t event)
{
    if((event == SYSTEM_EVENT_STA_START)
    || (event == SYSTEM_EVENT_AP_START)
    || (event == SYSTEM_EVENT_ETH_START))
         
    {
        Serial.printf("\n==> [WiFi-event] event (%d) \n", event);
        if(*ssid == '\0')
        {
            WiFi.softAPsetHostname(Host_Name);
            delay(100);
            //Serial.printf("Host name: %s\n", WiFi.softAPgetHostname());
        }
        else
        {
            WiFi.setHostname(Host_Name);
            delay(100);
            //Serial.printf("Host name: %s\n", WiFi.getHostname());
        }

    }
    Serial.println();
}

void setup() 
{
  Serial.begin(115200);

  WiFi.onEvent(WiFiEvent);

  if(*ssid == '\0')
  {
      Serial.printf("\n=======\n\nSTART %sHotspot\n", myType);
      WiFi.mode(WIFI_AP);

      // Connect to Wi-Fi network with SSID and hotspot_Password
      // Remove the hotspot_Password parameter, if you want it to be open
      WiFi.softAP(hotspot_SSID, hotspot_Password);
      delay(500); // Needed to solve a race condition

      String IP = WiFi.softAPIP().toString();
      Serial.printf("Hotspot IP address: %s\n         Host name: %s\n",
          IP.c_str(), WiFi.softAPgetHostname());
  }
  else
  {
      Serial.printf("\n=======\n\nSTART %s connecting to %s\n", myType, ssid);
      WiFi.begin(ssid, password);
     
      Serial.println("Connecting to WiFi.");
      while (WiFi.status() != WL_CONNECTED) 
      {
        delay(500);
        Serial.print(".");
      } 

      String IP = WiFi.localIP().toString();
      Serial.printf("Connected to: %s\n IP address: %s\n Host name:  %s\n",
          ssid, IP.c_str(), WiFi.getHostname());
  }
  
  server.on("/", HTTP_GET, &getInfo); 
  server.on("/info", HTTP_GET, &getInfo); 
  server.on("/ota", HTTP_GET, &getOTA); 
  server.on("/update", HTTP_GET, &getOTA); 
  server.on("/update", HTTP_POST, &postUpdateEnd, &postUpdateProcess);

  server.begin();
}

void loop()
{
#ifndef USE_ASYNC
  server.handleClient();
  delay(1);
#endif
}
