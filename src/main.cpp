#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);
TFT_eSprite back1 = TFT_eSprite(&tft);
TFT_eSprite back2 = TFT_eSprite(&tft);

int ADXLAddress = 0x53; // Device address in which is also included the 8th bit for selecting the mode, read in this case.

#define X_Axis_Register_DATAX0 0x32 // Hexadecima address for the DATAX0 internal register.
#define Y_Axis_Register_DATAY0 0x34 // Hexadecima address for the DATAY0 internal register.
#define Z_Axis_Register_DATAZ0 0x36 // Hexadecima address for the DATAY0 internal register.
#define Power_Register 0x2D         // Power Control Register

#define XCAL 3
#define YCAL 3
#define ZCAL 3

#define Dxy 10

float getX();
float getY();
float getZ();
void dessineCible();

int xballe = 30, yballe = 30;
int couleurBalle;

void setup()
{
  tft.begin();
  tft.setRotation(3); // paysage
  tft.fillScreen(TFT_WHITE);

  back1.createSprite(160, 240);
  back2.createSprite(160, 240);

  Wire.begin(); // Initiate the Wire library
  Serial.begin(115200);
  delay(100);
  // Enable measurement
  Wire.beginTransmission(ADXLAddress);
  Wire.write(Power_Register);
  // Bit D3 High for measuring enable (0000 1000)
  Wire.write(8);
  Wire.endTransmission();
}

void loop()
{
  if (getX() > 0.1)
  {
    if (getY() > 0.1)
    {
      xballe -= Dxy * 0.7;
      yballe += Dxy * 0.7;
    }
    else if (getY() < -0.1)
    {
      xballe -= Dxy * 0.7;
      yballe -= Dxy * 0.7;
    }
    else
    {
      xballe -= Dxy;
    }
  }
  else if (getX() < -0.1)
  {
    if (getY() > 0.1)
    {
      xballe += Dxy * 0.7;
      yballe += Dxy * 0.7;
    }
    else if (getY() < -0.1)
    {
      xballe += Dxy * 0.7;
      yballe -= Dxy * 0.7;
    }
    else
    {
      xballe += Dxy;
    }
  }
  else if (getY() < -0.1)
  {
    yballe -= Dxy;
  }
  else if (getY() > 0.1)
  {
    yballe += Dxy;
  }

  if(xballe<15) xballe =15;
  if(xballe>305) xballe = 305;
  if(yballe<15) yballe =15;
  if(yballe>225) yballe = 225;

  back1.fillScreen(TFT_WHITE);
  back1.fillCircle(160, 120, 90, TFT_BLUE);
  back1.fillCircle(160, 120, 80, TFT_WHITE);
  back1.fillCircle(160, 120, 60, TFT_BLUE);
  back1.fillCircle(160, 120, 50, TFT_WHITE);

  back1.fillCircle(160, 120, 10, TFT_BLUE);

  back2.fillScreen(TFT_WHITE);
  back2.fillCircle(0, 120, 90, TFT_BLUE);
  back2.fillCircle(0, 120, 80, TFT_WHITE);
  back2.fillCircle(0, 120, 60, TFT_BLUE);
  back2.fillCircle(0, 120, 50, TFT_WHITE);
  back2.fillCircle(0, 120, 10, TFT_BLUE);
  
 // Serial.printf("xballe = %d et yballe = %d\n", xballe, yballe);
  if(xballe==160 && yballe == 120)
  {
    couleurBalle = TFT_GOLD;
  }
  else
   couleurBalle = TFT_GREEN;

  back1.fillCircle(xballe, yballe, 20, couleurBalle);
  back2.fillCircle(xballe - 160, yballe, 20, couleurBalle);
  
  back1.drawFastHLine(0, 120, 160, TFT_RED);
  back2.drawFastHLine(0, 120, 160, TFT_RED);
  back1.drawFastVLine(159, 0, 240, TFT_RED);

  back1.pushSprite(0, 0);
  back2.pushSprite(160, 0);

}

float getX()
{
  int16_t x0, x1, x_out;
  float xG;

  Wire.beginTransmission(ADXLAddress); // Begin transmission to the Sensor
  // Ask the particular registers for data
  Wire.write(X_Axis_Register_DATAX0);
  Wire.endTransmission(); // Ends the transmission and transmits the data from the two registers

  Wire.requestFrom(ADXLAddress, 2); // Request the transmitted two bytes from the two registers
  if (Wire.available() <= 2)
  {                   //
    x0 = Wire.read(); // Reads the data from the register
    x1 = Wire.read();
  }

  x_out = ((x1 << 8) | x0) + XCAL;
  xG = ((float)x_out) / 255;
  return xG;
}

float getY()
{
  int16_t y0, y1, y_out;
  float yG;

  Wire.beginTransmission(ADXLAddress); // Begin transmission to the Sensor
  // Ask the particular registers for data
  Wire.write(Y_Axis_Register_DATAY0);
  Wire.endTransmission(); // Ends the transmission and transmits the data from the two registers

  Wire.requestFrom(ADXLAddress, 2); // Request the transmitted two bytes from the two registers
  if (Wire.available() <= 2)
  {                   //
    y0 = Wire.read(); // Reads the data from the register
    y1 = Wire.read();
  }

  y_out = ((y1 << 8) | y0) + YCAL;
  yG = ((float)y_out) / 255;
  return yG;
}

float getZ()
{
  int16_t z0, z1, z_out;
  float zG;

  Wire.beginTransmission(ADXLAddress); // Begin transmission to the Sensor
  // Ask the particular registers for data
  Wire.write(Z_Axis_Register_DATAZ0);
  Wire.endTransmission(); // Ends the transmission and transmits the data from the two registers

  Wire.requestFrom(ADXLAddress, 2); // Request the transmitted two bytes from the two registers
  if (Wire.available() <= 2)
  {                   //
    z0 = Wire.read(); // Reads the data from the register
    z1 = Wire.read();
  }

  z_out = ((z1 << 8) | z0) + ZCAL;
  zG = ((float)z_out) / 255;
  return zG;
}
void dessineCible()
{
  // back.fillScreen(TFT_WHITE);
  // back.fillCircle(100,100,90,TFT_BLUE);
  // back.fillCircle(100,100,80,TFT_WHITE);
  // back.fillCircle(100,100,60,TFT_BLUE);
  // back.fillCircle(100,100,50,TFT_WHITE);
  // back.fillCircle(100,100,10,TFT_BLUE);
  // back.pushSprite(60,20);
}