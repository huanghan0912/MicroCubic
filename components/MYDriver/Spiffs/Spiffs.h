#ifndef _SPIFFS_H_
#define _SPIFFS_H_

#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include <stdio.h>




void SpiffsInit();
void SpiffsWriteFile(const char *Filename, const char *message);
int SpiffsReadFile(const char *Filename, char *message);  
void SpiffsDelFile(const char *Filename);
bool SpiffsAnalyseParam(char *info, int argc, char **argv);






#endif