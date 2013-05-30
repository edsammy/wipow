#include <SPI.h>

int counter, harm_order;
long response;
float current[4];  // 0-A  1-B  2-C  3-N
float voltage[3];  // 0-A  1-B  2-C
float voltage_a_harm[10];
float current_a_harm[10];
float realp[4];    // 0-A  1-B  2-C  3-T
float reactivep[4];// 0-A  1-B  2-C  3-T
float apparentp[4];// 0-A  1-B  2-C  3-T
float reale[4];    // 0-A  1-B  2-C  3-T
float reactivee[4];// 0-A  1-B  2-C  3-T
float apparente[4];// 0-A  1-B  2-C  3-T
long upper_read, lower_read;

void setup() {
  counter = 0;
  harm_order = 0;
  
  for(int a=0;a<10;a++){
   voltage_a_harm[a]=0; 
   current_a_harm[a]=0; 
  }
  
  Serial.begin(115200);
  
  pinMode(2,OUTPUT);        //SSEL of Maxim, LOW to select it as slave.
  
  pinMode(3,OUTPUT);        //RESET of Maxim, initial reset.
  reset();
 
  startSPI();  
  
  initial_register_writes();

}

void reset(){
  digitalWrite(3, LOW);
  delay(1000);
  digitalWrite(3, HIGH);
  delay(1000);
}

void startSPI(){
  Serial.println("");
  Serial.println("starting SPI");
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16);   //  16MHz/64 = 250KHz
  SPI.setDataMode(SPI_MODE0);
  delay(3000);  
}

void delaybetweenbytes(){
  delayMicroseconds(100); 
}

float eight_to_four_bytes(long upper, long lower){
  long a = (unsigned long)lower >> 16;
  long b = upper << 16;
  float ret = float(a|b);
  return ret;
}

void loop() {
 
  if(Serial.available()) {
    String address;
    for(int i=0;i<3;i++) {
      if(Serial.available()){
        address += char(Serial.read());
      }
      else
       i--;
    }
    Serial.println(address);
    MaximRead(address,8);
  }

  
  if(counter < 50){
    normalread(harm_order, counter);
    counter++;
  }
  else{
      counter = 0;
    if(harm_order < 9){
      harm_order++;
    }
    else{
      harm_order = 0;
    }
  } 
}


void initial_register_writes(){
/*Serial.print("writing to current offs_lo A:  ");
  while (!MaximWrite("13C", "8000")){    //reg 13C
    Serial.print("writing to current offs_lo A:  ");
  }
  
  Serial.print("writing to current offs_lo B:  ");
  while (!MaximWrite("228", "8000")){    //reg 228
    Serial.print("writing to current offs_lo B:  ");
  }
  
  Serial.print("writing to current offs_lo C:  ");
  while (!MaximWrite("314", "8000")){    //reg 314
    Serial.print("writing to current offs_lo C:  ");
  }*/
  /*
  Serial.print("writing to CYCNT:  ");
  while (!MaximWrite("01C", "0004")){    //reg 01C
    Serial.print("writing to CYCNT:  ");
  }
  */
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
  while (!MaximWrite("130", "41AA")){    //reg 130
    Serial.print("writing to current gain A:  ");
  }
    
  Serial.print("writing to current gain B:  ");
  while (!MaximWrite("21C", "3F96")){    //reg 21C
    Serial.print("writing to current gain B:  ");
  }
    
  Serial.print("writing to current gain C:  ");
  while (!MaximWrite("308", "3F96")){    //reg 308
    Serial.print("writing to current gain C:  ");
  }
    
  Serial.print("writing to current gain N:  ");
  while (!MaximWrite("12E", "4427")){    //reg 12E
    Serial.print("writing to current gain N:  ");
  }

  Serial.print("writing to volt conversion coeff:  ");
  while (!MaximWrite("014", "81F1")){    //reg 014   81f1
    Serial.print("writing to volt conversion coeff:  ");
  }
  
    Serial.print("writing to current conversion coeff:  ");
  while (!MaximWrite("016", "0394")){    //reg 016
    Serial.print("writing to current conversion coeff:  ");
  }
  
    Serial.print("writing to power conversion coeff:  ");
  while (!MaximWrite("018", "07BD")){    //reg 018
    Serial.print("writing to power conversion coeff:  ");
  }
  /*
   Serial.print("writing to averaging filter:  ");
  while (!MaximWrite("030", "0000")){      //reg 030
    Serial.print("writing to averaging filter:  ");
  }
  */
  Serial.print("writing to SCAN_IN:  ");
  while (!MaximWrite("00E", "68")){      //reg 00E
    Serial.print("writing to SCAN_IN:  ");
  }
  
  Serial.print("writing to harmonic reg:  ");
  while (!MaximWrite("010", "03C3")){      //reg 010
    Serial.print("writing to harmonic reg:  ");
  }
  
  Serial.print("reading ADC Rate Mirror:  ");
  MaximRead("04E",2);
  
  Serial.print("writing to ADC Rate Mirror:  ");
  while (!MaximWrite("04E", "0167")){      //reg 04E
    Serial.print("writing to ADC Rate Mirror:  ");
  }

  Serial.print("Copying Mirror registers to Hardware registers:  ");
  while (!MaximRead("900",1)){
    Serial.print("Copying Mirror registers to Hardware registers:  ");
  }

  
  Serial.println("Hardware registers updated....checking hardware registers...");
  
  Serial.print("Copying Hardware registers to Mirror registers:  ");
  while (!MaximRead("A00",1)){
    Serial.print("Copying Hardware registers to Mirror registers:  ");
  }
  
  Serial.println("Mirror registers now contain hardware values...");
  
  Serial.print("Reading ADC Rate Mirror:  ");
  while (!MaximRead("04E", 2)){      //reg 04E
    Serial.print("Reading ADC Rate Mirror:  ");
  }
 }


boolean MaximRead(String reg,int read_i) {
   boolean success = false;
   upper_read = 0;
   lower_read = 0;
  int firstbyte = asciitoint(int(reg.charAt(0)));
  int secondbyte = asciitoint(int(reg.charAt(2)))+(16*asciitoint(int(reg.charAt(1))));

 unsigned long intresult=0;
 unsigned long intresult2=0;
 unsigned long interm=0;
 unsigned long interm2=0;
 float endresult = 0;
 float endresult2 = 0;

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
    
    if(response==0xC2) {                      //  If second response is 0xC2, then send dummy byte to wait for ACK (0x41)
      response = SPI.transfer(0x01);
      delaybetweenbytes();
      //Serial.print(response,HEX);
      //Serial.print("  ");
      
          while (response==0x4E){             //   If dummy response is NACK (0x4E), keep looping.
            response = SPI.transfer(0x00);
            delaybetweenbytes();
            //Serial.print(response,HEX);
            //Serial.print("  ");
          }
          
            if (response==0x41){ 
              for(int i = 0; i<read_i ; i++){              //   If dummy response is ACK (0x41),
                response = SPI.transfer(0x00);            //   Send another dummy byte, and recieve data.
                delaybetweenbytes();
                //Serial.print(response,HEX);
                //Serial.print("\t");
                if (i<4){
                 lower_read = lower_read | (response << (i*8)); 
                }
                else{
                 upper_read = upper_read | (response << ((i-4)*8)); 
                }
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
              }
              intresult2 = ~intresult;
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
              /*
              Serial.print(endresult);
              Serial.print("\t");
              Serial.print(endresult2);
              Serial.print("\t");
              Serial.print(intresult);
              */
              success = true;                
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
    delayMicroseconds(50);
    //Serial.println("");
      
  return success;
}



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



void normalread(int order, int counter){  
 
 long a = 0;  
  while(a==0){
   MaximRead("004",2);
   a = lower_read << 17;
   a = (unsigned long)a >> 31;
   delayMicroseconds(500); 
   //Serial.print("a:  ");
   //Serial.println(a,BIN);
  }
  
  
  if(counter==0){
    Serial.println("");
    Serial.print("***********New Repetition with order=");
    Serial.print(order);
    Serial.println("************");
    switch (order){
         case 0:
             while (!MaximWrite("00E", "60")){}
             while (!MaximWrite("010", "0041")){}    
             break;
         case 1:
             //while (!MaximWrite("00E", "68")){}
             while (!MaximWrite("010", "01C3")){}    
             break;
         case 2: 
             //while (!MaximWrite("00E", "68")){}
             while (!MaximWrite("010", "02C3")){}    
             break;
         case 3:
             //while (!MaximWrite("00E", "68")){}
             while (!MaximWrite("010", "03C3")){}    
             break;
         case 4:
             //while (!MaximWrite("00E", "68")){}
             while (!MaximWrite("010", "04C3")){}    
             break;
         case 5:
             //while (!MaximWrite("00E", "68")){}
             while (!MaximWrite("010", "05C3")){}    
             break;
         case 6:
             //while (!MaximWrite("00E", "68")){}
             while (!MaximWrite("010", "06C3")){}    
             break;
         case 7: 
             //while (!MaximWrite("00E", "68")){}
             while (!MaximWrite("010", "07C3")){}    
             break;
         case 8:
             //while (!MaximWrite("00E", "68")){}
             while (!MaximWrite("010", "08C3")){}    
             break;
         case 9:
             //while (!MaximWrite("00E", "68")){}
             while (!MaximWrite("010", "09C3")){}    
             break;
         default:
             break;
     }
  }
  
 /*
  
  Serial.print("voltages: ");
  Serial.print("A: ");
  
  MaximRead("831",8);
  voltage[0] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(voltage[0]/1000000000);
  
  Serial.print("   ");
  Serial.print("B: ");
  MaximRead("832",8);
  voltage[1] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(voltage[1]/1000000000);
  
  Serial.print("   ");
  Serial.print("C: ");
  MaximRead("834",8);
  voltage[2] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(voltage[2]/1000000000);

    
  
  Serial.print("     ");
  Serial.print("Currents: A: ");
  MaximRead("841",8);
  current[0] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(current[0]/1000000000);
  
  Serial.print("   ");
  Serial.print("B: ");
  MaximRead("842",8);
  current[1] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(current[1]/1000000000);
  
  Serial.print("   ");
  Serial.print("C: ");
  MaximRead("844",8);
  current[2] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(current[2]/1000000000);
  
  Serial.print("   ");
  Serial.print("N: ");
  MaximRead("840",8);
  current[3] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(current[3]/1000000000);
  
  
  Serial.print("     ");
  Serial.print("Apparent Powers: A: ");
  MaximRead("821",8);
  apparentp[0] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(apparentp[0]/1000000000);
  
  Serial.print("   ");
  Serial.print("B: ");
  MaximRead("822",8);
  apparentp[1] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(apparentp[1]/1000000000);
  
  Serial.print("   ");
  Serial.print("C: ");
  MaximRead("824",8);
  apparentp[2] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(apparentp[2]/1000000000);
  
  Serial.print("   ");
  Serial.print("T: ");
  MaximRead("827",8);
  apparentp[3] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(apparentp[3]/1000000000);
  
  Serial.print("     ");
  Serial.print("Power Factor: A: ");
  MaximRead("1C6",2);
  float pf = float(lower_read)/16384;
  Serial.println(pf);
  //Serial.print(lower_read);
  
*/

if(order==0){
  MaximRead("840",8);
  current[3] = eight_to_four_bytes(upper_read, lower_read)*(65536);
}
else{
  MaximRead("840",8);
  current_a_harm[order] = eight_to_four_bytes(upper_read, lower_read)*(65536); 
}

 for(int i=0;i<10;i++){
   if(i==0){
     Serial.print("N_current: ");
     Serial.print(current[3]/1000000000);
     Serial.print("   ");
   }
   else{
     Serial.print("harm ");
     Serial.print(i);
     Serial.print(": ");
     Serial.print(current_a_harm[i]/1000000000);
     Serial.print("   ");
   }
 }
  
  while (!MaximWrite("004", "0000")){}
  
  Serial.println("");
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
        else{
          while(true){};
        }
}

