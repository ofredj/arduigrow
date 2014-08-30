#include <Arduino.h> // needs to be enabled

//uncomment to enable debugging statements printed to the serial line
#define DEBUG

//change this to the analog pin you're reading
#define READ_PIN A5
int CALIBRATION_SOLUTION_1 = 0;
int CALIBRATION_SOLUTION_2 = 0;
int CALIBRATION_VALUE_1_LOW = 0;
int CALIBRATION_VALUE_1_HIGH = 0;
int CALIBRATION_VALUE_2_LOW = 0;
int CALIBRATION_VALUE_2_HIGH = 0;


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
  
  printf("Now configure for pH: ");
	printf("%d\n", CALIBRATION_SOLUTION_1);
	printf("Please place the probe in solution now\n");
	printf("I will automatically calibrate this, but it may take some time\n");
	printf("I'll let you know when it's time to move on\n");
  reading = 0;
  while(matchCount < 250) {
		oldValue = newValue;
		reading = 0;
		printf("Not stable, carrying on\n");
		for(byte i=0; i<255; i++) {
		  reading = reading + analogRead(READ_PIN);
		  delay(10);
		}
		newValue = reading/255;

		if(newValue == oldValue) {
	  matchCount++;
		}

		printf("Old value: ");
		printf("%d\n",oldValue);
		printf("New value: ");
		printf("%d\n",newValue);
		printf("Match Count: ");
		printf("%d\n",matchCount);   
		delay(500); 
  }
  matchCount = 0;
  CALIBRATION_VALUE_1_LOW = lowValue(newValue);
  CALIBRATION_VALUE_1_HIGH = highValue(newValue); 
  oldValue = 1;
  newValue = 0; 

  
  printf("CALIBRATION_SOLUTION_1: ");
  printf("%d\n",CALIBRATION_SOLUTION_1);
  printf("CALIBRATION_VALUE_1_LOW: ");
  printf("%d\n",CALIBRATION_VALUE_1_LOW);
  printf("CALIBRATION_VALUE_1_HIGH: ");
  printf("%d\n\n",CALIBRATION_VALUE_1_HIGH);

  
  printf("Now configure for pH: ");
  printf("%d\n",CALIBRATION_SOLUTION_1);
  printf("Please place the probe in solution now\n");
  printf("I will automatically calibrate this, but it may take some time\n");
  printf("I'll let you know when it's time to move on\n");
delay(2000);
  reading = 0;
  while(matchCount < 250) {
	oldValue = newValue;
	reading = 0;    
	printf("Not stable, carrying on\n");
	reading = 0;
	for(byte i=0; i<255; i++) {
	  reading = reading + analogRead(READ_PIN);
	  delay(10);
	}
	newValue = reading/255;

	if(newValue == oldValue) {
	  matchCount++;
	}

	printf("Old value: %d\n",oldValue);
	printf("New value: %d\n",newValue);
	printf("Match Count: %d\n", matchCount);
	delay(500);
  }
  matchCount = 0;
  CALIBRATION_VALUE_2_LOW = lowValue(newValue);
  CALIBRATION_VALUE_2_HIGH = highValue(newValue);  


  printf("CALIBRATION_SOLUTION_2: ");
  printf("%d\n",CALIBRATION_SOLUTION_2);
  printf("CALIBRATION_VALUE_2_LOW: ");
  printf("%d\n",CALIBRATION_VALUE_2_LOW);
  printf("CALIBRATION_VALUE_2_HIGH: ");
  printf("%d\n",CALIBRATION_VALUE_2_HIGH);
}

float readpH() {
  int y1 = CALIBRATION_SOLUTION_1;
  int y2 = CALIBRATION_SOLUTION_2;
  int x1 = combineValue(CALIBRATION_VALUE_1_LOW, CALIBRATION_VALUE_1_HIGH);  
  int x2 = combineValue(CALIBRATION_VALUE_2_LOW, CALIBRATION_VALUE_2_HIGH);
  //work out m for y = mx + b
  float m = ((float)y1 - (float)y2) / ((float)x1 - (float)x2);
  //work out b for y = mx + 
  float b = (float)y1 - ((float)m * (float)x1);
  
  long reading = 0;
  for(byte i=0; i<255; i++) {
	reading = reading + analogRead(READ_PIN);
	delay(10);
  }
  reading = reading/255;
  
  float y = (m*(float)reading) + b;
  return y;
}

void setup() {
  delay(1000);
}

void loop() { 
  if ( argc != 3 )
  {
	printf("\nUsage %s ph_low ph_high\n", argv[0]);
	exit(0);
  }

  CALIBRATION_SOLUTION_1 = atoi(argv[1]);
  CALIBRATION_SOLUTION_2 = atoi(argv[2]);

  configure();  
  readpH();
}
