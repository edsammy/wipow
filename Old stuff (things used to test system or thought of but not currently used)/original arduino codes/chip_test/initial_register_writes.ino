#include <maxim_read.ino>
#include <maxim_write.ino>

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
  while (!MaximWrite("130", "3F96")){    //reg 130
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
  while (!MaximWrite("12E", "3F96")){    //reg 12E
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
  
 /*  Serial.print("writing to averaging filter:  ");
  while (!MaximWrite("030", "0000")){      //reg 030
    Serial.print("writing to averaging filter:  ");
  }
  
 /* Serial.print("writing to SCAN_IN:  ");
  while (!MaximWrite("00E", "60")){      //reg 00E
    Serial.print("writing to SCAN_IN:  ");
  }
  Serial.print("writing to harmonic reg:  ");
  while (!MaximWrite("010", "03C2")){      //reg 010
    Serial.print("writing to harmonic reg:  ");
  }
  
  
  /*
  Serial.print("writing to ADC Rate Mirror:  ");
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
