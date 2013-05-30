// This function writes to the MAXQ3183 chip.
// It goes through the SPI communciation protocol
// specificied by the chip and writes to the
// desired register with the data presented.

//  "reg" is a String of the destination register in HEX to be written to (ex: "1C8" to write to register 0x1C8)
//  "u" is a String of the data to be written into "reg" in HEX. The function automatically calculates how many
//              data bytes is to be written to correctly update the firstbyte of the SPI communciation.
//  **** THE SIZE OF STRING "U" SHOULD BE EITHER 1 BYTE (2 HEX CHARACTERS), 2 BYTES (4 HEX CHARACTERS), 4 BYTES (8 HEX), OR 8 BYTES (16 HEX) **********

//  This function RETURNS a BOOLEAN value stating if the writing is successful(true) or a failure(false).

//  Refer to the MAXQ3183 datasheet for the SPI communcation protocol: http://datasheets.maxim-ic.com/en/ds/MAXQ3183.pdf


boolean MaximWrite(String reg, String u){
  boolean success = false;
  int bytes = (u.length())/2;
  int data[bytes];
  int firstbyte = asciitoint(int(reg.charAt(0)));                                        // firstbyte of the SPI communcation sequence (see datasheet)
  int secondbyte = asciitoint(int(reg.charAt(2)))+(16*asciitoint(int(reg.charAt(1))));   // secondbyte of the sequence
  
  for(int i=bytes-1; i>=0; i--){                          // Parsing the data String "u" into blocks of 1 byte (2 HEX) data to be sent one by one to the chip via SPI
      data[i] = asciitoint(int(u.charAt(i*2+1)));         // ** maybe this can be omitted if I directly parse "u" later when I am about to send data bytes to the chip?
      data[i] += 16*(asciitoint(int(u.charAt((i*2)))));
  }
  
  switch (bytes){          // updates firstbyte with the number of bytes desired to be written
   case 1:
       firstbyte += 128;
       break;
   case 2: 
       firstbyte += 144;
       break;
   case 4:
       firstbyte += 160;
       break;
   case 8:
       firstbyte += 176;
       break;
   default:
       break;
  }
  
  digitalWrite(2, LOW);    // Starting transfer, selecting Maxim slave.
  delayMicroseconds(50); 
  
 response = SPI.transfer(firstbyte);        //  First Byte in transfer cycle.
  delayMicroseconds(100); 
  //Serial.print(response,HEX);
  //Serial.print("  ");
  
  if(response==0xC1) {                      //  If first response is 0xC1, then send
    response = SPI.transfer(secondbyte);          //  second Byte in transfer cycle.
    delayMicroseconds(100); 
    //Serial.print(response,HEX);
    //Serial.print("  ");
    
    if(response==0xC2) {      //  If second response is 0xC2, then send data bytes
      response = SPI.transfer(data[bytes-1]);
      delayMicroseconds(100); 
      //Serial.print(response,HEX);
      //Serial.print("  ");
      
      int y = bytes-2;
        for(y; y>=0; y--){        // Loops through until all data is sent (1 byte at a time)
          if(response==0x41){
            response = SPI.transfer(data[y]);
            delayMicroseconds(100); 
            //Serial.print(response,HEX);
            //Serial.print("  ");
          }
          else{
            y = 10;
          }          
        }
              
              
                  if(response==0x41){      //all data bytes got in
                    response = SPI.transfer(0x00);        //  After sending data, send dummy bytes to wait for Maxim to finish writing
                    delayMicroseconds(100); 
                    //Serial.print(response,HEX);
                    //Serial.print("  ");
                    
                          while (response==0x4E){             //   If dummy response is NACK (0x4E), keep looping until Maxim finishes writing.
                            response = SPI.transfer(0x00);
                            delayMicroseconds(100); 
                            //Serial.print(response,HEX);
                            //Serial.print("  ");
                          }
                          
                      if (response==0x41){                   //   If dummy response is ACK (0x41),
                        //Serial.print("Finished writing");    //   then it's finished writing.
                        success = true;        
                      }
                      else{
                        //Serial.print("Some weird response after being busy writing:  ");    //   If (ACK or NACK) is not recieved, then Maxim is doing something weird in the transfer cycle after it's done writing.
                        //Serial.print(response,HEX);
                      }
                  }
                  else{
                    //Serial.print("Data did not go in successfully");
                  }
                  
   }      
   else{
      //Serial.println("fail second");    //  Did not recieve 0xC2 from Maxim after sending second byte.
   }
  }
  else{
    // Serial.println("fail first");      //  Did not recieve 0xC1 from Maxim after sending first byte.
  }
                           
  
  
    digitalWrite(2, HIGH);  //  De-selecting Maxim as slave, ending transmission.
    //Serial.println("");
    delayMicroseconds(50); 

  return success;
  
}

