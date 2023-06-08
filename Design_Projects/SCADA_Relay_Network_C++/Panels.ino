//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// Notes ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Relay 2 has caused some dificulties as setting its value to HIGH produces negative outputs that affect //
// the output of the wind module                                                                            //
// - The analog resolution of the arduino wasn't strong enough to detect the small current changes in the   //
// the simulation board and thus a voltage divider was added to the modules to provide enough power         // 
// - This tab has all the functions to read the different values of the different modules in the clients    //
// system as well as displaying them                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
















float voltageConversionConstant = .004882814;     //This constant maps the value provided from the analog read function, which ranges from 0 to 1023, to actual voltage, which ranges from 0V to 5V
int a,b,c,c_volt,d,e;                             // Variables used to store the value of mapped sensor values
int sensorValue_a = 0,sensorValue_b = 0,sensorValue_c = 0,sensorValue_c_voltage = 0,sensorValue_d = 0,sensorValue_e = 0;   // Variables to use for summed sensor values before averaging
int averageValue = 100, wind_sensor_value = 0;    // Variables for sample size/average and wind sensor value intialization

void Main_Panel_READ_setup(){                                       // Function used to setup Main Panel pin as input to read voltage from 
  pinMode(Main_Panel_Voltage_Read_InputPin, INPUT);                 // Command to set voltage as pin
}

void Main_Panel_READ() {                                            // Function to read Main Panel current value

for (int i = 0; i < averageValue; i++){                             // For loop to obtain average current value for more accurate readings
  
    sensorValue_e += analogRead(Main_Panel_Current_Read_InputPin);  // Values read from current pin are added to each other for sample size == average value
    delay(2);

  }

  sensorValue_e = (sensorValue_e / averageValue);                   // Divide the total sum by average value to obtain actual average sensor value
  //Serial.println(sensorValue_e);                                  // Printing statement to check sensor output is working
  if(sensorValue_e < 0){                                            // Condition for feed being off sets output value to 0
    e = 0;
  }
  else if( sensorValue_e > 0){                                      // Condition for flipped switch shows feed detected by setting variable e to 1
    e = 1; 
  }
  //Serial.println("Main Panel value");                             // Debugging print statements to Serial to verify flip switching
  //Serial.println(e);
  Main_Panel_Current_read_value = e;                                // Set Main Panel current value to e output for conditional statements
  sensorValue_e = 0;                                                // Reset summation value for next function call
//  Serial.println("");                                             // Print statements to add padding for values if need be
//  Serial.println("");
//  Serial.println("");
//  Serial.println("");
//  Serial.println("");
//  Serial.println("");
  delay (2000); 
}


void Solar_Voltage_Read_setup(){                                    // Function used to setup Solar voltage pin as input to read voltage from 
  pinMode(Solar_Voltage_Read_InputPin, INPUT);
}

void Solar_Voltage_Read_READ() {                                    // Reads the current values from solar sensor
  
for (int i = 0; i < averageValue; i++)                              // For loop to obtain average current value for more accurate readings
  {
    sensorValue_a += analogRead(Solar_Current_Read_InputPin);       // Values read from current pin are added to each other for sample size == average value
    //Serial.println(analogRead(Solar_Current_Read_InputPin));      // Statement to read outputs being read for debugging
    delay(2);

  }

  sensorValue_a = (sensorValue_a / averageValue);                   // Divide the total sum by average value to obtain actual average sensor value
  //Serial.println("solar");                                        // Debugging print statements to see average calculted output
  //Serial.println(sensorValue_a);
  a = map(sensorValue_a,160,181,0,20);                              // Used to map the potentiometer range to a simulated value from 0 - 20A
  //Serial.println(a);                                              // Print statements to debug mapping
  if(sensorValue_a < 160){                                          // Condition to prevent fluctutations from creating other values lower than 0A
    a = 0;
  }
  else if( sensorValue_a > 181){                                    // Condition to prevent fluctutations from creating other values higher than 20A
    a = 20;
  }
//  Serial.println("");                                             // Padding print statements for easier value readability
//  Serial.println("");
//  Serial.println("");
//  Serial.println("");
//  Serial.println("");
//  Serial.println("");
//  Serial.println("Solar value");
//  Serial.println(a); 
  Solar_Current_Read_value = a;                                     // Set values being outputted to element_status() function
  sensorValue_a = 0;                                                // Resetting averaged value for next iteration
}



void Wind_Voltage_Read_setup(){                                     // Function used to setup Wind voltage pin as input to read voltage from 
  pinMode(Wind_Voltage_Read_InputPin, INPUT);
}


void Wind_Voltage_Read_READ() {                                     // Reads the current values from Wind sensor
  
for (int i = 0; i < averageValue; i++){                             // For loop to obtain average current value for more accurate readings
  
    wind_sensor_value = analogRead(Wind_Current_Read_InputPin);     
    sensorValue_b = sensorValue_b + wind_sensor_value;              // Values read from current pin are added to each other for sample size == average value
    //Serial.println(sensorValue_b);
    delay(2);

  }

  sensorValue_b = (sensorValue_b / averageValue);                   // Divide the total sum by average value to obtain actual average sensor value
  //Serial.println("wind");                                         // Debugging print statements to see average calculted output
  //Serial.println(sensorValue_b);
  b = map(sensorValue_b,155,176,0,20);                              // Used to map the potentiometer range to a simulated value from 0 - 20A
  //b = b + 15;
  if(sensorValue_b < 155){                                          // Condition to prevent fluctutations from creating other values lower than 0A
    b = 0;
  }
  else if(sensorValue_b > 176){                                     // Condition to prevent fluctutations from creating other values higher than 20A
    b = 20;
  }
  //Serial.println("Wind value");
  //Serial.println(b); 
  Wind_Current_Read_value = b;                                      // Set values being outputted to element_status() function
  sensorValue_b = 0;                                                // Resetting averaged value for next iteration
}


void Battery_backup_voltage_and_current_Read() {                   // Reads the voltage value from backup battery only due to simulation limitations althoough current is also setup. Notice how only voltage is passed onto the battery voltage value
  
  for (int i = 0; i < averageValue; i++){                          // For loop to obtain average current value for more accurate readings like previous functions
    
    //sensorValue_c_voltage += analogRead(Battery_Voltage_Read_InputPin);
    sensorValue_c += analogRead(Battery_Current_Read_InputPin);
    delay(2);

  }

  sensorValue_c = (sensorValue_c / averageValue);                 // Divide the total sum by average value to obtain actual average sensor value
  sensorValue_c_voltage = analogRead(Battery_Voltage_Read_InputPin); // Pass actual backup battery voltage(level)
  //Serial.println("Battery value");
  //Serial.println("Battery voltage");
  //Serial.println(sensorValue_c);
  //Serial.println(sensorValue_c_voltage);
  c_volt = map(sensorValue_c_voltage,320,478,0,100);              // Map value to range that shows battery level from 0% to 100%
  //Serial.println(c_volt);
  if (sensorValue_c_voltage < 320){                               // Condition to prevent fluctutations from creating other values lower than 0%
    c_volt = 0;
  }
  else if(sensorValue_c_voltage > 478){                           // Condition to prevent fluctutations from creating other values higher than 100%
    c_volt = 100;
  }
  c = map(sensorValue_c,154,182,0,100);                           // Used to map the potentiometer range to a simulated value from 0 - 100% based on current 
  if( c<0){
    c = 0;
  }
  //Serial.println("Battery value");
  //Serial.println(c);
  Battery_backup_current_value = c;
  Battery_backup_voltage_value = c_volt;                          // Set value being outputted to element_status() function for battery level
  sensorValue_c = 0;                                              // Resetting averaged value for next iteration
}
void Generator_Voltage_Read(){                                    // Function reads backup generator output
    for (int i = 0; i < averageValue; i++)
  {
    sensorValue_d += analogRead(Generator_Current_Read_InputPin);
    delay(2);

  }

  sensorValue_d = (sensorValue_d / averageValue);                // Sensor variable is averaged out
  //Serial.println(sensorValue_d);
  d = map(sensorValue_d,135,162,5,20);                           // Mapping command for 5-20A range
 // Serial.println("Generator value");
 // Serial.println(d);
  Generator_Current_value = d;                                  // Set generator value to mapped value
  sensorValue_d = 0;                                            // Reset summed sensor value for next function call
}


void Sunlight_Module_Voltage_Read_setup(){
  pinMode(Sunlight_Module_Voltage_Read_InputPin, INPUT);
}

void Sunlight_Module_Voltage_Read() {                                   // Reads output from solar module on panel
  
  sensorValue_abc = analogRead(Sunlight_Module_Voltage_Read_InputPin);
  //Serial.println(sensorValue_abc);
  abc = map(sensorValue_abc,1000,730,0,100);                            // Maps output to see light intensity detected
  Sunlight_Module_Voltage_value = abc;
  
}


void Wind_Sensor_Voltage_Read_setup(){
  pinMode(Wind_Sensor_Voltage_Read_InputPin, INPUT);
}

void Wind_Sensor_Voltage_Read() {                                     // Reads output from Anemometer on panel

  sensorValue_bcd = analogRead(Wind_Sensor_Voltage_Read_InputPin);    
  Serial.println("Anemometer value");
  Serial.println(sensorValue_bcd);
  Wind_Sensor_Voltage_value = bcd;                                    // Sets read output to variable that stores value
  delay(1000);
  
  //Wind_Sensor_Voltage_value = analogRead(Wind_Sensor_Voltage_Read_InputPin);
}

void Battery_backup_voltage_setup() {
  pinMode(Battery_Voltage_Read_InputPin, INPUT);
}

void Battery_backup_voltage_Read() {
  Battery_backup_voltage_value = analogRead(Battery_Voltage_Read_InputPin);
}


void element_status(){                                               // Function prints all module outputs and relays values to see current state of SCADA system

  Serial.println("---------------------------------------------------------------------");
  Serial.println("");
  Serial.print("Solar status:  ");
  Serial.print(Relay_control_1_Mode);
  if(Relay_control_1_Mode ==1 ){
    Serial.println("  (Charging Battery)");
  }
  else if(Relay_control_1_Mode == 0){
    Serial.println("");
  }
  
  Serial.println("");
  Serial.print("Voltage value:  ");
  Serial.println(Solar_Current_Read_value);
  Serial.println("");

  Serial.println("");
  Serial.print("Turbine status:  ");
  Serial.print(Relay_control_2_Mode);
  if(Relay_control_2_Mode ==1 ){
    Serial.println("  (Charging Battery)");
  }
  else if(Relay_control_2_Mode == 0){
    Serial.println("");
  }
  Serial.print("Wind value:  ");
  Serial.println(Wind_Current_Read_value);
  Serial.println("");

 
  Serial.println("");
  Serial.print("Battery status: ");
  Serial.print(Relay_control_3_Mode);
  if(Relay_control_3_Mode ==1 ){
    Serial.println("  (Powering Main Panel)");
  }
  else if(Relay_control_3_Mode ==0){
    Serial.println("");
  }
  Serial.print("Battery level:   ");
  Serial.println(Battery_backup_voltage_value);
  Serial.println("");

  Serial.println("");
  Serial.print("Main Panel status:  ");
  Serial.println(Main_Panel_Current_read_value);
  Serial.println("");
  Serial.println("---------------------------------------------------------------------");
  delay(3000);
  

  
}
