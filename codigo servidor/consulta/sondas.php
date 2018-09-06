<?php
    //sondas.php--------------------------------------
    $tabla="sonda";

    $stmt = $con->stmt_init();
    
    //Frase para la consulta MySQL
    $query='SELECT * FROM '.$tabla.' WHERE id_dispositivo='.$id;

    if($stmt->prepare($query)){
        $stmt->execute();
        $stmt->store_result(); //Sin esta línea no podemos obtener el total de resultados anticipadamente
        $cuantos_registros = $stmt->num_rows;//Nº filas = Nº de sondas asociadas a ese dispositivo
        if ($cuantos_registros>=1) {
            
            //Hay varias sondas asociadas a ese dispositivo
            $resultado_peticion=$resultado_peticion."n_sondas=".$cuantos_registros.";";
            $stmt->bind_result($id_sonda,$real_ID,$modelo,$id_dispositivo,$ubicacion,$t_max,$t_min,$Campo_TS,$estado,$fecha);

            for ($i=1; $i <= $cuantos_registros; $i++) {
                $stmt->fetch();
                $resultado_peticion=$resultado_peticion."id_sonda".$i."=".$real_ID.";id_db".$i."=".$id_sonda.";ubicacion".$i."=".$ubicacion.";t_max".$i."=".$t_max.";t_min".$i."=".$t_min.";campo".$i."=".$Campo_TS.";";
            }

        }
        else{
            //ERROR:no hay sondas asociadas a ese dispositivo
            $ERROR=2;
        }

    }
    else{
            echo "Aqui fallo";
            $ERROR=5;
    }
    $stmt->close();
?>