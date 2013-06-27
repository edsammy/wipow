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

Major Change June 27, 2013      New communication method. Arduino sends GET request containing collected data which is parsed by "../website/arduino_DBconnect.php" 

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
//              ***June 27 update allows for user inputs to select which measurements to take but does not allow calibration***

			///////////////////
			// END OF TODOs //
			///////////////////


#include <SPI.h>
#include <WiFly.h>	// **** SpiUart.cpp's  begin() function has its call to SPI.begin() commented out!! This is to allow SPI settings for communication to the Maxim chip later.

const String deviceName = "cogen";

long upper_read, lower_read;    // When calling MaximRead, it will update these two variables with the read values (4 bytes per long variable, 8 bytes max).

long response;  // byte by byte response of the SPI communication protocol with the MAXIM chip. (Used for maxim_write and maxim_read)

//***Select the data you want to collect, 1 means collect it, 0 means don't (refer to user_sel_read for data order)***
String user_input="111111111111111111111111000000000000000000000000000000000";
//String user_input="111111111111111111111111111111111111111111111111111111110";

const int norm_functs = 24;  // number of non-harmonic MAXIM measurements (used for user input parsing)
const int harm_functs = 33;   // number of harmonic MAXIM measurements
const int total_functs = norm_functs + harm_functs;

const int data_led = 8;  // Status LED to indicate data TX (v3)
long timestamp;
long interval = 10000;

char site[] = "www.ece.rochester.edu";
String GETcommands = "GET /projects/power/tmp/";
int lineCount = 0;
String currLine;
boolean done = false;
String data = "";

WiFlyClient client;


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
  SpiSerial.write("\r", 1);	        // ** Wifly is already set to auto-join "Roving1".
  delay(3000);				//    This is just incase it doesn't.
    while(SpiSerial.available() > 0) {
    Serial.write(SpiSerial.read());
    delay(50);
  }
  
  String IP = WiFly.ip();
  Serial.println();
  Serial.println(IP);
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

  //while(!SpiSerial.available());	// Idle until WiFly has something to output (it should then mean that someone is trying to connect to our WiFly)
									// Ideally, that "someone" should be the University's server running our webpage's code, which would accept data from this device.
									// However, any pings or other devices could also try to connect to our device. 
									// In which case, this code does not know/care and would act the same way (i.e. blindly pushes out data)
									// TODO: Improve code so that it knows only to respond to the University's server running our webpage's code?
  
  //delay(500);	// Althought it adds significantly to the delay overhead of the entire data gathering system, these delays seem to be important.
				// Without them, the code runs too fast for (I believe) WiFly to handle.
				// TODO: Can fiddle around with these delays to lower them, or look at WiFly module more closely to see what is slowing it down.
  
  Serial.print("Getting user commands from web...");
  HTTP_GETcommands(GETcommands);
  
  // Parse the user input string and enter binary instructions into the "commands" array  
  user_input.toCharArray(charBuffer, total_functs);
  for (int i=0; i<total_functs-1; i++){
    commands[i] = (charBuffer[i]) - 48;
  }
  Serial.println("Done!");
  
  digitalWrite(data_led, HIGH);
  timestamp = millis();
   
  user_sel_read();	// This calls "user_sel_read.ino" and must pass a user_input parameter (String)
  while (!MaximWrite("004", "0000")){}      // Clear the chip's DSP READY BIT so the chip can set it again when its DSP is ready.
  delay(500);  
  //SpiSerial.write(41);  // ")"
  //SpiSerial.write(59);  // ";"  To finish the MySQL statement that normalread was pushing out (IF "harmread()" IS DISABLED/COMMENTED OUT)
  
  // Serial print total elapsed time to collect data
  Serial.println();
  Serial.print("Total DAQ time: ");
  Serial.println(millis()-timestamp);
  data.trim();
  HTTP_POSTrequest(data);
  Serial.print("Data: ");
  Serial.println(data);
  
//  SpiSerial.write("$$$", 3);				// CMD mode of WiFly
//  delay(300);
//  while(SpiSerial.available() > 0) {
//    Serial.write(SpiSerial.read());
//    delay(5);
//  }
//  SpiSerial.write("close", 5);			// Tell WiFly to close the HTTP connection since we should have finished transmitting the data.
//  SpiSerial.write("\r", 2);
//  delay(100);
//  while(SpiSerial.available() > 0) {
//    Serial.write(SpiSerial.read());
//    delay(5);
//  }
//    
//  SpiSerial.write("exit", 4);				// Exit CMD mode of WiFly
//  SpiSerial.write("\r", 1);
//  delay(100);
//
//while(SpiSerial.available() > 0) {		// Flushing of WiFly output (whatever's left, exit command should get "EXIT" response from WiFly, etc...)
//  Serial.write(SpiSerial.read());
//  delay(5);
//}
  
  
  digitalWrite(data_led, LOW);
  timestamp = millis();
  while (timestamp + interval > millis()){
  // Delay between readings
  }

}//////////// END OF LOOP /////////////


