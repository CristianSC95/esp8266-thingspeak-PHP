<?php
error_reporting(E_ALL);
ini_set('display_errors', '1');
//Librerías para el envío de mail
include_once('class.phpmailer.php');
$t_espera=600;
$tiempos=apcu_fetch('times');
$fallos=apcu_fetch('fallos');
$t_actual=time();
print_r($fallos);
while($tiempo = current($tiempos)){
	$id=key($tiempos);
	if($id>=1 && $id<=100){
		$t_anterior=$tiempos[$id];
		if(($t_actual-$t_anterior)>=$t_espera && $fallos[$id]==false){
	        //enviar email fallo conexión
	        $fallos[$id]=true;
	        proceso_envio_fallo($id);
	    }
	    else if(($t_actual-$t_anterior)<$t_espera && $fallos[$id]==true){
	    	//enviar email recuperación de conexión
			$fallos[$id]=false;
			proceso_envio_exito($id);
	    }
	}
	else{
		unset($tiempos[$id]);
	}
	next($tiempos);
}
print_r($fallos);
apcu_store('fallos',$fallos);
apcu_store('times',$tiempos);


//Proceso de envío de email de fallo---------------------------------------------------------------------
function proceso_envio_fallo(int $disp){
	//OBTENER EMAILs DE MANT. DE DISPOSITIVOS
	require("config.php");
    $stmt = $con->stmt_init();
    
    //Frase para la consulta MySQL
    $query="SELECT * FROM usuarios WHERE identificacion='comunicaciones'";

    if($stmt->prepare($query)){
        $stmt->execute();
        $stmt->store_result(); //Sin esta línea no podemos obtener el total de resultados anticipadamente
        $cuantos_registros = $stmt->num_rows;//Nº filas = Nº de sondas asociadas a ese dispositivo
        if ($cuantos_registros>=1) {
            
            $stmt->bind_result($id_usuario,$identificacion,$contacto,$id_tienda);

            for ($i=1; $i <= $cuantos_registros; $i++) {
                $stmt->fetch();
                $email=$contacto;
                envia_email_fallo($disp, $email);
            }

        }

    }
    $stmt->close();
}

//Proceso de envio de email de exito---------------------------------------------------------------------
function proceso_envio_exito(int $disp){
	//OBTENER EMAILs DE MANT. DE DISPOSITIVOS
	require("config.php");
    $stmt = $con->stmt_init();
    
    //Frase para la consulta MySQL
    $query="SELECT * FROM usuarios WHERE identificacion='comunicaciones'";

    if($stmt->prepare($query)){
        $stmt->execute();
        $stmt->store_result(); //Sin esta línea no podemos obtener el total de resultados anticipadamente
        $cuantos_registros = $stmt->num_rows;//Nº filas = Nº de sondas asociadas a ese dispositivo
        if ($cuantos_registros>=1) {
            
            $stmt->bind_result($id_usuario,$identificacion,$contacto,$id_tienda);

            for ($i=1; $i <= $cuantos_registros; $i++) {
                $stmt->fetch();
                $email=$contacto;
                envia_email_exito($disp, $email);
            }

        }

    }
    $stmt->close();
}


//Configuración email de conexión perdida----------------------------------------------------------------
function envia_email_fallo(int $id_dispositivo, string $email){
	
	$asunto = 'Conexion perdida';//$asunto  NO USAR TILDES
	$mensaje = 'Se ha perdido la conexión con el dispositivo con ID '.$id_dispositivo.'.';//$mensaje


	envia_email($email, $mensaje, $asunto);
}


//Configuración email de conexión recuperada-------------------------------------------------------------
function envia_email_exito(int $id_dispositivo, string $email){
	
	$asunto = 'Conexion recuperada';//$asunto  NO USAR TILDES
	$mensaje = 'Se ha recuperado la conexion con el dispositivo con ID '.$id_dispositivo.'.';//$mensaje

	envia_email($email, $mensaje, $asunto);

}

//Envío del email----------------------------------------------------------------------------------------
function envia_email(string $destinatario, string $mensaje, string $asunto){
	//cuenta
	$user = "ccs.formacion@gmail.com";
	$pass = "636373982";
	$ename = "Monitorizacion temperaturas Piedra";

	$mail = new PHPMailer();
	$mail->IsSMTP();
	$mail->SMTPAuth = true;
	$mail->SMTPSecure = "ssl";
	$mail->Host = "smtp.gmail.com";
	$mail->Port = 465;

	//Nuestra cuenta
	$mail->Username =$user;
	$mail->Password = $pass;
	$mail->From = $user;
	$mail->FromName = $ename;
	$mail->SetFrom($user,$ename);

	//Agregar destinatario
	$mail->AddAddress($destinatario);//$para
	$mail->Subject = $asunto;//$asunto  NO USAR TILDES
	$mail->IsHTML(true);
	$mail->Body = $mensaje;//$mensaje


	//Avisar si fue enviado o no y dirigir al index
	if($mail->Send())
	{
		echo "Mensaje enviado con éxito";
		echo "<br>";
	}
	else{
		echo "No se pudo enviar el mensaje.";
		echo "<br>";
	}
}
?>
