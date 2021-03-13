
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

#include "ESP32-Info.h"

void ESP32Info::add( const char* format, ... )
{
    va_list varArgs;
    va_start(varArgs, format);

    vsnprintf(buffer, sizeof(buffer), format, varArgs);
    buffer[sizeof(buffer)-1] = '\0';
    va_end(varArgs);

    info += buffer;
}

const char* ESP32Info::convChipMode(FlashMode_t f)
{
    switch(f)
    {
        case FM_QIO:
            return "QIO";
        case FM_QOUT:
            return "QOUT";
        case FM_DIO:
            return "DIO";
        case FM_DOUT:
            return "DOUT";
        case FM_FAST_READ:
            return "FAST READ";
        case FM_SLOW_READ:
            return "SLOW READ";
        case FM_UNKNOWN:
        default:
            return "Unknown";
    };
}

const char* ESP32Info::convPTType(esp_partition_type_t t) 
{
    switch(t)
    {
        case ESP_PARTITION_TYPE_APP:    // Application partition type
            return "app";
        case ESP_PARTITION_TYPE_DATA:   // Data partition type
            return "data";
        default:
            return "N/A";
    };
}

const char* ESP32Info::convPTSubType(esp_partition_subtype_t s)
{
    switch(s)
    {
        case ESP_PARTITION_SUBTYPE_APP_FACTORY:   // Factory application partition
            return "Factory";
      //case ESP_PARTITION_SUBTYPE_APP_OTA_MIN:   // Base for OTA partition subtypes
      //    return "OTA";
        case ESP_PARTITION_SUBTYPE_APP_OTA_0:     // OTA partition 0
            return "OTA_0";
        case ESP_PARTITION_SUBTYPE_APP_OTA_1:     // OTA partition 1
            return "OTA_1";
        case ESP_PARTITION_SUBTYPE_APP_OTA_2:     // OTA partition 2
            return "OTA_2";
        case ESP_PARTITION_SUBTYPE_APP_OTA_3:     // OTA partition 3
            return "OTA_3";
        case ESP_PARTITION_SUBTYPE_APP_OTA_4:     // OTA partition 4
            return "OTA_4";
        case ESP_PARTITION_SUBTYPE_APP_OTA_5:     // OTA partition 5
            return "OTA_5";
        case ESP_PARTITION_SUBTYPE_APP_OTA_6:     // OTA partition 6
            return "OTA_6";
        case ESP_PARTITION_SUBTYPE_APP_OTA_7:     // OTA partition 7
            return "OTA_7";
        case ESP_PARTITION_SUBTYPE_APP_OTA_8:     // OTA partition 8
            return "OTA_8";
        case ESP_PARTITION_SUBTYPE_APP_OTA_9:     // OTA partition 9
            return "OTA_9";
        case ESP_PARTITION_SUBTYPE_APP_OTA_10:    // OTA partition 10
            return "OTA_10";
        case ESP_PARTITION_SUBTYPE_APP_OTA_11:    // OTA partition 11
            return "OTA_11";
        case ESP_PARTITION_SUBTYPE_APP_OTA_12:    // OTA partition 12
            return "OTA_12";
        case ESP_PARTITION_SUBTYPE_APP_OTA_13:    // OTA partition 13
            return "OTA_13";
        case ESP_PARTITION_SUBTYPE_APP_OTA_14:    // OTA partition 14
            return "OTA_14";
        case ESP_PARTITION_SUBTYPE_APP_OTA_15:    // OTA partition 15
            return "OTA_15";
      //case ESP_PARTITION_SUBTYPE_APP_OTA_MAX:   // Max subtype of OTA partition
      //    return "OTA Max";
        case ESP_PARTITION_SUBTYPE_APP_TEST:      // Test application partition
            return "Test";
     // case ESP_PARTITION_SUBTYPE_DATA_OTA:      // OTA selection partition
     //     return "OTA select";
        case ESP_PARTITION_SUBTYPE_DATA_PHY:      // PHY init data partition
            return "PHY init";
        case ESP_PARTITION_SUBTYPE_DATA_NVS:      // NVS partition
            return "NVS";
        case ESP_PARTITION_SUBTYPE_DATA_COREDUMP: // COREDUMP partition
            return "COREDUMP";
        case ESP_PARTITION_SUBTYPE_DATA_NVS_KEYS: // Partition for NVS keys
            return "NVS keys";
        case ESP_PARTITION_SUBTYPE_DATA_ESPHTTPD: // ESPHTTPD partition
            return "HTTPD";
        case ESP_PARTITION_SUBTYPE_DATA_FAT:      // FAT partition
            return "FAT";
        case ESP_PARTITION_SUBTYPE_DATA_SPIFFS:   // SPIFFS partition
            return "SPIFFS";
        default:
            return "Unknown";
    };
}

void ESP32Info::printPTable(esp_partition_iterator_t iter)
{
    if(iter)
    {
        const esp_partition_t* part = esp_partition_get(iter);
        while(part)
        {
            add("<tr><td>%17s</td><td>%4s</td><td>%8s</td><td>%#8.4x</td><td>%8d</td><td>%s</td></tr>",
                part->label, convPTType(part->type), convPTSubType(part->subtype), 
                part->address, part->size, (part->encrypted ? "Encrypted" : ""));

            if((iter = esp_partition_next(iter)))
            {
                part = esp_partition_get(iter);
            }
            else
                part = NULL;
        }
        if(iter) esp_partition_iterator_release(iter);
    }
}
String* ESP32Info::getHTML(const char* pre, const char* post)
{
    uint32_t nHeapSize = ESP.getHeapSize(); //total heap size
    uint32_t nPsramSize = ESP.getPsramSize(); // total SPI RAM

    uint8_t  nChipRevision = ESP.getChipRevision();
    uint32_t nCpuFreqMHz = ESP.getCpuFreqMHz();
    uint32_t nCycleCount = ESP.getCycleCount();
    const char* pSdkVersion = ESP.getSdkVersion();
    if(!pSdkVersion)pSdkVersion = "";

    uint32_t nFlashChipSize = ESP.getFlashChipSize();
    uint32_t nFlashChipSpeed = ESP.getFlashChipSpeed();
    FlashMode_t sFlashChipMode = ESP.getFlashChipMode();

    uint32_t nSketchSize = ESP.getSketchSize();
    uint32_t nFreeSketchSpace = ESP.getFreeSketchSpace();
    uint32_t nSketchSpace = nFreeSketchSpace  + nSketchSize;

    uint64_t nEfuseMac = ESP.getEfuseMac();

    esp_chip_info_t ci;
    esp_chip_info(&ci);

    info = pre;
    info.reserve(INFO_BUFFER_SIZE);

    add("<p>Model: %s, Cores: %d, Revision: %d, Features(%08X): ",
        ((CHIP_ESP32==ci.model) ? "ESP32" : "Unknown"),
        ci.cores, ci.revision, ci.features);
    if(CHIP_FEATURE_EMB_FLASH & ci.features) add("Embedded flash memory, ");
    if(CHIP_FEATURE_WIFI_BGN  & ci.features) add("2.4GHz WiFi, ");
    if(CHIP_FEATURE_BLE       & ci.features) add("Bluetooth LE, ");
    if(CHIP_FEATURE_BT        & ci.features) add("Bluetooth Classic, ");

    add("</p><table>"
        "<tr><td>Chip ID</td><td> %04X </td></tr>"
        "<tr><td>MAC ID</td><td> %08X </td></tr>"
        "<tr><td>Total Heap Size</td><td> %d </td></tr>"
        "<tr><td>Total SPI RAM size</td><td> %d </td></tr>"
        "<tr><td>Chip Revision</td><td> %d </td></tr>" 
        "<tr><td>CPU Freq.</td><td> %d MHz </td></tr>" 
        "<tr><td>Cycle Count</td><td> %d </td></tr>" 
        "<tr><td>SDK Version</td><td> %s </td></tr>" 
        "<tr><td>Flash Chip Size</td><td> %d </td></tr>" 
        "<tr><td>Flash Chip Speed</td><td> %d </td></tr>" 
        "<tr><td>Flash Chip Mode</td><td> %s </td></tr>" 
        "<tr><td>Sketch Space</td><td> %d </td></tr></table>", 
        (uint16_t)(nEfuseMac >>32), (uint32_t)nEfuseMac, 
        nHeapSize,
        nPsramSize, 
        nChipRevision, 
        nCpuFreqMHz, 
        nCycleCount, 
        pSdkVersion, 
        nFlashChipSize, 
        nFlashChipSpeed, 
        convChipMode(sFlashChipMode), 
        nSketchSpace);

    add("<h2>Partition Table</h2><table><tr><th>Name</th><th>Type</th><th>SubType</th><th>Offset</th><th>Size</th><th>Flags</th></tr>");
    esp_partition_iterator_t iter = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL); 
    printPTable(iter);
    iter = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL); 
    printPTable(iter);
    add("</table>");

    if(SPIFFS.begin(true))
    {
        unsigned int totalBytes = SPIFFS.totalBytes();
        unsigned int usedBytes = SPIFFS.usedBytes();

        add("<h2>SPIFFS File system information</h2><table><tr><td>Total space</td><td>%d bytes</td></tr><tr><td>Total used space</td><td>%d bytes</td></tr></table>",
        totalBytes,usedBytes);

    } 
    add("<p>done</p>");

    info += post;
    return &info;
}

