#ifndef __LED_Controller_H
#define __LED_Controller_H

#include "stdlib.h"
#include "string.h"
#include "stm32f0xx_hal.h"

/* Define change brightness variables */
#define LOWER_BRIGHTNESS	0
#define HIGHER_BRIGHTNESS	1

/* Define number of LEDs */
#define NUM_LEDs 					7

/* OneRun Default Color */
#define USE_DEFAULT_COLOR	0
#define USE_RGBSET_COLOR	1

/* RGB LED parameter */
extern int rgbLED[3];

/* Functions	*/
void userRunControl(void);
void userTimeControl(void);

void smoothColorChange(void);
void prepareNextColorChange(void);

// Touch functions
void changeToNextEvent(void);
void changeBrightness(uint32_t ChangeBrightnessDir);
void loadBrightnessData(uint32_t brightness);

// LED_MODE functions
void LED_Mode_Star(void);
void LED_Mode_Rainbow(void);
void LED_Mode_RainbowRun(void);
void LED_Mode_RunOneColor(uint32_t flagColor, uint32_t numberLEDsRunning);
void LED_Mode_RunOneColor_Mirror(uint32_t flagColor, uint32_t numberLEDsRunning);
void LED_Mode_ShiningLights(uint32_t NUMBER_LEDs);
void LED_Mode_RunTwoColors(void);
void LED_Mode_RunLED_Wave(uint32_t flagReverse);

void Test_RGB_LEDs(void);
void litAllLEDs(void);
void Wheel(uint32_t WheelPos, uint16_t i, uint32_t divide);
int rand_lim(int limit);
void calculateLED_WaveBrightnessMatrix(uint32_t *ptrIterLED, int *ptrWaveBrightnessMatrix);
void calcOneLED(uint32_t intByteGreen, uint32_t intByteRed, uint32_t intByteBlue, uint32_t lightLED);

// HW layer functions
void calcLED(uint32_t intByteGreen, uint32_t intByteRed, uint32_t intByteBlue, uint32_t lightLED);
void shiftForLED(void);
void shift_SW_LED(uint32_t lightLEDs);

#endif /* __LED_Controller_H */
