
/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/
#define USE_ARDUINO_INTERRUPTS true
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PulseSensorPlayground.h> 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

static const unsigned char PROGMEM logo2_bmp[] =
{ 0x03, 0xC0, 0xF0, 0x06, 0x71, 0x8C, 0x0C, 0x1B, 0x06, 0x18, 0x0E, 0x02, 0x10, 0x0C, 0x03, 0x10,              //Logo2 and Logo3 are two bmp pictures that display on the OLED if called
0x04, 0x01, 0x10, 0x04, 0x01, 0x10, 0x40, 0x01, 0x10, 0x40, 0x01, 0x10, 0xC0, 0x03, 0x08, 0x88,
0x02, 0x08, 0xB8, 0x04, 0xFF, 0x37, 0x08, 0x01, 0x30, 0x18, 0x01, 0x90, 0x30, 0x00, 0xC0, 0x60,
0x00, 0x60, 0xC0, 0x00, 0x31, 0x80, 0x00, 0x1B, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x04, 0x00,  };

static const unsigned char PROGMEM logo3_bmp[] =
{ 0x01, 0xF0, 0x0F, 0x80, 0x06, 0x1C, 0x38, 0x60, 0x18, 0x06, 0x60, 0x18, 0x10, 0x01, 0x80, 0x08,
0x20, 0x01, 0x80, 0x04, 0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02, 0xC0, 0x00, 0x08, 0x03,
0x80, 0x00, 0x08, 0x01, 0x80, 0x00, 0x18, 0x01, 0x80, 0x00, 0x1C, 0x01, 0x80, 0x00, 0x14, 0x00,
0x80, 0x00, 0x14, 0x00, 0x80, 0x00, 0x14, 0x00, 0x40, 0x10, 0x12, 0x00, 0x40, 0x10, 0x12, 0x00,
0x7E, 0x1F, 0x23, 0xFE, 0x03, 0x31, 0xA0, 0x04, 0x01, 0xA0, 0xA0, 0x0C, 0x00, 0xA0, 0xA0, 0x08,
0x00, 0x60, 0xE0, 0x10, 0x00, 0x20, 0x60, 0x20, 0x06, 0x00, 0x40, 0x60, 0x03, 0x00, 0x40, 0xC0,
0x01, 0x80, 0x01, 0x80, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x30, 0x0C, 0x00,
0x00, 0x08, 0x10, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x01, 0x80, 0x00  };

const int PulseWire = A2; // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13; // The on-board Arduino LED, close to PIN 13.
int Threshold = 550; 
PulseSensorPlayground pulseSensor;

void setup() {
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  digitalWrite(10,HIGH);
  digitalWrite(11,LOW);
  Serial.begin(9600);
pinMode(9,INPUT);
pulseSensor.analogInput(PulseWire);
pulseSensor.blinkOnPulse(LED13); //auto-magically blink Arduino's LED with heartbeat.
pulseSensor.setThreshold(Threshold);
if (pulseSensor.begin()) {
Serial.println("We created a pulseSensor Object !"); //This prints one time at Arduino power-up, or on Arduino reset.
//lcd.setCursor(0,0);
//lcd.print(" Heart Rate Monitor");

}

//   SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  
  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);

  //testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}
int myBPM;
int button=0;
int i=0,g=0;
int h1,h2,m1,m2,s1,s2,r;
void loop() {
  
if(i==0){
         display.clearDisplay();
         display.setTextSize(2); // Draw 2X-scale text
         display.setTextColor(WHITE);
         display.setCursor(0, 0);
         display.println(F("ARC-WATCH"));
         display.display();      // Show initial text
         delay(100);
         display.startscrollright(0x00, 0x0F);
         delay(800);
         display.stopscroll();
         i++;
        }
//////////////////////////////////////////////////////////////////SCREEN 1/////////////////////////////////////////////////////////////////////////////////////////
hh:    if(i==1){
                while(h1<=1){
                              while(h2<9){
                                          while(m1<6){
                                                      while(m2<10){
                                                                   while(s1<6){
                                                                               while(s2<10){display.clearDisplay();
                                                                                            display.setTextSize(2);
                                                                                            display.setCursor(15, 0);
                                                                                            display.println(F("ARC-WATCH"));
                                                                                            display.setCursor(90,30);
                                                                                            display.println(s1);
                                                                                            display.setCursor(105,30);
                                                                                            display.println(s2);
                                                                                            display.setCursor(80,30);
                                                                                             display.println(":");
                                                                                             display.setCursor(50,30);
                                                                                             display.println(m1);
                                                                                             display.setCursor(65,30);
                                                                                             display.println(m2);
                                                                                             display.setCursor(40,30);
                                                                                             display.println(":");
                                                                                             display.setCursor(10,30);
                                                                                             display.println(h1);
                                                                                             display.setCursor(25,30);
                                                                                             display.println(h2);
                                                                                             display.display();      // Show initial text
                                                                                             s2++;
                                                                                             for(r=0;r<100;r++){
                                                                                                                 button=digitalRead(9);
                                                                                                                 if(button==0 && g==1)g=0;
                                                                                                                 if(button==1 && g==0){i++;
                                                                                                                 g=1;   
                                                                                                                 goto xy; }                                         
                                                                                                                 delay(10);
                                                                                  
                                                                                                                }
                                                                                           } 
                                                                            s2=0;
                                                                            s1++;}       
                                                              m2++; 
                                                              s1=0;}
                                      
             
                                                m1++;
                                                m2=0;}
                                      s1=0;
                                      m1=0;
                                      h2++;}
h2=0;
h1++;
}
if(h1==2){
          while(h2<4){
                      while(m1<6){
                                  while(m2<10){
                                               while(s1<6){
                                                           while(s2<10){display.clearDisplay();
                                                                        display.setTextSize(2);
                                                                        display.setCursor(15, 0);
                                                                        display.println(F("ARC-WATCH"));
                                                                        display.setCursor(90,30);
                                                                        display.println(s1);
                                                                        display.setCursor(105,30);
                                                                        display.println(s2);
                                                                        display.setCursor(80,30);
                                                                        display.println(":");
                                                                        display.setCursor(50,30);
                                                                        display.println(m1);
                                                                        display.setCursor(65,30);
                                                                        display.println(m2);
                                                                        display.setCursor(40,30);
                                                                        display.println(":");
                                                                        display.setCursor(10,30);
                                                                        display.println(h1);
                                                                        display.setCursor(25,30);
                                                                        display.println(h2);
                                                                        display.display();      // Show initial text
                                                                        s2++;
                                                                        for(r=0;r<100;r++){
                                                                                           button=digitalRead(9);
                                                                                           if(button==0 && g==1)g=0;
                                                                                           if(button==1 && g==0){i++;
                                                                                           g=1;   
                                                                                           goto xy; }                                         
                                                                                           delay(10);
                                            
                                                                                           }
                                                                       } 
                                                      s2=0;
                                                      s1++;}       
                                        m2++; 
                                        s1=0;}
                                      
             
                            m1++;
                            m2=0;}
                s1=0;
                h2++;}
h1++;
}
  else h1=0; 
  
  }

////////////////////////////////////////////////////////////////////////////////SCREEN 2///////////////////////////////////////////////////////////////////////////////////////////////

  
xy: if(i==2){if(h1<=1){
                       while(h2<9){
                                   while(m1<6){
                                               while(m2<10){
                                                            while(s1<6){
                                                                        while(s2<10){
                                                                                     display.clearDisplay();    
                                                                                     display.drawBitmap(0, 0, logo3_bmp, 32, 32, WHITE);              
                                                                                     display.setCursor(50,0);                
                                                                                     display.println("BPM");
                                                                                     display.setCursor(50,40);                 
                                                                                     display.print(myBPM);
                                                                                    // display.drawBitmap(5, 5, logo2_bmp, 24, 21, WHITE);
                                                                                     display.display();
                                                                                     s2++;
                                                                                     for(r=0;r<50;r++){ 
                                                                                                         delay(20);

                                                                                                      
                                                                                               
                                                                                                         myBPM = pulseSensor.getBeatsPerMinute();                 
                                                                                                         if (pulseSensor.sawStartOfBeat()) {
                                                                                                           display.clearDisplay();    
                                                                                    // display.drawBitmap(0, 0, logo3_bmp, 32, 32, WHITE);              
                                                                                     display.setCursor(50,0);                
                                                                                     display.println("BPM");
                                                                                     display.setCursor(50,40);                 
                                                                                     display.print(myBPM);
                                                                                     display.drawBitmap(5, 5, logo2_bmp, 24, 21, WHITE);
                                                                                     display.display();                                                 
                                                                                                                                           }
                                                                                                                                               
                                                                                                                                                          
                                                                                                        
                                                                                                                                        //Clear the display
                                                                                                       
                                                                                                        button=digitalRead(9);
                                                                                                        if(button==0 && g==1)g=0;
                                                                                                        if(button==1 && g==0){i--;
                                                                                                                              g=1;   
                                                                                                                              goto xy; }                                         
                                                                                                                                                                                 
                                                                                                        }
                                                                                           
                                                                                                                                            
                                                                                    } 
                                                s2=0;
                                                s1++;}       
                                                m2++; 
                              
                              s1=0;}
                              
             
                m1++;
                m2=0;}
                s1=0;
                
                h2++;}
                h2=0;
h1++;
}
else if(h1==2){
         if(h2<4){
                  while(m1<6){
                          while(m2<10){while(s1<6){
                                          while(s2<10){display.clearDisplay();
                                                       display.setTextSize(2);
                                                       display.setCursor(15, 0);
                                                       display.println(F("ARC-WATCH"));
                                                       display.setCursor(90,30);
                                                       display.println(s1);
                                                       display.setCursor(105,30);
                                                       display.println(s2);
                                                       display.setCursor(80,30);
                                                       display.println(":");
                                                       display.setCursor(50,30);
                                                       display.println(m1);
                                                       display.setCursor(65,30);
                                                       display.println(m2);
                                                       display.setCursor(40,30);
                                                       display.println(":");
                                                       display.setCursor(10,30);
                                                       display.println(h1);
                                                       display.setCursor(25,30);
                                                       display.println(h2);
                                                       display.display();      // Show initial text
                                                       s2++;
                                                       for(r=0;r<100;r++){
                                                                           button=digitalRead(9);
                                                                           if(button==0 && g==1)g=0;
                                                                           if(button==1 && g==0){i--;
                                                                           g=1;   
                                                                           goto xy; }                                         
                                                                           delay(10);
                                            
                                                                          }
                                                       } 
                                                s2=0;
                                                s1++;}       
                                                m2++; 
                              
                              s1=0;}
                              
             
                m1++;
                m2=0;}
                s1=0;
                
                h2++;}
            h2=0;
}
  else h1=0; 
  
  }
  

  
 /* 
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);




*/





}
