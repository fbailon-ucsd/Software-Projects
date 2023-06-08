//////////////////// This tab is coded to follow the main logic of the flow chart in order make a proper energy prioritization algorithm ////////////////

 // Solar Module inputs
const int Solar_Voltage_Read_InputPin = A1;     // Setting Variable to voltage sensor pin
const int Solar_Current_Read_InputPin = A2;     // Setting Variable to current sensor pin
const int Relay_control_1_Pin = 1;              // Setting Variable to digital relay control pin
int Relay_control_1_Mode = LOW;                 // Initializing the digital relay state to LOW (sending power to main panel)
int Solar_Voltage_Read_value = 0;               // Variable that stores the voltage reading value 
int Solar_Current_Read_value = 0;               // Variable that stores the current reading value
int Solar_Power_Read_value = 0;                 // Variable that stores the calculated Power reading value

// Wind Module inputs
const int Wind_Voltage_Read_InputPin = A3;      // Setting Variable to voltage sensor pin
const int Wind_Current_Read_InputPin = A4;      // Setting Variable to current sensor pin
const int Relay_control_2_Pin = 2;              // Setting Variable to digital relay control pin
int Relay_control_2_Mode = LOW;                 // Initializing the digital relay state to LOW (sending power to main panel)
int Wind_Voltage_Read_value = 0;                // Variable that stores the voltage reading value 
int Wind_Current_Read_value = 0;                // Variable that stores the current reading value
int Wind_Power_Read_value = 0;                  // Variabel that stores the calculated Power reading value

// Battery Module inputs
const int Battery_Voltage_Read_InputPin = A5;   // Setting Battery Variable to voltage sensor pin
const int Battery_Current_Read_InputPin = A6;   // Setting Battery Variable to current sensor pin
const int Relay_control_3_Pin = 3;              // Setting Variable to digital relay control pin
int Relay_control_3_Mode = LOW;                 // Initializing the digital relay state to LOW
int Battery_backup_voltage_value = 0;           // Holds value of Battery backup voltage
int Battery_backup_current_value = 0;
int Daily_kw_use = 165;                          // Change to value of client's Daily kw usage
int Battery_backup_capacity = 10240;            // Change to spec sheet value of backup battery model - 10240 Wh

// Generator inputs
const int Generator_Voltage_Read_InputPin = A7; // Setting Variable to voltage sensor pin
const int Generator_Current_Read_InputPin = A8; // Setting Variable to current sensor pin
const int Relay_control_4_Pin = 4;              // Setting Variable to digital relay control pin
int Relay_control_4_Mode = LOW;                 // Initializing the digital relay state to LOW 
int Generator_Voltage_value = 0;                // Variable that stores the voltage reading value 
int Generator_Current_value = 0;                // Variable that stores the current reading value

// Main Panel inputs
const int Main_Panel_Voltage_Read_InputPin = A9;  // Setting Variable to voltage sensor pin
const int Main_Panel_Current_Read_InputPin = A10; // Setting Variable to current sensor pin
int Main_Panel_voltage_read_value = 0;            // Variable that stores the Main Panel voltage reading value 
int Main_Panel_Current_read_value = 0;            // Variable that stores the Main Panel voltage reading value 

// Battery Charger inputs
const int Relay_control_5_Pin = 5;              // Setting Variable to digital relay control pin
int Relay_control_5_Mode = LOW;                 // Initializing the digital relay state to LOW 


// Sunlight Module inputs
const int Sunlight_Module_Voltage_Read_InputPin = A11;  // Setting Variable to voltage sensor pin
int Sunlight_Module_Voltage_value = 0;                  // Variable that stores the Sunlight Module voltage reading value 

// Wind Sensor 
const int Wind_Sensor_Voltage_Read_InputPin = A12;      // Setting Variable to voltage sensor pin 
int Wind_Sensor_Voltage_value = 0;                      // Variable that stores the wind sensor voltage reading value 

// Sensor values for accurate readings
//const int averageValue = 500;                           // Constant created for the number of samples
float sensitivity = 1000.0 / 200.0;                     //  1000mA per 200mV 
float Vref_solar = 67.68;                               // reference voltage fo the solar module
float Vref_wind = 573.46;                               // Reference voltage for the wind module



void setup() {
  
  // put your setup code here, to run once:
  
  pinMode(Relay_control_1_Pin, OUTPUT);                     // Set the pins correltaed to the Relay_control_1_Pin to be able to receive written outputs of LOW/HIGH and thus alter the state
  pinMode(Relay_control_2_Pin, OUTPUT);                     // Set the pins correltaed to the Relay_control_2_Pin to be able to receive written outputs of LOW/HIGH and thus alter the state
  pinMode(Relay_control_3_Pin, OUTPUT);                     // Set the pins correltaed to the Relay_control_3_Pin to be able to receive written outputs of LOW/HIGH and thus alter the state
  pinMode(Relay_control_4_Pin, OUTPUT);                     // Set the pins correltaed to the Relay_control_4_Pin to be able to receive written outputs of LOW/HIGH and thus alter the state
  pinMode(Relay_control_5_Pin, OUTPUT);                     // Set the pins correltaed to the Relay_control_5_Pin to be able to receive written outputs of LOW/HIGH and thus alter the state 
  
  digitalWrite(Relay_control_1_Pin, Relay_control_1_Mode);  // Initialize relay to proper simulation state (LOW)
  digitalWrite(Relay_control_2_Pin, Relay_control_2_Mode);  // Initialize relay to proper simulation state (LOW)
  digitalWrite(Relay_control_3_Pin, Relay_control_3_Mode);  // Initialize relay to proper simulation state (LOW)
  digitalWrite(Relay_control_4_Pin, Relay_control_4_Mode);  // Initialize relay to proper simulation state (LOW)
  digitalWrite(Relay_control_5_Pin, Relay_control_5_Mode);  // Initialize relay to proper simulation state (LOW)


  // These functions are under the Panels tab
  
  Main_Panel_READ_setup();                    // Sets the pin of the main panel as an input that can be read from 
  Solar_Voltage_Read_setup();                 // Sets the pin of the solar panels as an input that can be read from 
  Wind_Voltage_Read_setup();                  // Sets the pin of the wind turbine as an input that can be read from 
  Wind_sensor_READ_setup();                   // Sets the pin of the main panel as an input that can be read from 
  
  Serial.begin(9600);                         // Initialize Serial Monitor with baud rate set to 9600, make sure to have serial monitor set to same rate so outputs are seen

}

void loop() {
  
  // put your main code here, to run repeatedly:
  Serial.println("**************************   Beginning of loop   *****************************");   // Prints the beginning of the loop
  Serial.println("");
  Serial.println("");
  delay(3000);
//Wind_sensor_READ();
  Solar_Voltage_Read_READ();                  // Calls on the function that reads the current from the solar panel
  Wind_Voltage_Read_READ();                   // Calls on the function that reads the current from the wind turbine
  Main_Panel_READ();                          // Read Main Panel Voltage from function call


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// If Main Panel has feed //////////////// ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  if(Main_Panel_Current_read_value > 0) {                                               // If feed is detected in the Main Panel by solar and wind modules then keep on feeding power
      Serial.println("");
      Serial.println("");
      Serial.println("//////////////////////////////////////////////////////////////////////////////////////");
      Serial.println("/////// Feed Detected, Ensuring solar and wind are being relayed to Main Panel /////////");
      Serial.println("//////////////////////////////////////////////////////////////////////////////////////");
      Serial.println("");
      Serial.println("");
      delay(1000);
    
    if(Relay_control_1_Mode == HIGH | Relay_control_2_Mode == HIGH){ 
       
      Relay_control_1_Mode = LOW;                                                       // Switch feed from battery to main panel if feed is detected
      Relay_control_2_Mode = LOW;
      digitalWrite(Relay_control_1_Pin, Relay_control_1_Mode);                          // Set the Main Panel to recieve power by solar module
      delay(500);
      digitalWrite(Relay_control_2_Pin, Relay_control_2_Mode);                          // Set the Main Panel to recieve power by wind module
      delay(500);                                                                       // Delays are used to ensure states have time to switch
        }
  
      }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// If Main Panel has no feed //////////////// /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   else if (Main_Panel_Current_read_value == 0) {                                          // If the Main panel has no feed value
    //Serial.println("Use these values");

    Solar_Voltage_Read_READ();                                                            // Read the Solar Voltage output
    Wind_Voltage_Read_READ();                                                             // Read the Wind Voltage output
    Battery_backup_voltage_and_current_Read();                                            // Read voltage of Backup Battery ( correlates battery level)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// If Solar and Wind modules are producing ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
   
    if(Solar_Current_Read_value > 0 && Wind_Current_Read_value > 0){                        // If the Solar and Wind modules are outputting nonzero values
      Serial.println(" Power from Solar and Turbine system ");                              // Message to serial showing condition
      Battery_backup_voltage_and_current_Read();                                            // Read Backup Battery level again
      if( Relay_control_4_Mode == HIGH){                                                    // If Generator is on then turn off due to power coming from solar and turbine is nonzero
        Relay_control_4_Mode = LOW;
        delay(100);
        digitalWrite(Relay_control_4_Pin, Relay_control_4_Mode);                            // Write new state of relay to its pin, thus changing the realy state
        delay(1000);                                                                        // Delays allow system to let relays change properly 
      }

      if(Battery_backup_voltage_value > 30){                                                // If battery is normal(above 30%) then make sure that Main Panel is powered by Wind and Solar instead of charging battery

        Serial.println("Battery is sufficiently charged");
        
        if( Relay_control_1_Mode == HIGH){                                                  // Change Solar relay to direct power to Main Panel instead of charging battery if state is currently charging battery
          Serial.println(" Relaying Solar to Main Panel");          
          Relay_control_1_Mode = LOW;
          delay(100);
          digitalWrite(Relay_control_1_Pin, Relay_control_1_Mode);
          delay(1000);
        }
        if( Relay_control_2_Mode == HIGH){                                                  // Change Wind relay to direct power to Main Panel instead of charging battery if state is currently charging battery
          Serial.println(" Relaying Wind Turbine to Main Panel");
          Relay_control_2_Mode = LOW;
          delay(100);
          digitalWrite(Relay_control_2_Pin, Relay_control_2_Mode);
          delay(1000);
        }

       
       if( Relay_control_5_Mode == HIGH){                                                   // Change battery charger relay to stop the battery from recieving power 
          Serial.println("Turn off battery charger");
          Relay_control_5_Mode = LOW;
          delay(100);
          digitalWrite(Relay_control_5_Pin, Relay_control_5_Mode);
          delay(1000);
        }
        
        
        
      }
      
      else if(Battery_backup_voltage_value <= 30){                                        // If Backup Battery is below 30% then charge battery

        Serial.println(" Charging Battery ");                                             // Prints current state
        if( Relay_control_5_Mode == LOW){                                                 // Change the state of the relay to charge battery(HIGH state) if already not being charged
          Relay_control_5_Mode = HIGH;
          delay(100);
          digitalWrite(Relay_control_5_Pin, Relay_control_5_Mode);
          delay(1000);
          Serial.println(Relay_control_5_Mode);
        }

        if( Relay_control_1_Mode == LOW){                                              // Change solar relay to send power to battery charger (HIGH state)
          Serial.println(" Relaying Solar to Backup Battery");
          Relay_control_1_Mode = HIGH;
          delay(100);
          digitalWrite(Relay_control_1_Pin, Relay_control_1_Mode);
          delay(1000);
        }
        
        if( Relay_control_2_Mode == LOW){                                              // Change wind relay to send power to battery charger (HIGH state)
          Serial.println(" Relaying Wind Turbine to Backup Battery");          
          Relay_control_2_Mode = HIGH;
          delay(100);
          digitalWrite(Relay_control_2_Pin, Relay_control_2_Mode);
          delay(100);
          Relay_control_2_Mode = LOW;
          delay(100);
          digitalWrite(Relay_control_2_Pin, Relay_control_2_Mode);
          delay(1000);
        }
              
      }
        

      element_status();                                                                 // Displays current relay states and current readings
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// If Solar and Wind modules are not producing ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

     else if(Solar_Current_Read_value == 0 && Wind_Current_Read_value == 0){           // If no power is produced from solar and wind modules

      
      Battery_backup_voltage_and_current_Read();                                       // Check Backup Battery level
      if(Battery_backup_voltage_value > 30){                                           // If Backup Battery is above 30% then we can power Main Panel with it
        Serial.println(" Sufficient Battery");

      if( Relay_control_4_Mode == HIGH){                                               // Turn off generator if it was on (used when battery is charged up thus we are having the battery power the house)
        Serial.println("Turning off Generator");
        Relay_control_4_Mode = LOW;
        delay(100);
        digitalWrite(Relay_control_4_Pin, Relay_control_4_Mode);
        delay(1000);
      }
      
      if( Relay_control_3_Mode == LOW){                                                // Set the Backup Battery to power Main Panel if not already relayed to it
        Serial.println("Backup Battery to Main Panel");
        Relay_control_3_Mode = HIGH;
        delay(100);
        digitalWrite(Relay_control_3_Pin, Relay_control_3_Mode);
        delay(1000);
      }

      
      }

      else if(Battery_backup_voltage_value <= 30){                                     // If Backup Battery is below 30% then activate backup generator since no power is produced anywhere else
        Serial.println(" Low Battery");

      if( Relay_control_4_Mode == LOW){                                                // Set the Generator to power Main Panel if not already relayed to it
        Serial.println(" Activating Backup Generator");
        Relay_control_4_Mode = HIGH;
        delay(100);
        digitalWrite(Relay_control_4_Pin, Relay_control_4_Mode);
        delay(1000);
      }
        
        
      }


      element_status();                                                               // Display relay status and values
    }


    
    else if(Solar_Current_Read_value > 0 && Wind_Current_Read_value == 0){                // Simulation for daytime and no wind, solar but no wind power

      Serial.println("/////  Daytime (Sun & No Wind Relays) //////////");
      Serial.println(Relay_control_1_Mode);
      if(Relay_control_1_Mode == HIGH){                                                   // Send solar power to Main Panel if not already relayed to it

        Serial.println(" Send Solar to Main Panel");
        
        

        Relay_control_1_Mode = LOW;
        digitalWrite(Relay_control_1_Pin, Relay_control_1_Mode);
        delay(500);
        Serial.println(Relay_control_1_Mode);
        
        
      }
      
      if(Relay_control_2_Mode == LOW){                                                    // Disconnect wind and set it to charge the battery if not already charging battery

        Serial.println(" Send any Wind to charge Backup Battery");
        

        Relay_control_2_Mode = HIGH;
        digitalWrite(Relay_control_2_Pin, Relay_control_2_Mode);
        delay(100);
        Relay_control_2_Mode = LOW;
        digitalWrite(Relay_control_2_Pin, Relay_control_2_Mode);
        delay(100);
        
      }
      
      

      
      element_status();
      
    }

     else if(Solar_Current_Read_value == 0 && Wind_Current_Read_value > 0){               // Simulation for Wind and night time, wind but no solar power

      Serial.println("/////////////   Nightime (Wind & No Sun relays) //////////////");

      Serial.println(Relay_control_1_Mode);
      if(Relay_control_1_Mode == LOW){                                                    // Disconnect solar and set it to charge the battery if not already charging battery

        Serial.println(" Send any sunlight to charge Backup Battery");
        delay(100);
        Relay_control_1_Mode = HIGH;
        digitalWrite(Relay_control_1_Pin, Relay_control_1_Mode);
        delay(500);
        Serial.println(Relay_control_1_Mode);

        
      }
      
      if(Relay_control_2_Mode == HIGH){                                                    // Keep the wind powering the Main Panel if not already doing so

        Serial.println(" Send Wind to Main Panel");
        
        Relay_control_2_Mode = LOW;
        digitalWrite(Relay_control_2_Pin, Relay_control_2_Mode);
        delay(500);
        
      }
      
     
      element_status();
      
    }


    }
    
  }
