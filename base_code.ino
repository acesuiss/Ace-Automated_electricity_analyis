
/*******************************************************************************************
*                                                                                         *
              Project Title : Automated Billing , Base Code
             Date          : 13 Aug 2022                                                 *
              Author        : last edited by Sudharsan
              Board         : Esp8266
              Essentials    : Blynk_iot Accouct And blynk Libriaries
*                                                                                         *
*******************************************************************************************

  /* ********** defining Required blynk iot Libraries ********** */


#define BLYNK_TEMPLATE_ID "TMPLB9mvZmsn"
#define BLYNK_DEVICE_NAME "Testing ACE esp8266 Mark7"
#define BLYNK_FIRMWARE_VERSION "0.1.2"                 // frimware version
#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
#define APP_DEBUG
#define USE_NODE_MCU_BOARD
#include "BlynkEdgent.h"


/* ********** Defining the Required pins ********** */

#define Relay1  16   //d0
#define Relay2  5    //d1
#define Relay3  4    //d2
#define Relay4  0    //d3

#define switch1 2    //d4
#define switch2 14   //d5
#define switch3 12   //d6
#define switch4 15   //d7


/* ********** Function Prototype ********** */

void Runtime_counter(struct device_data * device);
void sence_ace();
void process_ace();
void calculate(struct time * str);
void broadcast_ace();
float get_slab_rate(float);

/* ********** To Store the Active Time  ********** */

struct device_data {
  double sec =       0;
  double minute =     0;
  double hour =      0;
  double total_min = 0;
  int power = 100;
  float units = 0 ;
  double total_amount = 0;
  float slab_rate ;

};

/* ********** Time Data for four devices ********** */

struct device_data device1, device2, device3, device4;


void setup()
{
  Serial.begin(115200);
  delay(100);
  /* ********** Setting the Relays as Outputs ********** */
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  /* ********** Setting the Switches as Inputs ********** */
  pinMode(switch1, INPUT);
  pinMode(switch2, INPUT);
  pinMode(switch3, INPUT);
  pinMode(switch4, INPUT);
  /* ********** Connecting to IoT Platform ********** */
  BlynkEdgent.begin();
}

void loop() {

  BlynkEdgent.run();
  sence_ace();
  process_ace();
  broadcast_ace();
  delay(980);

}


/* ********** check function to check each switch and update the relay and the timing  ********** */

void sence_ace() {
  Serial.println("Entering the check function.  ");

  //Device1
  if (digitalRead(switch1) == HIGH)
  {
    digitalWrite(Relay1, LOW);
    Relaytimeing(&device1);
  } else digitalWrite(Relay1, HIGH);
  //Device2
  if (digitalRead(switch2) == HIGH)
  {
    digitalWrite(Relay2, LOW);
    Relaytiming(&device2);
  } else digitalWrite(Relay2, HIGH);
  //Device3
  if (digitalRead(switch3) == HIGH)
  {
    digitalWrite(Relay3, LOW);
    Runtime_counter(&device3);
  } else digitalWrite(Relay3, HIGH);
  //Device4
  if (digitalRead(switch4) == HIGH)
  {
    digitalWrite(Relay4, LOW);
    Runtime_counter(&device4);
  } else digitalWrite(Relay1, HIGH);
}

void process_ace() {
  calculate(&device1);
  calculate(&device2);
  calculate(&device3);
  calculate(&device4);
}



/* ********** To update the timing of each device in  hours minute sec total sec format for  calculation  ********** */

void Runtime_counter(struct device_data * device);
{
  device->sec++;
  device->total_sec++;
  if (device->sec > 59) {
    device->sec = 0;
    device->minute++;
    if (device->minute > 59) {
      device->minute = 0;
      device->hour++;
    }
  }
}

void calculate(struct time * str) {
  double timeinhour = str-> minute / 60 ;
  str->kilowatts = str-> power * timeinhour ;
  str->units = str-> kilowatts /1000;
  str-> slab_rate = get_slab_rate(str->units);
  
  

}
float get_slab_rate(struct device *){
  float rate ;
  if(units <=500)
  {
      if(units >= 1 && units <=100)
        rate = 0;
      else if(units >= 101 && units <=200)
        rate = 2.25 ;
      else if(units >= 201 && units <=400)
        rate = 4.5 ;
      else if(units >= 401 && units <=500)
        rate = 6 ;
  }
  if(units > 500 && units <=9999999)
  {
      if(units >= 1 && units <=100)
        rate = 0;
      else if(units >= 101 && units <=400)
        rate = 4.5 ;
      else if(units >= 401 && units <=500)
        rate = 6 ;
      else if(units >= 501 && units <=600)
        rate = 8 ;
      else if(units >= 601 && units <=800)
        rate = 9 ;
      else if(units >= 801 && units <=1000)
        rate = 10 ;
      else if(units >= 1001 && units <=9999999)
        rate = 11 ;
     
  }
  return rate ;
}
void broadcast_ace() {

  Blynk.virtualWrite(V10, device1.units);     // virtual pin datas
  Blynk.virtualWrite(V11, device2.units);
  Blynk.virtualWrite(V12, device3.units);
  Blynk.virtualWrite(V13, device4.units);

  Blynk.virtualWrite(V14, device1.total_amount);
  Blynk.virtualWrite(V15, device2.total_amount);
  Blynk.virtualWrite(V16, device3.total_amount);
  Blynk.virtualWrite(V17, device4.total_amount);
}
