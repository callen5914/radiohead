#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <SPI.h>
#include <RH_RF95.h>

Adafruit_SSD1306 display = Adafruit_SSD1306();

/* For OLED Module Buttons */
#define BUTTON_A 9
#define BUTTON_B 6
#define BUTTON_C 5
#define LED      13

/* For Radio Module */
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

/* For Radio Module Frequency */
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

#if (SSD1306_LCDHEIGHT != 32)
 #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

 void setup(){
  Serial.begin(9600);
  delay(100);
  Serial.println("Starting OLED");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  // Reset Radio
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  while (!rf95.init()) {
    display.print("LoRa radio init failed");
    Serial.println("LoRa radio init failed");
    display.display();
    while (1);
  }
  display.println("LoRa radio init OK!");
  Serial.println("LoRa radio init OK!");
  display.display();
  delay(1000);
  
  if (!rf95.setFrequency(RF95_FREQ)) {
    display.print("setFrequency failed");
    Serial.println("setFrequency failed");
    display.display();
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  display.print("Set Freq to: "); display.println(RF95_FREQ);
  display.display();
  delay(1000);
  display.setCursor(0,0);
  display.clearDisplay();
  display.display();
  rf95.setTxPower(23, false);
 }
 void loop(){
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      display.clearDisplay();
      display.display();
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      Serial.println((char*)buf);
      display.setCursor(0,0);
      display.print("Got: ");display.println((char*)buf);
      Serial.print("RSSI: ");      
      Serial.println(rf95.lastRssi(), DEC);
      display.print("RSSI :");display.println(rf95.lastRssi(), DEC);
      display.display();
      delay(100);
      digitalWrite(LED, LOW);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
 }

