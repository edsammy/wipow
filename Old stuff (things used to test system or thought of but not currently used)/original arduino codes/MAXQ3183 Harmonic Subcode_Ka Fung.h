//Ka Fung
//University of Rochester
//Code for MAXQ3183 with Arduino as Master

float current[4];  		// 0-A  1-B  2-C  3-N
float voltage[3];  		// 0-A  1-B  2-C
float voltage_a_harm[10];	// array index corresponds to harmonic
float current_a_harm[10];
long upper_read, lower_read;	// used to temporarily store 8 bytes from virtual registers 
								// whenever MaximRead is called, the data read is in these variables
								//(Arduino's largest variables are only 4 bytes)
int counter = 0;
int harm_order = 0;
								
								
void loop() {					// function that gets looped through by the Arduino (main function)
  
  if(counter < 200){			// I had to increase this counter to about 500 before I can notice the correct measurement apparearing routinely when I read
    normalread(harm_order, counter);  // This is the function below.
    counter++;
  }
  else{
		counter = 0;			// Resetting counter, and looping through the first 4 harmonics and neutral current
		if(harm_order < 4){		// harm_order = 0  --> neutral current
		  harm_order++;
    }
    else{
		harm_order = 0;
    }
  }
}



void normalread(int order, int counter){  

	long a = 0;
	while(a==0){					// This loop checks the DSPRDY bit in IRQ_FLAG
	   MaximRead("004",2);			// a contains the DSPRDY bit
	   a = lower_read << 17;
	   a = (unsigned long)a >> 31;
	   delayMicroseconds(500); 
	}

  if(counter==0){			// At the start of every group of (counter) repetitions, write to AUX_CFG to configure what I want.
	Serial.println("");
    Serial.print("***********New Repetition with order=");
    Serial.print(order);
    Serial.println("************");
	
    switch (order){
         case 0:
             while (!MaximWrite("00E", "60")){}		// The while statement is just to make sure it finishes writing
             while (!MaximWrite("010", "0041")){}   // It's probably unnecessary now since it's very reliable in writing
             break;									// on the first try (after installing the crystal). Doesn't hurt though.
         case 1:
             while (!MaximWrite("010", "01C3")){}    
             break;
         case 2: 
             while (!MaximWrite("010", "02C3")){}    
             break;
         case 3:
             while (!MaximWrite("010", "03C3")){}    
             break;
         case 4:
             while (!MaximWrite("010", "04C3")){}    
             break;
         default:
             break;
     }
  }
  

if(order==0){		// Neutral current
  MaximRead("840",8);
  current[3] = eight_to_four_bytes(upper_read, lower_read)*(65536);		// eight_to_four bytes just takes the middle 4 bytes of the 8 byte (total) data.
																		// since I set the LSB of the currents and voltages to nano amps/volts, 
																		// and I am trying to read whole amps and hundreds of volts, I shouldn't lose much.
																		// This is mostly to make it easier for myself to print these values for now.
  }																		
else{																	
  MaximRead("840",8);													
  current_a_harm[order] = eight_to_four_bytes(upper_read, lower_read)*(65536); 
}

 for(int i=0;i<5;i++){						// Printing all the readings. Only one value should change every [counter] times.
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
 
 while (!MaximWrite("004", "0000")){}   // Resetting the IRQ_FLAG
  
  Serial.println("");
}