#include <TinyGPS++.h>                      // Library to read outputs from NE06m module

#include <SoftwareSerial.h>                // Library to relay outputs to serial

static const int RXPin = 15, TXPin = 14;   // Pins tied to transmission(Tx) and Recieving(Rx) modules
static const uint32_t GPSBaud = 9600;      // Baud rate for GPS signal
float gps_value_lat = 0.0;                 // Varibale to store latitude value of gps
float gps_value_lng = 0.0;                 // Varibale to store longitude value of gps
float gps_origin_lat = 0.0;                // Varibale to store latitude value of gps for origin point upon initial boot
float gps_origin_lng = 0.0;                // Varibale to store longitude value of gps for origin point upon initial boot
int sample_size = 50;                      // Sample size used to average gps values for consistent average values
float gps_perimeter_lat;                   
float gps_perimeter_lng;
const int buzzer_pin = 27 ;                // Pin set to Buzzer
int origin_write = 0;                      // Variable to have the origin be set initially when booting up the gps module

float perimeter_feet_x = 51.0;             // Variable used to determine the size of perimeter in feet
float perimeter_feet_y = 51.0;             // Variable used to determine the size of perimeter in feet
float perimeter_lat = 0.0;                 // Variable that stores converted range from feet to latitude units
float perimeter_lng = 0.0;                 // Variable that stores converted range from feet to longitude units

float lat_average_sum = 0;                 // Variable that stores summed latitude values before averaged
float lng_average_sum = 0;                 // Variable that stores summed longitude values before averaged
float lat_average = 0;                     // Variable that stores latitude average value
float lng_average = 0;                     // Variable that stores longitude average value

const int pwmFrequency = 5000;             // Set the PWM frequency to 5KHz
const int pwmChannel = 0;                  // Use PWM channel 0
const int pwmBitResolution = 8;            // Set a PWM resolution of 8 bits




// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(9600);                     // Begin Serial with 9600 baud rate
  ss.begin(GPSBaud);                      // Begin communication with GPS and Serial
  delay(200);
  
  // configure PWM on a specific pwmChannel
  ledcSetup (pwmChannel, pwmFrequency, pwmBitResolution);
  
  // attach the pwmChannel to the output GPIO to be controlled
  ledcAttachPin(buzzer_pin, pwmChannel);
  
  perimeter_coversion();                 // Call function to convert perimeter values from feet to latitude and longitude units
  setupDisplay();                        // Setup OLED display

}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0) {           // read values while serial is available
    gps.encode(ss.read());               // encode gps values that are read
    if (gps.location.isUpdated()) {      // New values for an updated location
      // Latitude in degrees (double)
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 6); // Print Latitude with 6 sig figs for usage with feet unit
      // Longitude in degrees (double)
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6); // Print Longitude with 6 sig figs for usage with feet unit
      Origin_set();                          // Call function that sets origin point of perimeter
      
///////////////////////////////////////////////////////////////////////////////////
      if( abs(abs(lat_average) - abs(gps_origin_lat))  > (.6*perimeter_lat) && abs(abs(lat_average) - abs(gps_origin_lat))  < perimeter_lat ){    // Condition to trigger approaching perimeter warning and buzzer for latitude
        //Serial.println(" 1:");                                                             // Print situation 1 for debugging
        //Serial.println(abs(abs(gps.location.lat()) - abs(gps_origin_lat)),6);              // Print condiiton value for debugging
        ledcWrite(pwmChannel, 125);                                                          // Set buzzer to half strength using pwm channel
        writeDisplay("Near perimeter!", 0, true);                                            // Send message to OLED for message simulation
        delay(500);

      }
      else if(abs(abs(lat_average) - abs(gps_origin_lat))  > perimeter_lat){                 // Condition to trigger passing perimeter and thus increasing strength on buzzer
        //Serial.println(" 2:");                                                             // Print Situation 2 for debugging
        //Serial.println(abs(abs(gps.location.lat()) - abs(gps_origin_lat)),6);              // Print conditional statement for debugging
        writeDisplay("Past Perimeter!!", 0, true);                                           // Print message to OLED for message simulation of pet passing perimeter
        ledcWrite(pwmChannel, 250);                                                          // Set buzzer to full strength using pwm channel for pet deterence to pass perimeter
        delay(500);
      }
      
      else {                                                                                 // Condition for being within the perimeter
        ledcWrite(pwmChannel, 0);                                                            // Set buzzer off on pwm channel while in perimeter
        delay(500);
        //Serial.println("here1");                                                           // Print statement for reference of being within perimeter for debugging purposes
        writeDisplay("In Perimeter :)", 0, true);                                            // Print message to OLED to verify that wearable is within perimeter parameters
        
        
        
      }


      if( abs(abs(lng_average) - abs(gps_origin_lng))  > (.6*perimeter_lng) && abs(abs(lng_average) - abs(gps_origin_lng))  < perimeter_lng ){  // Condition to trigger approaching perimeter warning and buzzer for longitude
        //Serial.println(" 3:");                                                             // Print situation 3 for debugging 
        //Serial.println(abs(abs(gps.location.lng()) - abs(gps_origin_lng)),6);              // Print condition to verify values for debugging
        
        writeDisplay("Near perimeter!", 0, true);                                            // Print message to OLED to verify the wearable is approaching perimeter for longitude
        ledcWrite(pwmChannel, 125);                                                          // Set buzzer to half strength using pwm channel
        delay(500);
        
      }
      
      else if(abs(abs(lng_average) - abs(gps_origin_lng))  >= perimeter_lng){                // Condition to trigger passing perimeter and thus increasing strength on buzzer to max
        //Serial.println(" 4:");                                                             // Print situation 4 for debugging
        //Serial.println(abs(abs(lng_average) - abs(gps_origin_lng)),6);                     // Print condition to check values for debugging
        writeDisplay("Past Perimeter!!", 0, true);                                           // Print message to OLED to show that wearable location has passed perimeter value
        ledcWrite(pwmChannel, 250);                                                          // Set buzzer to full strength using pwm channel
        delay(500);
      }

      else {                                                                                 // Condition for being within parameters
        //Serial.println("here2");                                                           // Print statement for reference of being within perimeter for debugging purposes
        ledcWrite(pwmChannel, 0);                                                            // Turn off buzzer while within perimeter using pwm channel
        writeDisplay("In Perimeter :)", 0, true);                                            // Print message to OLED to verify that wearable is within perimeter parameters    
        delay(500);
        
      }
      
    }
  }
}
