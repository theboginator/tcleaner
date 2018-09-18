/*TCleaner Controller r0.2.5
* This proprietary software is designed to be run on the McCleaner Controller board (ATMEGA 2560).
* Currently in beta testing, lots of bugs and lacking features with no set release date.
* Written by Jacob Bogner 9/5/2018
*/

/*
* Initialize the digital output pins that will switch relays to
* turn on the following devices:
*/
int pump = 1; //Set pin 1 to control Pump
int compressor = 2; //Set pin 2 to control Compressor
int compRelief = 3; //Set pin 3 to control Compressor Relief Solenoid
int compSupply = 4; //Set pin 4 to control Compressed Air Supply Solenoid
int compPurge = 5; //Set pin 5 to control Compressed Air Injection Solenoid
int waterSupply = 6; //Set pin 6 to control the water supply valve
int purgeSolenoid = 7; //Set pin 7 to control the Purge solenoid
int drainValve = 8; //Set pin 8 to control the Drain valve
int mainRelay = 11; //Set pin 11 to control the primary relay
					/*
					* Initialize the analog input pins that will monitor sensors
					* for the following devices:
					*/
int airPressurePin = 0; //Set analog pin 0 to monitor air pressure sensor
int supplyLevelPin = 1; //Set analog pin 1 to monitor the bucket supply level sensor
int prePumpFlowPin = 2; //Set analog pin 2 to monitor the pre-pump water pressure sensor
int postPumpFlowPin = 3; //Set analog pin 3 to monitor the post-pump water pressure sensor
int pumpTempPin = 4; //Set analog pin 4 to monitor the pump temperature sensor
int compTempPin = 5; //Set analog pin 5 to monitor the compressor temperature sensor
int powerSwitch = 6; //Set pin 9 to monitor the Power Switch status

					 /*
					 * Initialize the digital output pins that will switch relays to
					 * turn on the following accessory devices:
					 */
int leftFillValve = 9; //Set pin 10 to control the Left Hopper fill valve
int rightFillValve = 10; //Set pin 11 to control the Right Hopper fill valve
						 /*
						 * Initialize the analog input pins that will monitor sensors
						 * for the following accessory devices:
						 */
int leftFillProbe = 7; //Set analog pin 6 to monitor the Left Hopper fill sensor
int rightFillProbe = 8; //Set analog pin 7 to monitor the Right Hopper fill sensor

						/*
						* MODES:
						* 0: OFF
						* 1: STANDBY
						* 2: AUTO
						* 3: AUTO + FILL BUCKET
						* 4: AUTO + FILL HOPPER
						* 5: AIR DRY
						* 6: RINSE, DRAIN & PURGE
						*/

int previousMode = 0; //Variable that defines the previous mode of operation
int currentMode = 0; //Variable that defines the current mode of operation
int requestedMode = 0; //Variable that defines the requested mode from UI (set by serial data)
int airPressure = 0; //holds air pressure reading
int supplyFull = 0; //holds supply bucket level full reading
int supplyEmpty = 0; //Holds supply bucket level low reading
int postPumpFlow = 0;//Holds post-pump flow reading
int pumpTemp = 0; //Holds the pump temperature reading
int compTemp = 0; //Holds the compressor temperature reading
int leftFull = 0; //Holds the left hopper full reading
int rightFull = 0; //Holds the right hopper full reading
int powerSwitchStatus; //Holds the Power Switch sensor reading

boolean compEnabled = 1; //Determines whether or not the compressor can be used (set by serial data)
boolean pumpEnabled = 1; //Determines whether or not the pump can be used (set by serial data)
boolean useBucketSupply = 0; //Determines whether to use the supply bucket to feed water (set by serial data)
boolean powerSwitchOn; //Determines the status of the Power Switch. Either ON or OFF. Separate Status variable accounts for open circuit

int flowThreshold; //SET Minimum value for waterflow while pump is OFF
int bucketFilledThreshold; //SET Minimum value for water in supply tank (any reading less than this indicates bucket empty)
int hopperFilledThreshold; //SET Minimum value for water in the hopper (any reading less than this indicates hopper is not full)

String eventMsg = "No active events"; //String to contain variable event messages to transmit to UI
String errorMsg = "No fault found"; //String to contain variable error messages to transmit to UI
String warningMsg = "No fault found"; //String to contain variable warning messages to transmit to UI

int sendDataArray[9] = { currentMode, airPressure, supplyLevel, postPumpFlow, pumpTemp, compTemp, compEnabled, pumpEnabled, useBucketSupply };
String sendMsgArray[3] = { eventMsg, errorMsg, warningMsg };

void setup() {
	// put your setup code here, to run once:
	configureOutputs(); //Initialize the output pins
	setSerialLink(); //Set up the serial link
	readInputs(); //Read the inputs to see status
	checkFault(); //Check for a fault
	sendData(); //Send serial data
	getData(); //Recieve serial data
	while !powerSwitchOn{
		//If the power switch is not on, complete initialization and wait.
		readInputs();
	checkFault();
	//delay?
	sendData();
	getData();
	}

}

void configureOutputs() {
	/*
	* This function should be called ONLY during SETUP to initialize the outputs of the system
	*/
	pinMode(mainRelay, OUTPUT); //Set pin to control the main relay
	pinMode(pump, OUTPUT); //Set pin to control pump relay
	pinMode(compressor, OUTPUT); //Set pin to control compressor relay
	pinMode(compRelief, OUTPUT); //Set pin to control compressor relief valve relay
	pinMode(compSupply, OUTPUT); //Set pin to control compressor supply valve relay
	pinMode(waterSupply, OUTPUT); //Set pin to control water supply valve relay
	pinMode(purgeSolenoid, OUTPUT); //Set pin to control the purge solenoid relay
	pinMode(drainValve, OUTPUT); //Set pin to control the drain valve relay
	pinMode(leftFillValve, OUTPUT); //Set pin to control the Left Hopper Fill valve relay
	pinMode(rightFillValve, OUTPUT); //Set pin to control the Right Hopper Fill valve relay
}

void setSerialLink() {
	Serial.begin(9600);
}


void loop() {
	// put your main code here, to run repeatedly:
	readInputs(); //Read all sensor data
	checkFault(); //Check current data against expected values and handle errors
	setOutputs(); //Given current information and data, set outputs accordingly
	sendData(); //Send serial data
	getData(); //Recieve serial data

}

void readInputs() {
	/*
	* Read all data from the analog input pins and store them in appropriate variables:
	*/
	airPressure = analogRead(airPressurePin);
	supplyFull = analogRead(supplyFullPin);
	supplyEmpty = analogRead(supplyEmptyPin);
	postPumpFlow = analogRead(postPumpFlowPin);
	pumpTemp = analogRead(pumpTempPin);
	compTemp = analogRead(compTempPin);
	powerSwitchStatus = analogRead(powerSwitch);
}

void checkfault() {
	/*
	* Check all input values for open/short and/or failure indications and take appropriate action
	*/
	if (powerSwitchStatus > 1000) {
		// Indicates short-circuit (Power Switch ON)
		powerSwitchOn = true;
	}
	else if (powerSwitchStatus < 500) { //SET NUMBER TO SUPERVISION READING
		powerSwitchOn = true;
	}
}

void sendData() {

}

void automatic() {


}

void checkmode() {

}
