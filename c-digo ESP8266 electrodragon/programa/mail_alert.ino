//==============================================================
//    FALLOS DISPOSITIVO
//==============================================================


void error_consulta_reparado(unsigned int mysql_error,String tienda_disp, int n_mantenimiento, String mantenimientos[]){
        Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
        String subject = "ERROR3 reparado:consulta MySQL sin incidentes";
        String message = "CODIGO DE ERROR: ";
        message+=mysql_error;
        message+=" REPARADO - Se había producido un error en la consulta MySQL a la base de datos pero se ha restablecido el funcionamiento: Consulta a la base de datos realizada sin incidentes. ";
        message+="Tienda ";
        message+=tienda_disp;
        //message+=", zona ";
        //message+=loc_disp;
        for(int i=0;i<n_mantenimiento;i++){
          gsender->Subject(subject)->Send(mantenimientos[i], message);
        }
}


//==============================================================
//    FALLOS SONDAS
//==============================================================
//Sonda desconectada--------------------------------------------------------------------------
void fallo_sonda(int sonda, String tienda_disp, String loc, int n_mantenimiento, String mantenimientos[]){
        Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
        String subject = "Error en sonda de temperatura";
        String message = "Se ha producido un error en la sonda Nº ";
        message+=sonda;
        message+=" conectada al dispositivo con identificador ";
        message+=ID;
        message+=". Es posible que la sonda se haya roto o esté desconectada. CÓDIGO DE ERROR: 50 - ";
        message+="Tienda ";
        message+=tienda_disp;
        message+=", zona ";
        message+=loc;
        for(int i=0;i<n_mantenimiento;i++){
          gsender->Subject(subject)->Send(mantenimientos[i], message);
        }
        Serial.println("Enviado email de alerta por fallo en la sonda.");
}
void fallo_sonda_reparado(int sonda, String tienda_disp, String loc, int n_mantenimiento, String mantenimientos[]){
        Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
        String subject = "Error en sonda de temperatura REPARADO";
        String message = "Restablecido contacto con la sonda Nº ";
        message+=sonda;
        message+=" conectada al dispositivo con identificador ";
        message+=ID;
        message+=". CÓDIGO DE ERROR: 50 REPARADO - ";
        message+="Tienda ";
        message+=tienda_disp;
        message+=", zona ";
        message+=loc;
        for(int i=0;i<n_mantenimiento;i++){
          gsender->Subject(subject)->Send(mantenimientos[i], message);
        }
        Serial.println("Enviado email de alerta de recuperación de la sonda.");
}
    

//Temperatura demasiado baja-----------------------------------------------------------------
void low_temperature(int sonda, String tienda_disp, String loc, int n_mantenimiento, String mantenimientos[]){
        Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
        String subject = "Temperatura del frigorifico muy baja";
        String message = "La sonda Nº ";
        message+=sonda;
        message+=", conectada al dispositivo con identificador ";
        message+=ID;
        message+=", ha dectado que la temperatura está por debajo de la mínima temperatura frigorífica recomendada. Se recomienda supervisión del equipo frigorífico para evitar perder los alimentos. CÓDIGO DE ERROR: 51 - ";
        message+="Tienda ";
        message+=tienda_disp;
        message+=", zona ";
        message+=loc;
        for(int i=0;i<n_mantenimiento;i++){
          gsender->Subject(subject)->Send(mantenimientos[i], message);
        }
        Serial.println("Enviado email de alerta por temperatura baja.");
}
void low_temperature_reparado(int sonda, String tienda_disp, String loc, int n_mantenimiento, String mantenimientos[]){
        Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
        String subject = "Temperatura del frigorifico dentro del rango establecido";
        String message = "La sonda Nº ";
        message+=sonda;
        message+=",conectada al dispositivo con identificador ";
        message+=ID;
        message+=", ha detectado que la temperatura vuelve a estar dentro del rango establecido. CÓDIGO DE ERROR: 51 REPARADO - ";
        message+="Tienda ";
        message+=tienda_disp;
        message+=", zona ";
        message+=loc;
        for(int i=0;i<n_mantenimiento;i++){
          gsender->Subject(subject)->Send(mantenimientos[i], message);
        }
        Serial.println("Enviado email de temperatura restablecida.");
}


//Temperatura muy alta------------------------------------------------------------------------
void high_temperature(int sonda, String tienda_disp, String loc, int n_mantenimiento, String mantenimientos[]){
        Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
        String subject = "Temperatura del frigorifico muy alta";
        String message = "La sonda Nº ";
        message+=sonda;
        message+=", conectada al dispositivo con identificador ";
        message+=ID;
        message+=", ha dectado que la temperatura está por encima de la máxima temperatura frigorífica recomendada. Se recomienda supervisión del equipo frigorífico para evitar perder los alimentos. CÓDIGO DE ERROR: 52 - ";
        message+="Tienda ";
        message+=tienda_disp;
        message+=", zona ";
        message+=loc;
        for(int i=0;i<n_mantenimiento;i++){
          gsender->Subject(subject)->Send(mantenimientos[i], message);
        }
        Serial.println("Enviado email de alerta por temperatura alta.");
}
void high_temperature_reparado(int sonda, String tienda_disp, String loc, int n_mantenimiento, String mantenimientos[]){
        Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
        String subject = "Temperatura del frigorifico dentro del rango establecido";
        String message = "La sonda Nº ";
        message+=sonda;
        message+=",conectada al dispositivo con identificador ";
        message+=ID;
        message+=", ha detectado que la temperatura vuelve a estar dentro del rango establecido. CÓDIGO DE ERROR: 52 REPARADO - ";
        message+="Tienda ";
        message+=tienda_disp;
        message+=", zona ";
        message+=loc;
        for(int i=0;i<n_mantenimiento;i++){
          gsender->Subject(subject)->Send(mantenimientos[i], message);
        }
        Serial.println("Enviado email de temperatura restablecida.");
}
