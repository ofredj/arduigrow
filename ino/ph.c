#include <Arduino.h> // needs to be enabled
#include <EEPROM.h> // needs to be enabled

//uncomment to enable debugging statements printed to the serial line
#define DEBUG

//change this to the analog pin you're reading
#define READ_PIN 5
#define CALIBRATION_SOLUTION_1_MEM 1
#define CALIBRATION_SOLUTION_2_MEM 2
//takes 2 bytes to hold the analog value
#define CALIBRATION_VALUE_1_MEM 3
//takes 2 bytes to hold the analog value
#define CALIBRATION_VALUE_2_MEM 5

void setup() {
  Serial.begin(9600);  
  delay(1000);
  Serial.println("Send c to configure");
  Serial.println("Send r to read value");
}

void loop() {
  Serial.print("pH: ");
  Serial.println(readpH(), 2);   
  delay(1000);
  if(Serial.available() > 0) {
	byte incomingByte = Serial.read();
	//c = 99
	//r = 114
	if(incomingByte == 99) {
	  configure();  
	} else if(incomingByte == 114) {
	  Serial.print("pH: ");
	  Serial.println(readpH(), 2); 
	}
  }
}

int highValue(int value) {
  return value / 256;
}

int lowValue(int value) {
  return value % 256; 
}

int combineValue(unsigned int lb, unsigned int hb) {
  return ((hb * 256) + lb);
}

void configure() {
  byte incomingByte;
  byte counter = 0;
  
  int serialData;
  int oldValue = 1;
  int newValue = 0;

  int matchCount = 0;
  byte forCounter = 0;
  long reading = 0;
  
  while(Serial.available() == 0) {
	Serial.println("Please type in the value of the first calibration solution"); 
	delay(5000); 
  }
  if(Serial.available() > 0) {
	while(Serial.available() > 0) {
	  incomingByte = Serial.read() - '0';
	  if(counter == 0) {
		serialData = incomingByte;
		counter++;
	  } else if(counter == 1) {
		serialData = (serialData*10) + incomingByte;
	  }
	}
	counter = 0;
	EEPROM.write(CALIBRATION_SOLUTION_1_MEM, serialData);

	Serial.print("Now configure for pH: ");
	Serial.println(EEPROM.read(CALIBRATION_SOLUTION_1_MEM));
	Serial.println("Please place the probe in solution now");
	Serial.println("I will automatically calibrate this, but it may take some time");
	Serial.println("I'll let you know when it's time to move on");
  }
  reading = 0;
  while(matchCount < 250) {
	oldValue = newValue;
	reading = 0;
	Serial.println("Not stable, carrying on");
	for(byte i=0; i<255; i++) {
	  reading = reading + analogRead(READ_PIN);
	  delay(10);
	}
	newValue = reading/255;

	if(newValue == oldValue) {
	  matchCount++;
	}

	Serial.print("Old value: ");
	Serial.println(oldValue);
	Serial.print("New value: ");
	Serial.println(newValue);
	Serial.print("Match Count: ");
	Serial.println(matchCount);   
	delay(500); 
  }
  matchCount = 0;
  EEPROM.write(CALIBRATION_VALUE_1_MEM, lowValue(newValue));
  EEPROM.write(CALIBRATION_VALUE_1_MEM+1, highValue(newValue)); 
  oldValue = 1;
  newValue = 0; 
  
  
  
  //second calibration solution
  while(Serial.available() == 0) {
	Serial.println("Please type in the value of the second calibration solution"); 
	delay(5000); 
  }
  if(Serial.available() > 0) {
	while(Serial.available() > 0) {
	  incomingByte = Serial.read() - '0';
	  if(counter == 0) {
		serialData = incomingByte;
		counter++;
	  } else if(counter == 1) {
		serialData = (serialData*10) + incomingByte;
	  }
	}
	counter = 0;
	EEPROM.write(CALIBRATION_SOLUTION_2_MEM, serialData);

	Serial.print("Now configure for pH: ");
	Serial.println(EEPROM.read(CALIBRATION_SOLUTION_2_MEM));
	Serial.println("Please place the probe in solution now");
	Serial.println("I will automatically calibrate this, but it may take some time");
	Serial.println("I'll let you know when it's time to move on");
  }
  reading = 0;
  while(matchCount < 250) {
	oldValue = newValue;
	reading = 0;    
	Serial.println("Not stable, carrying on");
	reading = 0;
	for(byte i=0; i<255; i++) {
	  reading = reading + analogRead(READ_PIN);
	  delay(10);
	}
	newValue = reading/255;

	if(newValue == oldValue) {
	  matchCount++;
	}

	Serial.print("Old value: ");
	Serial.println(oldValue);
	Serial.print("New value: ");
	Serial.println(newValue);
	Serial.print("Match Count: ");
	Serial.println(matchCount);    
	delay(500);
  }
  matchCount = 0;
  EEPROM.write(CALIBRATION_VALUE_2_MEM, lowValue(newValue));
  EEPROM.write(CALIBRATION_VALUE_2_MEM+1, highValue(newValue));  
}

float readpH() {
  int y1 = EEPROM.read(CALIBRATION_SOLUTION_1_MEM);
  int y2 = EEPROM.read(CALIBRATION_SOLUTION_2_MEM);
  int x1 = combineValue(EEPROM.read(CALIBRATION_VALUE_1_MEM), EEPROM.read(CALIBRATION_VALUE_1_MEM+1));  
  int x2 = combineValue(EEPROM.read(CALIBRATION_VALUE_2_MEM), EEPROM.read(CALIBRATION_VALUE_2_MEM+1));
  //work out m for y = mx + b
  float m = ((float)y1 - (float)y2) / ((float)x1 - (float)x2);
  //work out b for y = mx + 
  float b = (float)y1 - ((float)m * (float)x1);
  
  #ifdef DEBUG
  Serial.print("X1: ");
  Serial.println(x1);
  Serial.print("X2: ");
  Serial.println(x2);  
  Serial.print("Y1: ");
  Serial.println(y1);  
  Serial.print("Y2: ");
  Serial.println(y2);  
  Serial.print("M: ");
  Serial.println(m);
  Serial.print("B: ");
  Serial.println(b);
  #endif

  long reading = 0;
  for(byte i=0; i<255; i++) {
	reading = reading + analogRead(READ_PIN);
	delay(10);
  }
  reading = reading/255;
  #ifdef DEBUG
  Serial.print("Analog: ");
  Serial.println(reading);
  #endif
  
  float y = (m*(float)reading) + b;
  return y;
}
