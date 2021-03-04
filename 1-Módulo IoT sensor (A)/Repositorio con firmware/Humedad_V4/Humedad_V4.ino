
//////////////////////////////////////////////////////////////////
//                      Ingresdo del Modulo GPS                 // 
//////////////////////////////////////////////////////////////////

#include <DHT.h>//incluimos DTH sensor de temperatura
#include <TimeLib.h>//incluimos Libreria de hora
#include <SoftwareSerial.h>//incluimos Conexion Serial a modulos
#include <TinyGPS.h>//incluimos Libreria de GPS

const int sensorPin_A0 =A0;
const int sensorPin_A1 =A1;
const int motorPin= 13;


#define DHTPIN 5 
#define DHTTYPE DHT22
#define GPSRX 3
#define GPSTX 4

DHT dht(DHTPIN, DHTTYPE);//Declaramos el objeto DTH
TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialgps(4,3);//Declaramos el pin 4 Tx y 5 Rx

//Declaramos la variables para la obtención de datos
int gyear;
byte gmonth, gday, ghour, gminute, gsecond, hundredths;
unsigned long chars;
unsigned short sentences, failed_checksum;

void setup()
{
  Serial.begin(115200);
  dht.begin();
  setTime(12,13,00,01,03,2021);//se carga con la hora actualizada //cada carga deverria generar una nueva version 
  serialgps.begin(9600);

  
}

void loop ()
{

  int tiempodesalida=60;
   
      
  if (tiempodesalida == 60) //espere un minuto para mostrar la informacion
  {
 
    if (humedad_A0 > 300 || humedad_A1 > 500 )
      {
        
        set_onmot();
      }
    
    else  
    //verificar que el valor agregado se encuentre muy seco
      {
        
        setoffmot();
      }
      Serial.print(";"); 
//////////////////////////////////////////////////////////////////////////////////////////////
      
.
        int c = serialgps.read();
     
        if(gps.encode(c))  
        {

          float latitude, longitude;
          gps.f_get_position(&latitude, &longitude);
          Serial.print("LAT:"); 
          Serial.print(latitude,5); 
          Serial.print(";"); 
          Serial.print("LON:"); 
    
          Serial.print(longitude,5);
          Serial.print(";"); 
          gps.crack_datetime(&gyear,&gmonth,&gday,&ghour,&gminute,&gsecond,&hundredths);
          Serial.print("DAT:");
          Serial.print(gday, DEC);
          Serial.print("/"); 
          Serial.print(gmonth, DEC);
          Serial.print("/");
          Serial.print(gyear);
          Serial.print(";"); 
          Serial.print("TIM:"); 
          Serial.print(ghour, DEC); 
          Serial.print(":"); 
          Serial.print(gminute, DEC); 
          Serial.print(":"); 
          Serial.print(gsecond, DEC); 
          Serial.print("."); 
          Serial.print(hundredths, DEC);
          Serial.print(";");
          Serial.print("ALT:");//Altitud Metros
          Serial.print(gps.f_altitude()); 
          Serial.print(";");
          Serial.print("COU:");
          Serial.print(gps.f_course());//runbo o curso en grados
          Serial.print(";");
          Serial.print("SPE:");
          Serial.print(gps.f_speed_kmph());//Velocidad en Kilometros
          Serial.print(";");//satelites
          Serial.print("SAT:");
          Serial.print(gps.satellites());
          
          gps.stats(&chars, &sentences, &failed_checksum);  
        }
        else
        {
          Serial.print("GPS:ND");
        }
        Serial.println(";");
      }
//////////////////////////////////////////////////////////////////////////////////////////////

      Serial.print(";");
      Serial.println("***");

      tiempodesalida=59;
  }
  delay(1000);  //Esperar un segundo
  tiempodesalida ++;//incrementar el valor del tiempo de salida
  
}

void am2302 ()
{
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

    Serial.print("HUM=");//Humerdad en porcentage
    Serial.print(humedad,1);
    Serial.print(";");
    Serial.print("TEM=");//tempertatura en centigrados
    Serial.print(temperatura,1);
    Serial.print(";");
}
void humsuel()
{
    int humedad_A0 = analogRead(sensorPin_A0); //leer el valor que envia en sensor de humedad
    int humedad_A1 = analogRead(sensorPin_A1); //leer el valor que envia en sensor de humedad
    Serial.print("SH0=");
    Serial.print(humedad_A0);
    Serial.print(";");
    Serial.print("SH1=");
    Serial.print(humedad_A1);
    Serial.print(";");

}


void settime ()
{
    time_t t = now();//Declaramos la variable time_t t
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
}

void set_onmot()
{
        digitalWrite(motorPin,HIGH);//Apagar el motor
        Serial.print("BA=ON");

}
void set_offmot()
{
          digitalWrite(motorPin,LOW);//Apagar el motor
        Serial.print("BA=OFF");

}
