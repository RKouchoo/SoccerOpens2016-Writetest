/*
 * Opens soccer smaller robot main code set,
 * Team circut breaker Dev @RKouchoo
 *
 * First created:  25 / 8 / 2015
 * Latest Update:  25 / 8 / 2016
 *
 * If you are wondering how this code works, send a message to r.kouchoo1@gmail.com
 *
 * anything with a " // " next to it means it has been modified or its a comment!
 *
 *
 *        ===============================================
 *        PLEASE READ THE DOCUMENTATION FOR MORE INFO!!!!
 *        ===============================================
 *
 *
 */

                            //IMPORTS
//=======================================================================

#include <Wire.h>
#include <Arduino.h>

                          //ROBOT MAP
//=======================================================================

static int Motor1A = 23;    //need to change these as they are not right
static int Motor1B = 24;

static int Motor2A = 25;
static int Motor2B = 26;

static int Motor3A = 27;
static int Motor3B = 28;

static int Motor4A = 29;
static int Motor4B = 30;

const int Motor_EN_S_1_1 = 2;   //motor speed pwm, possibly change?
const int Motor_EN_S_1_2 = 3;
const int Motor_EN_S_2_1 = 4;
const int Motor_EN_S_2_2 = 5;


//                          SEEKER
//==================================================================

/*
  IRSeeker.ino - A class for the HiTechnic IRSeeker V2 infrared sensor.
  Created by B. Blechschmidt, August 1, 2013.
  Edited by R. Kouchoo - STRICT USE for SPX.
  Clean version was released into the public domain.
*/

struct InfraredResult
{
  byte Direction;
  byte Strength;
};

class InfraredSeeker
{
  public:
    static void Initialize();
    static boolean Test();
    static void ReadACRaw(byte* buffer);
    static void ReadDCRaw(byte* buffer);
    static InfraredResult ReadAC();
    static InfraredResult ReadDC();
    static int DirectionAngle(byte Direction);
  private:
    static InfraredResult PopulateValues(byte* buffer);
    static void ReadValues(byte OffsetAddress, byte* buffer);
    static const int Address = 0x10 / 2; //Divide by two as 8bit-I2C address is provided
};

void InfraredSeeker::Initialize()
{
  Wire.begin();
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(0x00);
  Wire.endTransmission();
  while(Wire.available() > 0)
    Wire.read();
}

boolean InfraredSeeker::Test()
{
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.requestFrom(InfraredSeeker::Address, 16);
  char Manufacturer_Model[16];
  while(Wire.available() < 16);
  for(byte i=0; i < 16; i++)
  {
    Manufacturer_Model[i] = Wire.read();
  }
  while(Wire.available() > 0)
    Wire.read();
  return strncmp(Manufacturer_Model, "HiTechncNewIRDir", 16)==0;
}

void InfraredSeeker::ReadValues(byte OffsetAddress, byte* buffer)
{
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(OffsetAddress);
  Wire.endTransmission();
  Wire.requestFrom(InfraredSeeker::Address, 6);
  while(Wire.available() < 6);
  for(byte i = 0; i < 6; i++)
  {
    buffer[i] = Wire.read();
  }
  while(Wire.available() > 0)
    Wire.read();
}

void InfraredSeeker::ReadACRaw(byte* buffer)
{
  ReadValues(0x49, buffer);
}

void InfraredSeeker::ReadDCRaw(byte* buffer)
{
  ReadValues(0x42, buffer);
}

InfraredResult InfraredSeeker::PopulateValues(byte* buffer)
{
  InfraredResult Data;
  Data.Direction = buffer[0];
  if(buffer[0] != 0)
  {
    if(buffer[0] % 2 == 0)
    {
      Data.Strength = (buffer[buffer[0] / 2] + buffer[buffer[0] / 2 + 1]) / 2;
    }
    else
    {
      Data.Strength = buffer[buffer[0] / 2 + 1];
    }
  }
  else
  {
    Data.Strength = 0;
  }
  return Data;
}

InfraredResult InfraredSeeker::ReadAC()
{
  byte buffer[6];
  ReadACRaw(buffer);
  return PopulateValues(buffer);
}

InfraredResult InfraredSeeker::ReadDC()
{
  byte buffer[6];
  ReadDCRaw(buffer);
  return PopulateValues(buffer);
}

int DirectionAngle(byte Direction)
{
  return Direction * 30 - 150;
}
                            //Speed Control
//============================================================================================



void SetMotorSpeedLOW() {
  analogWrite(Motor_EN_S_1_1, MotorLOW_POWER);
  analogWrite(Motor_EN_S_1_2, MotorLOW_POWER);
  analogWrite(Motor_EN_S_2_1, MotorLOW_POWER);
  analogWrite(Motor_EN_S_2_2, MotorLOW_POWER);

  delay(50);

  analogWrite(Motor_EN_S_1_1, LOW);
  analogWrite(Motor_EN_S_1_2, LOW);
  analogWrite(Motor_EN_S_2_1, LOW);
  analogWrite(Motor_EN_S_2_2, LOW);
}

void SetMotorSpeedMED() {
  analogWrite(Motor_EN_S_1_1, MotorMED_POWER);
  analogWrite(Motor_EN_S_1_2, MotorMED_POWER);
  analogWrite(Motor_EN_S_2_1, MotorMED_POWER);
  analogWrite(Motor_EN_S_2_2, MotorMED_POWER);

  delay(50);

  analogWrite(Motor_EN_S_1_1, LOW);
  analogWrite(Motor_EN_S_1_2, LOW);
  analogWrite(Motor_EN_S_2_1, LOW);
  analogWrite(Motor_EN_S_2_2, LOW);
}

void SetMotorSpeedHIGH() {
  analogWrite(Motor_EN_S_1_1, MotorFULL_POWER);
  analogWrite(Motor_EN_S_1_2, MotorFULL_POWER);
  analogWrite(Motor_EN_S_2_1, MotorFULL_POWER);
  analogWrite(Motor_EN_S_2_2, MotorFULL_POWER);

  delay(50);

  analogWrite(Motor_EN_S_1_1, LOW);
  analogWrite(Motor_EN_S_1_2, LOW);
  analogWrite(Motor_EN_S_2_1, LOW);
  analogWrite(Motor_EN_S_2_2, LOW);
}

void SetMotorSpeedOFF(){
  analogWrite(Motor_EN_S_1_1, LOW);
  analogWrite(Motor_EN_S_1_2, LOW);
  analogWrite(Motor_EN_S_2_1, LOW);
  analogWrite(Motor_EN_S_2_2, LOW);
}

void setup() {


}

void loop() {


}
