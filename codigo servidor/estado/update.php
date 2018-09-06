<?php
    //update.php--------------------------------------

    $stmt = $con->stmt_init();
    
    //Frase para la consulta MySQL
    if($tabla=="dispositivo"){
        $query='UPDATE '.$tabla.' SET estado='.$estado.', fecha=NOW() WHERE id_dispositivo='.$id_disp;
    	if(!apcu_exists('fallos'){     //Creación de un ID fallo la primera vez
    		$fallo[$id_disp]=false;
    		apcu_store('fallos',$fallo);
    	}
    }
    else if($tabla=="sonda"){
        $query='UPDATE '.$tabla.' SET estado='.$estado.', fecha=NOW() WHERE id_dispositivo='.$id_disp.' AND id_sonda='.$id_sonda;
    }

    if($stmt->prepare($query)){
        $stmt->execute();
        echo "Datos actualizados correctamente.";
    }
    else{
        echo "ERROR ";
    }

    $stmt->close();
?>
