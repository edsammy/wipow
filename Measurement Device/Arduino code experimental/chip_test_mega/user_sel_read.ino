// This function reads the "normal" power values (no harmonics):  (SEE MAXIM DATASHEET FOR DETAILS ON THESE REGISTERS)
//       Line Frequency						(LSB is 1/1000 or 0.001 Hz)
//       Voltage/Current RMS				(LSB is 10^-9 Volts or Amps, i.e. nano-volts/amps)
//       Powers(Real, Reactive, Apparent)	(LSB is 10^-9 Watts or VA or VAR, i.e. nano-watts/VA/VAR)
//       PF									(See datasheet for LSBs)
//       Voltage Phasors					(LSB is 1/100 or 0.01 degrees)

int order;  // Used to indicate order of harmonic readings
boolean last;  // Used to indicate when the user selected inputs have reached the last one
String c = "840";  // MAXIM virtual registry for harmonic current readings
String v = "830";  // MAXIM virtual registry for harmonic voltage readings
String buffer;

void user_sel_read(){ 
  last = false;
  data = "";
  static void (*ptr[total_functs])() = {none,freq_r,volta_r,voltb_r,voltc_r,curra_r,currb_r,currc_r,currn_r,reala_r,realb_r,realc_r,reaca_r,reacb_r,reacc_r,appaa_r,appab_r,appac_r,pfa_r,pfb_r,pfc_r,pft_r,phaseab_r,phaseac_r,thda_r,cha1_r,cha2_r,cha3_r,cha4_r,cha5_r,vha1_r,vha2_r,vha3_r,vha4_r,vha5_r,thdb_r,chb1_r,chb2_r,chb3_r,chb4_r,chb5_r,vhb1_r,vhb2_r,vhb3_r,vhb4_r,vhb5_r,thdc_r,chc1_r,chc2_r,chc3_r,chc4_r,chc5_r,vhc1_r,vhc2_r,vhc3_r,vhc4_r,vhc5_r};
  
  // normalread register writes
  while (!MaximWrite("00E", "60")){}          // These 4 lines gets the DSP ready to read Neutral Current.
  while (!MaximWrite("010", "0041")){}   
  dspready();
  while (!MaximWrite("004", "0000")){}	// Clear Interrupt flag register
  
  /*********** Start of MySQL Statement. First mysqlheader[] is if both normalread() and harmread() are called. ********/
  /*********** Second mysqlheader[] is if only normalread() is called. In which case, remember to end the statement in chip_test_mega.ino ******/
  // Move MySQL statement to server side, only send string of data 
  //char mysqlheader[] = "insert into experimental (freq,volta,voltb,voltc,curra,currb,currc,currn,reala,realb,realc,reaca,reacb,reacc,appaa,appab,appac,pfa,pfb,pfc,pft,phaseab,phaseac,thda,cha1,cha2,cha3,cha4,cha5,vha1,vha2,vha3,vha4,vha5,thdb,chb1,chb2,chb3,chb4,chb5,vhb1,vhb2,vhb3,vhb4,vhb5,thdc,chc1,chc2,chc3,chc4,chc5,vhc1,vhc2,vhc3,vhc4,vhc5) values(";
  //char mysqlheader[] = "insert into hopeman (freq,volta,voltb,voltc,curra,currb,currc,currn,reala,realb,realc,reaca,reacb,reacc,appaa,appab,appac,pfa,pfb,pfc,pft,phaseab,phaseac) values(";
  //SpiSerial.write(mysqlheader, String(mysqlheader).length());
  
  dspready();  // Waits until the chip's DSP is ready (new data is ready).
  
  for (int i=0; i<norm_functs-1; i++){
    if (commands[i] == 1){
      (*ptr[i+1])();
      buffer = buf;
      buffer.trim();
      data += buffer;
      data += ",";
    }
    else if (commands[i] == 0){
      (*ptr[0])();
      buffer = buf;
      buffer.trim();
      data += buffer;
      data += ",";    
    }
  }
  
  while (!MaximWrite("004", "0000")){}      // Clear the chip's DSP READY BIT so the chip can set it again when its DSP is ready.
  delay(500);
  
  // harmread register writes
  while (!MaximWrite("00E", "68")){};	// Disable Neutral Current ADC to not sample Neutral Current for DSP Cycles. (This is not needed? Does it really save time?)
  while (!MaximWrite("030", "0000")){};	// Again disabling the averaging filter. (It this necessary since it is already called in initial_register_writes?)
										// (Highly doubt that this does anything much, but it shouldn't harm anything)
										// (SEE TODOs in chip_test_mega.ino)
  
  dspready(); 
 
  float total_thd = 0; // Temp variables for finializing the output value of THDN's
  float lower_thd = 0;
  
  for (int i=norm_functs-1; i<total_functs-1; i++){
    if (i == total_functs-2){
      last = true;
    }
    if (commands[i] == 1){
      (*ptr[i+1])();
      buffer = buf;
      buffer.trim();
      data += buffer;
      if (!last){  
        data += ",";
      }
    }
    else if (commands[i] == 0){
      (*ptr[0])();
      buffer = buf;
      buffer.trim();
      data += buffer;
      if (!last){  
        data += ",";
      }
    }    
  }
  
  // End of MySQL statement
//  SpiSerial.write(41);  // ")"
//  SpiSerial.write(59);  // ";"

  delay(300);
}

//Sends 0 back to server when user does not select a datapoint to collect
void none(){
  dtostrf(0,10,3,buf);    
}
//////////////////////////////////////// Line Frequency (NS) /////////////////////////////
void freq_r(){
  while(!MaximRead("062",2)){};
  freq = float(lower_read)/1000;

  dtostrf(freq,10,3,buf);
}////////////////////////////////////////////////////////////////////////////////////////////
  

/////////////////////////////////////// RMS (current and voltage) /////////////////////////////////////
void volta_r(){
  //SpiSerial.write("VoltA: ", 7);
  while(!MaximRead("831",8)){};
  voltage[0] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(voltage[0],10,3,buf);
}
  
void voltb_r(){
  //SpiSerial.write("VoltB: ", 7);
  while(!MaximRead("832",8)){};
  voltage[1] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(voltage[1],10,3,buf);
}
  
void voltc_r(){
  //SpiSerial.write("VoltC: ", 7);
  while(!MaximRead("834",8)){};
  voltage[2] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(voltage[2],10,3,buf);
}
  
void curra_r(){
  //SpiSerial.write("CurrA: ", 7);
  while(!MaximRead("841",8)){};
  current[0] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(current[0],10,3,buf);
}
  
void currb_r(){
  //SpiSerial.write("CurrB: ", 7);
  while(!MaximRead("842",8)){};
  current[1] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(current[1],10,3,buf);
}
  
void currc_r(){
  //SpiSerial.write("CurrC: ", 7);
  while(!MaximRead("844",8)){};
  current[2] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(current[2],10,3,buf);
}
  
void currn_r(){          
  //SpiSerial.write("CurrN: ", 7);
  while(!MaximRead("840",8)){};
  current[3] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(current[3],10,3,buf);
}
////////////////////////////////////////////////////////////////////////////////////////////

 
/////////////////////////////////////// Power Readings (real, reactive, apparent) /////////////////////////////////////
void reala_r(){    
  //SpiSerial.write("RealA: ", 7);
  while(!MaximRead("801",8)){};
  realp[0] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(realp[0],10,3,buf);
}
    
void realb_r(){  
  //SpiSerial.write("RealB: ", 7);
  while(!MaximRead("802",8)){};
  realp[1] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(realp[1],10,3,buf);
}

void realc_r(){ 
  //SpiSerial.write("RealC: ", 7);
  while(!MaximRead("804",8)){};
  realp[2] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(realp[2],10,3,buf);
}
    
void reaca_r(){ 
  //SpiSerial.write("ReacA: ", 7);
  while(!MaximRead("811",8)){};
  reactivep[0] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(reactivep[0],10,3,buf);
}
  
void reacb_r(){  
  //SpiSerial.write("ReacB: ", 7);
  while(!MaximRead("812",8)){};
  reactivep[1] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(reactivep[1],10,3,buf);
}
  
void reacc_r(){ 
  //SpiSerial.write("ReacC: ", 7);
  while(!MaximRead("814",8)){};
  reactivep[2] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(reactivep[2],10,3,buf);
}
  
void appaa_r(){ 
  //SpiSerial.write("AppaA: ", 7);
  while(!MaximRead("821",8)){};
  apparentp[0] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(apparentp[0],10,3,buf);
}
 
void appab_r(){ 
  //SpiSerial.write("AppaB: ", 7);
  while(!MaximRead("822",8)){};
  apparentp[1] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(apparentp[1],10,3,buf);
}
  
void appac_r(){ 
  //SpiSerial.write("AppaC: ", 7);
  while(!MaximRead("824",8)){};
  apparentp[2] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(apparentp[2],10,3,buf);
}
////////////////////////////////////////////////////////////////////////////////////////////
  
 

/////////////////////////////////////// Power Factors /////////////////////////////////////
void pfa_r(){  
//SpiSerial.write("PFA: ", 5);
  while(!MaximRead("1C6",2)){};
  pfa = float(((lower_read << 16) >> 16)/16384.00);
  dtostrf(pfa,10,3,buf);
} 
  
void pfb_r(){
  //SpiSerial.write("PFB: ", 5);
  while(!MaximRead("2B2",2)){};
  pfb = float(((lower_read << 16) >> 16)/16384.00);
  dtostrf(pfb,10,3,buf);
}
  
void pfc_r(){
  //SpiSerial.write("PFC: ", 5);
  while(!MaximRead("39E",2)){};
  pfc = float(((lower_read << 16) >> 16)/16384.00);
  dtostrf(pfc,10,3,buf);
}
  
void pft_r(){
  //SpiSerial.write("PFT: ", 5);
  while(!MaximRead("867",8)){};
  pft = ((float)(lower_read))/100000;
  dtostrf(pft,10,3,buf);
}
////////////////////////////////////////////////////////////////////////////////////////////
  
  
/////////////////////////////////////// Voltage Phasor Readings /////////////////////////////////////
void phaseab_r(){  
  //SpiSerial.write("PhaseAB: ", 9);
  while(!MaximRead("852",2)){};
  phasorb = float(lower_read)/100;
  dtostrf(phasorb,10,3,buf);
}
  
void phaseac_r(){
  //SpiSerial.write("PhaseAC: ", 9);
  while(!MaximRead("854",2)){};
  phasorc = float(lower_read)/100;
  dtostrf(phasorc,10,3,buf);
}
////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////// Current A Harm ////////////////////////////////////////////////////////
void thda_r(){
  while (!MaximWrite("010", "01C3")){};	// First change the harmonic (auxiliary) channel config register (SEE MAXIM DATASHEET FOR REGISTER 0x010)
  dspready();							// 3 dspready()'s  (SEE TODOs and April 5, 2013 update)
  dspready();
  dspready();
  MaximRead("859",8);				// Then read the measurement register.
  thd[0] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))/(65536);
  dtostrf(thd[0],10,3,buf);

}

void cha1_r(){
  order = 1;
  while (!MaximWrite("010", "01C3")){};	// First change the harmonic (auxiliary) channel config register (SEE MAXIM DATASHEET FOR REGISTER 0x010)
  dspready();							// 3 dspready()'s  (SEE TODOs and April 5, 2013 update)
  dspready();    
  data_push(current_a_harm, order, c);
}

void cha2_r(){
  order = 2;
  while (!MaximWrite("010", "02C3")){};
  //delay(700);
  dspready();
  dspready();  
  data_push(current_a_harm, order, c);
}

void cha3_r(){
  order = 3;
  while (!MaximWrite("010", "03C3")){};
  dspready();
  dspready();
  data_push(current_a_harm, order, c);
}

void cha4_r(){
  order = 4;
  while (!MaximWrite("010", "04C3")){};
  dspready();
  dspready();
  data_push(current_a_harm, order, c);
}

void cha5_r(){
  order = 5;
  while (!MaximWrite("010", "05C3")){};
  dspready();
  dspready();
  data_push(current_a_harm, order, c);
}
////////////////////////////////////////// End of Current A Harm ////////////////////////////////////////////////////////////////////////


////////////////////////////////////////// Voltage A Harm ////////////////////////////////////////////////////////////////////////
void vha1_r(){
  order = 1;
  while (!MaximWrite("010", "01C2")){};
  dspready();
  dspready();
  data_push(voltage_a_harm, order, v);
}

void vha2_r(){
  order = 2;
  while (!MaximWrite("010", "02C2")){};
  dspready();
  dspready();
  data_push(voltage_a_harm, order, v);
}

void vha3_r(){
  order = 3;
  while (!MaximWrite("010", "03C2")){};
  dspready();
  dspready();
  data_push(voltage_a_harm, order, v);
}

void vha4_r(){
  order = 4;
  while (!MaximWrite("010", "04C2")){};
  dspready();
  dspready();
  data_push(voltage_a_harm, order, v);
}

void vha5_r(){
  order = 5;
  while (!MaximWrite("010", "05C2")){};
  dspready();
  dspready();
  data_push(voltage_a_harm, order, v);
}
////////////////////////////////////////// End of Voltage A Harm ////////////////////////////////////////////////////////////////////////


////////////////////////////////////////// Current B Harm ////////////////////////////////////////////////////////////////////////
void thdb_r(){
  while (!MaximWrite("010", "01C5")){};
  dspready();							// 3 dspready()'s  (SEE TODOs and April 5, 2013 update)
  dspready();
  dspready();
  MaximRead("85A",8);
  thd[1] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))/(65536);
  dtostrf(thd[1],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
}

void chb1_r(){
  order = 1;
  while (!MaximWrite("010", "01C5")){};
  dspready();
  dspready();
  data_push(current_b_harm, order, c);
}

void chb2_r(){
  order = 2;
  while (!MaximWrite("010", "02C5")){};
  dspready();
  dspready();
  data_push(current_b_harm, order, c);
}

void chb3_r(){
  order = 3;
  while (!MaximWrite("010", "03C5")){};
  dspready();
  dspready();
  data_push(current_b_harm, order, c);
}

void chb4_r(){
  order = 4;
  while (!MaximWrite("010", "04C5")){};
  dspready();
  dspready();
  data_push(current_b_harm, order, c);
}

void chb5_r(){
  order = 5;
  while (!MaximWrite("010", "05C5")){};
  dspready();
  dspready();
  data_push(current_b_harm, order, c);
}
////////////////////////////////////////// End of Current B Harm ////////////////////////////////////////////////////////////////////////


////////////////////////////////////////// Voltage B Harm ////////////////////////////////////////////////////////////////////////
void vhb1_r(){
  order = 1;
  while (!MaximWrite("010", "01C4")){};
  dspready();
  dspready();
  data_push(voltage_b_harm, order, v);
}

void vhb2_r(){
  order = 2;
  while (!MaximWrite("010", "02C4")){};
  dspready();
  dspready();
  data_push(voltage_b_harm, order, v);
}

void vhb3_r(){
  order = 3;
  while (!MaximWrite("010", "03C4")){};
  dspready();
  dspready();
  data_push(voltage_b_harm, order, v);
}

void vhb4_r(){
  order = 4;
  while (!MaximWrite("010", "04C4")){};
  dspready();
  dspready();
  data_push(voltage_b_harm, order, v);
}

void vhb5_r(){
  order = 5;
  while (!MaximWrite("010", "05C4")){};
  dspready();
  dspready();
  data_push(voltage_b_harm, order, v);
}
////////////////////////////////////////// End of Voltage B Harm ////////////////////////////////////////////////////////////////////////


////////////////////////////////////////// Current C Harm ////////////////////////////////////////////////////////////////////////
void thdc_r(){
  while (!MaximWrite("010", "01C7")){};
  dspready();
  dspready();
  dspready();
  MaximRead("85C",8);
  thd[2] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))/(65536);
  dtostrf(thd[2],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
}

void chc1_r(){
  order = 1;
  while (!MaximWrite("010", "01C7")){};
  dspready();
  dspready();
  data_push(current_c_harm, order, c);
}

void chc2_r(){
  order = 2;
  while (!MaximWrite("010", "02C7")){};
  dspready();
  dspready();
  data_push(current_c_harm, order, c);
}

void chc3_r(){
  order = 3;
  while (!MaximWrite("010", "03C7")){};
  dspready();
  dspready();
  data_push(current_c_harm, order, c);
}

void chc4_r(){
  order = 4;
  while (!MaximWrite("010", "04C7")){};
  dspready();
  dspready();
  data_push(current_c_harm, order, c);
}

void chc5_r(){
  order = 5;
  while (!MaximWrite("010", "05C7")){};
  dspready();
  dspready();
  data_push(current_c_harm, order, c);
}
////////////////////////////////////////// End of Current C Harm ////////////////////////////////////////////////////////////////////////


////////////////////////////////////////// Voltage C Harm ////////////////////////////////////////////////////////////////////////
void vhc1_r(){
  order = 1;
  while (!MaximWrite("010", "01C6")){};
  dspready();
  dspready();
  data_push(voltage_c_harm, order, v);
}

void vhc2_r(){
  order = 2;
  while (!MaximWrite("010", "02C6")){};
  dspready();
  dspready();
  data_push(voltage_c_harm, order, v);
}

void vhc3_r(){
  order = 3;
  while (!MaximWrite("010", "03C6")){};
  dspready();
  dspready();
  data_push(voltage_c_harm, order, v);
}

void vhc4_r(){
  order = 4;
  while (!MaximWrite("010", "04C6")){};
  dspready();
  dspready();
  data_push(voltage_c_harm, order, v);
}

void vhc5_r(){
  order = 5;
  while (!MaximWrite("010", "05C6")){};
  dspready();
  dspready();
  data_push(voltage_c_harm, order, v);
}
////////////////////////////////////////// End of Voltage C Harm ////////////////////////////////////////////////////////////////////////


////////////////////////////////////////// Data push for harmonics //////////////////////////////////////////////////////////////////////
void data_push(float data[], int order, String type){
  dspready();
  MaximRead(type,8);		// Reading the measurement register for current harmonics
  data[order] = (float(((unsigned long)lower_read >> 16) | (upper_read << 16)))*(65536)/1000000000;  // All LSBs are 10^-9
 
  dtostrf(data[order],10,3,buf);	// Change float value to string to be transmitted as part of MySQL statement
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
