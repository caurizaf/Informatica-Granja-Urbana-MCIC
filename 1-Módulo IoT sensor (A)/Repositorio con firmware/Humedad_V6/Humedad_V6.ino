
//////////////////////////////////////////////////////////////////
//   Ingresdo del Modulo GPS   Y   Promacion por Modulo         // 
//////////////////////////////////////////////////////////////////

#include <DHT.h>//incluimos DTH sensor de temperatura
#include <SoftwareSerial.h>//incluimos Conexion Serial a modulos
#include <TinyGPS.h>//incluimos Libreria de GPS

const int sensorPin_A0 =A0; //Sensor de humedad 1
const int sensorPin_A1 =A1; //sensor de humedad 2
const int motorPin= 5; // Salida Bonba de agua

#define DHTPIN 7 //sensor digital DHT22
#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPIN, DHTTYPE);//Declaramos el objeto DTH encargado de sensar temperatura ambiente

TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialWifi(8,9);//Declaramos el pin 8 Tx y 9 Rx
SoftwareSerial serialgps(4,3);//Declaramos el pin 4 Tx y 3 Rx


//Declaramos la variables para la obtención de datos del GPS
int gyear;
byte gmonth, gday, ghour, gminute, gsecond, hundredths;
unsigned long chars;
unsigned short sentences, failed_checksum;
float humedad, celsius, fahrenheit;
String msg;



void setup()
{
  serialWifi.begin(9600);
  serialgps.begin(9600);
  dht.begin();



  
}

void loop ()
{
  set_sergps();

  msg="";

}

////////////////////////////////////// Muestra sensado de temperatura y humedad de ambiente ////////////////////////////////////////////////////////
void send_wifi_deimos()
{

  serialWifi.println(msg);
  Serial.begin(9600);
  Serial.println(msg);
}
void set_dht22 ()
{

      /***********************************/
      if (dht.readHumidity() > 0 )
      {
      humedad = dht.readHumidity();
      celsius = dht.readTemperature();
      fahrenheit = dht.readTemperature(true);
      }
      msg += ";HUM=";
      msg += humedad,1 ;
      msg += ";T-F=";
      msg += fahrenheit,1;
      msg += ";T-C=";
      msg += celsius,1;


      
}

/////////////////////////////////////////// Muestra Informacion de Sensor de humeda de suelo ///////////////////////////////////////////////////

bool set_humsuel ()
{
    int humedad_A0 = analogRead(sensorPin_A0); //leer el valor que envia en sensor de humedad
    int humedad_A1 = analogRead(sensorPin_A1); //leer el valor que envia en sensor de humedad

    msg += ";SH0=";
    msg += humedad_A0;
    msg += ";SH1=";
    msg += humedad_A1;
    /* 
    Serial.print("SH0=");
    Serial.print(humedad_A0);
    Serial.print(";");
    Serial.print("SH1=");
    Serial.print(humedad_A1);
    Serial.print(";");
    */
    
    if (humedad_A0 > 500 || humedad_A1 > 500 )
      {
        return true;
      }
    else
      {
        return false;
      }

}

////////////////////////////////////////////Prende Bonba de Agua//////////////////////////////////////////////////

void set_onba()
{
        digitalWrite(motorPin,HIGH);//Apagar el motor
        msg += "BA=ON;";
}

/////////////////////////////////////////////Apaga Bonba de Agua/////////////////////////////////////////////////

void set_offba()
{
        digitalWrite(motorPin,LOW);//Apagar el motor
        msg += "BA=OFF;";
}

//////////////////////////////////////////////////////////////////////////////////////////////
void set_sergps()
{
  bool ba; 

  while(serialgps.available()) 
  {
    int c = serialgps.read();
    float latitude, longitude;
    
    if(gps.encode(c))  
      {     
          //set_time();
                    
          gps.crack_datetime(&gyear,&gmonth,&gday,&ghour,&gminute,&gsecond,&hundredths);
          
          msg += "DAT=";
          msg += gday, DEC;
          msg += "/";
          msg += gmonth, DEC;
          msg += "/";  
          msg += gyear;
          msg += ";TIM=";
          msg += ghour, DEC;
          msg += ":";      
          msg += gminute, DEC;
          msg += ":"; 
          msg += gsecond, DEC;
          msg += ":"; 
          msg += hundredths, DEC;

          ba = set_humsuel();
          
          set_dht22();

             if (ba == true )
              {
                set_offba();
              }            
            else  
              {
                set_onba();
              }
 
          gps.f_get_position(&latitude, &longitude);

          msg += ";LAT=";
          msg += latitude,5;
          msg += ";LON=";
          msg += longitude,5;
          msg += ";ALT=";
          msg += gps.f_altitude();
          msg += ";COU:";
          msg += gps.f_course();
          msg += ";SPE=";
          msg += gps.f_speed_kmph();
          msg += ";SAT=";
          msg += gps.satellites();
          gps.stats(&chars, &sentences, &failed_checksum); 
          msg += ";CHA=";
          msg += chars;
          msg += ";SEN=";
          msg += sentences;
          msg += ";FAI=";
          msg += failed_checksum;
          send_wifi_deimos();
      }
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////
