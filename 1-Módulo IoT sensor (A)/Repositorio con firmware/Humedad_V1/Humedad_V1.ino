const int sensorPin_A0 =A0;
const int sensorPin_A1 =A1;
const int motorPin= 13;

void setup()
{
	Serial.begin(9600);

}

void loop ()
{
	int humedad_A0 =analogRead(sensorPin_A0); //leer el valor que envia en sensor de humedad
	int humedad_A1 =analogRead(sensorPin_A1);
	int aspersor = 0;
	//verificar que el valor agreagado se encuentre muy humero
	Serial.println(humedad_A0);

	if (humedad_A0 < 500 )
	{
		Serial.println("M0 Humedad Alta");
    aspersor = 1;
	}
	
	else  
	//verificar que el valor agregado se encuentre muy seco
	{
  	Serial.println("M0 Humeda Baja");
    aspersor = 0;
	}
  
  Serial.println(humedad_A1);
  
  if (humedad_A1 < 500 )
  {
    Serial.println("M1 Humeda Alta");
    aspersor = 1;
  }
  else  
  //verificar que el valor agregado se encuentre muy seco
  {
    Serial.println("M1 Humeda Baja");
    aspersor = 0;
  }

  if (aspersor == 0)
  {
    digitalWrite(motorPin,LOW);//Apagar el motor
     Serial.println("Prender Aspersor");
  }
  else 
  {
    digitalWrite(motorPin,HIGH);//Encender el motor
     Serial.println("Apagar Aspersor");
  }
 
	delay(1000);  //Esperar un segundo  
}
