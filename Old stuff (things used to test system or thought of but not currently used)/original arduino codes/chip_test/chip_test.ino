#include <SPI.h>
#include <maxim_read.ino>
#include <maxim_write.ino>
#include <asciitoint.ino>
#include <normalread.ino>
#include <initial_register_writes.ino>
long response;
float current[4];  // 0-A  1-B  2-C  3-N
float voltage[3];  // 0-A  1-B  2-C
float voltage_a_harm[6];
float current_a_harm[6];
float realp[4];    // 0-A  1-B  2-C  3-T
float reactivep[4];// 0-A  1-B  2-C  3-T
float apparentp[4];// 0-A  1-B  2-C  3-T
float reale[4];    // 0-A  1-B  2-C  3-T
float reactivee[4];// 0-A  1-B  2-C  3-T
float apparente[4];// 0-A  1-B  2-C  3-T
long upper_read, lower_read;

void setup() {
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
  delay(2000);  
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
  
  normalread();
}
