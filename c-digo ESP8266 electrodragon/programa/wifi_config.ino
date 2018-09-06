//CONEXION WiFi------------------------------------------------------
char* ssid = "BatCueva";
char* pass = "electronica";

//IP ESTATICA--------------------------------------------------------
IPAddress ip_addr(192, 168, 1, 127);
IPAddress netmask(255, 255, 255, 0);
IPAddress gateway_addr(192, 168, 1, 1);
//IPAddress dns_addr1(8, 8, 8, 8);
//IPAddress dns_addr2(4, 4, 4, 4);

unsigned long tiempo_error=0;

boolean WiFiConnection(){
    WiFi.begin(ssid,pass);
    WiFi.config(ip_addr, gateway_addr, netmask/*, dns_addr1, dns_addr2*/);
    unsigned int dot=0;

    //Conexion Wifi
    Serial.println("Conectando WiFi:");
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
        if(millis()-tiempo_error>=WIFI_ERROR_TIME){
            tiempo_error=millis();
            Serial.println("Conexión WiFi fallida.");
            return 0;
        }
    }
    
    Serial.println("");
    Serial.println("WiFi conectada.");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    return 1;
}
