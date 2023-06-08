void Origin_set(){                            // Function to set Origin coordinates as well as update current location
  for(int i = 0; i< sample_size; i++){        // For loop designed to get average value of 50 gps location readings for more accurate location readings
    if(origin_write == 0){                    // Initial origin write value to set origin coordinates once
    gps_value_lat += gps.location.lat();      // Adding latitude values to vairable during each iteration
    //Serial.println(gps.location.lat());     // Print latitudes value for debugging
    gps_value_lng += gps.location.lng();      // Adding longitude values to vairable during each iteration
    //Serial.println(gps.location.lng());     // Print longitudes value for debugging
    }
    
    else{                                     // Caculate current gps location after initial origin coordinate was set
      
      lat_average_sum += gps.location.lat();  // Adding latitude values to vairable during each iteration  
      lng_average_sum += gps.location.lng();  // Adding longitude values to vairable during each iteration
      
      }
    }
  
  origin_write = 1;                           // Sets origin write to a nonzero value so that the origin coordinates are not overwritten whenever function is called
  gps_origin_lat = gps_value_lat/sample_size; // Sets average latitude value to variable for origin coordinates
  gps_origin_lng = gps_value_lng/sample_size; // Sets average longitude value to variable for origin coordinates
  lat_average = lat_average_sum / sample_size;// Sets average latitude value to variable for condition statements
  Serial.println(" Lat Average");             // Print latitidue average value with 6 significant figures for perimeter in feet
  Serial.println(lat_average,6);
  lng_average = lng_average_sum / sample_size;// Sets average longitude value to variable for condition statements
  Serial.println(" Lng Average");             // Print longitude average value with 6 significant figures for perimeter in feet
  Serial.println(lng_average,6);
  lat_average_sum = 0;                        // Reset latitude sum value to 0 for next location iterations
  lng_average_sum = 0;                        // Reset longitude sum value to 0 for next location iterations
  
  // This statement prints the Origin value for reference in the Serial Monitor for debugging and referencing
  Serial.println("//////////////////////////////////////////////////////////////");     
  Serial.println("////////////////////////// Origin ////////////////////////////");
  Serial.println("//////////////////////////////////////////////////////////////");
  Serial.println("gps_origin_lat");
  Serial.println(gps_origin_lat,6);
  Serial.println("gps_origin_lng");
  Serial.println(gps_origin_lng,6);
  
  }
  

void perimeter_coversion(){                          // Function used to convert perimeter in feet to latitude and longitude units

  perimeter_lat = (perimeter_feet_x/364000.0)*3.0;   // Conversion formula for feet to latitude
  //Serial.println(perimeter_lat,6);                 // Printing converted value for verification with inteded results
  perimeter_lng = (perimeter_feet_y/288200.0)*3.0;   // Conversion formula for feet to longitude
  //Serial.println(perimeter_lng,6);                 // Printing converted value for verification with inteded results
  
  
}
