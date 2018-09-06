<?php
    // estado.php
    //parámetros recibidos en petición http
    $tabla=$_GET['tabla'];
    $id_disp=$_GET['id_disp'];
    $id_sonda=$_GET['id_sonda'];
    $estado=$_GET['estado'];

    $tiempo_disp=apcu_fetch('times');
//    $fallo=apcu_fetch('fallos');
    $tiempo_disp[$id_disp]=time();
//    $fallo[$id_disp]=false;
    apcu_store('times',$tiempo_disp);
//    apcu_store('fallos',$fallo);

    echo $tabla;
    echo $id_disp;
    echo $id_sonda;
    echo $estado;
//config.php-------------------------------------------
    require("config.php");

//update.php--------------------------------------
    require("update.php");
?>
