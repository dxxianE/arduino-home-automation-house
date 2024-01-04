<?php
//Establecemos los valores para la conexión a la base de datos mySQL
$hostname = "localhost"; 
$username = "root"; 
$password = ""; 
$database = "sensor_db"; 

//Establecemos la conexión a la base de datos utilizando "mysqli_connect"
$conn = mysqli_connect($hostname, $username, $password, $database);

//Si fallamos simplemente nos suelta un mensaje de error
if (!$conn) { 
	die("Connection failed: " . mysqli_connect_error()); 
} 
//en su defecto da un OK!
echo "Database connection is OK"; 

//Verifica si se han enviado los datos de temperatura y humedad mediante una solicitud POST:
if(isset($_POST["temperature"]) && isset($_POST["humidity"])) {

//Obtiene los valores de temperatura y humedad de la solicitud POST y los almacena en variables:
	$t = $_POST["temperature"];
	$h = $_POST["humidity"];

	//Con esta linea creamos la consulta en SQL para insertar los valores del DHT11
	$sql = "INSERT INTO dht11 (temperature, humidity) VALUES (".$t.", ".$h.")"; 

	//Ejecuta la consulta SQL utilizando la función mysqli_query()
	//depende de si ha ido bien o no lanzara un mensaje
	if (mysqli_query($conn, $sql)) { 
		echo "\nNew record created successfully"; 
	} else { 
		echo "Error: " . $sql . "<br>" . mysqli_error($conn); 
	}
}

?>
