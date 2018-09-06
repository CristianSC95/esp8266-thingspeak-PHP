<?php
    //sondas.php--------------------------------------
    $tabla="usuarios";
//OBTENER EMAILs DE MANTENIMIENTO FRIGORIFICO---------------------------------------------------------------
    $stmt = $con->stmt_init();
    
    //Frase para la consulta MySQL
    $query="SELECT * FROM ".$tabla." WHERE id_tienda=".$id_tienda." AND identificacion='mantenimiento'";

    if($stmt->prepare($query)){
        $stmt->execute();
        $stmt->store_result(); //Sin esta línea no podemos obtener el total de resultados anticipadamente
        $cuantos_registros = $stmt->num_rows;//Nº filas = Nº de sondas asociadas a ese dispositivo
        if ($cuantos_registros>=1) {
            
            //Hay varias sondas asociadas a ese dispositivo
            $resultado_peticion=$resultado_peticion."n_email0=".$cuantos_registros.";";
            $stmt->bind_result($id_usuario,$identificacion,$contacto,$id_tienda);

            for ($i=1; $i <= $cuantos_registros; $i++) {
                $stmt->fetch();
                $resultado_peticion=$resultado_peticion."mantenimiento".$i."=".$contacto.";";
            }

        }
        else{
            //ERROR:no hay sondas asociadas a ese dispositivo
            $ERROR=4;
        }

    }
    else{
            $ERROR=5;
    }
    $stmt->close();

//OBTENER EMAILs DE MANT. DE DISPOSITIVOS---------------------------------------------------------------
    $stmt = $con->stmt_init();
    
    //Frase para la consulta MySQL
    $query="SELECT * FROM ".$tabla." WHERE id_tienda=".$id_tienda." AND identificacion='comunicaciones'";

    if($stmt->prepare($query)){
        $stmt->execute();
        $stmt->store_result(); //Sin esta línea no podemos obtener el total de resultados anticipadamente
        $cuantos_registros = $stmt->num_rows;//Nº filas = Nº de sondas asociadas a ese dispositivo
        if ($cuantos_registros>=1) {
            
            //Hay varias sondas asociadas a ese dispositivo
            $resultado_peticion=$resultado_peticion."n_email1=".$cuantos_registros.";";
            $stmt->bind_result($id_usuario,$identificacion,$contacto,$id_tienda);

            for ($i=1; $i <= $cuantos_registros; $i++) {
                $stmt->fetch();
                $resultado_peticion=$resultado_peticion."comunicaciones".$i."=".$contacto.";";
            }

        }
        else{
            //ERROR:no hay sondas asociadas a ese dispositivo
            $ERROR=4;
        }

    }
    else{
            $ERROR=5;
    }
    $stmt->close();

?>