
/**********************************************************************************
*                                                                                 *
*       V1-Recive la informacion por el puerto serial desde el arduino uno        *
*          y la muesta por consola                                                *        
*       V2-Ingresdo del A1 Sensor de Humedad de tierra                            *       
*       V3-Configuracion parametros de salida para entrega de datos               * 
*       V4-Configurn pin para encendido de bomba de agua                          *        
*       V5-Ingresdo del AM2302 sensor de humedad y ambiente                       * 
*       V6-Ingresdo del Modulo GPS   Y   Promacion por Modulo                     * 
*       V7-Ingresdo del Modulo WIFI DEIMOS                                        *
*                                                                                 *
***********************************************************************************/


#include <SoftwareSerial.h>//incluimos SoftwareSerial


const byte wrxPin = 2;
const byte wtxPin = 3;

SoftwareSerial serialWifi(wrxPin,wtxPin);//Declaramos el pin 2 Rx y 3 Tx

String msg_wifi ;
 
void setup()
{
  Serial.begin(9600);//Iniciamos el puerto serie
  serialWifi.begin(9600);//Iniciamos el puerto serie del wifi 
}

void loop()
{
  msg_wifi  = "";
  if (serialWifi.available() > 0) 
    {
      msg_wifi += "WIFI=S1;";
      msg_wifi += serialWifi.readStringUntil('\n'); // read the incoming byte:
      Serial.println(msg_wifi);

    }
      if (Serial.available() > 0) 
    {
      msg_wifi += "WIFI=S0;";
      msg_wifi += Serial.readStringUntil('\n'); // read the incoming byte:
      Serial.println(msg_wifi);

    }
}
