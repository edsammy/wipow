// This function is called at the beginning
// to write the initial configuration registers
// with the appropriate values for correct
// conversions and data acquisition.

// **** CALIBRATION IS CURRENTLY DONE HERE BY HARDCODING THE CALIBRATION REGISTERS

// **** ONCE USER INPUTS ARE IMPLEMENTED, CALIBRATION CAN POTENTIALLY BE DONE REMOTELY

// A lot of these are commented out. Some is because that change is currently not needed/implemented.
// Others are because they were used for troubleshooting before.
// I left them all here just incase I needed to go back.

// SEE MAXIM DATASHEET FOR DETAILS ON THESE REGISTERS. I have kept the register names here for easy reading (Serial print lines).

void initial_register_writes(){
/*
//Serial.print("writing to current offs_lo A:  ");
  while (!MaximWrite("13C", "8000")){    //reg 13C
    //Serial.print("writing to current offs_lo A:  ");
  }
  
  //Serial.print("writing to current offs_lo B:  ");
  while (!MaximWrite("228", "8000")){    //reg 228
    //Serial.print("writing to current offs_lo B:  ");
  }
  
  //Serial.print("writing to current offs_lo C:  ");
  while (!MaximWrite("314", "8000")){    //reg 314
    //Serial.print("writing to current offs_lo C:  ");
  }
  
  //Serial.print("writing to CYCNT:  ");
  while (!MaximWrite("01C", "0004")){    //reg 01C
    //Serial.print("writing to CYCNT:  ");
  }
  */
  //Serial.print("writing to voltage gain A:  ");
  while (!MaximWrite("132", "4001")){    //reg 132
    //Serial.print("writing to voltage gain A:  ");
  }
  
  
    ////Serial.print("writing to voltage gain B:  ");
  while (!MaximWrite("21E", "4017")){    //reg 21E
    ////Serial.print("writing to voltage gain B:  ");
  }
  
  
    ////Serial.print("writing to voltage gain C:  ");
  while (!MaximWrite("30A", "3F72")){    //reg 30A
    ////Serial.print("writing to voltage gain C:  ");
  }
  
  
  ////Serial.print("writing to current gain A:  ");
  while (!MaximWrite("130", "3D82")){    //reg 130
    ////Serial.print("writing to current gain A:  ");
  }
  
    
  ////Serial.print("writing to current gain B:  ");
  while (!MaximWrite("21C", "41BB")){    //reg 21C
    ////Serial.print("writing to current gain B:  ");
  }
  
    
  ////Serial.print("writing to current gain C:  ");
  while (!MaximWrite("308", "419C")){    //reg 308
    ////Serial.print("writing to current gain C:  ");
  }
  
    
  ////Serial.print("writing to current gain N:  ");
  while (!MaximWrite("12E", "3BF1")){    //reg 12E
    ////Serial.print("writing to current gain N:  ");
  }

  ////Serial.print("writing to volt conversion coeff:  ");
  while (!MaximWrite("014", "4DD0")){    //reg 014   V_FS=33420V/V, VOLT_LSB=10^-7
    ////Serial.print("writing to volt conversion coeff:  ");
  }
  
    //Serial.print("writing to current conversion coeff:  ");
  while (!MaximWrite("016", "68C6")){    //reg 016    I_FS=4500A/V, AMP_LSB=10^-8
    //Serial.print("writing to current conversion coeff:  ");
  }
  
  
    //Serial.print("writing to power conversion coeff:  ");
  while (!MaximWrite("018", "88C7")){    //reg 018    PWR_LSB=10^-6
    //Serial.print("writing to power conversion coeff:  ");
  }
  
  
   //Serial.print("writing to averaging filter:  ");
  while (!MaximWrite("030", "0000")){      //reg 030
    //Serial.print("writing to averaging filter:  ");
  }
  
  
  //Serial.print("writing to Harmonic filter B0:  ");
  while (!MaximWrite("03A", "0130")){      //reg 03A (0130)
    //Serial.print("writing to Harmonic filter B0:  ");
  }
  
    //Serial.print("writing to Disable Fundamental Calculations:  ");
  while (!MaximWrite("003", "30")){      //reg 03A
    //Serial.print("writing to Harmonic filter B0:  ");
  }
  
  
  //Serial.print("writing to NS Rejection filter:  ");
  //while (!MaximWrite("02C", "0050")){      //reg 02C
    //Serial.print("writing to NS Rejection filter:  ");
  //}
  
  
  //Serial.print("writing to SCAN_IN:  ");
  while (!MaximWrite("00E", "60")){      //reg 00E
    //Serial.print("writing to SCAN_IN:  ");
  }
  
  
  //Serial.print("writing to harmonic reg:  ");
  //while (!MaximWrite("010", "0041")){      // 0x0041 for neutral current
    //Serial.print("writing to harmonic reg:  ");
  //}
  
  
  
  /*
  //Serial.print("reading ADC Rate Mirror:  ");
  MaximRead("04E",2);
  
  //Serial.print("writing to ADC Rate Mirror:  ");
  while (!MaximWrite("04E", "0167")){      //reg 04E
    //Serial.print("writing to ADC Rate Mirror:  ");
  }
  
  /*
  //Serial.print("writing to ADC acquisition Mirror:  ");
  while (!MaximWrite("050", "05")){      //reg 050
    //Serial.print("writing to ADC acquisition Mirror:  ");
  }
  
  
  //Serial.print("Copying Mirror registers to Hardware registers:  ");
  while (!MaximRead("900",1)){
    //Serial.print("Copying Mirror registers to Hardware registers:  ");
  }
  /*      ***********FAKE DATA to see if mirror is working correctly**********
  //Serial.print("writing to ADC Rate Mirror:  ");
  while (!MaximWrite("04E", "0123")){      //reg 04E
    //Serial.print("writing to ADC Rate Mirror:  ");
  }
  MaximRead("04E",2);
  
  
  //Serial.println("Hardware registers updated....checking hardware registers...");
  
  //Serial.print("Copying Hardware registers to Mirror registers:  ");
  while (!MaximRead("A00",1)){
    //Serial.print("Copying Hardware registers to Mirror registers:  ");
  }
  
  //Serial.println("Mirror registers now contain hardware values...");
  
  //Serial.print("Reading ADC Rate Mirror:  ");
  while (!MaximRead("04E", 2)){      //reg 04E
    //Serial.print("Reading ADC Rate Mirror:  ");
  }
  /*
  //Serial.print("Reading ADC acquisition Mirror:  ");
  while (!MaximRead(80, 2)){      //reg 050
    //Serial.print("Reading ADC acquisition Mirror:  ");
  }
  */
  
  
  while (!MaximWrite("004", "0000")){}		// ***** CLEAR INTERRUPT FLAG REGISTER since we changed a lot of interrupt-flag-enabled registers
  
  
 }
