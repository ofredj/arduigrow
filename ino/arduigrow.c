#include <time.h>
#include <dht.h>

/* Constants */
#define ARDUIGROW_VERSION 0.1

#define LIGHT_ON 50

/* Analog sensors */
#define LIGHT_PIN A0
#define PH_PIN 	A5

/* Digital sensors */
#define EC_ENABLE_PIN 4
#define EC_READING_PIN 7

#define PUMP_FILL 9
#define PUMP_EMPTY 10

#define WATERLEVEL_LOW_PIN 11
#define WATERLEVEL_HIGH_PIN 12

#define DHT11_PIN 13

/* PH Calibration Values */
#define CALIBRATION_SOLUTION_1 1
#define CALIBRATION_SOLUTION_2 2
#define CALIBRATION_VALUE_1_LOW 3
#define CALIBRATION_VALUE_1_HIGH 4
#define CALIBRATION_VALUE_2_LOW 5
#define CALIBRATION_VALUE_2_HIGH 6

dht DHT;
static unsigned long last_dht_read = 0;

/*
 * Read sensor value from analog pin
 * returns bool value according to heuristic treshold
 * 0 light is off
 * 1 light is on
 */
bool read_light()
{
  int light = analogRead(LIGHT_PIN);
  printf("light: %d\n", light);
  return (light <= LIGHT_ON) ? 1 : 0;
}

/*
 * Read sensor value from digital pin
 * returns celcius value 
 */
float read_temperature(void)
{
  int chk = 0 ;

  /* don't stress the sensor, 1 read every 3 seconds max */
  if (!last_dht_read || time(NULL) > (last_dht_read + 3)){
	chk = DHT.read11(DHT11_PIN);
  } else {
        return(DHT.temperature);
  }

  switch (chk)
  {
    case DHTLIB_OK:  
	    last_dht_read = time(NULL);
        return(DHT.temperature);
        break;
    case DHTLIB_ERROR_CHECKSUM: 
        printf("Checksum error\n"); 
        break;
    case DHTLIB_ERROR_TIMEOUT: 
        printf("DHT.temperature: Time out error\n"); 
        break;
    default: 
        printf("Unknown error\n"); 
        break;
  }
  return 0;
}

/*
 * Read sensor value from digital pin
 * returns humidity percentage value 
 */
float read_humidity(void)
{
  int chk = 0 ;
  /* don't stress the sensor, 1 read every 3 seconds max */
  if (!last_dht_read || time(NULL) > (last_dht_read + 5)){
	chk = DHT.read11(DHT11_PIN);
  } else {
        return(DHT.humidity);
  }
  switch (chk)
  {
    case DHTLIB_OK:  
	last_dht_read = time(NULL);
        return(DHT.humidity);
        break;
    case DHTLIB_ERROR_CHECKSUM: 
        printf("Checksum error\n"); 
        break;
    case DHTLIB_ERROR_TIMEOUT: 
        printf("DHT.humidity: Time out error\n"); 
        break;
    default: 
        printf("Unknown error\n"); 
        break;
  }
  return 0;
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

/* 
 * Read ph from sensors 
 * returns ph float value
 */
float read_ph()
{
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
	reading = reading + analogRead(PH_PIN);
	delay(10);
  }
  reading = reading/255;
  
  float y = (m*(float)reading) + b;
  return y;
}


#define EC_SAMPLES 1024
#define EC_LOW_FREQ 19424
#define EC_LOW 53
#define EC_HIGH_FREQ 29421
#define EC_HIGH 80
/* 
 * Read ph from sensors 
 * adjust with temperature
 * returns ph float value
 */
float read_ec()
{
    unsigned long freqhigh = 0;
    unsigned long freqlow = 0;
    unsigned long frequency = 0;

    digitalWrite(EC_ENABLE_PIN, HIGH);

    for(unsigned int j=0; j<EC_SAMPLES; j++){
        freqhigh+= pulseIn(EC_READING_PIN, HIGH, 250000);
        freqlow+= pulseIn(EC_READING_PIN, LOW, 250000);
    }
 
    frequency = 1000000 / ( (freqhigh / EC_SAMPLES) + (freqlow / EC_SAMPLES) );
 
    // now we need to work out all the values for this formula
    // y = mx + b
    float m = (EC_HIGH_FREQ - EC_LOW_FREQ) / (EC_HIGH - EC_LOW);
    float b = EC_LOW_FREQ - (m * EC_LOW);
    float x = ((float)frequency - b) / m;
 
    /*
     * to temperature compensate this you'll need to get a temp reading and plug it in to this formula:
     * EC25 = ECf / (1 + x(t - 25)) change 25 to 18 if you want to compensate to 18.
     */
 
    //lets say it's 29 degrees C
    float temp = read_temperature();
    float EC25 = x / (1 + (0.019 * (temp - 25)));
 
    digitalWrite(EC_ENABLE_PIN, LOW);  
    return EC25;
}

/*
 * Read the two water level switchs sensors
 * returns the sum values of sensors
 * 0 water level is bellow the bottom sensor
 * 1 water level is between the two sensors
 * 2 water level is over the high sensor
 */
int read_water()
{
  int water_lvl = 0; 
  water_lvl += digitalRead(WATERLEVEL_LOW_PIN);
  water_lvl += digitalRead(WATERLEVEL_HIGH_PIN);
  return water_lvl;
}

/*
 * Turn on pump on given digital pin for howlong seconds
 */
void pump(int pin, int howlong)
{
  printf("pump %d on\n", pin);
  digitalWrite(pin,LOW);   // set pin low  
  delay( howlong * 100);            // wait for 10 sec
  digitalWrite(pin,HIGH);  // set pin high
  printf("pump %d off\n", pin);
}

/*
 * Pump for 5 seconds while water level switch
 * is not low
 */
void tank_empty()
{
  int water = read_water();	
  while (true){
    pump( PUMP_EMPTY, 10);
    water = read_water();
    printf("water %d\n", water);
    if (water == 0)
	break;
  }
}

/*
 * Pump for 5 seconds while water level switch 
 * is not high
 */
void tank_fill()
{
  int water = read_water();	
  while (true){
    pump( PUMP_FILL, 10);
    water = read_water();
    printf("water %d\n", water);
    if (water == 2)
	break;
  }
}

/* 
 * -- Pretty print stuff --
 */
void print_light(void)
{
  bool light = read_light();
  printf("Light: %s\n", (light) ? "on" : "off");
  if (light)
	exit(1);
}

void print_temperature(void)
{
	float t = read_temperature();
	printf("Temperature: %.1f\n", t);
}

void print_humidity(void)
{
	float h = read_humidity();
	printf("Humidity: %.1f%%\n", h);
}


void print_ph(void)
{
	float pH = read_ph();
	printf("pH: %.1f\n", pH);
}

void print_ec(void)
{
	float ec = read_ec();
	printf("EC: %.1f\n", ec);
}

void print_water(void)
{
    int lvl = read_water();
    char *lvl_string = NULL;
    switch (lvl)
    {
      case 2:
        lvl_string = (char *)"FULL";
        break;
      case 1:
        lvl_string = (char *)"OK";
        break;
      default:
        lvl_string = (char *)"EMPTY";
        break;
    }
	printf("Water Level: %s %d\n",lvl_string, lvl);
}

/*
 * read all sensors, output csv line
 */
void print_csv(void)
{
  bool light = read_light(); // read light
  float pH = read_ph();
  float ec = read_ec();
  int water = read_water();
  float humidity = read_humidity();
  float temperature = read_temperature();

  printf("%lu,%.1f,%.1f,%s,%.1f,%.1f,%d\n", 
	time(NULL),
	humidity,
	temperature,
	(light) ? "true" : "false",
	pH,
        ec,
	water
    );
}

void print_version(void)
{
	printf("ArduiGrow Version %s\n", ARDUIGROW_VERSION);
}

void print_all(void)
{
  print_version();
  print_light();
  print_humidity();
  print_temperature();
  print_water();
  print_ph();
  print_ec();
}

struct methods_t
{
  String method;
  void (*function)();
} methods_t ;

struct methods_t methods[] = {
	{ "temp", print_temperature },
  { "humidity", print_humidity },
  { "light", print_light },
  { "water", print_water },
  { "ph", print_ph },
  { "ec", print_ec },
  { "tank_fill", tank_fill },
  { "tank_empty", tank_empty },
  { "csv", print_csv },
  { "all", print_all },
  { NULL, NULL }
};

/*
 * Arduino lib callbacks
 */

void setup()
{
  pinMode(WATERLEVEL_LOW_PIN, INPUT); 
  pinMode(WATERLEVEL_HIGH_PIN, INPUT); 
  pinMode(PUMP_FILL, OUTPUT);
  digitalWrite(PUMP_FILL,HIGH);  // set pin high
  pinMode(PUMP_EMPTY, OUTPUT);
  digitalWrite(PUMP_EMPTY,HIGH);  // set pin high
	pinMode(EC_ENABLE_PIN, INPUT);
  pinMode(EC_READING_PIN, OUTPUT);
}

void loop()
{
	if ( argc != 2 )
  {
		printf("\nUsage %s [command]\n", argv[0]);
    printf("\ttemp:\t\tshow temperature celcius\n");
    printf("\thumidity:\tshow humidity percentage\n");
    printf("\tlight:\t\tshow light status\n");
    printf("\tec:\t\tshow EC\n");
    printf("\tph:\t\tshow pH\n");
    printf("\twater:\t\tshow water tank level\n");
    printf("\ttank_fill:\tfill water tank\n");
    printf("\ttank_empty:\tempty water tank\n");
    printf("\tcsv:\t\toutput a csv line with all sensors values\n");
    printf("\tall:\t\tshow all sensors values\n\n");
		print_version();
    exit(-1);
  }

  int i;
  for (i=0;methods[i].method != NULL;i++){
    if (methods[i].method == argv[1])
      methods[i].function() ;
  }
  exit(0);
}
