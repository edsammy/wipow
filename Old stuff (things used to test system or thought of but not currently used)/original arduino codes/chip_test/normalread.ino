#include <SPI.h>
#include <maxim_read.ino>
#include <maxim_write.ino>
#include <asciitoint.ino>


void normalread(){  
  //Serial.print("writing to SCAN_IN:  ");
  //while (!MaximWrite("00E", "68")){      //reg 00E
    //Serial.print("writing to SCAN_IN:  ");
  //}
  
  Serial.print("voltages: ");
  Serial.print("A: ");
  
  MaximRead("831",8);
  voltage[0] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(voltage[0]/1000000000);
  
  //Serial.print("writing to harmonic reg:  ");
  //while (!MaximWrite("010", "01C2")){      //reg 010
    //Serial.print("writing to harmonic reg:  ");
  //}
  
  Serial.print("   ");
  Serial.print("B: ");
  MaximRead("832",8);
  voltage[1] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(voltage[1]/1000000000);
  
  //MaximRead("830",8);
  //voltage_a_harm[1] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  
    //Serial.print("writing to harmonic reg:  ");
  //while (!MaximWrite("010", "03C2")){      //reg 010
    //Serial.print("writing to harmonic reg:  ");
  //}
  
  Serial.print("   ");
  Serial.print("C: ");
  MaximRead("834",8);
  voltage[2] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  Serial.print(voltage[2]/1000000000);
  
  //MaximRead("830",8);
  //voltage_a_harm[3] = eight_to_four_bytes(upper_read, lower_read)*(65536);
  
  /*Serial.print("   ");
  Serial.print("1st: ");
  Serial.print(voltage_a_harm[1]/1000000000);
  
  Serial.print("   ");
  Serial.print("3rd: ");
  Serial.print(voltage_a_harm[3]/1000000000);*/
  

  //Serial.print("writing to harmonic reg:  ");
  while (!MaximWrite("010", "0041")){      //reg 010
    //Serial.print("writing to harmonic reg:  ");
  }
  
  //Serial.print("writing to SCAN_IN:  ");
  while (!MaximWrite("00E", "60")){      //reg 00E
    //Serial.print("writing to SCAN_IN:  ");
  }
  
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
  Serial.print(pf);
  //Serial.print(lower_read);
  
  //MaximRead("00E",1);
  
  Serial.println("");
  //Serial.println("");

  /*Serial.print("\r");
  Serial.print("\r");
  Serial.print("\r");
  Serial.print("\r");*/
  
}
