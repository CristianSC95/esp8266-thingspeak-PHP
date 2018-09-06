<?php
    //canal.php--------------------------------------
	$tabla="canales";

    $stmt = $con->stmt_init();
    
    //Frase para la consulta MySQL
    $query='SELECT * FROM '.$tabla.' WHERE id_dispositivo='.$id;

    if($stmt->prepare($query)){
        $stmt->execute();
        $stmt->store_result(); //Sin esta línea no podemos obtener el total de resultados anticipadamente
        $cuantos_registros = $stmt->num_rows;//Nº filas = Nº de canales asociados a ese dispositivo
        if($cuantos_registros==1){

            //No puede haber más de un canal asociado a un dispositivo.
            $stmt->bind_result($id_canal,$ID,$id_dispositivo,$WAK,$RAK);
            $stmt->fetch();

            $resultado_peticion=$resultado_peticion."id_canal=".$ID.";WAK=".$WAK.";";

        }
        else{
            //ERROR:no existe ningún canal o hay varios asociados a ese dispositivo
            $ERROR=3;
        }

    }
    else{
            $ERROR=5;
    }
    $stmt->close();
?>