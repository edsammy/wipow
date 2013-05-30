#include <SPI.h>
int response;

void setup() {
  Serial.begin(9600);
  pinMode(2,OUTPUT);        //SSEL of Maxim, LOW to select it as slave.
  digitalWrite(2,LOW);
  
  pinMode(3,OUTPUT);        //RESET of Maxim, initial reset.
  digitalWrite(3, LOW);
  delay(1000);
  digitalWrite(3, HIGH);
  
  Serial.println("starting SPI");
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV64);   //  16MHz/64 = 250KHz
  SPI.setDataMode(SPI_MODE0);
  delay(2000);  
}

void loop() {
 digitalWrite(2, LOW);    // Starting transfer, selecting Maxim slave.   *(not sure it's doing much)
  
  response = SPI.transfer(0x91);        //  First Byte in transfer cycle.
  delay(0001);                          //  can consider using millis();
  Serial.print(response,HEX);
  Serial.print("  ");
  
  if(response==0xC1) {                      //  If first response is 0xC1, then send
    response = SPI.transfer(0x30);          //  second Byte in transfer cycle.
    delay(0001);
    Serial.print(response,HEX);
    Serial.print("  ");
    
    if(response==0xC2) {                          //  If second response is 0xC2, then send
      //for(int i=0; i<write_i; i++){             //  data bytes to wait for ACK (0x41)
        response = SPI.transfer(0x11);            //  **********Data to be written (FIRST BYTE)
        delay(0001);
        Serial.print(response,HEX);
        Serial.print("  ");
      //}
      
           if(response==0x41){    //First byte got in.
              response = SPI.transfer(0x11);      //   Send second written byte.
              delay(0001);
              Serial.print(response,HEX);
              Serial.print("  ");
              
              
                  if(response==0x41){      //Second byte got in.
                    response = SPI.transfer(0x00);        //  After sending data, send dummy bytes to wait for Maxim to finish writing
                    delay(0001);
                    Serial.print(response,HEX);
                    Serial.print("  ");
                    
                          while (response==0x4E){             //   If dummy response is NACK (0x4E), keep looping until Maxim finishes writing.
                            response = SPI.transfer(0x00);
                            delay(0001);
                            Serial.print(response,HEX);
                            Serial.print("  ");
                          }
                          
                      if (response==0x41){                   //   If dummy response is ACK (0x41),
                        Serial.print("Finished writing");    //   then it's finished writing.
                          
                      }
                      else{
                        Serial.print("Some weird response after being busy writing:  ");    //   If (ACK or NACK) is not recieved, then Maxim is doing something weird in the transfer cycle after it's done writing.
                        Serial.print(response,HEX);
                      }
                      
                  }
                  else{
                     Serial.println("Did not recieve 0x41 after sending second data byte"); 
                  }
     }
     else{
        Serial.println("Did not recieve 0x41 after sending first data byte"); 
     }
   }      
   else{
      Serial.println("fail second");    //  Did not recieve 0xC2 from Maxim after sending second byte.
   }
  }
  else{
    Serial.println("fail first");      //  Did not recieve 0xC1 from Maxim after sending first byte.
  }
  
  
    digitalWrite(2, HIGH);  //  De-selecting Maxim as slave, ending transmission.  *(not sure it's doing much)
    Serial.println("");
    delay(0400);      
    
    

digitalWrite(2, LOW);    // Starting transfer, selecting Maxim slave.   *(not sure it's doing much)
  
  response = SPI.transfer(0x11);        //  First Byte in transfer cycle.
  delay(0001);                  //  can consider using millis();
  Serial.print(response,HEX);
  Serial.print("  ");
  
  if(response==0xC1) {                      //  If first response is 0xC1, then send
    response = SPI.transfer(0x30);          //  second Byte in transfer cycle.
    delay(0001);
    Serial.print(response,HEX);
    Serial.print("  ");
    
    if(response==0xC2) {                      //  If second response is 0xC2, then send
      response = SPI.transfer(0x01);          //  dummy byte to wait for ACK (0x41)
      delay(0001);
      Serial.print(response,HEX);
      Serial.print("  ");
      
          while (response==0x4E){             //   If dummy response is NACK (0x4E), keep looping.
            response = SPI.transfer(0x00);
            delay(0001);
            Serial.print(response,HEX);
            Serial.print("  ");
          }
          
            if (response==0x41){ 
              for(int i = 0; i<2; i++){              //   If dummy response is ACK (0x41),
                response = SPI.transfer(0x00);            //   Send another dummy byte, and recieve data.
                delay(0001);
                Serial.print(response,HEX);
                Serial.print("\t");
              }          
            }
            else{
              Serial.print("Some weird response after 0xC2:  ");    //   If (ACK or NACK) is not recieved, then Maxim is doing something weird in the transfer cycle.
              Serial.print(response,HEX);
              
            }
          
   }      
   else{
      Serial.println("fail second");    //  Did not recieve 0xC2 from Maxim after sending second byte.
   }
  }
  else{
    Serial.println("fail first");      //  Did not recieve 0xC1 from Maxim after sending first byte.
  }
  
  
    digitalWrite(2, HIGH);  //  De-selecting Maxim as slave, ending transmission.  *(not sure it's doing much)
    Serial.println("");
    delay(0400);


}
