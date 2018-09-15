#include <OneWire.h>

OneWire ourWire(5);                //Se establece el pin 5 como bus OneWire
byte addr[8]; 

void setup(void) {
  Serial.begin(115200);
}

void loop(void) { 
  Serial.println("Obteniendo direcciones:");
  while (ourWire.search(addr)) 
  {  
    //bytes ID en formato decimal
    Serial.print("Address     = ");
    for( int i = 0; i < 8; i++) {
      Serial.print(addr[i]);
      Serial.print(" ");
    }
    Serial.println();

    //bytes ID en formato hexadecimal
    Serial.print("Address HEX = ");
    for( int i = 0; i < 8; i++) {
      Serial.print(addr[i],HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

Serial.println();
ourWire.reset_search();
delay(5000);
}
