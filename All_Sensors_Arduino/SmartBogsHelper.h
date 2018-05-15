#pragma once

#include "SmartBogsHelper.h"

#ifndef SmartBogsHelper.h

#define SmartBogsHelper.h

#include "Arduino.h"

class SmartBogsHelper {
public:

	double celcius;

	double humidity;

	double Fahrenheit(double celsius);

	double Kelvin(double celsius);

	double dewPoint(double celsius, double humidity);

	double dewPointFast(double celsius, double humidity);
};

#endif