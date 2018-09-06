<?php
    //sleep.php--------------------------------------


    $mail = "Conectado con éxito";
    //Titulo
    $titulo = "Conexión exitosa";
    //cabecera
    $headers = "MIME-Version: 1.0\r\n"; 
    $headers .= "Content-type: text/html; charset=iso-8859-1\r\n"; 
    //dirección del remitente 
    $headers .= "From: Servidor Local < cristian.cs.contact@gmail.com >\r\n";
    //Enviamos el mensaje a tu_dirección_email 
    $bool = mail("cristian.cs.contact@gmail.com",$titulo,$mail,$headers);
    if($bool){
        echo "Mensaje enviado";
    }else{
        echo "Mensaje no enviado";
    }

    sleep(10);

    $mail = "Se ha perdido la conexión con el dispositivo";
    //Titulo
    $titulo = "Conexión fallida";
    //cabecera
    $headers = "MIME-Version: 1.0\r\n"; 
    $headers .= "Content-type: text/html; charset=iso-8859-1\r\n"; 
    //dirección del remitente 
    $headers .= "From: Servidor Local < ccs.formacion@gmail.com >\r\n";
    //Enviamos el mensaje a tu_dirección_email 
    $bool = mail("cristian.cs.contact@gmail.com",$titulo,$mail,$headers);
    if($bool){
        echo "Mensaje enviado";
    }else{
        echo "Mensaje no enviado";
    }

?>