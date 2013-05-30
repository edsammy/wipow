#include <SPI.h>
#include <asciitoint.ino>


boolean MaximWrite(String reg, String u){
  boolean success = false;
  int bytes = (u.length())/2;
  int data[bytes];
  int firstbyte = asciitoint(int(reg.charAt(0)));
  int secondbyte = asciitoint(int(reg.charAt(2)))+(16*asciitoint(int(reg.charAt(1))));
  
  for(int i=bytes-1; i>=0; i--){
      int c = int(u.charAt(i*2+1));
      data[i] = asciitoint(c);
      c = int(u.charAt((i*2)));
      data[i] += 16*(asciitoint(c));
  }
  
  switch (bytes){
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
  delaybetweenbytes();
  //Serial.print(response,HEX);
  //Serial.print("  ");
  
  if(response==0xC1) {                      //  If first response is 0xC1, then send
    response = SPI.transfer(secondbyte);          //  second Byte in transfer cycle.
    delaybetweenbytes();
    //Serial.print(response,HEX);
    //Serial.print("  ");
    
    if(response==0xC2) {      //  If second response is 0xC2, then send data bytes
      response = SPI.transfer(data[bytes-1]);
      delaybetweenbytes();
      //Serial.print(response,HEX);
      //Serial.print("  ");
      
      int y = bytes-2;
        for(y; y>=0; y--){
          if(response==0x41){
            response = SPI.transfer(data[y]);
            delaybetweenbytes();
            //Serial.print(response,HEX);
            //Serial.print("  ");
          }
          else{
            y = 10;
          }          
        }
              
              
                  if(response==0x41){      //all data bytes got in
                    response = SPI.transfer(0x00);        //  After sending data, send dummy bytes to wait for Maxim to finish writing
                    delaybetweenbytes();
                    //Serial.print(response,HEX);
                    //Serial.print("  ");
                    
                          while (response==0x4E){             //   If dummy response is NACK (0x4E), keep looping until Maxim finishes writing.
                            response = SPI.transfer(0x00);
                            delaybetweenbytes();
                            //Serial.print(response,HEX);
                            //Serial.print("  ");
                          }
                          
                      if (response==0x41){                   //   If dummy response is ACK (0x41),
                        //Serial.print("Finished writing");    //   then it's finished writing.
                        success = true;        
                      }
                      else{
                        Serial.print("Some weird response after being busy writing:  ");    //   If (ACK or NACK) is not recieved, then Maxim is doing something weird in the transfer cycle after it's done writing.
                        Serial.print(response,HEX);
                      }
                  }
                  else{
                    Serial.print("Data did not go in successfully");
                  }
                  
   }      
   else{
      Serial.println("fail second");    //  Did not recieve 0xC2 from Maxim after sending second byte.
   }
  }
  else{
    Serial.println("fail first");      //  Did not recieve 0xC1 from Maxim after sending first byte.
  }
                           
  
  
    digitalWrite(2, HIGH);  //  De-selecting Maxim as slave, ending transmission.
    //Serial.println("");
    delayMicroseconds(50); 

  return success;
  
}

