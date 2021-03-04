
/**********************************************************************************
*                                                                                 *
*       V1-Ingresdo del A0 Sensor de Humedad de tierra                            *        
*       V2-Ingresdo del A1 Sensor de Humedad de tierra                            *       
*       V3-Configuracion parametros de salida para entrega de datos               * 
*       V4-Configurn pin para encendido de bomba de agua                          *        
*       V5-Ingresdo del AM2302 sensor de humedad y ambiente                       * 
*       V6-Ingresdo del Modulo GPS   Y   Promacion por Modulo                     * 
*       V7-Ingresdo del Modulo WIFI DEIMOS                                        *
*                                                                                 *
***********************************************************************************/

#include <DHT.h>//incluimos DTH sensor de temperatura
#include <SoftwareSerial.h>//incluimos Conexion Serial a modulos
#include <TinyGPS.h>//incluimos Libreria de GPS

const int sensorPin_A0 =A0; //Sensor de humedad 1
const int sensorPin_A1 =A1; //sensor de humedad 2
const int motorPin= 5; // Salida Bonba de agua
const byte wrxPin = 6;
const byte wtxPin = 7;
const byte grxPin = 2;
const byte gtxPin = 3;

#define DHTPIN 4        //sensor digital DHT22
#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPIN, DHTTYPE);//Declaramos el objeto DTH encargado de sensar temperatura ambiente

TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialWifi(wtxPin,wrxPin);//Declaramos el pin 7 Tx y 6 Rx
SoftwareSerial serialgps(gtxPin,grxPin);//Declaramos el pin 3 Tx y 32 Rx


//Declaramos la variables para la obtención de datos del GPS
int gyear;
byte gmonth, gday, ghour, gminute, gsecond, hundredths;
unsigned long chars;
unsigned short sentences, failed_checksum;
float humedad, celsius, fahrenheit;
String msg_wifi, msg_date, msg_gps;



void setup()
{

  Serial.begin(9600);  //inicia el puerto Com con el Pc //Original a 115200
  dht.begin();
  serialWifi.begin(9600);

}

void loop ()
{
  set_dht22();
  set_sergps();
  msg_wifi ="";
  msg_date ="";
  msg_gps ="";
}

////////////////////////////////////// Muestra sensado de temperatura y humedad de ambiente ////////////////////////////////////////////////////////
void send_wifi_deimos()
{ 

  msg_wifi += msg_date;
  msg_wifi += set_humsuel();
  msg_wifi += set_dht22();
  msg_wifi += msg_gps;
  
  serialWifi.println(msg_wifi);    //envia los datos al modulo del Wifi
       
  Serial.println(msg_wifi);        //envia los datos al puerto del pc
      
}
String set_dht22 ()
{     

      String msg_ht;
 
      /***********************************/
      if (dht.readHumidity() > 0 )
      {
      humedad = dht.readHumidity();
      celsius = dht.readTemperature();
      fahrenheit = dht.readTemperature(true);
      }

      msg_ht += ";HUM=";
      msg_ht += humedad,1 ;
      msg_ht += ";T-F=";
      msg_ht += fahrenheit,1;
      msg_ht += ";T-C=";
      msg_ht += celsius,1;
      
      return msg_ht;

}

/////////////////////////////////////////// Muestra Informacion de Sensor de humeda de suelo ///////////////////////////////////////////////////

String set_humsuel ()
{
    String msg_hs;
    
    int humedad_A0 = analogRead(sensorPin_A0); //leer el valor que envia en sensor de humedad
    int humedad_A1 = analogRead(sensorPin_A1); //leer el valor que envia en sensor de humedad
    
    msg_hs += ";SH0=";
    msg_hs += humedad_A0;
    msg_hs += ";SH1=";
    msg_hs += humedad_A1;
  
    if (humedad_A0 > 500 || humedad_A1 > 500 )
      {
        msg_hs += set_offba();
      }
    else
      {
        msg_hs += set_onba();
      }

   return msg_hs;
}

////////////////////////////////////////////Prende Bonba de Agua//////////////////////////////////////////////////

String set_onba()
{
    digitalWrite(motorPin,HIGH);//Apagar el motor
    return ";BA=ON";
}

/////////////////////////////////////////////Apaga Bonba de Agua/////////////////////////////////////////////////

String set_offba()
{
     digitalWrite(motorPin,LOW);//Apagar el motor
     return ";BA=OFF";
}

//////////////////////////////////////////////////////////////////////////////////////////////
void set_sergps()
{
  
  serialgps.begin(9600);

  while(serialgps.available()) 
  {
    int c = serialgps.read();
    float latitude, longitude;

    if(gps.encode(c))  
      {
          //set_time();
                    
          gps.crack_datetime(&gyear,&gmonth,&gday,&ghour,&gminute,&gsecond,&hundredths);
          
          msg_date += "DAT=";
          msg_date += gday, DEC;
          msg_date += "/";
          msg_date += gmonth, DEC;
          msg_date += "/";  
          msg_date += gyear;
          msg_date += ";TIM=";
          msg_date += ghour, DEC;
          msg_date += ":";      
          msg_date += gminute, DEC;
          msg_date += ":"; 
          msg_date += gsecond, DEC;
          msg_date += ":"; 
          msg_date += hundredths, DEC;
            
          gps.f_get_position(&latitude, &longitude);

          msg_gps += ";LAT=";
          msg_gps += latitude,5;
          msg_gps += ";LON=";
          msg_gps += longitude,5;
          msg_gps += ";ALT=";
          msg_gps += gps.f_altitude();
          msg_gps += ";COU:";
          msg_gps += gps.f_course();
          msg_gps += ";SPE=";
          msg_gps += gps.f_speed_kmph();
          msg_gps += ";SAT=";
          msg_gps += gps.satellites();
          gps.stats(&chars, &sentences, &failed_checksum); 
          
          msg_gps += ";CHA=";
          msg_gps += chars;
          msg_gps += ";SEN=";
          msg_gps += sentences;
          msg_gps += ";FAI=";
          msg_gps += failed_checksum;
          send_wifi_deimos();
      }
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////
