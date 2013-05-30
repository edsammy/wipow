// Old code, NOT USED CURRENTLY.
// Function outputs (transmits with WiFly) ASCII byte representations of the binary data in lower_read and upper_read

// Input arguments are the size of the data in bytes that is required to transmit, and if the data should be considered signed or unsigned.

void ParseSendData(size_t size, boolean sign){
  byte data[4];
  boolean positive = true;
  byte MSB = 0;
  byte MSByte = 0;
  
  if(sign){
    if (size > 4){
      MSByte = byte(upper_read>>(8*(size-5)));
    }
    else{
      MSByte = byte(lower_read>>(8*(size-1)));
    }
    
    MSB = bitRead(MSByte, 3);
    
    if(MSB==1){
      lower_read = ~lower_read;
      upper_read = ~upper_read;
    }
  }
  
  for(int i = 0; i<size; i++){
    delayMicroseconds(500);
    if(i>3){
        data[i] = byte(upper_read>>(8*(i-4)));
      //  Serial.print(" ");
      //  Serial.print(data[i]);
      //  Serial.print(" ");
        SpiSerial.write(data[i]);
      }
      else{
        data[i] = byte(lower_read>>(8*i));
      //  Serial.print(" ");
      //  Serial.print(data[i]);
      //  Serial.print(" ");
        SpiSerial.write(data[i]);
      }
  }
  
  if(!sign){
    SpiSerial.write("END",3);
  }
  else{
    if(MSB==1){
      SpiSerial.write("END2",4);
    }
    else{
      SpiSerial.write("END1",4); 
    }
  }
}
