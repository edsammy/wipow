// This function reads the "normal" power values (no harmonics):  (SEE MAXIM DATASHEET FOR DETAILS ON THESE REGISTERS)
//       Line Frequency						(LSB is 1/1000 or 0.001 Hz)
//       Voltage/Current RMS				(LSB is 10^-9 Volts or Amps, i.e. nano-volts/amps)
//       Powers(Real, Reactive, Apparent)	(LSB is 10^-9 Watts or VA or VAR, i.e. nano-watts/VA/VAR)
//       PF									(See datasheet for LSBs)
//       Voltage Phasors					(LSB is 1/100 or 0.01 degrees)

void normalread(){  
 while (!MaximWrite("00E", "60")){}          // These 4 lines gets the DSP ready to read Neutral Current.
 while (!MaximWrite("010", "0041")){}   
 dspready();
 while (!MaximWrite("004", "0000")){}	// Clear Interrupt flag register

 /*********** Start of MySQL Statement. First mysqlheader[] is if both normalread() and harmread() are called. ********/
 /*********** Second mysqlheader[] is if only normalread() is called. In which case, remember to end the statement in chip_test_mega.ino ******/
 char mysqlheader[] = "insert into hopeman (freq,volta,voltb,voltc,curra,currb,currc,currn,reala,realb,realc,reaca,reacb,reacc,appaa,appab,appac,pfa,pfb,pfc,pft,phaseab,phaseac,thda,cha1,cha2,cha3,cha4,cha5,vha1,vha2,vha3,vha4,vha5,thdb,chb1,chb2,chb3,chb4,chb5,vhb1,vhb2,vhb3,vhb4,vhb5,thdc,chc1,chc2,chc3,chc4,chc5,vhc1,vhc2,vhc3,vhc4,vhc5) values(";
 //char mysqlheader[] = "insert into hopeman (freq,volta,voltb,voltc,curra,currb,currc,currn,reala,realb,realc,reaca,reacb,reacc,appaa,appab,appac,pfa,pfb,pfc,pft,phaseab,phaseac) values(";
 SpiSerial.write(mysqlheader, String(mysqlheader).length());
 dspready();  // Waits until the chip's DSP is ready (new data is ready).
 
  //////////////////////////////////////// Line Frequency (NS) /////////////////////////////
  while(!MaximRead("062",2)){};
  freq = float(lower_read)/1000;

  dtostrf(freq,10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
 ////////////////////////////////////////////////////////////////////////////////////////////
  

/////////////////////////////////////// RMS (current and voltage) /////////////////////////////////////
  //SpiSerial.write("VoltA: ", 7);
  while(!MaximRead("831",8)){};
  voltage[0] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(voltage[0],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("VoltB: ", 7);
  while(!MaximRead("832",8)){};
  voltage[1] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(voltage[1],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("VoltC: ", 7);
  while(!MaximRead("834",8)){};
  voltage[2] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(voltage[2],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("CurrA: ", 7);
  while(!MaximRead("841",8)){};
  current[0] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(current[0],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("CurrB: ", 7);
  while(!MaximRead("842",8)){};
  current[1] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(current[1],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("CurrC: ", 7);
  while(!MaximRead("844",8)){};
  current[2] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(current[2],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
             
  //SpiSerial.write("CurrN: ", 7);
  while(!MaximRead("840",8)){};
  current[3] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(current[3],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
////////////////////////////////////////////////////////////////////////////////////////////

 
/////////////////////////////////////// Power Readings (real, reactive, apparent) /////////////////////////////////////
  //SpiSerial.write("RealA: ", 7);
  while(!MaximRead("801",8)){};
  realp[0] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(realp[0],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("RealB: ", 7);
  while(!MaximRead("802",8)){};
  realp[1] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(realp[1],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("RealC: ", 7);
  while(!MaximRead("804",8)){};
  realp[2] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(realp[2],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  
  //SpiSerial.write("ReacA: ", 7);
  while(!MaximRead("811",8)){};
  reactivep[0] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(reactivep[0],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("ReacB: ", 7);
  while(!MaximRead("812",8)){};
  reactivep[1] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(reactivep[1],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("ReacC: ", 7);
  while(!MaximRead("814",8)){};
  reactivep[2] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(reactivep[2],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  
  //SpiSerial.write("AppaA: ", 7);
  while(!MaximRead("821",8)){};
  apparentp[0] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(apparentp[0],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("AppaB: ", 7);
  while(!MaximRead("822",8)){};
  apparentp[1] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(apparentp[1],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("AppaC: ", 7);
  while(!MaximRead("824",8)){};
  apparentp[2] = (float)(((long)((unsigned long)lower_read >> 16))|(upper_read << 16))*(65536)/1000000000;
  dtostrf(apparentp[2],10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
////////////////////////////////////////////////////////////////////////////////////////////
  
 

/////////////////////////////////////// Power Factors /////////////////////////////////////
  //SpiSerial.write("PFA: ", 5);
  while(!MaximRead("1C6",2)){};
  pfa = float(((lower_read << 16) >> 16)/16384.00);
  dtostrf(pfa,10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("PFB: ", 5);
  while(!MaximRead("2B2",2)){};
  pfb = float(((lower_read << 16) >> 16)/16384.00);
  dtostrf(pfb,10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("PFC: ", 5);
  while(!MaximRead("39E",2)){};
  pfc = float(((lower_read << 16) >> 16)/16384.00);
  dtostrf(pfc,10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("PFT: ", 5);
  while(!MaximRead("867",8)){};
  pft = ((float)(lower_read))/100000;
  dtostrf(pft,10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
////////////////////////////////////////////////////////////////////////////////////////////
  
  
/////////////////////////////////////// Voltage Phasor Readings /////////////////////////////////////
  //SpiSerial.write("PhaseAB: ", 9);
  while(!MaximRead("852",2)){};
  phasorb = float(lower_read)/100;
  dtostrf(phasorb,10,3,buf);
  SpiSerial.write(buf);
  SpiSerial.write(",",1);
  
  //SpiSerial.write("PhaseAC: ", 9);
  while(!MaximRead("854",2)){};
  phasorc = float(lower_read)/100;
  dtostrf(phasorc,10,3,buf);
  SpiSerial.write(buf);
////////////////////////////////////////////////////////////////////////////////////////////

  while (!MaximWrite("004", "0000")){}      // Clear the chip's DSP READY BIT so the chip can set it again when its DSP is ready.
  
}
