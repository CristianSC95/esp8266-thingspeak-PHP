   
void error_dispositivo(unsigned int estado){   
    Serial.println("Actualizando estado del dispositivo en el servidor...");
    if (client.connect(server_ip, 80)>0) {  // Conexion con el servidor
        client.print("GET /piedra/estado/estado.php?tabla=dispositivo&id_disp="); // Enviamos los datos por GET
        client.print(ID);
        client.print("&estado=");
        client.print(estado);
        client.println(" HTTP/1.0");
        client.println("User-Agent: Arduino 1.0");
        client.println();
        Serial.println("Estado actualizado con éxito");
        delay(100);
    } else {
        Serial.println("No se pudo notificar error a la base de datos.");
    }
}

void error_sonda(unsigned int sonda, unsigned int estado){   
    Serial.println("Actualizando estado de la sonda en el servidor...");
    if (client.connect(server_ip, 80)>0) {  // Conexion con el servidor
        client.print("GET /piedra/estado/estado.php?tabla=sonda&id_disp="); // Enviamos los datos por GET
        client.print(ID);
        client.print("&id_sonda=");
        client.print(sonda);
        client.print("&estado=");
        client.print(estado);
        client.println(" HTTP/1.0");
        client.println("User-Agent: Arduino 1.0");
        client.println();
        Serial.println("Estado actualizado con éxito");
        delay(100);
    } else {
        Serial.println("No se pudo notificar error a la base de datos.");
    }
}

