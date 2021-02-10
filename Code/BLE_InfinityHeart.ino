/*
 * I Heart U Infinity!
 * Heart-shaped infinity mirror with RGB LEDs
 * and controlled via Bluetooth with an app
 * built with MIT App Inventor by
 * Rob Reynolds
 * SparkFun Electronics
 * Original build date - February 10, 2021
 * License: This code is public domain but you 
 * buy me a beer if you use this and we meet someday (Beerware license).
 * 
 * Feel like supporting our work? Buy a board from SparkFun!
 * Build components:
 * SparkFun ESP32 Thing - https://www.sparkfun.com/products/13907
 * Skinny RGB 144 LED strip https://www.sparkfun.com/products/14732
 * 2Ah Lithium Ion Battery https://www.sparkfun.com/products/13855
 * 
 * 
 */

////////////Bluetooth setup////////////

#include "BluetoothSerial.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int received;// received value will be stored in this variable


////////////NeoPixel setup////////////

#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN    2    // Digital IO pin connected to the NeoPixels. 2 for the Thing, 27 for the Thing Plus

#define PIXEL_COUNT 36

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool litUp = false;  // Let's you (and the code) know if the LEDs are on or off
int showType = 0;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("I_Heart_U_Infinity"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial.println("To turn ON send: a");//print on serial monitor  
  Serial.println("To turn OFF send: b"); //print on serial monitor 
  
  pinMode(LED_BUILTIN, OUTPUT);

  // Blink twice so I know you're working
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);


  strip.setBrightness(125); // No need to overpower the world with these LEDs
  strip.begin();
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'

  Serial.print("The state of litUp is ");
  Serial.println(litUp);
  Serial.println();
 
}

void loop() {
    received = SerialBT.read();

  if (Serial.available()) {
    SerialBT.write(Serial.read());
  
  }
  if (SerialBT.available()) 
  {    
    if (received == 1){ // Blinks LEDs five times
     //turnOn(); // FOr testing. Turn on the onboard LED
     for (int i = 0; i < 5; i++){
     colorWipe(strip.Color(200,15,15), 1); // turn on pink
     delay(400);
     strip.clear();  // Turn off all LEDs
     strip.show();
     delay(400);
     }  
    }
    
    if(received == 2) // fades lights up and down once
    {
     //turnOff(); // For testing. Turns off the onboard LED
     //rainbow(10); 
     breathAll();
    }
            
  if(received == 3) // give five heartbeat pulses
    {
     //blinky(); // For testing. Blinks onboard LED five times
     pulsing(strip.Color(155,5,5));  
    }       
  }

  if(received == 4) // Standard theatre marquee chase in red
  {
    theaterChase(strip.Color(100, 2, 2), 50); // Red
    //rainbowCycle(10);
  }

  if(received == 5) //Turns all lights on or off
  {
    onOff();
  }
  delay(20);
}


void turnOn()
{
  // This block is for testing before connecting LED strip
  SerialBT.println("LED ON:");// write on BT app
  Serial.println("LED ON:");//write on serial monitor
  digitalWrite(LED_BUILTIN, HIGH);// turn the LED ON
}

void turnOff()
{
  // This block is for testing before connecting LED strip
  SerialBT.println("LED OFF:");// write on BT app
  Serial.println("LED OFF:");//write on serial monitor
  digitalWrite(LED_BUILTIN, LOW);// turn the LED off 
}

void blinky()
{
  // This block is for testing before connecting LED strip
  SerialBT.println("Blinking");// write on BT app
  Serial.println("Blinking");//write on serial monitor
  for (int x = 0; x < 5; x++){
    digitalWrite(LED_BUILTIN, HIGH);// turn the LED on
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);// turn the LED off
    delay(300);
   }
}

void colorWipe(uint32_t color, int wait)
{
  if (litUp == true)
    {
      litUp = !litUp;
    }
    for (int i = 0; i < strip.numPixels(); i++)
    {                                  // For each pixel in strip...
        strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
        strip.show();                  //  Update strip to match
        delay(wait);                   //  Pause for a moment
    }
}


void pulsing(uint32_t color)
{
  if (litUp == true)
    {
      litUp = !litUp;
    }
  for (int d = 0; d < 5; d++){
    for (int i = 0; i < strip.numPixels(); i++)
    {                                  // For each pixel in strip...
        strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
        strip.show();                  //  Update strip to match
        delay(2);                   //  Pause for a moment
    }

    delay(100);
    strip.clear();
    strip.show();
    delay(100);

    for (int i = 0; i < strip.numPixels(); i++)
    {                                  // For each pixel in strip...
        strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
        strip.show();                  //  Update strip to match
        delay(2);                   //  Pause for a moment
    }

    delay(100);
    strip.clear();
    strip.show();
    delay(600);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) 
{
  if (litUp == true)
    {
      litUp = !litUp;
    }
  for (int j=0; j<20; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(100);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  strip.clear();
  strip.show();
}


void breathAll() 
{
  if (litUp == true)
    {
      litUp = !litUp;
    }
  for (int i = 0; i < 125 ; i++) {
    for(int j=0; j<strip.numPixels(); j++) {
      strip.setPixelColor(j, strip.Color(i,i,i)); // yellow with a little extra red to make it warmer 
    }
    strip.show();
    delay(10);
  }
  for (int i = 125; i > 0; i--) {
    for(int j=0; j<strip.numPixels(); j++) {
      strip.setPixelColor(j, strip.Color(i,i,i));
    }
    strip.show();
    delay(10);
  }
  strip.show();
  delay(10);
}

void onOff()
{
  if (litUp == false)
  {for (int i = 0; i < strip.numPixels(); i++)
    {                                  // For each pixel in strip...
        strip.setPixelColor(i, 155, 0, 0); //  Set pixel's color (in RAM)
        strip.show();                  //  Update strip to match
        delay(25);                   //  Pause for a moment
    }
  }
    else
    {
      for (int i = 0; i < strip.numPixels(); i++)
    {                                  // For each pixel in strip...
        strip.setPixelColor(i, 0, 0, 0); //  Set pixel's color (in RAM)
        strip.show();                  //  Update strip to match
        delay(25);                   //  Pause for a moment
    }
    delay(100);
    strip.clear();
    strip.show();
    delay(100);      
    }
    litUp = !litUp;
    Serial.print("The state of litUp is "); // Use this for testing
    Serial.println(litUp);
    Serial.println();
}
