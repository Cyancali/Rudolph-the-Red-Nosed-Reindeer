#include "LED_Controller.h"

/*	Time control	*/
uint32_t lastTick;
uint32_t milliTick;
uint32_t msTick;
uint32_t secTick;
uint32_t minTick;
uint32_t hTick;

/* PCB Specific live color change */
// Reindeer Mode
int red_value = 0;
int steps = 8;

uint32_t delay_smoothColorChange_PCB			= 55;
uint32_t flag_delay_smoothColorChange_PCB = 0;

/* Live color change */
uint32_t color = 0;

int Rfactor = 2;
int Gfactor = 2;
int Yfactor = 1;
int Ofactor = 1;
int Pfactor = 2;

uint32_t delay_smoothColorChange 			= 500;
uint32_t flag_delay_smoothColorChange = 0;

/* Operation GENERAL*/
uint32_t operation 						= 0;
uint32_t operationRunning 		= 0;
uint32_t delay_operation 			= 0;
uint32_t flag_delay_operation = 0;

uint32_t delay_operation_null 		 = 3;
uint32_t flag_delay_operation_null = 0;

/* Operation LED_Mode_Star	*/
int delay_LED_Mode_Star = 25;
int iter_LED_Mode_Star = 0;
int Iter_LED_Mode_Star = 200;

int LED_Mode_Star_Corner[5] = {1, 3, 5, 7, 9};
int LED_Mode_Star_Circle[5] = {2, 4, 6, 8, 10};

int LED_Mode_Star_CornerColor[3] = {10, 10, 0};
int LED_Mode_Star_CircleColor[2][3] = { {10, 0, 0}, {0, 10, 0} };

int LED_Mode_Star_BrightnessAdjust 			= 10;
int LED_Mode_Star_BrightnessAdjustStep 	= -1;

/* Operation LED_Mode_RainbowRun*/
/* Rainbow run */
int delay_LED_Mode_RainbowRun = 25;
int delay_LED_Mode_RainbowRun_Step = 1;
uint32_t j = 0;
int brightness_LED_Mode_RainbowRun = 10;
int brightness_LED_Mode_RainbowRun_Step = 1;
/* Rainbow run delay */
int delay_LED_Mode_RainbowRun_Iter			 = 1;
int delay_LED_Mode_RainbowRun_IterDelay	 = 500;
int delay_LED_Mode_RainbowRun_IterFlag	 = 0;

/* Operation LED_MODE_Wave*/
/*	*/
uint32_t delay_operation_wave 			= 0;
uint32_t flag_delay_operation_wave 	= 0;

/* Operation LED_MODE_RunLED_Wave*/
/* Calculate LED colors for wave form */
uint32_t runLED_Wave_delay = 100;
uint32_t runLED_Wave_iterLED = 0;
int waveBrightnessMatrix[12] = {0};
int runLED_Wave_rgbLED[3] = {0, 0, 0};
uint32_t runLED_Wave_reverseLUT[11] = {8, 7, 6, 5, 4, 3, 2, 1, 0 ,10, 9};

/* Operation LED_MODE_RunOneColor	*/
/* Run with one color */
int runOneColor_iterLED 		= 0;
uint32_t delay_RunOneColor 			= 250;
uint8_t runOneColor_rgbLED[3]  	=  {15, 0, 0};

int runOneColor_IndexCircle[6] 	= {1, 2, 4, 6, 8, 9};
int runOneColor_StartCircle 		= 1;
int runOneColor_EndCircle 			= 9;

/* Operation LED_MODE_RunOneColor_Mirror	*/
/* Run reverse with one color and light one LED */
uint32_t runOneColor_Mirror_iterLED = 9;

int runOneColor_Mirror_IndexCircle[6] 	= {9, 8, 6, 4, 2, 1};

/* Operation LED_MODE_ShiningLights	*/
/* Shining lights on star */
uint32_t delay_ShiningLights = 200;
uint32_t shiningLightsOnTree_ITER = 5;
uint32_t shiningLightsOnTree_iter = 5;
uint32_t positionLEDs[NUM_LEDs];

/* Operation LED_MODE_RunTwoColors	*/
/* Two colors on all LEDs */
uint32_t runTwoColors_ITER = 3;
uint32_t runTwoColors_iter = 3;
uint32_t runTwoColors_delay = 200;

int runTwoColors_iterShift = 0;
int runTwoColors_iterShiftAdd = 1;

/* RGB LED parameter */
int rgbLED[3] = {40, 0, 0};

/* Brightness Index (1 min; 10 max) */
uint32_t slideColorIndex = 2;
uint32_t slideColorIndex_MIN = 1;
uint32_t slideColorIndex_MAX = 10;

/* Byte array for LED data to sent out */
int thebit[24];
int thebitArray[24*NUM_LEDs];


/* Functions	*/

/*
* Function: userRunControl
* ------------------------
* Iterates through operation modes and resets LEDs after operation is done
*
* returns:							None
*/
void userRunControl(void)
{
	/* Check if any operation is running and delay is over or no operation running */		
	if (flag_delay_operation && operationRunning == 2)
	{			
		/* Normal run mode */
		/* Set variables */
		flag_delay_operation = 0;
		delay_operation = delay_LED_Mode_RainbowRun;
		/* Start actions */		
		LED_Mode_RainbowRun();			
	}
	else if( ( operationRunning && flag_delay_operation)
				|| (!operationRunning && flag_delay_operation_null) )
	{
		operationRunning = 1;
		flag_delay_operation = 0;
		flag_delay_operation_null = 0;
	
		/* Execute operation */
		switch(operation) {
			case 0:
				/* Set variables */
				delay_operation = delay_RunOneColor;
				/* Start actions */
				LED_Mode_RunOneColor(USE_RGBSET_COLOR, 1);
				break;
			case 1:	
				/* Set variables */
				delay_operation = delay_RunOneColor;
				/* Start actions */
				LED_Mode_RunOneColor_Mirror(USE_RGBSET_COLOR,2);
				break;
			case 2:
				/* Set variables */
				delay_operation = delay_RunOneColor;
				/* Start actions */
				LED_Mode_RunOneColor(USE_RGBSET_COLOR, 3);
				break;
			case 3:
				/* Set variables */
				delay_operation = delay_ShiningLights;
				/* Start actions */
				LED_Mode_ShiningLights(3);
				break;
			case 4:
				/* Set variables */
				delay_operation = runLED_Wave_delay;
				/* Start actions */
				LED_Mode_RunLED_Wave(0);
				break;
			case 5:
				/* Set variables */
				delay_operation = runLED_Wave_delay;
				/* Start actions */
				LED_Mode_RunLED_Wave(1);
				break;
			case 6:
				/* Set variables */
				delay_operation = runLED_Wave_delay;
				/* Start actions */
				LED_Mode_RunLED_Wave(2);
				break;
			case 7:
				/* Set variables */
				delay_operation = runTwoColors_delay;
				/* Start actions */
				LED_Mode_RunTwoColors();
				break;
			case 8:
				operation = 0;
				break;			
			case 9:
				/* Set variables */
				delay_operation = delay_LED_Mode_Star;
				/* Start actions */
				LED_Mode_Star();
				break;				
			case 11:
				LED_Mode_Rainbow();
				operation = 0;
				break;
			default:
				operation = 0;
				break;
		
		}		
		if ( !(operationRunning) ) 
		{
			/* Reset to rainbow run */
			//operationRunning = 2;	// set to 2 to add between every operation a rainbow run
			
			/* Reset timeout counter */
			flag_delay_operation_null = 0;
			
			/* Reset operation parameter */
			runOneColor_iterLED = 0;	
			
			runOneColor_Mirror_iterLED = 9;
			
			shiningLightsOnTree_iter = shiningLightsOnTree_ITER;			
			
			runLED_Wave_iterLED = 0;
			memset(waveBrightnessMatrix, 0, 12 * sizeof(int));
							
			runTwoColors_iter = runTwoColors_ITER;
			
			/* Blackout all LEDs */
			calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], 0);
			shiftForLED();
			HAL_Delay(50);
		}
	}
	
	/* PCB Specific Smooth color change */
	if (flag_delay_smoothColorChange_PCB)
	{
		smoothColorChange_PCB();
		flag_delay_smoothColorChange_PCB = 0;
		shiftForLED();
	}
	
	/* Smooth color change */
	if (flag_delay_smoothColorChange)
	{
		smoothColorChange();
		flag_delay_smoothColorChange = 0;
	}
}


/*
* Function: userTimeControl
* -------------------------
* Counts ticks (in Milliseconds) and accordingly sets flags
*
* returns:							None
*/
void userTimeControl(void)
{
	/*	Count millis	*/
	if(lastTick != HAL_GetTick())
	{
		lastTick = HAL_GetTick();
		milliTick++;
		msTick++;
		/*Set milliFlags*/
		if (!(milliTick%delay_operation)) 										flag_delay_operation 								= 1;
		if (!(milliTick%delay_smoothColorChange)) 						flag_delay_smoothColorChange 				= 1;
		if (!(milliTick%delay_smoothColorChange_PCB)) 				flag_delay_smoothColorChange_PCB 		= 1;
		if (!(milliTick%delay_LED_Mode_RainbowRun_IterDelay)) delay_LED_Mode_RainbowRun_IterFlag 	= 1;
		
		/*	Count seconds	*/
		if (milliTick > 1000)
		{
			milliTick = 1;
			secTick++;
			/*Set secFlags*/			
			if (!(secTick%delay_operation_null)) flag_delay_operation_null = 1;
			
			/*	Count minutes	*/
			if (secTick > 60)
			{
				secTick = 1;
				minTick++;	
				
				/*Set minFlags*/
				//if (minTick >= 60) minTick = 0;
				
				/*	Count hours	*/
				if (minTick >= 5)
				{
					minTick = 0;
					hTick++;	
				}
			}
		}
	}
}

void smoothColorChange_PCB(void)
{
	/* Change Colors for PCB Specific LED Pattern */
	/* Calculate red values for reindeer nose */
	if (red_value >= 40) steps = -1;
	if (red_value <=  0) steps = +1;
	red_value += steps;
}

void smoothColorChange(void)
{
	/* Change colors */
	
	/* Red */
	if (rgbLED[0] >= 40 && color == 0) Rfactor = -2;
	if (rgbLED[0] == 2 && Rfactor == -2) 
	{
		color++;
		prepareNextColorChange();
	}
	
	/* Orange */
	if (rgbLED[0] >= 45 && rgbLED[1] >= 15 && color == 1) Ofactor = -1;
	if (rgbLED[1] == 2 && Ofactor == -1) 
	{
		color++;
		prepareNextColorChange();
	}
	
	/* Green */
	if (rgbLED[1] >= 40 && color == 2) Gfactor = -2;
	if (rgbLED[1] == 2 && Gfactor == -2)
	{
		color++;
		prepareNextColorChange();
	}
	
	/* Yellow */
	if (rgbLED[0] >= 40 && rgbLED[1] >= 40 && color == 3) Yfactor = -2;
	if (rgbLED[1] == 2 && Yfactor == -2) 
	{
		color++;
		prepareNextColorChange();
	}
	
	/* Purple */
	if (rgbLED[0] >= 40 && rgbLED[2] >= 40 && color == 4) Pfactor = -2;
	if (rgbLED[2] == 2 && Pfactor == -2) 
	{
		color++;
		prepareNextColorChange();
	}
	

	switch(color) {
		case 0:
			rgbLED[0] +=   Rfactor;
			break;
		case 1:
			rgbLED[0] += 3*Ofactor;
			rgbLED[1] +=   Ofactor;
			break;
		case 2:
			rgbLED[1] +=   Gfactor;
			break;
		case 3:
			rgbLED[0] +=   Yfactor;
			rgbLED[1] +=   Yfactor;
			break;
		case 4:
			rgbLED[0] +=   Pfactor;
			rgbLED[2] +=   Pfactor;
			break;
		default:
			color = 0;
			break;
	
	}
	
	flag_delay_smoothColorChange = 0;
}


void prepareNextColorChange(void)
{
	switch(color) {
		case 0:
			rgbLED[0] =  2;
			rgbLED[1] =  0;
			rgbLED[2] =  0;
			Rfactor 	= +2;
			break;
		case 1:
			rgbLED[0] =  6;
			rgbLED[1] =  2;
			rgbLED[2] =  0;
			Ofactor 	= +1;
			break;
		case 2:
			rgbLED[0] =  0;			
			rgbLED[1] =  2;
			rgbLED[2] =  0;
			Gfactor 	= +2;
			break;
		case 3:
			rgbLED[0] =  2;
			rgbLED[1] =  2;
			rgbLED[2] =  0;
			Yfactor 	= +2;
			break;
		case 4:
			rgbLED[0] =  2;
			rgbLED[1] =  0;
			rgbLED[2] =  2;
			Pfactor 	= +2;
			break;
		default:
			color = 0;
			break;
	
	}
}




/*	Touch functions	*/
void changeToNextEvent(void)
{
	// Go to next event
	operation = (operation==7) ? 0 : operation+1;
	// Reset Mode running
	operationRunning = 0;
}
void changeBrightness(uint32_t ChangeBrightnessDir)
{
	// HIGHER_BRIGHTNESS	1
	if(ChangeBrightnessDir)
	{
		slideColorIndex = (slideColorIndex<slideColorIndex_MAX) ? slideColorIndex+1 : slideColorIndex;
	}
	// LOWER_BRIGHTNESS	0
	else if(!ChangeBrightnessDir)
	{
		slideColorIndex = (slideColorIndex>slideColorIndex_MIN) ? slideColorIndex-1 : slideColorIndex;
	}
}

void loadBrightnessData(uint32_t brightness)
{
	// Reset 10 min timer to save value
	minTick = 0;
	// If value is in range, set it
	if (brightness <= slideColorIndex_MAX && brightness >= slideColorIndex_MIN)
	{
		slideColorIndex = brightness;
	}
	// If not in range, set default value
	else
	{
		slideColorIndex = 2;
	}	
}
/*****************************************************/
/*	LED_Modes	START																	 */
/*****************************************************/

/*	LED_Mode_Star	*/
void LED_Mode_Star(void)
{
		if 			(LED_Mode_Star_BrightnessAdjust <= 1  && LED_Mode_Star_BrightnessAdjustStep == -1)		LED_Mode_Star_BrightnessAdjustStep = +1;
		else if (LED_Mode_Star_BrightnessAdjust >= 10 && LED_Mode_Star_BrightnessAdjustStep == +1)		LED_Mode_Star_BrightnessAdjustStep = -1;
		/* Operation done, reset all parameters */
		if ( iter_LED_Mode_Star == Iter_LED_Mode_Star )
		{
			LED_Mode_Star_BrightnessAdjust 		 = 10;
			LED_Mode_Star_BrightnessAdjustStep = -1;
			iter_LED_Mode_Star = 0;
			operationRunning = 0;		
			operation++;
		}
		/*	Corner LEDs	*/
		// Blackout all LED bytes
		calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], 0);
		// Calculate bytes for the certain LEDs set to HIGH
		for (uint32_t iterLED = 0; iterLED < 5; iterLED++)
		{
			calcOneLED(LED_Mode_Star_CornerColor[1]/LED_Mode_Star_BrightnessAdjust, LED_Mode_Star_CornerColor[0]/LED_Mode_Star_BrightnessAdjust, LED_Mode_Star_CornerColor[2]/LED_Mode_Star_BrightnessAdjust, LED_Mode_Star_Corner[iterLED]);
		}
		/* Circle LEDs	*/
		for (uint32_t iterLED = 0; iterLED < 5; iterLED++)
		{
			calcOneLED(LED_Mode_Star_CircleColor[1][1]/LED_Mode_Star_BrightnessAdjust, LED_Mode_Star_CircleColor[1][0]/LED_Mode_Star_BrightnessAdjust, LED_Mode_Star_CircleColor[1][2]/LED_Mode_Star_BrightnessAdjust, LED_Mode_Star_Circle[iterLED]);
		}
		// Send bytes for LEDs
		shiftForLED();
		
		if (!(iter_LED_Mode_Star%5)) LED_Mode_Star_BrightnessAdjust += LED_Mode_Star_BrightnessAdjustStep;
		iter_LED_Mode_Star++;
}
/*	LED_Mode_Rainbow	*/
void LED_Mode_Rainbow(void)
{	
	calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], 0);
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=1; i < NUM_LEDs+1; i++) 
		{
      Wheel((i*20+j) & 255, i, 10);
    }
		shiftForLED();
		HAL_Delay(10);
  }
}


/*	LED_Mode_RainbowRun	*/
void LED_Mode_RainbowRun(void)
{	
	/* Operation done, reset all parameters */
	if ( j > 1000 )
	{
		j = 0;
		delay_LED_Mode_RainbowRun = 15;
		delay_LED_Mode_RainbowRun_Step 			= +1;
		operationRunning = 0;
	}
	/* Set delay change to positive	*/
	if (delay_LED_Mode_RainbowRun <= 5)
	{
		delay_LED_Mode_RainbowRun_Step 			= +1;
	}
	/* Set delay change to negative	*/
	else if (delay_LED_Mode_RainbowRun >= 25)
	{
		delay_LED_Mode_RainbowRun_Step 			= -1;
	}
	/* Change delay time for flag	*/
  if (!(j%(1000/delay_LED_Mode_RainbowRun)))
	{
		delay_LED_Mode_RainbowRun 			+= delay_LED_Mode_RainbowRun_Step;
	}
	/* Fade in and out: brightness */
	if (j < 50)
	{
		brightness_LED_Mode_RainbowRun = 60 - j;
	}
	else if (j > 950)
	{
		brightness_LED_Mode_RainbowRun = 60 + (j-1000);		
	}
	
	/* Start	*/
  uint16_t i;

	for(i=1; i < NUM_LEDs+1; i++) 
	{
		Wheel((i*20+j) & 255, i, brightness_LED_Mode_RainbowRun);
	}
	shiftForLED();
	//operationRunning = 0;
	j++;
}

/*	LED_Mode_RunOneColorOneLED	*/
void LED_Mode_RunOneColor(uint32_t flagColor, uint32_t numberLEDsRunning)
{		
	// Correction factor
	uint32_t correctionFactor = 0;
	if (numberLEDsRunning == 3) correctionFactor = 2;
	
	/*Init check if startup and color not set by now or run with default color*/
	if (flagColor == USE_DEFAULT_COLOR)
	{
		rgbLED[0] = runOneColor_rgbLED[0];
		rgbLED[1] = runOneColor_rgbLED[1];
		rgbLED[2] = runOneColor_rgbLED[2];
	}
	
	/* Operation done, reset all parameters */
	if ( runOneColor_iterLED == NUM_LEDs )
	{
		operationRunning = 0;		
		operation++;
	}
	/* Running operation */
	else
	{
		// Blackout all LED bytes
		calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], 0);
		// Calculate bytes for the certain LEDs set to HIGH
		for (signed int iterLED = 0+runOneColor_iterLED; iterLED > (signed) (runOneColor_iterLED-numberLEDsRunning); iterLED--)
		{
			//if (iterLED >= 0 && iterLED <= 5) calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], runOneColor_IndexCircle[iterLED]);
			calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], iterLED);
		}
		// Send bytes for LEDs
		shiftForLED();
		
		runOneColor_iterLED++;
	}
}

/*	LED_Mode_RunOneColorOneLED_Mirror	*/
void LED_Mode_RunOneColor_Mirror(uint32_t flagColor, uint32_t numberLEDsRunning)
{		
	// Correction factor
	uint32_t correctionFactor = 0;
	//if (numberLEDsRunning == 2) correctionFactor = 1;
	
	/*Init check if startup and color not set by now or run with default color*/
	if (flagColor == USE_DEFAULT_COLOR)
	{
		rgbLED[0] = runOneColor_rgbLED[0];
		rgbLED[1] = runOneColor_rgbLED[1];
		rgbLED[2] = runOneColor_rgbLED[2];
	}
	
	/* Operation done, reset all parameters */
	if ( runOneColor_iterLED == 5+1+correctionFactor )
	{
		operationRunning = 0;		
		operation++;
	}
	/* Running operation */
	else
	{
		// Blackout all LED bytes
		calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], 0);
		// Calculate bytes for the certain LEDs set to HIGH
		for (signed int iterLED = 0+runOneColor_iterLED; iterLED > (signed) (runOneColor_iterLED-numberLEDsRunning); iterLED--)
		{
			//if (iterLED >= 0 && iterLED <= 5) calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], runOneColor_Mirror_IndexCircle[iterLED]);
			calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], iterLED);
		}
		// Send bytes for LEDs
		shiftForLED();
		
		runOneColor_iterLED++;
	}
}

/* LED_Mode_ShiningLights */
void LED_Mode_ShiningLights(uint32_t NUMBER_LEDs)
{
	
	uint32_t smooth_rgbLEDs[3] = {0};
	
	/* Smooth brightness */
	smooth_rgbLEDs[0] = (rgbLED[0]*runTwoColors_iterShift)/4;
	smooth_rgbLEDs[1] = (rgbLED[1]*runTwoColors_iterShift)/4;
	smooth_rgbLEDs[2] = (rgbLED[2]*runTwoColors_iterShift)/4;
	
	/* Check if LEDs have any iterations left */
	if ( shiningLightsOnTree_iter )
	{
		/* Create random positions */
		if (!runTwoColors_iterShift)
		{
			for (int i=0; i<NUMBER_LEDs; i++)
			{
				positionLEDs[i] = rand_lim(NUM_LEDs-1)+1;
			}
		}
		
		// Blackout all LED bytes
		calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], 0);
		// Set all LEDs to orange
//		for (uint32_t iterLED = 0; iterLED < NUM_LEDs; iterLED++)
//		{
//			calcOneLED(10, 30, 0, iterLED+1);
//		}
		// Calculate bytes for the certain LEDs set to HIGH
		for (uint32_t iterLED = 0; iterLED < NUMBER_LEDs; iterLED++)
		{
			calcOneLED(smooth_rgbLEDs[1], smooth_rgbLEDs[0], smooth_rgbLEDs[2], positionLEDs[iterLED]);
		}
		// Send bytes for LEDs
		shiftForLED();
		
		/* Smooth brightness */
		runTwoColors_iterShift += runTwoColors_iterShiftAdd;
		if(runTwoColors_iterShift == 4)
		{
			runTwoColors_iterShiftAdd = -1;
		}
		else if (runTwoColors_iterShift == 0 && runTwoColors_iterShiftAdd == -1)
		{
			runTwoColors_iterShiftAdd = 1;		
			shiningLightsOnTree_iter--;
		}
	}
	/* Operation done, reset all parameters */
	else 
	{
		operationRunning = 0;		
		operation++;
	}
}

/* LED_Mode_RunTwoColors */
void LED_Mode_RunTwoColors(void)
{
	uint32_t twoColor_rgbLEDs[4][3] = { {12, 0, 0}, {0, 12, 0}, {12, 12, 0}, {12, 8, 0} };
	
	/* Smooth brightness */
	twoColor_rgbLEDs[0][0] = (twoColor_rgbLEDs[0][0]*runTwoColors_iterShift)/4;
	twoColor_rgbLEDs[1][1] = (twoColor_rgbLEDs[1][1]*runTwoColors_iterShift)/4;
	twoColor_rgbLEDs[2][0] = (twoColor_rgbLEDs[2][0]*runTwoColors_iterShift)/4;
	twoColor_rgbLEDs[2][1] = (twoColor_rgbLEDs[2][1]*runTwoColors_iterShift)/4;
	twoColor_rgbLEDs[3][0] = (twoColor_rgbLEDs[2][0]*runTwoColors_iterShift)/4;
	twoColor_rgbLEDs[3][2] = (twoColor_rgbLEDs[2][1]*runTwoColors_iterShift)/4;
	
	/* Check if LEDs have any iterations left */
	if ( runTwoColors_iter )
	{		
		// Blackout all LED bytes
		calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], 0);
		
		// Calculate bytes for the certain LEDs set to HIGH
		for (uint32_t iterLED = 0; iterLED < NUM_LEDs; iterLED+=2)
		{
			calcOneLED(twoColor_rgbLEDs[!(runTwoColors_iter%2)][1], twoColor_rgbLEDs[!(runTwoColors_iter%2)][0], 	twoColor_rgbLEDs[!(runTwoColors_iter%2)][2], 	iterLED+1);
		}
		for (uint32_t iterLED = 1; iterLED < NUM_LEDs; iterLED+=2)
		{
			calcOneLED(twoColor_rgbLEDs[runTwoColors_iter%2][1], 		twoColor_rgbLEDs[runTwoColors_iter%2][0], 		twoColor_rgbLEDs[runTwoColors_iter%2][2], 		iterLED+1);
		}
		
		// Send bytes for LEDs
		shiftForLED();
		
		/* Smooth brightness */
		runTwoColors_iterShift += runTwoColors_iterShiftAdd;
		if(runTwoColors_iterShift == 4)
		{
			runTwoColors_iterShiftAdd = -1;
		}
		else if (runTwoColors_iterShift == 0 && runTwoColors_iterShiftAdd == -1)
		{
			runTwoColors_iterShiftAdd = 1;
			runTwoColors_iter--;
		}
	}
	/* Operation done, reset all parameters */
	else 
	{
		operationRunning = 0;		
		operation++;
	}
}


/* LED_Mode_RunLED_Wave */
void LED_Mode_RunLED_Wave(uint32_t flagReverse)
{
	
	/* Calculate Matrix */
	calculateLED_WaveBrightnessMatrix(&runLED_Wave_iterLED, waveBrightnessMatrix);
	
	
	// Blackout all LED bytes
	calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], 0);
	
	/* Calculate all LED rgb-values */
	for ( uint32_t i = 0; i < NUM_LEDs; i++)
	{		
		int iter = i;
		/* Run from two sides */
		if (flagReverse == 2)
		{
			/* Wave left side */
			runLED_Wave_rgbLED[0] = (rgbLED[0] * waveBrightnessMatrix[iter])/6;
			runLED_Wave_rgbLED[1] = (rgbLED[1] * waveBrightnessMatrix[iter])/6;
			runLED_Wave_rgbLED[2] = (rgbLED[2] * waveBrightnessMatrix[iter])/6;
			/* Wave right side */
			iter = runLED_Wave_reverseLUT[i];		
			runLED_Wave_rgbLED[0] += (rgbLED[0] * waveBrightnessMatrix[iter])/6;
			runLED_Wave_rgbLED[1] += (rgbLED[1] * waveBrightnessMatrix[iter])/6;
			runLED_Wave_rgbLED[2] += (rgbLED[2] * waveBrightnessMatrix[iter])/6;
			/* Norm brightness */
			runLED_Wave_rgbLED[0] = runLED_Wave_rgbLED[0]/2;
			runLED_Wave_rgbLED[1] = runLED_Wave_rgbLED[1]/2;
			runLED_Wave_rgbLED[2] = runLED_Wave_rgbLED[2]/2;
		}
		/* Run from only one side */
		else
		{
		if (flagReverse) iter = runLED_Wave_reverseLUT[i];
		
		runLED_Wave_rgbLED[0] = (rgbLED[0] * waveBrightnessMatrix[iter])/6;
		runLED_Wave_rgbLED[1] = (rgbLED[1] * waveBrightnessMatrix[iter])/6;
		runLED_Wave_rgbLED[2] = (rgbLED[2] * waveBrightnessMatrix[iter])/6;
		}
		
		calcOneLED(runLED_Wave_rgbLED[1], runLED_Wave_rgbLED[0], runLED_Wave_rgbLED[2], i+1);
	}
	// Send bytes for LEDs
	shiftForLED();
	
	
	/* Operation done, reset all parameters */
	if ( runLED_Wave_iterLED > NUM_LEDs )
	{
		runLED_Wave_iterLED = 0;
		operationRunning = 0;		
		operation++;
	}
}

/*****************************************************/
/*	LED_Modes	ENDE																	*/
/*****************************************************/


/*
* Function: Test_RGB_LEDs
* -----------------------
* Lights all LEDs for one second (red, green, blue)
*
* returns:							None
*/
void Test_RGB_LEDs(void)
{
	if (1)
	{
		/* Red test case */
		rgbLED[0] = 40;
		rgbLED[1] = 0;
		rgbLED[2] = 0;
		litAllLEDs();
		HAL_Delay(1000);
		/* Green test case */
		rgbLED[0] = 0;
		rgbLED[1] = 40;
		rgbLED[2] = 0;
		litAllLEDs();
		HAL_Delay(1000);
		/* Blue test case */
		rgbLED[0] = 0;
		rgbLED[1] = 0;
		rgbLED[2] = 40;
		litAllLEDs();
		HAL_Delay(1000);
		
		rgbLED[0] = 40;
		rgbLED[1] = 0;
		rgbLED[2] = 0;
	}
}


/*
* Function: litAllLEDs
* --------------------
* Lights all LEDs with the set color in rgbLED
*
* returns:							None
*/
void litAllLEDs(void)
{

	for (uint32_t iterLED = 0; iterLED < NUM_LEDs; iterLED++)
	{
		calcOneLED(rgbLED[1], rgbLED[0], rgbLED[2], iterLED+1);
	}
	// Send bytes for LEDs
	shiftForLED();
}

/*
* Function: Wheel
* ---------------
* Input a value 0 to 255 to get a color value.
* The colours are a transition r - g - b - back to r.
*
* returns:							None
*/
void Wheel(uint32_t WheelPos, uint16_t i, uint32_t divide) 
{
  if(WheelPos < 85) {
		calcOneLED( (WheelPos*3)/divide , (255-WheelPos*3)/divide , 0, i);
    return;
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
		calcOneLED( (255-WheelPos*3)/divide, 0, (WheelPos*3)/divide, i);
    return;
  }
  WheelPos -= 170;
	calcOneLED(0, (WheelPos*3)/divide, (255-WheelPos*3)/divide, i);
  return;
}


/*
* Function: rand_lim
* ------------------
* Returns random number within set limits
* 
* int limit:						Max. limit for random number
*
* returns:							Random number as int
*/
int rand_lim(int limit) 
{
/* return a random number between 0 and limit inclusive.
 */

    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}


/*
* Function: calculateLED_WaveBrightnessMatrix
* -------------------------------------------
* Fills WaveBrightnessMatrix with values (resemble wave moving along the LEDs)
* 
* uint32_t *ptrIterLED:						Pointer to the current LED index
* int *ptrWaveBrightnessMatrix:		Pointer to the WaveBrightnessMatrix
*
* returns:								None
*/
void calculateLED_WaveBrightnessMatrix(uint32_t *ptrIterLED, int *ptrWaveBrightnessMatrix)
{
	/* Calculate Matrix */

	/* Increase LED brightness */		
	(*(ptrWaveBrightnessMatrix+*ptrIterLED))++;

	/* If LED is higher than 3 increase/decrease the neighbor brightness */
	if (*(ptrWaveBrightnessMatrix+*ptrIterLED) > 3) (*(ptrWaveBrightnessMatrix+*ptrIterLED+1))++;
	if (*(ptrWaveBrightnessMatrix+*ptrIterLED-1)) (*(ptrWaveBrightnessMatrix+*ptrIterLED-1))--;
	if (*(ptrWaveBrightnessMatrix+*ptrIterLED-2)) (*(ptrWaveBrightnessMatrix+*ptrIterLED-2))--;
	

	/* Move to next LED if brightness is equal to 6 */
	if (*(ptrWaveBrightnessMatrix+*ptrIterLED) > 5)
	{
		(*ptrIterLED)++;
	}	
}


/*
* Function: shift_SW_LED
* ----------------------
* Lights up the number of LEDs with defined color in the rgbLED array
* 
* returns:								None
*/
void shift_SW_LED(uint32_t lightLEDs)
{
	calcLED(rgbLED[1], rgbLED[0], rgbLED[2], lightLEDs);
	shiftForLED();
}
/*
* Function: calcOneLED
* --------------------
* Calculates the values for the bit array for one certain LED
* 
* uint32_t intByteGreen:	Color intensity as one byte (dark: 0, max: 255)
* uint32_t intByteRed:		Color intensity as one byte (dark: 0, max: 255)
* uint32_t intByteBlue:		Color intensity as one byte (dark: 0, max: 255)
* uint32_t lightLED:			LED index, if set to 0 all entries in the bit array get set to 0
*
* returns:								None
*/
void calcOneLED(uint32_t intByteGreen, uint32_t intByteRed, uint32_t intByteBlue, uint32_t lightLED)
{
		
	/* Adjust brightness */
	intByteGreen 	= (intByteGreen	*	slideColorIndex)/4;
	intByteRed 		= (intByteRed		*	slideColorIndex)/4;
	intByteBlue 	= (intByteBlue	*	slideColorIndex)/4;
			
	uint32_t mask;
	uint32_t masked_n;
	
	for (uint32_t color=0; color<3; color++) 
	{
		// Green
		if (color==0) {
			for (uint32_t k=0; k<8; k++) 
			{
				mask =  1 << (7-k);
				masked_n = intByteGreen & mask;
				thebit[k] = masked_n >> (7-k);
			}
		}
		// Red
		else if (color==1) {
			for (uint32_t k=0; k<8; k++) 
			{
				mask =  1 << (7-k);
				masked_n = intByteRed & mask;
				thebit[k+8] = masked_n >> (7-k);
			}
		}
		// Blue
		else if (color==2) {
			for (uint32_t k=0; k<8; k++) 
			{
				mask =  1 << (7-k);
				masked_n = intByteBlue & mask;
				thebit[k+16] = masked_n >> (7-k);
			}
		}
		/* Color data */
		for (uint32_t jter = 0; jter < 24; jter++)
		{
			thebitArray[jter+24*(lightLED-1)] = thebit[jter];
		}
		/* Black data */
		if (!lightLED)
		{
			for (uint32_t kter = 0; kter < NUM_LEDs; kter++)
			{
				for (uint32_t lter = 0; lter < 24; lter++) 
				{
					thebitArray[lter+24*(kter+lightLED)] = 0;
				}
			}
		}		
	}
}
/*
* Function: calcLED
* -----------------
* Calculates the values for the bit array for the first LEDs, sets the rest to 0
* 
* uint32_t intByteGreen:	Color intensity as one byte (dark: 0, max: 255)
* uint32_t intByteRed:		Color intensity as one byte (dark: 0, max: 255)
* uint32_t intByteBlue:		Color intensity as one byte (dark: 0, max: 255)
* uint32_t lightLED:			LED number
*
* returns:								None
*/
void calcLED(uint32_t intByteGreen, uint32_t intByteRed, uint32_t intByteBlue, uint32_t lightLED)
{
	
	uint32_t mask;
	uint32_t masked_n;
	
	for (uint32_t color=0; color<3; color++) 
	{
		// Green
		if (color==0) {
			for (uint32_t k=0; k<8; k++) 
			{
				mask =  1 << (7-k);
				masked_n = intByteGreen & mask;
				thebit[k] = masked_n >> (7-k);
			}
		}
		// Red
		else if (color==1) {
			for (uint32_t k=0; k<8; k++) 
			{
				mask =  1 << (7-k);
				masked_n = intByteRed & mask;
				thebit[k+8] = masked_n >> (7-k);
			}
		}
		// Blue
		else if (color==2) {
			for (uint32_t k=0; k<8; k++) 
			{
				mask =  1 << (7-k);
				masked_n = intByteBlue & mask;
				thebit[k+16] = masked_n >> (7-k);
			}
		}
		// Colored
		for (uint32_t iter = 0; iter < lightLED; iter++)
		{
			for (uint32_t jter = 0; jter < 24; jter++)
			{
				thebitArray[jter+24*iter] = thebit[jter];
			}
		}
		// Blacked
		for (uint32_t kter = 0; kter < NUM_LEDs-lightLED; kter++)
		{
			for (uint32_t lter = 0; lter < 24; lter++) 
			{
				thebitArray[lter+24*(kter+lightLED)] = 0;
			}
		}		
	}
}


/*
* Function: Insert_PCB_Specific_Pattern_Into_BitArray
* ---------------------
* According to set pattern for a set LED the values in the bit array gets updated before shifted out
* 
* returns:								None
*/
void Insert_PCB_Specific_Pattern_Into_BitArray(void)
{	
	uint32_t mask;
	uint32_t masked_n;
	/* Change values in the bit array */
	for (uint32_t ind = 0; ind < COLORBITS_PER_LED; ind++)
	{
		// Blue - set to 0
		thebitArray[ind + (REINDEER_NOSE_LED_INDEX-1)*BITS_PER_LED] 							        = 0;
		// Red  - set to red_value
		mask =  1 << (7-ind);
		masked_n = red_value & mask;
		thebitArray[ind + (REINDEER_NOSE_LED_INDEX-1)*BITS_PER_LED + COLORBITS_PER_LED]   = masked_n >> (7-ind);
		// Green - set to 0 
		thebitArray[ind + (REINDEER_NOSE_LED_INDEX-1)*BITS_PER_LED + COLORBITS_PER_LED*2] = 0;
	}
}
/*
* Function: shiftForLED
* ---------------------
* Pushes the values of the bit array via the defined GPIO out to the LEDs
* 
* returns:								None
*/
void shiftForLED(void)
{
	Insert_PCB_Specific_Pattern_Into_BitArray();
	//__attribute__((optimize(0)));
	__disable_irq();
	#pragma inline
	for (uint32_t run = 0; run < BITS_PER_LED*NUM_LEDs; run++)
	{
		if(thebitArray[run] == 0)
		{
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		
		GPIOF->BRR = GPIO_PIN_1;
		GPIOF->BRR = GPIO_PIN_1;
		GPIOF->BRR = GPIO_PIN_1;
		}
		else
		{
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		GPIOF->BSRR = GPIO_PIN_1;
		
		GPIOF->BRR = GPIO_PIN_1;
		}
	}
	__enable_irq();
	#pragma no_inline
}


