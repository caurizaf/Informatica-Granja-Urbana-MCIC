
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


const byte grxPin = 2;
const byte gtxPin = 3;

SoftwareSerial serialWifi(gtxPin,grxPin);//Declaramos el pin 2 Tx y 3 Rx

String msg_wifi ;
 
void setup()
{
  Serial.begin(9600);//Iniciamos el puerto serie
  serialWifi.begin(9600);//Iniciamos el puerto serie del gps
 
}

void loop()
{
  msg_wifi  = "";
  if (Serial.available() > 0) 
    {
      msg_wifi += "WIFI;";
      msg_wifi += Serial.readStringUntil('\n'); // read the incoming byte:
      Serial.println(msg_wifi);

    }
}
