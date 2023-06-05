<!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="UTF-8">
	<title>ConnectPHP</title>
</head>

<body>

	<?php

	$dbname = 'dashboard_arduinoBD';
	$dbuser = 'root';
	$dbpass = '';
	$dbhost = 'localhost';

	$connect = @mysqli_connect($dbhost, $dbuser, $dbpass, $dbname);

	if (!$connect) {
		echo "Error: " . mysqli_connect_error();
		exit();
	}

	echo "Conexao bem sucedida!<br><br>";

	$temperature = $_GET["temperature"];
	$humidity = $_GET["humidity"];
	$luminosity = $_GET["luminosity"];

	$query = "INSERT INTO
		monitoring_data (temperature, humidity, luminosity)
		VALUES
	 	('$temperature', '$humidity', '$luminosity')";

	$result = mysqli_query($connect, $query);

	echo "Cadastrado com sucesso!<br>";

	?>
</body>

</html>