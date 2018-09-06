#define SELECCIONAR_SONDA 0x44
#define LEER_SCRATCHPAD 0xBE
#define LONGITUD_SCRATCHPAD 9
//#define REGISTRO_CONFIGURACION 4
#define REGISTRO_CRC 8
//#define MASCARA_RESOLUCION 0B1100000
#define FRACCION_DS18B20 16.0


#define VALOR_INCORRECTO 100.0 // Un valor mayor que el máximo o menor que el mínimo
#define PIN_BUS_SONDAS 5 // Número de pin de Arduino donde se conecta la entrada digital con la resistencia de 4,7 KΩ
#define BYTES_DIRECCION 8 // Número de bytes (máximo) que tiene la dirección de cada dispositivo del bus

OneWire bus_sondas(PIN_BUS_SONDAS); // Modo de alimentación parásito
unsigned char direccion[BYTES_DIRECCION];


float lee_dato(unsigned char *sonda_id){

    OneWire bus_sondas(PIN_BUS_SONDAS);

    float celsiusTemperature;

    preparar_sonda(bus_sondas,sonda_id); // Enviar la señal de preparar dispositivo a cada sensor de temperatura conocido

    celsiusTemperature=leer_sonda(bus_sondas,sonda_id); // Cargar el valor de temperatura del scratchpad
        
    return celsiusTemperature;
}


void preparar_sonda(OneWire bus_sondas, unsigned char *direccion)
{
  bus_sondas.reset();
  bus_sondas.select(direccion);
  bus_sondas.write(SELECCIONAR_SONDA,true); // Iniciar la lectura sonda con alimentación en modo parásito
}

float leer_sonda(OneWire bus_sondas, unsigned char *direccion)
{
  byte buffer_lectura[LONGITUD_SCRATCHPAD];
  int lectura;
  //byte resolucion;
  //delay(ESPERA_LECTURAS); // >750 ms a 12 bits de resolución
  //bus_sondas.depower();
  bus_sondas.reset();
  bus_sondas.select(direccion);
  bus_sondas.write(LEER_SCRATCHPAD,false); // Leer datos
  for(byte indice=0;indice<LONGITUD_SCRATCHPAD;indice++) // Recorrer todo el scratchpad
  {
    buffer_lectura[indice]=bus_sondas.read(); // Almacenar en el buffer los valores del scratchpad
  }
  if(bus_sondas.crc8(buffer_lectura,LONGITUD_SCRATCHPAD-1)==buffer_lectura[REGISTRO_CRC]) // Si el CRC es correcto
  {
    /*
    // Si se trabaja con resoluciones diferentes a la máxima, que es lo habitual, se deben establecer a cero los bits no usados en cada resolución (que no serán válidos)
    resolucion=buffer_lectura[REGISTRO_CONFIGURACION]&MASCARA_RESOLUCION;
    Serial.print("Usando ");
    switch(resolucion)
    {
      case 0: // 9 bits, 0.5000 °C
        //Serial.print("9 bits");
        buffer_lectura[0]&=0b11111000;
        break;
      case 32: // 10 bits, 0.2500 °C
        //Serial.print("10 bits");
        buffer_lectura[0]&=0b11111100;
        break;
      case 64: // 11 bits, 0.1250 °C
        //Serial.print("11 bits");
        buffer_lectura[0]&=0b11111110;
        break;
      default: // 12 bits, 0.0625 °C
        //Serial.print("12 bits");
        break;
    }
    Serial.println(" de resolución");
    */
    lectura=(buffer_lectura[1]<<8)|buffer_lectura[0];
    return (float)lectura/FRACCION_DS18B20;
  }
  else // Si el CRC no es correcto devolver un valor menor que el mínimo o mayor que el máximo para indicar que no es válido
  {
    return VALOR_INCORRECTO;
  }
}
