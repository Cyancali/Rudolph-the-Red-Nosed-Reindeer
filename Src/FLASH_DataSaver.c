#include "FLASH_DataSaver.h"

/*	Variables	*/
uint32_t memory_address_FLASH_Start = 0x08000000; 
uint32_t memory_address_FLASH_Page 	= 0x08007000; 
uint32_t memory_address_FLASH_End 	= 0x08008000; 

FLASH_EraseInitTypeDef FLASH_Erase_data;

uint32_t PageError;



/*	Functions	*/
void initFLASH_DataSaver(void)
{
	FLASH_Erase_data.TypeErase 		= FLASH_TYPEERASE_PAGES;
	FLASH_Erase_data.PageAddress 	= memory_address_FLASH_Page;
	FLASH_Erase_data.NbPages 			= 1;
}

void saveDataToFLASH(uint32_t data)
{
	// FLASH unlocken, um eine Seite beschreiben zu können
	if (HAL_FLASH_Unlock() == HAL_OK)
	{
		// Erase one page
		HAL_FLASHEx_Erase(&FLASH_Erase_data, &PageError);
		// Daten speichern
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, memory_address_FLASH_Page, data);
		// Lock FLASH
		HAL_FLASH_Lock();
	}
}

uint32_t 	readDataFromFLASH(void)
{	
	return *((uint32_t*)memory_address_FLASH_Page);
}
