#include <SPI.h>
long response;

void setup() {
  Serial.begin(9600);
  
  pinMode(2,OUTPUT);        //SSEL of Maxim, LOW to select it as slave.
  
  pinMode(3,OUTPUT);        //RESET of Maxim, initial reset.
  reset();
 
  startSPI();  
}

void reset(){
  digitalWrite(3, LOW);
  delay(1000);
  digitalWrite(3, HIGH);
  delay(1000);
}

void startSPI(){
  Serial.println("starting SPI");
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV64);   //  16MHz/64 = 250KHz
  SPI.setDataMode(SPI_MODE0);
  delay(2000);  
  
  /*Serial.print("writing to voltage gain:  ");
  while (!MaximWrite_TwoByte(145, 50, 0, 64)){    //reg 132
    Serial.print("writing to voltage gain:  ");
  }
  
  Serial.print("writing to volt conversion coeff:  ");
  while (!MaximWrite_TwoByte(144, 20, 252, 6)){    //reg 014
    Serial.print("writing to volt conversion coeff:  ");
  }*/
    
  Serial.print("writing to CYCNT:  ");
  while (!MaximWrite("01C", "0004")){    //reg 01C
    Serial.print("writing to CYCNT:  ");
  }
  
  Serial.print("writing to voltage gain A:  ");
  while (!MaximWrite("132", "3F25")){    //reg 132
    Serial.print("writing to voltage gain A:  ");
  }
  
    Serial.print("writing to voltage gain B:  ");
  while (!MaximWrite("21E", "3F25")){    //reg 21E
    Serial.print("writing to voltage gain B:  ");
  }
  
    Serial.print("writing to voltage gain C:  ");
  while (!MaximWrite("30A", "3F25")){    //reg 30A
    Serial.print("writing to voltage gain C:  ");
  }
  
  Serial.print("writing to current gain A:  ");
  while (!MaximWrite("130", "3B60")){    //reg 130
    Serial.print("writing to current gain A:  ");
  }
    
  Serial.print("writing to current gain B:  ");
  while (!MaximWrite("21C", "3B60")){    //reg 21C
    Serial.print("writing to current gain A:  ");
  }
    
  Serial.print("writing to current gain C:  ");
  while (!MaximWrite("308", "3B60")){    //reg 308
    Serial.print("writing to current gain A:  ");
  }
    
  Serial.print("writing to current gain N:  ");
  while (!MaximWrite("12E", "3B60")){    //reg 12E
    Serial.print("writing to current gain A:  ");
  }

  Serial.print("writing to volt conversion coeff:  ");
  while (!MaximWrite("014", "81F1")){    //reg 014   81f1
    Serial.print("writing to volt conversion coeff:  ");
  }
  
    Serial.print("writing to current conversion coeff:  ");
  while (!MaximWrite("016", "0394")){    //reg 016
    Serial.print("writing to volt conversion coeff:  ");
  }
  
    Serial.print("writing to power conversion coeff:  ");
  while (!MaximWrite("018", "07BD")){    //reg 018
    Serial.print("writing to volt conversion coeff:  ");
  }
  
   Serial.print("writing to averaging filter:  ");
  while (!MaximWrite("030", "0000")){      //reg 030
    Serial.print("writing to averaging filter:  ");
  }
  
  /*Serial.print("writing to ADC Rate Mirror:  ");
  while (!MaximWrite("04E", "002C")){      //reg 04E
    Serial.print("writing to ADC Rate Mirror:  ");
  }
  
  Serial.print("writing to ADC acquisition Mirror:  ");
  while (!MaximWrite("050", "05")){      //reg 050
    Serial.print("writing to ADC acquisition Mirror:  ");
  }
  
  Serial.print("Copying Mirror registers to Hardware registers:  ");
  while (!MaximRead(2304,1)){
    Serial.print("Copying Mirror registers to Hardware registers:  ");
  }
  
  Serial.println("Hardware registers updated....checking hardware registers...");
  
  Serial.print("Copying Hardware registers to Mirror registers:  ");
  while (!MaximRead(2560,1)){
    Serial.print("Copying Hardware registers to Mirror registers:  ");
  }
  
  Serial.println("Mirror registers now contain hardware values...");
  
  Serial.print("Reading ADC Rate Mirror:  ");
  while (!MaximRead(78, 2)){      //reg 04E
    Serial.print("Reading ADC Rate Mirror:  ");
  }
  
  Serial.print("Reading ADC acquisition Mirror:  ");
  while (!MaximRead(80, 2)){      //reg 050
    Serial.print("Reading ADC acquisition Mirror:  ");
  }*/
  
}

void delaybetweenbytes(){
  delayMicroseconds(300); 
}


boolean MaximRead(int reg,int read_i) {
 boolean success = false;
 unsigned long intresult=0;
 unsigned long intresult2=0;
 unsigned long interm=0;
 unsigned long interm2=0;
 float endresult = 0;
 float endresult2 = 0;

 int firstbyte = reg >> 8;
 int secondbyte = reg & 255;
 switch (read_i){
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
 
 //Serial.println(firstbyte, HEX);
 //Serial.println(secondbyte, HEX);
 
  //digitalWrite(2, HIGH);      //Reasserting SSEL for new transfer cycle.
  //delay(1);
  //digitalWrite(2, LOW);
  //delay(1);
  digitalWrite(2, LOW);    // Starting transfer, selecting Maxim slave.
  delay(1);
  
 
  
 
    response = SPI.transfer(firstbyte);        //  First Byte in transfer cycle.
    delaybetweenbytes();
    Serial.print(response,HEX);
    Serial.print("  ");
  
  if(response==0xC1) {                      //  If first response is 0xC1, then send
    response = SPI.transfer(secondbyte);          //  second Byte in transfer cycle.
    delaybetweenbytes();
    Serial.print(response,HEX);
    Serial.print("  ");
    
    if(response==0xC2) {                      //  If second response is 0xC2, then send dummy byte to wait for ACK (0x41)
      response = SPI.transfer(0x01);
      delaybetweenbytes();
      Serial.print(response,HEX);
      Serial.print("  ");
      
          while (response==0x4E){             //   If dummy response is NACK (0x4E), keep looping.
            response = SPI.transfer(0x00);
            delaybetweenbytes();
            Serial.print(response,HEX);
            Serial.print("  ");
          }
          
            if (response==0x41){ 
              for(int i = 0; i<read_i ; i++){              //   If dummy response is ACK (0x41),
                response = SPI.transfer(0x00);            //   Send another dummy byte, and recieve data.
                delaybetweenbytes();
                Serial.print(response,HEX);
                Serial.print("\t");
                if (i>1&&i<6){
                  //interm = response << ((i-1)*8);
                  //intresult = intresult | interm;
                  interm = response << ((i-2)*8);
                  intresult = intresult | interm;
                }
              }
              intresult2 = ~intresult;
              intresult2++;
              endresult = float(intresult)*(65536);  
              endresult2 = float(intresult2)*(65536);        
              //endresult = endresult/100000;
              endresult = endresult/1000000000;
              endresult2 = endresult2/1000000000;
              Serial.print(endresult);
              Serial.print("\t");
              Serial.print(endresult2);
              Serial.print("\t");
              success = true;  
              Serial.print(intresult);
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
  
    digitalWrite(2, HIGH);  //  De-selecting Maxim as slave, ending transmission.
    delay(1);
    Serial.println("");
    //delay(0400);            //  "timing-out" for now after each transfer cycle. *(not sure it's doing much)
    
    
    
    
    
  /*digitalWrite(2,HIGH); // deactivate slave selection
  delay(100);
  digitalWrite(2,LOW); // activate slave selection*/
  
  return success;
}

int asciitoint(int c){
        if(c<58&&c>47) { // 0-9
          return (c-48);
        }
        else if(c<71&&c>64) { // A-F
          return (c-55);
        }
        else if(c<103&&c>96) { // a-f
          return (c-87);
        }
}
    
boolean MaximWrite(String reg, String u){
  boolean success = false;
  int bytes = (u.length())/2;
  int data[bytes];
  int firstbyte = asciitoint(int(reg.charAt(0)));
  int secondbyte = asciitoint(int(reg.charAt(2)))+16*asciitoint(int(reg.charAt(1)));
  
  for(int i=bytes-1; i>=0; i--){
      int c = int(u.charAt(i*2+1));
      int a = bytes-i;
      data[a] = asciitoint(c);
      c = int(u.charAt((i*2)));
      data[a] += 16*(asciitoint(c));
  }
  
  switch (bytes){
   case 1:
       firstbyte += 128;
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
  delay(1);
  
  
 response = SPI.transfer(firstbyte);        //  First Byte in transfer cycle.
  delaybetweenbytes();
  Serial.print(response,HEX);
  Serial.print("  ");
  
  if(response==0xC1) {                      //  If first response is 0xC1, then send
    response = SPI.transfer(secondbyte);          //  second Byte in transfer cycle.
    delaybetweenbytes();
    Serial.print(response,HEX);
    Serial.print("  ");
    
    if(response==0xC2) {      //  If second response is 0xC2, then send data bytes
      response = SPI.transfer(data[0]);
      delaybetweenbytes();
      Serial.print(response,HEX);
      Serial.print("  ");
      
        for(int y = 1; y<bytes; y++){
          if(response==0x41){
            response = SPI.transfer(data[y]);
            delaybetweenbytes();
            Serial.print(response,HEX);
            Serial.print("  ");
          }
          else{
            y = 10;
          }          
        }
              
              
                  if(response==0x41){      //all data bytes got in
                    response = SPI.transfer(0x00);        //  After sending data, send dummy bytes to wait for Maxim to finish writing
                    delaybetweenbytes();
                    Serial.print(response,HEX);
                    Serial.print("  ");
                    
                          while (response==0x4E){             //   If dummy response is NACK (0x4E), keep looping until Maxim finishes writing.
                            response = SPI.transfer(0x00);
                            delaybetweenbytes();
                            Serial.print(response,HEX);
                            Serial.print("  ");
                          }
                          
                      if (response==0x41){                   //   If dummy response is ACK (0x41),
                        Serial.print("Finished writing");    //   then it's finished writing.
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
    Serial.println("");
    delay(1);

  return success;
  
}

/*boolean MaximWrite(int x, int y, int u, int v){
  boolean success = false;
  //digitalWrite(2, HIGH);      //Reasserting SSEL for new transfer cycle.
  //delay(1);
  //digitalWrite(2, LOW);
  //delay(1);
  digitalWrite(2, LOW);    // Starting transfer, selecting Maxim slave.
  delay(1);
  
  response = SPI.transfer(x);        //  First Byte in transfer cycle.
  delaybetweenbytes();
  Serial.print(response,HEX);
  Serial.print("  ");
  
  if(response==0xC1) {                      //  If first response is 0xC1, then send
    response = SPI.transfer(y);          //  second Byte in transfer cycle.
    delaybetweenbytes();
    Serial.print(response,HEX);
    Serial.print("  ");
    
    if(response==0xC2) {                          //  If second response is 0xC2, then send
      //for(int i=0; i<write_i; i++){             //  data bytes to wait for ACK (0x41)
        response = SPI.transfer(u);            //  **********Data to be written (FIRST BYTE)
        delaybetweenbytes();
        Serial.print(response,HEX);
        Serial.print("  ");
      //}
      
           if(response==0x41){    //First byte got in.
              response = SPI.transfer(v);      //   Send second written byte.
              delaybetweenbytes();
              Serial.print(response,HEX);
              Serial.print("  ");
              
              
                  if(response==0x41){      //Second byte got in.
                    response = SPI.transfer(0x00);        //  After sending data, send dummy bytes to wait for Maxim to finish writing
                    delaybetweenbytes();
                    Serial.print(response,HEX);
                    Serial.print("  ");
                    
                          while (response==0x4E){             //   If dummy response is NACK (0x4E), keep looping until Maxim finishes writing.
                            response = SPI.transfer(0x00);
                            delaybetweenbytes();
                            Serial.print(response,HEX);
                            Serial.print("  ");
                          }
                          
                      if (response==0x41){                   //   If dummy response is ACK (0x41),
                        Serial.print("Finished writing");    //   then it's finished writing.
                        success = true;        
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
  
    digitalWrite(2, HIGH);  //  De-selecting Maxim as slave, ending transmission.
    Serial.println("");
    delay(1);
    //delay(0100);            //  "timing-out" for now after each transfer cycle.
                              //  ** For some reason it won't write if delay is too short.
    
    
    
    
  /*digitalWrite(2,HIGH); // deactivate slave selection
  delay(100);
  digitalWrite(2,LOW); // activate slave selection

  return success;
}*/

void loop() {
  if(Serial.available()) { // start building an address
    int address = 0;
    for(int i=0;i<=2;i++) {
      if(Serial.available()) {
        int c = int(Serial.read());
        if(c<58&&c>47) { // 0-9
          int scale = 1;
          for(int j=0;j<(2-i);j++)
            scale *= 16;
          address += (c-48)*scale;
        }
        else if(c<71&&c>64) { // A-F
          int scale = 1;
          for(int j=0;j<(2-i);j++)
            scale *= 16;
          address += (c-55)*scale;
        }
        else if(c<103&&c>96) { // a-f
          int scale = 1;
          for(int j=0;j<(2-i);j++)
            scale *= 16;
          address += (c-87)*scale;
        }
      }
      else
        i--; // reverse the i++
    }
    Serial.println(address,HEX);
    
        for(int i=0;i<12;i++)
          MaximRead(address,8);   
  }

  //MaximRead(2113,8);    // virtual reg 841 (A.RMS current)
  MaximRead(2114,8);    // virtual reg 842 (B.RMS current)
  //MaximRead(2116,8);    // virtual reg 844 (C.RMS current)
  //MaximRead(2097,8);    // virtual reg 831 (A.RMS voltage)
  //MaximRead(2098,8);    // virtual reg 832 (B.RMS voltage)
  //MaximRead(2100,8);    // virtual reg 834 (C.RMS voltage)
  //MaximRead(456,4);  // reg 1C8 (A.VRMS metering units)
  //MaximRead(460,4);  // reg 1CC (A.IRMS metering units)
  
  //Serial.println("end of loop");
  delay(5);
  //Serial.print("writing:  ");
  //while (!MaximWrite_TwoByte()){
  //  Serial.print("writing:  ");
  //}
  
  //Serial.println("end of loop");
  
}
