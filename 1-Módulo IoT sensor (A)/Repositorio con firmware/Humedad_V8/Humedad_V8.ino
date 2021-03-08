
/**********************************************************************************
*                                                                                 *
*       V1-Ingresdo del A0 Sensor de Humedad de tierra                            *        
*       V2-Ingresdo del A1 Sensor de Humedad de tierra                            *       
*       V3-Configuracion parametros de salida para entrega de datos               * 
*       V4-Configurn pin para encendido de bomba de agua                          *        
*       V5-Ingresdo del AM2302 sensor de humedad y ambiente                       * 
*       V6-Ingresdo del Modulo GPS   Y   Promacion por Modulo                     * 
*       V7-Ingresdo del Modulo WIFI DEIMOS                                        *
*       V8-Ajustes en sincronizacion de modulo serial con el GPS                  *
*                                                                                 *
***********************************************************************************/

#include <DHT.h>//incluimos DTH sensor de temperatura
#include <SoftwareSerial.h>//incluimos Conexion Serial a modulos
#include <TinyGPS.h>//incluimos Libreria de GPS

const int sensorPin_A0 =A0; //Sensor de humedad 1
const int sensorPin_A1 =A0; //sensor de humedad 2
const int motorPin= 5; // Salida Bonba de agua
const byte wrxPin = 6;
const byte wtxPin = 7;
const byte grxPin = 3;
const byte gtxPin = 2;

#define DHTPIN 4        //sensor digital DHT22
#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPIN, DHTTYPE);//Declaramos el objeto DTH encargado de sensar temperatura ambiente

TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialWifi(wtxPin, wrxPin);//Declaramos el pin 7 Tx y 6 Rx
SoftwareSerial serialgps(gtxPin,grxPin);//Declaramos el pin 2 Tx y 3 Rx

int year;
byte month, day, hour, minute, second, hundredths;
unsigned long chars,age;
unsigned short sentences, failed_checksum;
String msg_ht, msg_wifi, msg_gps, msg_hs;


void setup()
{

  Serial.begin(9600);  //inicia el puerto Com con el Pc //Original a 115200
  serialgps.begin(9600);
  //serialWifi.begin(4800);
  dht.begin();

}

void loop ()
{
  msg_gps =""; 
  msg_wifi = "";
  msg_hs = "";
  msg_ht = "";
  set_humsuel();
  set_dht22();
  set_sergps();
 
}

////////////////////////////////////// Muestra sensado de temperatura y humedad de ambiente ////////////////////////////////////////////////////////
void send_wifi_deimos()
{
  msg_wifi += msg_gps;
  msg_wifi += msg_hs;
  msg_wifi += msg_ht;
  
  serialWifi.println(msg_wifi);    //envia los datos al modulo del Wifi
  Serial.println(msg_wifi);        //envia los datos al puerto del pc
}

////////////////////////////////////// Muestra sensado de temperatura y humedad de ambiente ////////////////////////////////////////////////////////

String set_dht22 ()
{     
       float humedad, celsius, fahrenheit;
      /***********************************/
       if (dht.readHumidity() > 0 )
      {
      humedad = dht.readHumidity();
      celsius = dht.readTemperature();
      fahrenheit = dht.readTemperature(true);
      }
      msg_ht += "HUM=";
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
    int humedad_A0 = analogRead(sensorPin_A0); //leer el valor que envia en sensor de humedad
    int humedad_A1 = analogRead(sensorPin_A1); //leer el valor que envia en sensor de humedad
    
    msg_hs += "SH0=";
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
    
    msg_hs += ";"; 
      
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
String set_sergps_1()
{
 if (serialgps.available())
   {
      char data;
      data = serialgps.read();
      Serial.print(data);
   }
}

//////////////////////////////////////////////////////////////////////////////////////////////
String set_sergps()
{
    while(serialgps.available()) 
    {
    int c = serialgps.read();
    String msg_time, msg_at_lat, msg_error;
    
    msg_at_lat ="";
    msg_time = "" ;
    msg_error = "";
    
    if(gps.encode(c))  
    {
      float latitude, longitude;
      
      gps.f_get_position(&latitude, &longitude);
          
          //Captura de Latitud, Longitud, Altitud, Curso, Velocifad en KM, Satelite de referencia
          
      msg_at_lat += "LAT=";
      msg_at_lat += latitude,5;
      msg_at_lat += ";LON=";
      msg_at_lat += longitude,5;
      msg_at_lat += ";ALT=";
      msg_at_lat += gps.f_altitude();
      msg_at_lat += ";COU:";
      msg_at_lat += gps.f_course();
      msg_at_lat += ";SPE=";
      msg_at_lat += gps.f_speed_kmph();
      msg_at_lat += ";SAT=";
      msg_at_lat += gps.satellites();
      msg_at_lat += ";"; 
      
      gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths,&age);
                
                //Captura de tiempo
                
      msg_time += "DAT=";
      msg_time += day, DEC;
      msg_time += "/";
      msg_time += month, DEC;
      msg_time += "/";  
      msg_time += year;
      msg_time += ";TIM=";
      msg_time += hour, DEC;
      msg_time += ":";      
      msg_time += minute, DEC;
      msg_time += ":"; 
      msg_time += second, DEC;
      msg_time += ":"; 
      msg_time += hundredths, DEC;
                
                //Tienpo Ida y Regreso en Milisegundos
                
      msg_time += ";AGE=";
      msg_time += age;
      msg_time += ";";    

      //Preparacion del string que entrgar al modulo Wifi
      
      msg_gps += msg_time;
      msg_gps += msg_at_lat;      
        
      send_wifi_deimos();
    }
  }
  
return msg_gps;
}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
