<?php
    // consulta.php
    //parámetros recibidos en petición http
    $tabla=$_GET['tabla'];
    $id=$_GET['id'];
    //almacén de datos que se envían a arduino
    $resultado_peticion="";
    //comienzo sin errores
    $ERROR=0;
//config.php-------------------------------------------
    require("config.php");
//dispositivo.php--------------------------------------
    require("dispositivo.php");
    if($ERROR) goto ERROR;
//sondas.php-------------------------------------------
    require("sondas.php");
    if($ERROR) goto ERROR;
//canal.php--------------------------------------------
    require("canal.php");
    if($ERROR) goto ERROR;
//tienda.php--------------------------------------------
    require("tienda.php");
    if($ERROR) goto ERROR;
//usuarios.php-----------------------------------------
    require("usuarios.php");
    if(!$ERROR) goto FIN;

ERROR:
    /*
    $ERROR contiene el código del error
    Posibles errores:
    1-No existe dispositivo con ese ID o existe más de un dispositivo con él.
    2-No hay sondas asociadas a ese dispositivo.
    3-No hay canales asociados a ese dispositivo.
    4-No existen usuarios de mantenimiento asociados al dispositivo.
    5-Fallo al realizar la consulta.
    6-No existe ninguna tienda con ese ID o hay más de una tienda con él.
    */

    echo "ERROR".$ERROR;

FIN:
    echo $resultado_peticion;
?>