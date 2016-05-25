<?php
	
	$mysqli = new mysqli("localhost", "two31", "DmquVrtM6ADNWSrq", "two31");
	if($mysqli->connect_errno){
		echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
		exit();
	}
	echo $mysqli->host_info . "\n";
	
	echo '<br>';
	
	$res = $mysqli->query("SELECT `COLUMN_NAME` FROM `INFORMATION_SCHEMA`.`COLUMNS` WHERE `TABLE_SCHEMA`='two31' AND `TABLE_NAME`='highscore'");
	var_dump($res->fetch_all());
	
?>