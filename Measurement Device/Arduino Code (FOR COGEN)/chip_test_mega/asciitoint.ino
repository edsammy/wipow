// TODO: Better handling of invalid input arguments.
//		 Maybe just make sure that the inputs to MaximRead() and MaximWrite() are valid in those functions themselves?
//		 (See the end of this function) 

// This function changes the ASCII characters 
// inputted to their corresponding values in HEX.
// Made this because Arduino does not have its own function for this?

// ********USED FOR maxim_write.ino AND maxim_read.ino. 
// This allows those functions to be called with strings as arguments for the destination registers and data.
// (Avoids the need for tedious conversions when writing code)
// ********Also allows for easy input of USER COMMANDS via STRINGS (USEFUL FOR IMPLEMENTING USER INPUTS)        

int asciitoint(int c){		// Input arugment is one ASCII character (one byte, represented in an INT variable)
							// Outputs the corresponding character as a true INT byte (binary value).
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
          Serial.println("Error in Reading Register value!");
          while(true){};		// Hangs the device to prevent undesirable actions to the chip 
								// since the input is not a character usable as a register address or data.
								// TODO: better handling of this?
        }
}
