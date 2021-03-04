
#include <DHT.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>

SoftwareSerial gps(4,5);

const int sensorPin_A0 =A0;
const int sensorPin_A1 =A1;
const int motorPin= 13;
const int thora
const int tmin;
const int tsec;
const int tdia;
const int tmes;
const int taño;

#define DHTPIN 3 
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(3,OUTPUT);
  setTime(06,55,00,01,03,2021);//se carga con la hora actualizada //cada carga deverria generar una nueva version 

  
}

void loop ()
{
  int humedad_A0 = analogRead(sensorPin_A0); //leer el valor que envia en sensor de humedad
  int humedad_A1 = analogRead(sensorPin_A1);
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();
  int aspersor = 0;
  int tiempodesalida;

  time_t t = now();//Declaramos la variable time_t t

  if (tiempodesalida == 60) //espere un minuto para mostrar la informacion
  {
    Serial.print(day(t));
    Serial.print(+ "/") ;
    Serial.print(month(t));
    Serial.print(+ "/") ;
    Serial.print(year(t)); 
    Serial.print( " ") ;
    Serial.print(hour(t));  
    Serial.print(+ ":") ;
    Serial.print(minute(t));
    Serial.print(":") ;
    Serial.print(second(t));
    Serial.print(";");
    Serial.print("SH0=");
    Serial.print(humedad_A0);
    Serial.print(";");
    Serial.print("SH1=");
    Serial.print(humedad_A1);
    Serial.print(";");
    Serial.print("HUM=");//Humerdad en porcentage
    Serial.print(humedad,1);
    Serial.print(";");
    Serial.print("TEM=");//tempertatura en centigrados
    Serial.print(temperatura,1);
    Serial.print(";");
   
    if (humedad_A0 > 500 || humedad_A1 > 500 )
      {
        aspersor = 1;
        digitalWrite(motorPin,HIGH);//Apagar el motor
        Serial.print("BA=ON");
      }
    
    else  
    //verificar que el valor agregado se encuentre muy seco
      {
        aspersor = 0;
        digitalWrite(motorPin,LOW);//Encender el motor
        Serial.print("BA=OFF");
      }
      
      Serial.print(";");
      Serial.println("***");
  }
  delay(1000);  //Esperar un segundo
  tiempodesalida ++;//incrementar el valor del tiempo de salida
  
}
