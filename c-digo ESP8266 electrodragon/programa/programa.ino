//==================================================================//
//                                                                  //
//                    Identificador del dispositivo                 //
                            #define ID  1                           //
//                                                                  //
//==================================================================//


//LIBRERIAS----------------------------------------------------------
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>

#include "Gsender.h"

#define WAIT_TIME 120000//480000    //tiempo entre lecturas
#define WAIT_TIME_WITH_ERROR  20000 //tiempo entre lecturas en caso de fallo de una sonda
#define TS_UPDATE_TIME 480000       //cada 8 min se sube a ThingSpeak
#define WIFI_ERROR_TIME 120000      //tiempo que espera a la conexion WiFi
#define HTTP_ERROR_TIME 5000        //tiempo entre cada intento de petición
#define QUERY_ERROR_TIME 5000       //tiempo de espera en caso de fallo MySQL
#define TS_ERROR_TIME 15000         //tiempo de espera en caso de error en la subida de datos
#define RESET_TIME 43200000         //cada 12 horas se resetea 
#define INICIO_SERVIDOR 20000       //Espera a la puesta en marcha del servidor
#define nfacumulado_total 5         //nº limite de fallos antes de enviar una alerta

#define limite_sondas 10            //máximo de sondas que permite el dispositivo

WiFiClient client;
HTTPClient http;

const char * server_ip = "192.168.1.40";    //Sustituir por dirección web del servidor

//estructura que recoge los parametros necesarios
struct datos_disp{
    unsigned int n_sondas=0;              //Nº de sondas asociadas al dispositivo
    unsigned char id_sondas[limite_sondas][8];     //Es poco probable que se usen mas de 10 sondas  
    unsigned int id_db[limite_sondas];
    signed int t_min_sondas[limite_sondas];               //Array de tmin de las sondas
    signed int t_max_sondas[limite_sondas];               //Array de tmax de las sondas
    int campo[limite_sondas];                      //Campo de TS al que se subirán los datos de la sonda
    unsigned int id_canal;              //ID del canal de Thingspeak
    String WAK;                          //Clave de escritura de Thinspeak
    unsigned int n_email_mant;          //Nº email mantenimiento frigorífico
    String email_mant[limite_sondas];              //Array de emails de mantenimiento
    unsigned int n_email_com;           //Nº de emails de mantenimiento del dispositivo ESP
    String email_com[limite_sondas];               //Array de emails para dispositivos de comunicaciones
    unsigned int error=0;                        //ERRORES en el dispositivo
    String tienda;
    String ubicacion[limite_sondas];
};
struct datos_disp datos;
String mensaje_datos;
float temperatura[limite_sondas];
int errores_sonda[limite_sondas];
unsigned long int tiempo_TS=0;
unsigned long int tiempo_RESET=0;
unsigned int error=0;                               // Indicador de error en la consulta MySQL

unsigned int nfacumulado[limite_sondas+2];          //los dos campos extra se usarán para el acumulado de fallos del disp

//SETUP==============================================================
void setup() {

    Serial.begin(115200);
    delay(100);
    
    while(!WiFiConnection());

    nfacumulado[limite_sondas+1]=0;
  
    delay(100);

    recibe_parametros();

    if(!datos.error){
        Serial.print("Nº sondas = ");
        Serial.println(datos.n_sondas);
        Serial.print("Nº email mantenimiento = ");
        Serial.println(datos.n_email_mant);
        Serial.print("Nº email comunicaciones = ");
        Serial.println(datos.n_email_com);
        Serial.println("============================================================");
        Serial.print("- Tienda del dispositivo = ");
        Serial.println(datos.tienda);
        for(int i=0;i<datos.n_sondas;i++){
          Serial.print("- ID sonda ");
          Serial.print(i+1);
          Serial.print(" = ");
          for(int j=0;j<8;j++){
              Serial.print(datos.id_sondas[i][j]);
              Serial.print(" ");
          }
          Serial.println();
          Serial.print("- Localizador sonda en db= ");
          Serial.println(datos.id_db[i]);
          Serial.print("- Ubicacion sonda = ");
          Serial.println(datos.ubicacion[i]);
          Serial.print("- Campo sonda en TS = ");
          Serial.println(datos.campo[i]);
          Serial.print("- t max sonda = ");
          Serial.println(datos.t_max_sondas[i]);
          Serial.print("- t min sonda = ");
          Serial.println(datos.t_min_sondas[i]);
        }
        Serial.print("- ID canal = ");
        Serial.println(datos.id_canal);
        Serial.print("- Clave de escritura = ");
        Serial.println(datos.WAK);
        Serial.println("- Email mantenimiento = ");
        for(int i=0;i<datos.n_email_mant;i++){
            Serial.print("     ");
            Serial.println(datos.email_mant[i]);
        }
        Serial.println("- Email comunicaciones = ");
        for(int i=0;i<datos.n_email_com;i++){
            Serial.print("     ");
            Serial.println(datos.email_com[i]);
        }
        Serial.println("============================================================");
    }
    delay(100);
    tiempo_RESET=millis();
    tiempo_TS=millis();
}
//LOOP===============================================================
void loop() {
    
    for(int i=0;i<datos.n_sondas;i++){
        temperatura[i]=lee_dato(datos.id_sondas[i]);
        Serial.print("Temperatura: ");
        Serial.print(temperatura[i]);
        Serial.println(" ºC");
        
        if(temperatura[i]==85){
            if(nfacumulado[i]){
                if(nfacumulado[i]>=nfacumulado_total){
                  switch(errores_sonda[i]){
                      case 50:
                          fallo_sonda_reparado(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_com,datos.email_com);     //reparado
                          break;

                      case 51:
                          low_temperature_reparado(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_mant,datos.email_mant);
                          break;              //reparado

                      case 52:
                          high_temperature_reparado(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_mant,datos.email_mant);//reparado
                  }
                }
                nfacumulado[i]=0;
            }
            if(errores_sonda[i]!=1111){
                errores_sonda[i]=1111;
                error_sonda(datos.id_db[i],errores_sonda[i]);     //Subir NO error al servidor
            }
        }
        else if(temperatura[i]==100) {
            if(nfacumulado[i]){
                switch(errores_sonda[i]){
                    case 51:
                        nfacumulado[i]=0;
                        break;            

                    case 52:
                        nfacumulado[i]=0;
                }
            }
            if(errores_sonda[i]!=50){
                errores_sonda[i]=50;
                error_sonda(datos.id_db[i],errores_sonda[i]);     //subimos error al servidor
            }
            nfacumulado[i]++;
            if(nfacumulado[i]==nfacumulado_total){
                fallo_sonda(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_com,datos.email_com);
            }
        }
        else if(temperatura[i]>=datos.t_min_sondas[i] && temperatura[i]<=datos.t_max_sondas[i]) {
              if(nfacumulado[i]){
                if(nfacumulado[i]>=nfacumulado_total){
                  switch(errores_sonda[i]){
                      case 50:
                          fallo_sonda_reparado(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_com,datos.email_com);     //reparado
                          break;

                      case 51:
                          low_temperature_reparado(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_mant,datos.email_mant);
                          break;              //reparado

                      case 52:
                          high_temperature_reparado(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_mant,datos.email_mant);//reparado
                  }
                }
                nfacumulado[i]=0;
              }
              if(errores_sonda[i]!=0){
                  errores_sonda[i]=0;
                  error_sonda(datos.id_db[i],errores_sonda[i]);              //Subimos No error al servidor
              }
        }
        else if(temperatura[i]<datos.t_min_sondas[i]) {
              if(nfacumulado[i]){
                if(nfacumulado[i]>=nfacumulado_total){
                  switch(errores_sonda[i]){
                      case 50:
                          fallo_sonda_reparado(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_com,datos.email_com);     //reparado
                          nfacumulado[i]=0;
                          break;

                      case 52:
                          high_temperature_reparado(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_mant,datos.email_mant);//reparado
                          nfacumulado[i]=0;
                  }
                }
              }
              if(errores_sonda[i]!=51){
                  errores_sonda[i]=51;
                  error_sonda(datos.id_db[i],errores_sonda[i]);    //subir error
              }
              nfacumulado[i]++;
              if(nfacumulado[i]==nfacumulado_total){
                  low_temperature(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_mant,datos.email_mant);
              }    
        }
        else if(temperatura[i]>datos.t_max_sondas[i]) {
              if(nfacumulado[i]){
                if(nfacumulado[i]>=nfacumulado_total){
                  switch(errores_sonda[i]){
                      case 50:
                          fallo_sonda_reparado(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_com,datos.email_com);     //reparado
                          nfacumulado[i]=0;
                          break;

                      case 51:
                          low_temperature_reparado(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_mant,datos.email_mant); //reparado
                          nfacumulado[i]=0;
                  }
                }
              }
              if(errores_sonda[i]!=52){  
                  errores_sonda[i]=52;
                  error_sonda(datos.id_db[i],errores_sonda[i]);     //subir error
              }
              nfacumulado[i]++;
              if(nfacumulado[i]==nfacumulado_total){
                  high_temperature(datos.id_db[i],datos.tienda,datos.ubicacion[i],datos.n_email_mant,datos.email_mant);
              }  
        }
    }

    for(int i=0;i<datos.n_sondas;i++){
        if(errores_sonda[i]){
            Serial.println("Error detectado en sonda.");
            delay(WAIT_TIME_WITH_ERROR);
            break;
        }
        else if(i==(datos.n_sondas-1)){
            Serial.println("Temperatura adecuada.");
            delay(WAIT_TIME);
        }
    }

    
    if((millis()-tiempo_TS)>=TS_UPDATE_TIME){
        subir_TS(datos.WAK,datos.n_sondas,datos.campo,temperatura);         //Subimos temperatura a ThingSpeak
        tiempo_TS=millis();
        Serial.println("Temperatura subida a ThingSpeak");
        for(int i=0;i<datos.n_sondas;i++){
            error_sonda(datos.id_db[i],errores_sonda[i]);                   //Actualizamos estado de la sonda de temperatura
        }
    }

    if((millis()-tiempo_RESET)>=RESET_TIME) {
          Serial.println("===================================================");
          Serial.println("                      Reseteo                      ");
          Serial.println("===================================================");
          ESP.restart();
     }
}


void recibe_parametros(){
   PETICION:
      mensaje_datos=peticion_http();
      delay(100);
      Serial.println("MENSAJE CON PARÁMETROS: ");
      Serial.println(mensaje_datos);
      while(datos.error==2){
          //No se sube error al servidor ya que se están teniendo problemas con el HTTP
          nfacumulado[limite_sondas+1]++;
          Serial.println("ERROR en petición HTTP");
          Serial.println("Reintentando...");
          delay(HTTP_ERROR_TIME);
          mensaje_datos=peticion_http();
      }
      if(nfacumulado[limite_sondas+1]){
          nfacumulado[limite_sondas+1]=0;
      }
      Serial.println("Petición HTTP enviada con éxito.");
      delay(100);

      datos=initialization(datos,mensaje_datos);
      
      if(datos.error) {
          nfacumulado[limite_sondas]++;
          if(nfacumulado[limite_sondas]==nfacumulado_total){
            error_dispositivo(datos.error);       //sube error al servidor
          }
          Serial.println("ERROR detectado en la consulta a la base de datos.");
          error=datos.error;
          delay(QUERY_ERROR_TIME);
          goto PETICION;
      }else{
          if(nfacumulado[limite_sondas]){
              if(nfacumulado[limite_sondas]>=nfacumulado_total){
                  error_consulta_reparado(error,datos.tienda,datos.n_email_com,datos.email_com);
              }
              nfacumulado[limite_sondas]=0;
          }
          error_dispositivo(0);       //sube NO error al servidor
          Serial.println("Consulta a la base de datos realizada con éxito.");
      }
      delay(100);
}

