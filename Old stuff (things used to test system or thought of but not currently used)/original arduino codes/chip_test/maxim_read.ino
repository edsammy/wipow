#include <SPI.h>
#include <asciitoint.ino>


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

             // Serial.print(endresult);
             // Serial.print("\t");
             // Serial.print(endresult2);
            //  Serial.print("\t");
              success = true;  
             // Serial.print(intresult);
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
