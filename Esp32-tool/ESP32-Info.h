
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

#ifndef ESP32_INFO_H
#define ESP32_INFO_H

extern "C"
{
#include <esp_system.h>
#include <esp_partition.h>
#include <esp_spi_flash.h>
#include <esp_spiffs.h>
}
#include <Esp.h>
#include <SPIFFS.h>

#define INFO_BUFFER_SIZE 1024

class ESP32Info
{
    String info;
    char buffer[INFO_BUFFER_SIZE];

    void add(const char* format, ...);

    const char* convChipMode(FlashMode_t f);
    const char* convPTType(esp_partition_type_t t) ;
    const char* convPTSubType(esp_partition_subtype_t s);
    void printPTable(esp_partition_iterator_t iter);
public:
    ESP32Info(void){}
   ~ESP32Info(void){}

    String* getHTML(const char* pre, const char* post);
    void    release(void) {info = "";}
};

#endif

