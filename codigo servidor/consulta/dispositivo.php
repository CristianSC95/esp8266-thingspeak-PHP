<?php
    //dispositivo.php--------------------------------------

    $stmt = $con->stmt_init();
    
    //Frase para la consulta MySQL
    $query='SELECT * FROM '.$tabla.' WHERE id_dispositivo='.$id;

    if($stmt->prepare($query)){
        $stmt->execute();
        $stmt->store_result(); //Sin esta línea no podemos obtener el total de resultados anticipadamente
        $cuantos_registros = $stmt->num_rows;//Nº filas = Nº de dispositivos con ese id
        if($cuantos_registros==1){

            //No puede haber más de un dispositivo con ese ID.
            $stmt->bind_result($id_dispositivo,$id_tienda,$id_canal,$estado,$fecha);
            $stmt->fetch();
            $tienda=$id_tienda;

        }
        else{
            //ERROR:no existe ningún dispositivo con ese identificador o corresponde a más de un dispositivo
            $ERROR=1;
        }

    }
    else{
            $ERROR=5;
    }
    $stmt->close();
?>