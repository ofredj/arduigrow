#include <dht.h>
#include <time.h>
#define ARDUIGROW_VERSION
#define LIGHT_PIN A0
#define PH_PIN 	A1
#define DHT11_PIN 13
#define LIGHT_ON 40
// if you want it to be highly accurate than do this step
#define ARDUINO_VOLTAGE 3.3
// PH_GAIN is (4000mv / (59.2 * 7)) // 4000mv is max output and 59.2 * 7 is the maximum range (in millivolts) for the ph probe. 
#define PH_GAIN 9.6525 

dht DHT;
long last_dht_read = 0;

bool _read_light()
{
  int light = analogRead(LIGHT_PIN);
  return (light <= LIGHT_ON) ? 1 : 0;
}

float _read_temperature(void)
{
  int chk = 0 ;
  if (!last_dht_read || time(NULL) > (last_dht_read + 2)){
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

float _read_humidity(void)
{
  int chk = 0 ;
  if (!last_dht_read || time(NULL) > (last_dht_read + 2)){
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

void read_light()
{
  bool light = _read_light();
  printf("Light: %s\n", (light) ? "on" : "off");
  if (light)
	exit(1);
}

void read_temperature()
{
	float t = _read_temperature();
	printf("Temperature: %.1f\n", t);
}

void read_humidity()
{
	float h = _read_humidity();
	printf("Humidity: %.1f%%\n", h);
}


void read_ph(void)
{
	float pH = _read_ph();
	printf("pH: %.1f\n", pH);
}


/*
 * read all sensors, output csv line
 */
void read_csv(void)
{
  bool light = _read_light(); // read light
  float pH = _read_ph();
  float humidity = _read_humidity();
  float temperature = _read_temperature();

  // output values
  printf("%lu,%.1f,%.1f,%s,%.1f\n", 
	time(NULL),
	humidity,
	temperature,
	(light) ? "true" : "false",
	pH );
}

void read_all(void)
{
  read_ph();
  read_humidity();
  read_temperature();
  read_light();
}

struct methods_t
{
  String method;
  void (*function)();
} methods_t ;

struct methods_t methods[] = {
  { "temp", read_temperature },
  { "humidity", read_humidity },
  { "light", read_light },
  { "ph", read_ph },
  { "csv", read_csv },
  { "all", read_all },
  { NULL, NULL }
};

/* TODO 
	- EC
	- water level switch
	- pump
*/

void setup()
{
}

void loop()
{
    if ( argc != 2 )
    {
        printf("Usage %s [temp|humidity|light|ph|csv|all]\n", argv[0]);
        exit(-1);
    }

    int i;
    for (i=0;methods[i].method != NULL;i++){
      if (methods[i].method == argv[1])
        methods[i].function() ;
    }
    exit(0);
}
