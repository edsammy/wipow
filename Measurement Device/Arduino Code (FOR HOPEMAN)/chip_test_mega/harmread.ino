// This function reads the Harmonic-related data: THDN's and Current/Voltage Harmonic RMS.
//  Voltage/Current Harmonic RMS	(LSB is 10^-9 Volts or Amps, i.e. nano-volts/amps)
//	THDN (Check datasheet for LSB)
 
void harmread(){ 
 while (!MaximWrite("00E", "68")){};	// Disable Neutral Current ADC to not sample Neutral Current for DSP Cycles. (This is not needed? Does it really save time?)
 while (!MaximWrite("030", "0000")){};	// Again disabling the averaging filter. (It this necessary since it is already called in initial_register_writes?)
										// (Highly doubt that this does anything much, but it shouldn't harm anything)
										// (SEE TODOs in chip_test_mega.ino)
										
 /****The Start of the MySQL statement. (USED WHEN "normalread()" is disabled/commented out, and only harmread() is called)****/
 //char mysqlheader[] = "insert into hopeman (thda,cha1,cha2,cha3,cha4,cha5,vha1,vha2,vha3,vha4,vha5,thdb,chb1,chb2,chb3,chb4,chb5,vhb1,vhb2,vhb3,vhb4,vhb5,thdc,chc1,chc2,chc2,chc3,chc4,chc5,vhc1,vhc2,vhc3,vhc4,vhc5) values(";
 //SpiSerial.write(mysqlheader, String(mysqlheader).length());
 
 SpiSerial.write(",",1);	// This is to continue the MySQL statement from normalread(). ***Should be disabled if only harmread() is called!
 
 dspready(); 
 
  float total_thd = 0; // Temp variables for finializing the output value of THDN's
  float lower_thd = 0;

  
/**** The steps/code for each phase's current and voltage harmonic reads are the same *****/
////////////////////////////////////////// Current A Harm ////////////////////////////////////////////////////////
byte order = 1;
for(order=1;order<6;order++){    //Current A Harm
  switch (order){
         case 1:	// For order=1, both first harmonic and THDN (if current) is read in the same loop cycle. This is possible because they are in two separate measurement registers.
		 
             while (!MaximWrite("010", "01C3")){};	// First change the harmonic (auxiliary) channel config register (SEE MAXIM DATASHEET FOR REGISTER 0x010)
             dspready();							// 3 dspready()'s  (SEE TODOs and April 5, 2013 update)
             dspready();
             dspready();
                  MaximRead("859",8);				// Then read the measurement register.
                  thd[0] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))/(65536);
                  dtostrf(thd[0],10,3,buf);
                  SpiSerial.write(buf);
                  SpiSerial.write(",",1);
             break;
         case 2: 
             while (!MaximWrite("010", "02C3")){};
             //delay(700);
             dspready();
             dspready();
             break;
         case 3:
             while (!MaximWrite("010", "03C3")){};
             //delay(700);
             dspready();
             dspready();
             break;
         case 4:
             while (!MaximWrite("010", "04C3")){};
             //delay(700);
             dspready();
             dspready();
             break;
         case 5:
             while (!MaximWrite("010", "05C3")){};
             //delay(700);
             dspready();
             dspready();
             break;
         default:
             break;
     }
  
  dspready();
  MaximRead("840",8);		// Reading the measurement register for current harmonics
  current_a_harm[order] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))*(65536)/1000000000;  // All LSBs are 10^-9
  

  dtostrf(current_a_harm[order],10,3,buf);	// Change float value to string to be transmitted as part of MySQL statement
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
}
////////////////////////////////////////// End of Current A Harm ////////////////////////////////////////////////////////////////////////




////////////////////////////////////////// Voltage A Harm ////////////////////////////////////////////////////////////////////////
order = 1;
for(order=1;order<6;order++){  // Voltage A Harm
  switch (order){
         case 1:
             while (!MaximWrite("010", "01C2")){};
             //delay(1000);
             dspready();
             dspready();
             break;
         case 2: 
             while (!MaximWrite("010", "02C2")){};
             //delay(1000);
             dspready();
             dspready();
             break;
         case 3:
             while (!MaximWrite("010", "03C2")){};
             //delay(1000);
             dspready();
             dspready();
             break;
         case 4:
             while (!MaximWrite("010", "04C2")){};
             //delay(1000);    
             dspready();
             dspready();
             break;
         case 5:
             while (!MaximWrite("010", "05C2")){};
             //delay(1000);  
             dspready();
             dspready();
             break;
         default:
             break;
     }
  
  dspready();
  MaximRead("830",8);
  voltage_a_harm[order] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))*(65536)/1000000000; 
  
  dtostrf(voltage_a_harm[order],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
}
////////////////////////////////////////// End of Voltage A Harm ////////////////////////////////////////////////////////////////////////




////////////////////////////////////////// Current B Harm ////////////////////////////////////////////////////////////////////////
order = 1;
for(order=1;order<6;order++){    // Current B Harm
  switch (order){
         case 1:
             while (!MaximWrite("010", "01C5")){};
             dspready();
             dspready();
             dspready();
                    MaximRead("85A",8);
                    thd[1] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))/(65536);
                    dtostrf(thd[1],10,3,buf);
                    SpiSerial.write(buf);
                    SpiSerial.write(",",1);
             break;
         case 2: 
             while (!MaximWrite("010", "02C5")){};
             //delay(700);
             dspready();
             dspready();
             break;
         case 3:
             while (!MaximWrite("010", "03C5")){};
             //delay(700);
             dspready();
             dspready();
             break;
         case 4:
             while (!MaximWrite("010", "04C5")){};
             //delay(700);    
             dspready();
             dspready();
             break;
         case 5:
             while (!MaximWrite("010", "05C5")){};
             //delay(700);  
             dspready();
             dspready();
             break;
         default:
             break;
     }
  
  dspready();
  MaximRead("840",8);
  current_b_harm[order] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))*(65536)/1000000000; 
  
  dtostrf(current_b_harm[order],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
}
////////////////////////////////////////// End of Current B Harm ////////////////////////////////////////////////////////////////////////





////////////////////////////////////////// Voltage B Harm ////////////////////////////////////////////////////////////////////////
order = 1;
for(order=1;order<6;order++){  // Voltage B Harm
  switch (order){
         case 1:
             while (!MaximWrite("010", "01C4")){};
             //delay(1000);
             dspready();
             dspready();
             break;
         case 2: 
             while (!MaximWrite("010", "02C4")){};
             //delay(1000);
             dspready();
             dspready();
             break;
         case 3:
             while (!MaximWrite("010", "03C4")){};
             //delay(1000);
             dspready();
             dspready();
             break;
         case 4:
             while (!MaximWrite("010", "04C4")){};
             //delay(1000);    
             dspready();
             dspready();
             break;
         case 5:
             while (!MaximWrite("010", "05C4")){};
             //delay(1000);  
             dspready();
             dspready();
             break;
         default:
             break;
     }
  
  dspready();
  MaximRead("830",8);
  voltage_b_harm[order] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))*(65536)/1000000000; 
  
  dtostrf(voltage_b_harm[order],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
}
////////////////////////////////////////// End of Voltage B Harm ////////////////////////////////////////////////////////////////////////



////////////////////////////////////////// Current C Harm ////////////////////////////////////////////////////////////////////////
order = 1;
for(order=1;order<6;order++){    // Current C Harm
  switch (order){
         case 1:
             while (!MaximWrite("010", "01C7")){};
             dspready();
             dspready();
             dspready();
                      MaximRead("85C",8);
                      thd[2] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))/(65536);
                      dtostrf(thd[2],10,3,buf);
                      SpiSerial.write(buf);
                      SpiSerial.write(",",1);
             break;
         case 2: 
             while (!MaximWrite("010", "02C7")){};
             //delay(700);
             dspready();
             dspready();
             break;
         case 3:
             while (!MaximWrite("010", "03C7")){};
             //delay(700); 
             dspready();
             dspready();
             break;
         case 4:
             while (!MaximWrite("010", "04C7")){};
             //delay(700);   
             dspready();  
             dspready();
             break;
         case 5:
             while (!MaximWrite("010", "05C7")){};
             //delay(700);  
             dspready();
             dspready();
             break;
         default:
             break;
     }
  
  dspready();
  MaximRead("840",8);
  current_c_harm[order] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))*(65536)/1000000000; 

  dtostrf(current_c_harm[order],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
}
////////////////////////////////////////// End of Current C Harm ////////////////////////////////////////////////////////////////////////





////////////////////////////////////////// Voltage C Harm ////////////////////////////////////////////////////////////////////////
order = 1;
for(order=1;order<6;order++){  // Voltage C Harm
  switch (order){
         case 1:
             while (!MaximWrite("010", "01C6")){};
             //delay(1000);
             dspready();
             dspready();
             break;
         case 2: 
             while (!MaximWrite("010", "02C6")){};
             //delay(1000);
             dspready();
             dspready();
             break;
         case 3:
             while (!MaximWrite("010", "03C6")){};
             //delay(1000);
             dspready();
             dspready();
             break;
         case 4:
             while (!MaximWrite("010", "04C6")){};
             //delay(1000);    
             dspready();
             dspready();
             break;
         case 5:
             while (!MaximWrite("010", "05C6")){};
             //delay(1000);  
             dspready();
             dspready();
             break;
         default:
             break;
     }
  
  dspready();
  MaximRead("830",8);
  voltage_c_harm[order] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))*(65536)/1000000000; 

  dtostrf(voltage_c_harm[order],10,3,buf);
  SpiSerial.write(buf);
  if(order < 5){
    SpiSerial.write(",",1);
  }
  else{
    SpiSerial.write(41);
    SpiSerial.write(59);
  }
}
////////////////////////////////////////// End of Voltage C Harm ////////////////////////////////////////////////////////////////////////
}

