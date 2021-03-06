#ifndef __MY_CONFIG_H__
#define __MY_CONFIG_H__

#define TAG "ZAHRADA"

#ifndef MIN
#define MIN(x, y)  ((x) < (y) ? (x) : (y))
#endif

#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASSWORD CONFIG_WIFI_PASSWORD

//******** Define level(s) of logging ***********
#undef LOG_LOCAL_LEVEL
// #define LOG_LOCAL_LEVEL ESP_LOG_ERROR
// #define LOG_LOCAL_LEVEL ESP_LOG_WARNING
#define LOG_LOCAL_LEVEL ESP_LOG_INFO
// #define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
// #define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
//***********************************************

//******** Buffer size for flashing ROM**********
#define BUFFSIZE 1024
//***********************************************

//******** Maximum size of tokens in HTTP *******
#define MAXTOKENSIZE 64
//***********************************************

//******** RELAY PINs connections ***************
#define RELAY_1_0 17
#define RELAY_1_1 16
#define RELAY_1_2 4
#define RELAY_1_3 2    // Bootstrap - "Low" for flashing
#define RELAY_2_0 23
#define RELAY_2_1 18
#define RELAY_2_2 5    // Blue onboard LED 
#define RELAY_2_3 19
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<RELAY_1_0) | (1ULL<<RELAY_1_1) | (1ULL<<RELAY_1_2) | (1ULL<<RELAY_1_3) | (1ULL<<RELAY_2_0) | (1ULL<<RELAY_2_1) | (1ULL<<RELAY_2_2) | (1ULL<<RELAY_2_3))

// index in arrays     0          1           2                 3                       4          5          6            7
#define RELAYS_GPIOS { RELAY_1_0, RELAY_1_1,  RELAY_1_2,        RELAY_1_3,              RELAY_2_0, RELAY_2_1, RELAY_2_2,   RELAY_2_3 }
#define RELAYS_NAMES { "Rajčata", "Zelenina", "Habry u Prejzy", "rybíz a habry u pole", "tráva",   "bazén",   "RELAY_2_2", "RELAY_2_3" }
#define RELAYS_USED 8

typedef struct {
    char *name;
    int gpio_num;
    int state;
    int enabled;
} relay_t;

extern relay_t relays[];
extern const int relays_gpios[];
extern char* relays_names[];
//***********************************************

//******** Intervals definitions ****************
// structure for single start definition
typedef struct {
    int relay;          // relay index
    // uint16_t monthbits;    // bit array of month (bits 0-11 toggle the months)
    uint8_t daybits;        // bit array of week (0 = monday to 6 = sunday)
                            // i.e.: 0b01000000 = sunday
                            // i.e.: 0b00000001 = monday
                            // i.e.: 0b00011111 = weekdays
                            // i.e.: 0b01100000 = weekend (saturday + sunday)
    int hour;
    int minute;
    int duration;       // duration in minutes
} showtime_t;

#define RAJCATA \
  {0, 0b01111111, 6, 0, 5}, \
  {0, 0b01111111, 6, 30, 5}, \
  {0, 0b01111111, 18, 0, 5}, \
  {0, 0b01111111, 18, 30, 5}
//***********************************************


//******** Definitions for the I2C bus **********
#define I2C_BUS (0)
#define SCL_PIN (5)
#define SDA_PIN (4)
#define I2C_MASTER_WRITE 0x0   /*!< I2C write data */
#define I2C_MASTER_READ 0x1   /*!< I2C read data */
//***********************************************

//******** DHT senzor definitions ***************
// uint8_t const dht_gpio = 4;
#define GPIO_DHT (4)
// const dht_sensor_type_t sensor_type = DHT_TYPE_DHT11;
#define SENSOR_TYPE (dht_sensor_type_t)DHT_TYPE_DHT12
//***********************************************

//******** MOSQUITTO CONNECTION *****************
/* You can use http://test.mosquitto.org/ to test mqtt_client instead
 * of setting up your own MQTT server */
// #define MQTT_HOST ("192.168.1.21")
#define MQTT_HOST ("cloud.iot-playground.com")
#define MQTT_PORT 1883

#define MQTT_USER "Droncz"
#define MQTT_PASS "Sentine.75"

#define MQTT_TOPIC_MAX_LENGTH 20
#define MQTT_TOPIC_MOD1_TEMPE "/1/D1mini-01.Tempe"
#define MQTT_TOPIC_MOD1_HUMID "/1/D1mini-01.Humid"
#define MQTT_TOPIC_MOD1_SOILM "/1/D1mini-01.SoilM"
#define MQTT_TOPIC_MOD2_LIGHT1 "/2/Sensor.Parameter1"

#define MQTT_PUB_TOPIC "esp8266/status"
#define MQTT_SUB_TOPIC "esp8266/control"

// #define PUB_MSG_LEN 64
#define PUB_MSG_LEN 16
// Constant used for delimiting new portion of data
#define DATA_START 0xffff
//***********************************************

//******** Soil moisture sensor levels **********
// What are the border levels of the sensor found by testing
#define DRY_SENSOR 1024
#define WET_SENSOR 400
//***********************************************


//******** SSD1306 OLED display******************
// Next line selects I2C or SPI display connection 
// #define I2C_CONNECTION

// Change this according to you schematics and display size
#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 64

#ifdef I2C_CONNECTION
    #define PROTOCOL SSD1306_PROTO_I2C
    #define ADDR     SSD1306_I2C_ADDR_0
#else
    #define PROTOCOL SSD1306_PROTO_SPI4
    #define CS_PIN   15
    #define DC_PIN   12
#endif
//***********************************************



// For the delays...
#define SECOND (1000 / portTICK_PERIOD_MS)

//******** Global variables definition **********
// extern float temperature;
// extern float humidity;
// extern float pressure;
// extern uint16_t analogValue;
extern int32_t restart_counter;
//***********************************************

#endif // ifndef __MY_CONFIG_H__