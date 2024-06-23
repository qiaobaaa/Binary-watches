//author: 刘恒（Heng Liu）
#include <FastLED.h>
//https://github.com/FastLED/FastLED
#include <DS3231M.h>  // Include the DS3231M RTC library
//https://github.com/Zanduino/DS3231M

const uint32_t SERIAL_SPEED{ 115200 };  ///< Set the baud rate for Serial I/O
const uint8_t SPRINTF_BUFFER_SIZE{ 32 };

DS3231M_Class DS3231M;  ///< Create an instance of the DS3231M class

#define LED_PIN A1
#define NUM_LEDS 13
CRGB leds[NUM_LEDS];
/*
Tens of hours
Units of hours
Tens of minutes
Units of minutes
Like 19:56  hour1=1,hour2=9,minute1=5,minute2=6
*/
int hour1;
int hour2;
int minute1;
int minute2;
void setup() {
  Serial.begin(SERIAL_SPEED);
#ifdef __AVR_ATmega32U4__  // If this is a 32U4 processor, then wait for serial to initialize
  delay(3000);
#endif
  Serial.print(F("\nStarting Set program\n"));
  Serial.print(F("- Compiled with c++ version "));
  Serial.print(F(__VERSION__));
  Serial.print(F("\n- On "));
  Serial.print(F(__DATE__));
  Serial.print(F(" at "));
  Serial.print(F(__TIME__));
  Serial.print(F("\n"));
  while (!DS3231M.begin())  // Initialize RTC communications
  {
    Serial.println(F("Unable to find DS3231M. Checking again in 3s."));
    delay(3000);
  }                        
  DS3231M.pinSquareWave();  // Make INT/SQW pin toggle at 1Hz
  Serial.println(F("DS3231M initialized."));
  // DS3231M.adjust();  // Set to library compile Date/Time
  // DS3231M.adjust(DateTime(2023, 10, 17, 18, 53, 0));
  Serial.print(F("DS3231M chip temperature is "));
  Serial.print(DS3231M.temperature() / 100.0, 1);  // Value is in 100ths of a degree Celsius
  Serial.println(
    "\xC2\xB0"
    "C");
  Serial.println(F("\nEnter the following serial command:"));
  Serial.println(F("SETDATE yyyy-mm-dd hh:mm:ss"));

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(26);
}
void loop() {

  //  FastLED.setBrightness(26);
  DateTime now = DS3231M.now();
  char output_buffer[SPRINTF_BUFFER_SIZE];
  sprintf(output_buffer, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(),now.hour(), now.minute(), now.second());
  int hour = now.hour();
  int minute = now.minute();
  hour1 = hour / 10;
  hour2 = hour % 10;
  minute1 = minute / 10;
  minute2 = minute % 10;
  Serial.println(hour);
  Serial.println(minute);
  Serial.println(hour1);
  Serial.println(hour2);

  int num[13] = { 0 };
  int b[4] = { 6, 3, 7, 1 };
  int t[4] = { 1, 2, 4, 8 };
  b[0]=minute2;
  b[1]=minute1;
  b[2]=hour2;
  b[3]=hour1;
  int j=0;
  for (int x = 0; x < 4; x++) {
    if (x == 0 || x == 2) {
      for (int i = 0; i < 4; i++) {
        if ((b[x] & t[i]) != 0) {
          //printf("第 %d 位为1 \n", i + 1);
          num[j] = 1;
          j++;
        } else {
          //printf("第 %d 位为0 \n", i + 1);
          num[j] = 0;
          j++;
        }
      }
    } else if (x == 1) {
      for (int i = 0; i < 3; i++) {
        if ((b[x] & t[i]) != 0) {
          //printf("第 %d 位为1 \n", i + 1);
          num[j] = 1;
          j++;
        } else {
          //printf("第 %d 位为0 \n", i + 1);
          num[j] = 0;
          j++;
        }
      }
    } else {
      for (int i = 0; i < 2; i++) {
        if ((b[x] & t[i]) != 0) {
          //printf("第 %d 位为1 \n", i + 1);
          num[j] = 1;
          j++;
        } else {
          //printf("第 %d 位为0 \n", i + 1);
          num[j] = 0;
          j++;
        }
      }
    }
  }
  for (int i = 0; i < 13; i++)
  {
    //
	  //printf("...num 第几位 %d  为  %d\n", i, num[i]);
    if(num[i]==1)
    {
      leds[i] = CRGB(0, 0, 255);
      FastLED.show();
      delay(90);
    }
  }

  delay(1000);
}
