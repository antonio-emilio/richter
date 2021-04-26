#include "MICS6814.h"

MICS6814 :: MICS6814 (int pinCO, int pinNO2, int pinNH3)
{
	_pinCO = pinCO;
	_pinNO2 = pinNO2;
	_pinNH3 = pinNH3;
}


void MICS6814 :: calibrate ()
{
	// The number of seconds that must pass before
	// than we will assume that the calibration is complete
	// (Less than 64 seconds to avoid overflow)
	uint8_t seconds = 10;

	// Tolerance for the average of the current value
	uint8_t delta = 2;

	// Measurement buffers
	uint16_t bufferNH3 [seconds];
	uint16_t bufferCO [seconds];
	uint16_t bufferNO2 [seconds];

	// Pointers for the next item in the buffer
	uint8_t pntrNH3 = 0;
	uint8_t pntrCO = 0;
	uint8_t pntrNO2 = 0;

	// The current floating amount in the buffer
	uint16_t fltSumNH3 = 0;
	uint16_t fltSumCO = 0;
	uint16_t fltSumNO2 = 0;

	// Current measurement
	uint16_t curNH3;
	uint16_t curCO;
	uint16_t curNO2;

	// Flag of stability of indications
	bool isStableNH3 = false;
	bool isStableCO = false;
	bool isStableNO2 = false;

	// We kill the buffer with zeros
	for (int i = 0; i <seconds; ++ i)
	{
		bufferNH3 [i] = 0;
		bufferCO [i] = 0;
		bufferNO2 [i] = 0;
	}

	// Calibrate
	do
	{
		delay (1000);

		unsigned long rs = 0;

		delay (50);
		for (int i = 0; i <3; i ++)
		{
			delay (1);
			rs += analogRead (_pinNH3);
		}

		curNH3 = rs / 3;
		rs = 0;

		delay (50);
		for (int i = 0; i <3; i ++)
		{
			delay (1);
			rs += analogRead (_pinCO);
		}

		curCO = rs / 3;
		rs = 0;

		delay (50);
		for (int i = 0; i <3; i ++)
		{
			delay (1);
			rs += analogRead (_pinNO2);
		}

		curNO2 = rs / 3;

		// Update the floating amount by subtracting the value, 
		// to be overwritten, and adding a new value.
		fltSumNH3 = fltSumNH3 + curNH3 - bufferNH3 [pntrNH3];
		fltSumCO = fltSumCO + curCO - bufferCO [pntrCO];
		fltSumNO2 = fltSumNO2 + curNO2 - bufferNO2 [pntrNO2];

		// Store d buffer new values
		bufferNH3 [pntrNH3] = curNH3;
		bufferCO [pntrCO] = curCO;
		bufferNO2 [pntrNO2] = curNO2; 

		// Define flag states
		isStableNH3 = abs (fltSumNH3 / seconds - curNH3) <delta;
		isStableCO = abs (fltSumCO / seconds - curCO) <delta;
		isStableNO2 = abs (fltSumNO2 / seconds - curNO2) <delta;

		// Pointer to a buffer
		pntrNH3 = (pntrNH3 + 1)% seconds;
		pntrCO = (pntrCO + 1)% seconds;
		pntrNO2 = (pntrNO2 + 1)% seconds;
	} while (! isStableNH3 ||! isStableCO ||! isStableNO2);

	_baseNH3 = fltSumNH3 / seconds;
	_baseCO = fltSumCO / seconds;
	_baseNO2 = fltSumNO2 / seconds;
}

void MICS6814 :: loadCalibrationData (
	uint16_t baseNH3,
	uint16_t baseCO,
	uint16_t baseNO2)
{
	_baseNH3 = baseNH3;
	_baseCO = baseCO;
	_baseNO2 = baseNO2;
}

float MICS6814 :: measure (gas_t gas)
{
	float ratio;
	float c = 0;

	switch (gas)
	{
	case CO:
		ratio = getCurrentRatio (CH_CO);
		c = pow (ratio, -1.179) * 4.385;
		break;
	case NO2:
		ratio = getCurrentRatio (CH_NO2);
		c = pow (ratio, 1.007) / 6.855;
		break;
	case NH3:
		ratio = getCurrentRatio (CH_NH3);
		c = pow (ratio, -1.67) / 1.47;
		break;
	}

	return isnan (c)? -1: c;
}

uint16_t MICS6814 :: getResistance (channel_t channel) const
{
	unsigned long rs = 0;
	int counter = 0;

	switch (channel)
	{
	case CH_CO:
		for (int i = 0; i <300; i ++)
		{
			rs += analogRead (_pinCO)/3;
			counter ++;
			delayMicroseconds(300);
		}
	case CH_NO2:
		for (int i = 0; i <300; i ++)
		{
			rs += analogRead (_pinNO2)/3;
			counter ++;
			delayMicroseconds(300);
		}
 	case CH_NH3:
		for (int i = 0; i <300; i ++)
		{
			rs += analogRead (_pinNH3)/3;
			counter ++;
			delayMicroseconds(300);
      
		}
    
	}

	return counter!= 0? rs / counter: 0;
}

uint16_t MICS6814 :: getBaseResistance (channel_t channel) const
{
	switch (channel)
	{
	case CH_NH3:
		return _baseNH3;
	case CH_CO:
		return _baseCO;
	case CH_NO2:
		return _baseNO2;
	}

	return 0;
}


float MICS6814 :: getCurrentRatio (channel_t channel) const
{
	float baseResistance = (float) getBaseResistance (channel);
	float resistance = (float) getResistance (channel);

	return resistance / baseResistance * (1023.0 - baseResistance) / (1023.0 - resistance);

	return -1.0;
}