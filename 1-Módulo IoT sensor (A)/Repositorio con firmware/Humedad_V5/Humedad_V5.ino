
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

SoftwareSerial serialgps(4,3);//Declaramos el pin 4 Tx y 3 Rx

//Declaramos la variables para la obtención de datos del GPS
int gyear;
byte gmonth, gday, ghour, gminute, gsecond, hundredths;
unsigned long chars;
unsigned short sentences, failed_checksum;
float humedad, celsius, fahrenheit;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  serialgps.begin(9600);
}

void loop ()
{
  set_sergps();
  
}

////////////////////////////////////// Muestra sensado de temperatura y humedad de ambiente ////////////////////////////////////////////////////////

void set_dht22 ()
{

      /***********************************/
      if (dht.readHumidity() > 0 )
      {
      humedad = dht.readHumidity();
      celsius = dht.readTemperature();
      fahrenheit = dht.readTemperature(true);
      }
      Serial.print("HUM=");//Humerdad en porcentage
      Serial.print(humedad,1);
      Serial.print(";");
      Serial.print("T-F=");//tempertatura en fahrenheit
      Serial.print(fahrenheit,1);
      Serial.print(";");
      Serial.print("T-C=");
      Serial.print(celsius,1);
      Serial.print(";");
      /***********************************/
}

/////////////////////////////////////////// Muestra Informacion de Sensor de humeda de suelo ///////////////////////////////////////////////////

bool set_humsuel ()
{
    int humedad_A0 = analogRead(sensorPin_A0); //leer el valor que envia en sensor de humedad
    int humedad_A1 = analogRead(sensorPin_A1); //leer el valor que envia en sensor de humedad
    Serial.print("SH0=");
    Serial.print(humedad_A0);
    Serial.print(";");
    Serial.print("SH1=");
    Serial.print(humedad_A1);
    Serial.print(";");

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
        Serial.print("BA=ON");
        Serial.print(";"); 

}

/////////////////////////////////////////////Apaga Bonba de Agua/////////////////////////////////////////////////

void set_offba()
{
        digitalWrite(motorPin,LOW);//Apagar el motor
        Serial.print("BA=OFF");
        Serial.print(";"); 

}

//////////////////////////////////////////////////////////////////////////////////////////////
void set_sergps()
{
  bool ba; 

  while(serialgps.available()) 
  {
    int c = serialgps.read();
 
    if(gps.encode(c))  
      {     
          //set_time();
                    
          gps.crack_datetime(&gyear,&gmonth,&gday,&ghour,&gminute,&gsecond,&hundredths);
          Serial.print("DAT=");
          Serial.print(gday, DEC);
          Serial.print("/"); 
          Serial.print(gmonth, DEC);
          Serial.print("/");
          Serial.print(gyear);
          Serial.print(";"); 
          Serial.print("TIM="); 
          Serial.print(ghour, DEC); 
          Serial.print(":"); 
          Serial.print(gminute, DEC); 
          Serial.print(":"); 
          Serial.print(gsecond, DEC); 
          Serial.print("."); 
          Serial.print(hundredths, DEC);
          Serial.print(";");
          
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
 
          float latitude, longitude;
          gps.f_get_position(&latitude, &longitude);
          Serial.print("LAT="); 
          Serial.print(latitude,5); 
          Serial.print(";"); 
          Serial.print("LON="); 
          Serial.print(longitude,5);
          Serial.print(";"); 
         
          Serial.print("ALT=");//Altitud Metros
          Serial.print(gps.f_altitude()); 
          
          Serial.print(";");
          Serial.print("COU:");
          Serial.print(gps.f_course());//runbo o curso en grados
          Serial.print(";");
          Serial.print("SPE=");
          Serial.print(gps.f_speed_kmph());//Velocidad en Kilometros
          Serial.print(";");//satelites
          Serial.print("SAT=");
          Serial.println(gps.satellites());
          gps.stats(&chars, &sentences, &failed_checksum);  
      }
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////
