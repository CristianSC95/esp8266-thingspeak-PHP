<?php
    //tienda.php--------------------------------------
    $tabla="tienda";

    $stmt = $con->stmt_init();
    
    //Frase para la consulta MySQL
    $query='SELECT * FROM '.$tabla.' WHERE id_tienda='.$tienda;

    if($stmt->prepare($query)){
        $stmt->execute();
        $stmt->store_result(); //Sin esta línea no podemos obtener el total de resultados anticipadamente
        $cuantos_registros = $stmt->num_rows;//Nº filas = Nº de dispositivos con ese id
        if($cuantos_registros==1){

            //No puede haber más de un dispositivo con ese ID.
            $stmt->bind_result($id_tienda,$tienda);
            $stmt->fetch();
            $resultado_peticion=$resultado_peticion."nombre_tienda=".$tienda.";";

        }
        else{
            //ERROR:no existe ninguna tienda con ese identificador o corresponde a más de una tienda
            $ERROR=6;
        }

    }
    else{
            $ERROR=5;
    }
    $stmt->close();
?>