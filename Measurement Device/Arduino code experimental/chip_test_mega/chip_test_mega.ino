/*****************************************************************************************************************
This code is written in C/C++ for the Arduino MEGA using the Arduino IDE for the Wireless Power Monitor device.

Code by: Ka Fung
University of Rochester
Mentor: Professor Thomas B. Jones
Thanks to Gabrial Unger, Lucas Crandall, and Pak Lam Yung.

***********Changes/updates/fixes: 
Update November 27, 2012:	Code to read all phases' harmonics (1-5) added and tested, THDN's of currents as well.

Fix March 17, 2013: 		Fixed Phases' Power Factor bug where negative values were not interpreted correctly as negative (32 bits long value contains only 16 bits of info)

Update April 5, 2013: 		A total of three "dspready()"'s were added inbetween setting the harmonic filter register and actually reading a harmonic.

Change April 9, 2013: 		Further commented entire code. Cleaned up some of the old commented out code.

Change May 13, 2013:		Changed the way THD measurement is interpreted. The way the code interpreted it was wrong. Now it should be consistent with the equation given in the datasheet.

*****************************************************************************************************************/
			////////////
			// TODOs //
			////////////
			
//TODO: April 5, 2013 update works, but is very slow. Is there a way to increase speed/use less DSP cycles to get harmonic measurment registers ready?
//		As far as I am concerned (4/9/2013), I don't have any ideas looking at the datasheet. Email MAXIM support? (I might do that soon)

//TODO: Investigate the averaging filter register 0x030. 
//		It is currently set to zero (no averaging to allow a measurement register to completely change to a new value every DSP cycle)
//		However, the chip does not appear to behave as mentioned above. (Values in measurement registers still seem to be averaged when less "dspread()"'s are used)
//		***This occured with the HOPEMAN board, not sure if it happens with other boards. Maybe the chip on HOPEMAN board is just damaged?

//TODO: Implement the ability for this code/device to accept USER INPUTS. So that a user (via online or USB serial console) can 
//		specify which measurements to obtain, as well as write to calibration registers so that calibration does not have to be hardcoded
//		and the entire code uploaded each time calibration needs to be done.

//TODO: Have a better way to build up that MySQL string (also the ending ");"). (See normalread.ino and harmread.ino)
//		Currently it is hardcoded to just push/dump out a certain string statement. It is troublesome to even change the hardcode to
//		have the device output a different set of data. FURTHERMORE, this hardcode implementation assumes that everything will be read
//		from the MAXIM chip in the order specified in the MySQL statement. This can potentially cause problems with data going into the wrong place.

			///////////////////
			// END OF TODOs //
			///////////////////


#include <SPI.h>
#include <WiFly.h>	// **** SpiUart.cpp's  begin() function has its call to SPI.begin() commented out!! This is to allow SPI settings for communication to the Maxim chip later.


long upper_read, lower_read;    // When calling MaximRead, it will update these two variables with the read values (4 bytes per long variable, 8 bytes max).

long response;  // byte by byte response of the SPI communication protocol with the MAXIM chip. (Used for maxim_write and maxim_read)

int order;  // Used to indicate order of harmonic readings
String c = "840";  // MAXIM virtual registry for harmonic current readings
String v = "830";  // MAXIM virtual registry for harmonic voltage readings

//***Future use for user_input from website***
String user_input;

const int norm_functs = 24;  // number of non-harmonic MAXIM measurements, used for user input parsing
const int harm_functs = 33;   // number of harmonic MAXIM measurements
const int total_functs = norm_functs + harm_functs;

const int data_led = 8;  // Status LED to indicate data TX (v3)

////////////////////////////////////////////////////////////////
// The rest of the variables below are data holding variables.//
////////////////////////////////////////////////////////////////
float current[4];  // 0-A  1-B  2-C  3-N
float voltage[3];  // 0-A  1-B  2-C

float voltage_a_harm[5];  // number corresponds to harmonic (0's are not used)
float voltage_b_harm[5];
float voltage_c_harm[5];
float current_a_harm[5];
float current_b_harm[5];
float current_c_harm[5];

float realp[4];    // 0-A  1-B  2-C  3-T
float reactivep[4];// 0-A  1-B  2-C  3-T
float apparentp[4];// 0-A  1-B  2-C  3-T

float frealp[4];    // 0-A  1-B  2-C  3-T	// fundamental readings are currently not implemented
float freactivep[4];// 0-A  1-B  2-C  3-T
float fapparentp[4];// 0-A  1-B  2-C  3-T

float reale[4];    // 0-A  1-B  2-C  3-T	// energy readings are currently not implemented
float reactivee[4];// 0-A  1-B  2-C  3-T
float apparente[4];// 0-A  1-B  2-C  3-T

float thd[3];// 0-A  1-B  2-C	// Total harmonic distortion plus noise of current

float phasorb, phasorc, pfa, pfb, pfc, pft;

float freq;

char buf[10];	// Temp variable to store the string that gets transmitted. The string is converted from the float values above.

char charBuffer[total_functs];  // Used to convert user_input from String to char to int (v3)

int commands[total_functs];  // Array that holds which data functions to read from MAXIM chip (v3)

///////////////////////////////////
// End of data holding variables //
///////////////////////////////////

void setup() {
  
  pinMode(10,INPUT);    //High Z the old UNO SPI pins for the MEGA
  pinMode(11,INPUT);    //***But have to remember to jump MEGA SPI pins to
  pinMode(12,INPUT);    //***these locations. (New PCB board already takes care of the jumping)
  pinMode(13,INPUT);	//

  Serial.begin(115200);
  
  pinMode(2,OUTPUT);        //SSEL of Maxim, LOW to select it as slave.
  
  pinMode(3,OUTPUT);        //RESET of Maxim, LOW to reset.
  
  pinMode(data_led,OUTPUT);        //LED to indicate data TX
  
  digitalWrite(3, LOW);     //Initial Reset of Maxim.
  delay(1000);
  digitalWrite(3, HIGH);
  delay(1000);
                   
  Serial.print("starting SPI...");   //Starting SPI. The specifications for the SPI protocol is listed in MAXIM chip's datasheet.
  SPI.begin();		// *** AS STATED ABOVE, REMEMBER TO DISABLE THIS SAME LINE IN THE WIFLY'S LIBRARY CODE (SpiUart.cpp)
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16);   //  16MHz/16 = 1MHz
  SPI.setDataMode(SPI_MODE0);
  delay(3000);
  Serial.println("Done!");  
  
  digitalWrite(2, HIGH);	// Make sure Maxim chip is de-selected as SPI slave, since we are now SPI-connecting to WiFly
  
  Serial.print("SPI Uart initializing...");	// Starting the serial connection to WiFly
  SpiSerial.begin();
  Serial.println("Done!");
  
  Serial.print("Writing initial MAXQ3183 registers...");	// The initial register setups are in "initial_register_writes.ino"
  initial_register_writes();
  Serial.println("Done!");
  
  delay(1000);
  SpiSerial.write("$$$", 3);		// Go to CMD in WiFly
  delay(500);
    while(SpiSerial.available() > 0) {
    Serial.write(SpiSerial.read());
    delay(50);
  }

  SpiSerial.write("join", 4);		// Join the pre-set network "Roving1"
  SpiSerial.write("\r", 1);			// ** Wifly is already set to auto-join "Roving1".
  delay(3000);						//    This is just incase it doesn't.
    while(SpiSerial.available() > 0) {
    Serial.write(SpiSerial.read());
    delay(50);
  }
  
  SpiSerial.write("get i", 5);		// "get i" prints out ip settings of Wifly
  SpiSerial.write("\r", 1);			// This lets us see the IP of the WiFly in the network
  delay(1000);
  while(SpiSerial.available() > 0) {
    Serial.write(SpiSerial.read());
    delay(50);
  }

  delay(2000);
  while(SpiSerial.available() > 0) { // Flushing out the "get i" lines
    Serial.write(SpiSerial.read());
  }
  delay(3000);
  while(SpiSerial.available() > 0) {
    Serial.write(SpiSerial.read());
  }
  
  SpiSerial.write("exit", 4);		 // Exit CMD mode
  SpiSerial.write("\r", 1);
  delay(1000);
  while(SpiSerial.available() > 0) { // Flush out remaining serial data from WiFly
    Serial.write(SpiSerial.read());
    delay(50);
  }
  Serial.println("Ready for communication!");
}




void dspready(){          // This function is called to wait until the chip's DSP cycle (and new data) is ready.
  boolean a = 0;          // It then writes back to the chip's flag register to unset the READY BIT at the end.
    while(!a){
     delayMicroseconds(500);
     MaximRead("004",2);
     //Serial.println(lower_read,BIN);
     a = bitRead(lower_read,14);
    }
    while (!MaximWrite("004", "0000")){}
}







void loop() {

  while(!SpiSerial.available());	// Idle until WiFly has something to output (it should then mean that someone is trying to connect to our WiFly)
									// Ideally, that "someone" should be the University's server running our webpage's code, which would accept data from this device.
									// However, any pings or other devices could also try to connect to our device. 
									// In which case, this code does not know/care and would act the same way (i.e. blindly pushes out data)
									// TODO: Improve code so that it knows only to respond to the University's server running our webpage's code?
  
  delay(500);	// Althought it adds significantly to the delay overhead of the entire data gathering system, these delays seem to be important.
				// Without them, the code runs too fast for (I believe) WiFly to handle.
				// TODO: Can fiddle around with these delays to lower them, or look at WiFly module more closely to see what is slowing it down.
  
  while(SpiSerial.available() > 0) {	// Flushes out the HTTP headers. (Or whatever else header specific to the connection)
    Serial.write(SpiSerial.read());		// TODO: Have WiFly not give us the HTTP headers to speed things up?
    delay(5);
  }
    digitalWrite(data_led, HIGH);
    user_sel_read(user_input);	// This calls "user_sel_read.ino" and must pass a user_input parameter (String)
    while (!MaximWrite("004", "0000")){}      // Clear the chip's DSP READY BIT so the chip can set it again when its DSP is ready.
    delay(500);  
    //SpiSerial.write(41);  // ")"
    //SpiSerial.write(59);  // ";"  To finish the MySQL statement that normalread was pushing out (IF "harmread()" IS DISABLED/COMMENTED OUT)
  
    SpiSerial.write("$$$", 3);				// CMD mode of WiFly
    delay(300);
    while(SpiSerial.available() > 0) {
      Serial.write(SpiSerial.read());
      delay(5);
    }
    SpiSerial.write("close", 5);			// Tell WiFly to close the HTTP connection since we should have finished transmitting the data.
    SpiSerial.write("\r", 2);
    delay(100);
    while(SpiSerial.available() > 0) {
      Serial.write(SpiSerial.read());
      delay(5);
    }
    
    SpiSerial.write("exit", 4);				// Exit CMD mode of WiFly
    SpiSerial.write("\r", 1);
    delay(100);
  
  while(SpiSerial.available() > 0) {		// Flushing of WiFly output (whatever's left, exit command should get "EXIT" response from WiFly, etc...)
    Serial.write(SpiSerial.read());
    delay(5);
  }
  
  digitalWrite(data_led, LOW);
  
					// End of loop is at the end |
					//							 |
					//							 V
  
  
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THE CODE COMMENTED OUT BELOW IS OLD CODE THAT TRIES TO START IMPLEMENTING USER COMMAND INPUT TO THE DEVICE.					//
// This would allow the user/server to send commands to this device/code via over WiFly or through USB serial console,			//
// and this device will output data accordingly instead of outputting a hardcoded set of data that it is currently made to do.	//
//																																//
// The code is definitely poorly coded and implemented, but I left this here just for reference for future ideas.				//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
   // if(Serial.available()) { // Outgoing data
   //   SpiSerial.write(Serial.read());
   // }
  
//byte cmd = 9;    // cmd = 0 --> Normal read is enabled

/*   // Outside Wireless Command to send
byte temp;
char firsttwo[3];

int incomingdata = SpiSerial.available();
for(int y=0;y<incomingdata;y++){
  temp = SpiSerial.read();
      Serial.write(temp);
      if(y==0){
         firsttwo[0] = temp;
      }
      if(y==1){
         firsttwo[1] = temp;
           if(firsttwo[0]==0x67 && firsttwo[1]==0x6F){
             cmd = 0;
             break;
           }
           else{
             cmd = 9;
           }
      }
}
*/


 /* while(SpiSerial.available() > 0) {
      Serial.write(SpiSerial.read());
  }
  */
  
  /*   //Console command to send
  if(Serial.available()) { // Outgoing data
    if(Serial.peek()==0x30){
      cmd = 0;
      while(Serial.available()){ Serial.read();}
    }
    else{
      cmd = 9;
      //while(Serial.available()){
        Serial.write(Serial.peek());
        SpiSerial.write(Serial.read());
      //}
    }
  }
  */
  
  /*
  if(Serial.available()) {              // Reads the Serial buffer 3 characters at a time to read that register (of the chip).
    String address;
    for(int i=0;i<3;i++) {
      if(Serial.available()){
        address += char(Serial.read());
      }
      else
       i--;
    }
    Serial.println(address);
    if(address.equals("res")){
      digitalWrite(3, LOW);     //Reset of Maxim.
      delay(1000);
      digitalWrite(3, HIGH);
      delay(1000);
      initial_register_writes();
    }
    else{
        for(int i =0;i<10;i++){            // Currently hard-coded to read the register 10 times, reading 8 bytes going up from register number inputted.
          MaximRead(address,8);
          Serial.println(upper_read,HEX);
          Serial.println(lower_read,HEX);
        }
    }
  }
 */
 
 
 /*    //Start sending when cmd=0
 if (cmd == 0){
    normalread();    // This function reads the "normal" power values: Line Frequency; Voltage/Current RMS; Total and Fundamental Powers(Real, Reactive, Apparent); PF; Voltage Phasors.
 }
 */
 
 
 //delay(1000);
 
 /*if (cmd == 1){
                                       // The rest is for harmonic readings.
  if(counter < 2){
    harmread(harm_order, counter);
    counter++;
  }
  else{
      counter = 0;
    if(harm_order < 5){
      harm_order++;
    }
    else{
      harm_order = 1;
    }
  }
}
*/
// Serial.println("");
								//////////////////////
								// END OF OLD CODE  //
								//////////////////////

delay(100);

}		//////////// END OF LOOP /////////////
