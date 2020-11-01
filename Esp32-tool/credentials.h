#ifndef CREDENTIALS_H
#define CREDENTIALS_H

// This file contains the WiFi configuration and should be modified
// 

// The following lines can be ignored 
// (I use then to avoid placing my router credentials in github) 
#if defined __has_include
#if __has_include(<mycredentials.h>)
#include <mycredentials.h>
#endif
#else

// These are the WiFi credential for your router
// (leave them empty to force hotspot behaviour)
const char* ssid = "";
const char* password = "";
#endif

// This willbe the host name for the device
const char* Host_Name        = "ESP32-host";
//
// Hotspot credentials
const char* hotspot_SSID     = "ESP32-hotspot";
const char* hotspot_Password = "123456789"; // MUST be more than 8 characters

#endif
