void subir_TS(String w_key, int cantidad,int field[], float temp[]){
    //ThingSpeak information
    String request_string;
    String thingSpeakAddress= "http://api.thingspeak.com/update?";

    client.connect("api.thingspeak.com",80);
    request_string = thingSpeakAddress;
    request_string += "key=";
    request_string += w_key;
    for(int i=0;i<cantidad;i++){
      if(temp[i]!=85.0 || temp[i]!=100.0){
        request_string += "&field";
        request_string += field[i];
        request_string += "=";
        request_string += temp[i];
      }
    }
    http.begin(request_string);
    http.GET();
    http.end();
}
