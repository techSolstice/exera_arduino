// The source for the Android application can be found at the following link: https://github.com/Lauszus/ArduinoBlinkLED
// The code for the Android application is heavily based on this guide: http://allaboutee.com/2011/12/31/arduino-adk-board-blink-an-led-with-your-phone-code-and-explanation/ by Miguel

#include <Usb.h>
#include <adk.h>
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 10, 5, 4, 3, 2);

USB Usb;
ADK adk(&Usb,"TKJElectronics", // Manufacturer Name
             "ArduinoBlinkLED", // Model Name
             "Example sketch for the USB Host Shield", // Description (user-visible string)
             "1.0", // Version
             "http://www.tkjelectronics.dk/uploads/ArduinoBlinkLED.apk", // URL (web page to visit if no installed apps support the accessory)
             "123456789"); // Serial Number (optional)

//#define LED 7 // Pin 13 is occupied by the SCK pin on a normal Arduino (Uno, Duemilanove etc.), so use a different pin


/*===============
  Setup Methods
===============*/
void setup()
{
  setupSerial();
  setupLcd();
}

void setupSerial(){
  Serial.begin(115200);
  Serial.print("\r\nADK demo start");
  if (Usb.Init() == -1) {
    Serial.print("\r\nOSCOKIRQ failed to assert");
    while(1); //halt
  }
}

void setupLcd(){
   // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("hello, world!"); 
}

/*===============
  Loop Methods
===============*/
void loop()
{    
  Usb.Task();
  if(adk.isReady()) {
    uint8_t msg[1];
    uint16_t len = sizeof(msg);

    //This receives the data along USB.  This may or may not contain the entire 'packet'.  Probably not.
    //The bytes go into Byte msg.  Coerce into char to encode to ASCII, otherwise Byte contains an integer [0,255]
    uint8_t rcode = adk.RcvData(&len, msg);

    //USBTRACE(2) are debug messages
    if(rcode && rcode != hrNAK)
      USBTRACE2("Data rcv. :", rcode);
      
    //If received message is more than 0
    if(len > 0) {
      USBTRACE2("LENGTH:", len);
      lcd.setCursor(0,0);
      lcd.print('x');  //hey we received an input, so show that on the LCD
     
      if(len == 1) lcd.print((char)msg[0]);
      /*  Untested code
      else{
         while (int i=0; i<len; ++i){
            lcd.print((char)msg[i]);
         } 
      }
      */
    } else{
      USBTRACE("EMPTY\n");
      lcd.setCursor(0, 1);
      lcd.print('y');
    }
  }
  lcd.setCursor(0, 1);
  lcd.print("Bottom");
}
