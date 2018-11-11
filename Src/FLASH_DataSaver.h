#ifndef __FLASH_DataSaver_H
#define __FLASH_DataSaver_H

#include "stdlib.h"
#include "string.h"
#include "stm32f0xx_hal.h"

void initFLASH_DataSaver(void);
void 			saveDataToFLASH(uint32_t data);
uint32_t 	readDataFromFLASH(void);

#endif /* __FLASH_DataSaver_H */
