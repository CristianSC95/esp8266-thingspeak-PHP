
//Envio de peticion http al servidor============================================
String peticion_http(){
    String mensaje="";       //los parametros que se solicitan
    // Proceso de envio de muestras al servidor
    Serial.println("Conectando con el servidor...");
    if (client.connect(server_ip, 80)>0) {  // Conexion con el servidor
        client.print("GET /piedra/consulta/consulta.php?tabla=dispositivo&id="); // Enviamos los datos por GET
        client.print(ID);
        client.println(" HTTP/1.0");
        client.println("User-Agent: Arduino 1.0");
        client.println();
        Serial.println("Conectado");
        delay(100);
        datos.error=0;
    } else {
        datos.error=2;
        Serial.println("Conexión fallida.");
    }
    delay(INICIO_SERVIDOR);
    while (client.available()) {
        char c = client.read();
        mensaje+=c;
    }
    delay(100);
    return mensaje;
}

struct datos_disp initialization(struct datos_disp parametros, String mensaje_recibido){

    //Variables internas de la funcion
    String sondas[20];                  //ID de las sondas en formato String
    char temp[10];
    boolean varios_datos;               //indica la existencia de varios datos ( varias sondas,emails, etc);
    const unsigned int n_datos=11;       //1-ID sondas;2-tmax sondas;3-tmin sondas;4-campo sonda;5-ID canal;6-Clave de escritura;7-Email mant;8-Email com
    String dato;                        //indica el dato que vamos buscar en el mensaje recibido
    unsigned int ubicador;              //puntero para actualizar la posicion en el mensaje
    signed int posicion;                //indica posicion en el mensaje
    String pasarela;                    //recibe el valor de dato desde la cadena

//Interpretacion del mensaje recibido===========================================
        posicion=mensaje_recibido.indexOf("n_sondas=");
        if(posicion>=0){
          parametros.error=0;
          //recopilo n_sondas--------------------------------
          posicion+=9;
          while (mensaje_recibido[posicion]!=';') {
              pasarela+=mensaje_recibido[posicion];
              posicion++;
          }
          int n=pasarela.toInt();
          parametros.n_sondas=n;
          pasarela = "";
          //recopilo n_email_mant----------------------------
          posicion=mensaje_recibido.indexOf("n_email0")+9;
          while (mensaje_recibido[posicion]!=';') {
              pasarela+=mensaje_recibido[posicion];
              posicion++;
          }
          parametros.n_email_mant=pasarela.toInt();
          pasarela = "";
          //recopilo n_email_com-----------------------------
          posicion=mensaje_recibido.indexOf("n_email1")+9;
          while (mensaje_recibido[posicion]!=';') {
              pasarela+=mensaje_recibido[posicion];
              posicion++;
          }
          parametros.n_email_com=pasarela.toInt();
          pasarela = "";
          //inidices para trabajar sobre el mensaje----------
          dato="id_sonda";
          dato+=n;
          ubicador=10;
          varios_datos=true;
          for (int i=1 ; i<=n_datos ; i++) {
            do{
              posicion=mensaje_recibido.indexOf(dato)+ubicador;
              while (mensaje_recibido[posicion]!=';') {
                  pasarela+=mensaje_recibido[posicion];
                  posicion++;
              }
              switch (i) {
          //Sondas
                case 1:
                    sondas[n-1]=pasarela;
                    n--;
                    if(n!=0){
                        dato="id_sonda";
                        dato+=n;
                    }
                    else{
                        n=parametros.n_sondas;
                        dato="id_db";
                        dato+=n;
                        ubicador=7;
                        varios_datos=false;
                    }
                    break;
          //localizador en la db
                case 2:
                    varios_datos=true;
                    parametros.id_db[n-1]=pasarela.toInt();
                    n--;
                    if(n!=0){
                        dato="id_db";
                        dato+=n;
                    }
                    else{
                        n=parametros.n_sondas;
                        dato="ubicacion";
                        dato+=n;
                        ubicador=11;
                        varios_datos=false;
                    }
                    break;
          //ubicacion de la sonda
                case 3:
                    varios_datos=true;
                    parametros.ubicacion[n-1]=pasarela;
                    n--;
                    if(n!=0){
                        dato="ubicacion";
                        dato+=n;
                    }
                    else{
                        n=parametros.n_sondas;
                        dato="t_max";
                        dato+=n;
                        ubicador=7;
                        varios_datos=false;
                    }
                    break;
          //Temperaturas
                case 4:
                    varios_datos=true;
                    parametros.t_max_sondas[n-1]=pasarela.toInt();
                    n--;
                    if(n!=0){
                        dato="t_max";
                        dato+=n;
                    }
                    else{
                        n=parametros.n_sondas;
                        dato="t_min";
                        dato+=n;
                        ubicador=7;
                        varios_datos=false;
                    }
                    break;
          
                case 5:
                    varios_datos=true;
                    parametros.t_min_sondas[n-1]=pasarela.toInt();
                    n--;
                    if(n!=0){
                        dato="t_min";
                        dato+=n;
                    }
                    else{
                        n=parametros.n_sondas;
                        dato="campo";
                        dato+=n;
                        ubicador=7;
                        varios_datos=false;
                    }
                    break;
          //campos ThingSpeak
                case 6:
                    varios_datos=true;
                    parametros.campo[n-1]=pasarela.toInt();
                    n--;
                    if(n!=0){
                        dato="campo";
                        dato+=n;
                    }
                    else{
                        dato="nombre_tienda=";
                        ubicador=14;
                        varios_datos=false;
                    }
                    break;
                    
          //Tienda---------------------------------------------
                case 7:
                    parametros.tienda=pasarela;
                    dato="id_canal=";
                    ubicador=9;
                    break;
                    
          //Canales---------------------------------------------
                case 8:
                    parametros.id_canal=pasarela.toInt();
                    dato="WAK=";
                    ubicador=4;
                    break;
          
                case 9:
                    parametros.WAK=pasarela;
                    n=parametros.n_email_mant;
                    dato="mantenimiento";
                    dato+=n;
                    ubicador=15;
                    break;
          //Emails---------------------------------------------
                case 10:
                    varios_datos=true;
                    parametros.email_mant[n-1]=pasarela;
                    n--;
                    if(n!=0){
                        dato="mantenimiento";
                        dato+=n;
                    }
                    else{
                        n=parametros.n_email_com;
                        dato="comunicaciones";
                        dato+=n;
                        ubicador=16;
                        varios_datos=false;
                    }
                    break;
          
                case 11:
                    varios_datos=true;
                    parametros.email_com[n-1]=pasarela;
                    n--;
                    if(n!=0){
                        dato="comunicaciones";
                        dato+=n;
                    }
                    else{
                        varios_datos=false;
                    }
                    break;
              }
              delay(200);
              pasarela = "";
            }while(varios_datos==true);
          }
          
          parametros=procesa_ID_sonda(sondas,parametros);

        }
        else{
          posicion=mensaje_recibido.indexOf("ERROR")+5;
          pasarela=mensaje_recibido[posicion];
          parametros.error=pasarela.toInt();
          //Subo error al Servidor
          pasarela="";
          switch(parametros.error){
              case 1:
                  parametros.error=31;
                  Serial.println("ERROR1: Error en la petición, no existe ningún dispositivo con este ID en la base de datos o coincide con el ID de otro dispositivo");
                  break;
              case 2:
                  parametros.error=32;
                  Serial.println("ERROR2: Error en la petición, no hay sondas en la base de datos que estén asociadas a este dispositivo");
                  break;
              case 3:
                  parametros.error=33;
                  Serial.println("ERROR3: Error en la petición, no hay canales en la base de datos que estén asociados a este dispositivo");
                  break;
              case 4:
                  parametros.error=34;
                  Serial.println("ERROR4: Error en la petición, no existen usuarios de mantenimiento registrados en la base de datos y que estén asociadas a este dispositivo");
                  break;
              case 5:
                  parametros.error=35;
                  Serial.println("ERROR5: Error en la petición, fallo al realizar la consulta");
                  break;
              case 6:
                  parametros.error=36;
                  Serial.println("ERROR6: Error en la petición, no existe la tienda a la que el dispositivo está conectada");
          }
        }
    return parametros;
}


struct datos_disp procesa_ID_sonda(String IDs[], struct datos_disp estructura){
          int t=0;
          String pasarela="";
          for(int x=0;x<estructura.n_sondas;x++){
              for(int z=0;z<IDs[x].length();z++){
                  if(IDs[x][z]!=' '){
                      pasarela+=IDs[x][z];
                  }
                  if(IDs[x][z]==' ' || z==(IDs[x].length()-1)){
                      estructura.id_sondas[x][t]=pasarela.toInt();
                      t++;
                      pasarela="";
                      if(z==(IDs[x].length()-1)) t=0;
                  }
              }
          }
          return estructura;
}



