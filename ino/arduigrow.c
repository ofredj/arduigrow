#include <time.h>
#include <dht.h>

/* Constants */
#define ARDUIGROW_VERSION 0.1
#define LIGHT_ON 40

/* Analog sensors */
#define LIGHT_PIN A0
#define PH_PIN 	A1

/* Digital sensors */
#define EC_ENABLE_PIN 4
#define EC_READING_PIN 7
#define PUMP_FILL 9
#define PUMP_EMPTY 10
#define WATERLEVEL_LOW_PIN 11
#define WATERLEVEL_HIGH_PIN 12
#define DHT11_PIN 13

#define EC_SAMPLES 4096
#define EC_LOW_FREQ 19424
#define EC_LOW 53
#define EC_HIGH_FREQ 29421
#define EC_HIGH 80
	 
// if you want it to be highly accurate than do this step
#define ARDUINO_VOLTAGE 3.3
// PH_GAIN is (4000mv / (59.2 * 7)) // 4000mv is max output and 59.2 * 7 is the maximum range (in millivolts) for the ph probe. 
#define PH_GAIN 9.6525 

dht DHT;
static unsigned long last_dht_read = 0;

/*
 * Read sensor value from analog pin
 * returns bool value according to heuristic treshold
 * 0 light is off
 * 1 light is on
 */
bool _read_light()
{
  int light = analogRead(LIGHT_PIN);
  return (light <= LIGHT_ON) ? 1 : 0;
}

/*
 * Read sensor value from digital pin
 * returns celcius value 
 */
float _read_temperature(void)
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
float _read_humidity(void)
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

float _read_ph()
{
	int i; 
	int reading; 
	int millivolts; 
	float ph_value = 0;
	//take a sample of 50 readings 
	reading = 0; 

	for(i = 1; i < 50; i++){
 		reading += analogRead(PH_PIN); 
		delay(10); 
 	}

	//average it out 
	reading /= i; 
	// convert to millivolts. remember for higher accuracy measure your arduino&#39;s 
	// voltage with a multimeter and change ARDUINO_VOLTAGE 
 	millivolts = ((reading * ARDUINO_VOLTAGE) / 1024) * 1000; 
	printf("millivolts: %d \n", millivolts);
 	ph_value = ((millivolts / PH_GAIN) / 59.2) + 7; 
	return(ph_value);
}


float _read_ec()
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
    float EC25 = x / (1 + (0.019 * (2 - 25)));
 
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
int _read_water()
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
  digitalWrite(pin,HIGH);  // set pin high
  delay( howlong * 100);            // wait for 10 sec
  digitalWrite(pin,LOW);   // set pin low  
}

/*
 * Pump for 5 seconds while water level switch
 * is not low
 */
void tank_empty()
{
  int water = _read_water();
  while (water =! 0){
    pump( PUMP_EMPTY, 5);
    water = _read_water();
  }
}

/*
 * Pump for 5 seconds while water level switch 
 * is not high
 */
void tank_fill()
{
  int water = _read_water();
  while (water =! 2){
    pump( PUMP_FILL, 5);
    water = _read_water();
  }
}

/* 
 * -- Pretty print stuff --
 */
void print_light(void)
{
  bool light = _read_light();
  printf("Light: %s\n", (light) ? "on" : "off");
  if (light)
	exit(1);
}

void print_temperature(void)
{
	float t = _read_temperature();
	printf("Temperature: %.1f\n", t);
}

void print_humidity(void)
{
	float h = _read_humidity();
	printf("Humidity: %.1f%%\n", h);
}


void print_ph(void)
{
	float pH = _read_ph();
	printf("pH: %.1f\n", pH);
}

void print_ec(void)
{
	float ec = _read_ec();
	printf("EC: %.1f\n", ec);
}

void print_water(void)
{
	float lvl = _read_water();
    char *lvl_string = NULL;
    switch (lvl){
      case 2:
        lvl_string = "FULL";
        break;
      case 1:
        lvl_string = "OK";
        break;
      default:
        lvl_string = "EMPTY";
        break;
    }
	printf("Water Level: %s\n",lvl_string);
}

/*
 * read all sensors, output csv line
 */
void print_csv(void)
{
  bool light = _read_light(); // read light
  float pH = _read_ph();
  float ec = _read_ec();
  float water = _read_water();
  float humidity = _read_humidity();
  float temperature = _read_temperature();

  printf("%lu,%.1f,%.1f,%s,%.1f\n", 
	time(NULL),
	humidity,
	temperature,
	(light) ? "true" : "false",
	pH,
    ec,
    );
}

void print_all(void)
{
  print_ph();
  print_ec();
  print_water();
  print_humidity();
  print_temperature();
  print_light();
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
    pinMode(PUMP_EMPTY, OUTPUT);
    pinMode(EC_ENABLE_PIN, INPUT);
    pinMode(EC_READING_PIN, OUTPUT);
}

void loop()
{
    if ( argc != 2 )
    {
        printf("Usage %s [command]\n", argv[0]);
        printf("\ttemp:\tshow temperature celcius\n");
        printf("\thumidity:\tshow humidity percentage\n");
        printf("\tlight:\tshow light status\n");
        printf("\tec:\tshow EC\n");
        printf("\tph:\tshow pH\n");
        printf("\twater:\tshow water tank level\n");
        printf("\ttank_fill:\tfill water tank\n");
        printf("\ttank_empty:\tempty water tank\n");
        printf("\tcsv:\toutput a csv line with all sensors values\n");
        printf("\tall:\tshow all sensors values\n");
        exit(-1);
    }

    int i;
    for (i=0;methods[i].method != NULL;i++){
      if (methods[i].method == argv[1])
        methods[i].function() ;
    }
    exit(0);
}
