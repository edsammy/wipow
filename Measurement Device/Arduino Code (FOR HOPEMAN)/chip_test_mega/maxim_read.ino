// This function reads from the MAXQ3183 chip.
// It goes through the SPI communciation protocol
// specificied by the chip and updates
// the registers (upper_read and lower_read)
// with the reading from the chip (8 bytes max).

//  "reg" is a String of the register in HEX to be read (ex: "1C8" to read register 0x1C8)
//  "read_i" is an integer representing the number of bytes to be read. Can be 1, 2, 4, or 8 Bytes.

//  This function RETURNS a BOOLEAN value stating if the reading is successful(true) or a failure(false).

//  Refer to the MAXQ3183 datasheet for the SPI communcation protocol: http://datasheets.maxim-ic.com/en/ds/MAXQ3183.pdf



boolean MaximRead(String reg,int read_i) {
   boolean success = false;
   upper_read = 0;
   lower_read = 0;
   
  int firstbyte = asciitoint(int(reg.charAt(0)));                                        // firstbyte of the SPI communcation sequence (see datasheet)
  int secondbyte = asciitoint(int(reg.charAt(2)))+(16*asciitoint(int(reg.charAt(1))));   // secondbyte of the sequence

/*
 unsigned long intresult=0;
 unsigned long intresult2=0;
 unsigned long interm=0;
 unsigned long interm2=0;
 float endresult = 0;
 float endresult2 = 0;
*/

 switch (read_i){          // updates firstbyte with the number of bytes desired to be read
   case 2: 
       firstbyte += 16;
       break;
   case 4:
       firstbyte += 32;
       break;
   case 8:
       firstbyte += 48;
       break;
   default:
       break;
 }

  digitalWrite(2, LOW);    // Starting transfer, selecting Maxim slave.
  delayMicroseconds(50); 
  
 
    response = SPI.transfer(firstbyte);        //  First Byte in transfer cycle.
    delayMicroseconds(100); 
    ////Serial.print(response,HEX);
    ////Serial.print("  ");
  
  if(response==0xC1) {                      //  If first response is 0xC1, then send
    response = SPI.transfer(secondbyte);          //  second Byte in transfer cycle.
    delayMicroseconds(100); 
    ////Serial.print(response,HEX);
    ////Serial.print("  ");
    
    if(response==0xC2) {                      //  If second response is 0xC2, then send dummy byte to wait for ACK (0x41)
      response = SPI.transfer(0x01);
      delayMicroseconds(100); 
      ////Serial.print(response,HEX);
      ////Serial.print("  ");
      
          while (response==0x4E){             //   If dummy response is NACK (0x4E), keep looping.
            response = SPI.transfer(0x00);
            delayMicroseconds(100); 
            ////Serial.print(response,HEX);
            ////Serial.print("  ");
          }
          
            if (response==0x41){ 
              for(int i = 0; i<read_i ; i++){              //   If dummy response is ACK (0x41),
                response = SPI.transfer(0x00);            //   Send another dummy byte, and recieve data.
                delayMicroseconds(100);
                //Serial.print(response,HEX);
                //Serial.print("\t");
                if (i<4){
                 lower_read = lower_read | (response << (i*8));       // Putting the lower read bytes in lower_read. (bytes 1 to 4 of the reading)
                }                                                     // If the reading is less than or equal to 4 bytes, then only lower_read is relevant (has data).
                else{
                 upper_read = upper_read | (response << ((i-4)*8));   // Putting the upper read bytes in upper_read. (bytes 5 to 8 of the reading)
                }
                /*
                if (read_i>5){
                  if (i>1&&i<6){
                    interm = response << ((i-2)*8);
                    intresult = intresult | interm;
                  }
                }
                else{
                  interm = response << (i*8);
                  intresult = intresult | interm;
                }
                */
              }
             /* intresult2 = ~intresult;
              intresult2++;
              if (read_i==8){
                endresult = float(intresult)*(65536);  
                endresult2 = float(intresult2)*(65536);    
                endresult = endresult/1000000000;
                endresult2 = endresult2/1000000000;    
              }
              else{
                endresult = float(intresult);  
                endresult2 = float(intresult2);
              }
              */
              /*
              //Serial.print(endresult);
              //Serial.print("\t");
              //Serial.print(endresult2);
              //Serial.print("\t");
              //Serial.print(intresult);
              */
              success = true;                
            }
            else{
              //Serial.print("Some weird response after 0xC2:  ");    //   If (ACK or NACK) is not recieved, then Maxim is doing something weird in the transfer cycle.
              //Serial.print(response,HEX);
              
            }
          
   }      
   else{
      //Serial.println("fail second");    //  Did not recieve 0xC2 from Maxim after sending second byte.
   }
  }
  else{
    //Serial.println("fail first");      //  Did not recieve 0xC1 from Maxim after sending first byte.
  }
  
    digitalWrite(2, HIGH);  //  De-selecting Maxim as slave, ending transmission.
    delayMicroseconds(50);
    //Serial.println("");
      
  return success;
}
