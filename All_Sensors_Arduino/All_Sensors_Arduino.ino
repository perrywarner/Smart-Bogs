#include <Time.h>
#include <TimeLib.h> //https://github.com/PaulStoffregen/Time

//
//   FILE:  dht11_test1.pde
// PURPOSE: DHT11 library test sketch for Arduino
//
#include "dht11.h"
#include <SD.h>
#include "Wire.h"
#include <Time.h>
#include <BH1750FVI.h>
#include <DallasTemperature.h> // For Ds18b20, Stolen from https://create.arduino.cc/projecthub/TheGadgetBoy/ds18b20-digital-temperature-sensor-and-arduino-9cc806
#include <OneWire.h> // Also for Ds18b20 DATE: 1/19/18

BH1750FVI LightSensor;

// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2 

// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Begin SD Card
File myFile;
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.

// End SD Card

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}

// fast integer version with rounding
//int Celcius2Fahrenheit(int celcius)
//{
//  return (celsius * 18 + 5)/10 + 32;
//}


//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
  return celsius + 273.15;
}

// dewPoint function NOAA
// reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
// reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm
//
double dewPoint(double celsius, double humidity)
{
  // (1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);

  // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;

  // (2) DEWPOINT = F(Vapor Pressure)
  double T = log(VP / 0.61078); // temp var
  return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}

dht11 DHT11;

#define DHT11PIN 3

void setup() {   // put your setup code here, to run once:
  
  time_t t = now(); // Store the current time in time
  setTime(t);
  
  Serial.begin(9600);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
  LightSensor.begin();
  sensors.begin(); // refers to ds18b20 sensor only
  /*
  Set the address for this sensor
  you can use 2 different address
  Device_Address_H "0x5C"
  Device_Address_L "0x23"
  you must connect Addr pin to A3 .
  */
  LightSensor.SetAddress(Device_Address_H);//Address 0x5C
  // To adjust the slave on other address , uncomment this line
  // lightMeter.SetAddress(Device_Address_L); //Address 0x5C
  //-----------------------------------------------
  /*
   set the Working Mode for this sensor
   Select the following Mode:
    Continuous_H_resolution_Mode
    Continuous_H_resolution_Mode2
    Continuous_L_resolution_Mode
    OneTime_H_resolution_Mode
    OneTime_H_resolution_Mode2
    OneTime_L_resolution_Mode
   
    The data sheet recommanded To use Continuous_H_resolution_Mode
  */
  LightSensor.SetMode(Continuous_H_resolution_Mode);

  Serial.print(F("Initializing SD card..."));

  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);

  if (!SD.begin(10))
  {
    Serial.print("Init failed!");
    return;
  }

  Serial.println(F("initialization done."));
 
  Serial.println("Running...");
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("\n");  
  uint16_t lux = LightSensor.GetLightIntensity();// Get Lux value
  int chk = DHT11.read(DHT11PIN);
 

  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile)
  {
    //Serial.print(F("File opened for writing."));
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println(F("error opening test.txt"));
    return;
  }
  
  Serial.print("Light: ");
  myFile.println("Light: ");
  Serial.print(lux);
  myFile.print(lux);
  Serial.print(" lux");
  myFile.print(" lux");
  myFile.println();
  myFile.println();

  switch (chk)
  {
    case DHTLIB_OK:
      Serial.println();
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("Time out error");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }



  //Using DateTime library from https://github.com/PaulStoffregen/Time 
  //Need to import this external library to Arduino and use TimeLib.h
  
  time_t t = now(); // Store the current time in time
  // variable t
  hour(t); // Returns the hour for the given
  // time t
  minute(t); // Returns the minute for the given
  // time t
  second(t); // Returns the second for the given
  // time t
  day(t); // The day for the given time t
  month(t); // The month for the given time t

  year(t); // The year for the given time t
  
  String hours = String(hour(t));
  String minutes = String(minute(t));
  String seconds = String(second(t));
  String days = String(day(t));
  String months = String(month(t));
  String years = String(year(t));

 
  myFile.print("hour:");
  myFile.print(hours);
  myFile.print("  minute:");
  myFile.print(minutes);
  myFile.print("  second:");
  myFile.print(seconds);
  myFile.print("  day:");
  myFile.print(days);
  myFile.print("  month:");
  myFile.print(months);
  myFile.print("  year:");
  myFile.print(years);
  myFile.println();

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  myFile.println("\nHumidity (%): ");
  myFile.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (°C): ");
  Serial.println((float)DHT11.temperature, 2);
  myFile.println("Temperature (°C): ");
  myFile.println((float)DHT11.temperature, 2);

  Serial.print("Temperature (°F): ");
  Serial.println(Fahrenheit(DHT11.temperature), 2);
  myFile.println("Temperature (°F): ");
  myFile.println(Fahrenheit(DHT11.temperature), 2);

  Serial.print("Temperature (°K): ");
  Serial.println(Kelvin(DHT11.temperature), 2);
  myFile.println("Temperature (°K): ");
  myFile.println(Kelvin(DHT11.temperature), 2);

  Serial.print("Dew Point (°C): ");
  Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));
  myFile.println("Dew Point (°C): ");
  myFile.println(dewPoint(DHT11.temperature, DHT11.humidity));

  Serial.print("Dew PointFast (°C): ");
  Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));
  myFile.println("Dew PointFast (°C): ");
  myFile.println(dewPointFast(DHT11.temperature, DHT11.humidity));

  Serial.println();
  myFile.println();
  Serial.print("Ds18B20 water temperature reading:");
  myFile.print("Ds18B20 water temperature reading:");
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  Serial.print(sensors.getTempCByIndex(0));
  myFile.print(sensors.getTempCByIndex(0));

  myFile.println();
  myFile.println();
  myFile.println();
  myFile.println();

  delay(2000);

  myFile.close();
}
