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

