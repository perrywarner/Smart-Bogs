/*
  This is a simple code to test BH1750FVI Light senosr
  communicate using I2C Protocol
  this library enable 2 slave device address
  Main address  0x23
  secondary address 0x5C
  connect this sensor as following :
  VCC >>> 3.3V
  SDA >>> A4
  SCL >>> A5
  addr >> A3
  Gnd >>>Gnd

  Written By : Mohannad Rawashdeh
  Stolen from: http://www.instructables.com/id/BH1750-Digital-Light-Sensor/
 
*/

#include <BH1750FVI.h> // Sensor Library
#include <Wire.h> // I2C Library
#include <SD.h>

BH1750FVI LightSensor;


// Begin SD Card
File myFile;
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.

// End SD Card


void setup() {   // put your setup code here, to run once:
   Serial.begin(9600);
  LightSensor.begin();
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
 

  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile)
  {
    Serial.print(F("File opened for writing."));
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println(F("error opening test.txt"));
    return;
  }

  Serial.print("Read sensor: ");
  Serial.print("Light: ");
  myFile.println("Light: ");
  Serial.print(lux);
  myFile.print(lux);
  Serial.print(" lux");
  myFile.print(" lux");
  myFile.println();
  myFile.println();

  delay(1000);

  myFile.close();
}

