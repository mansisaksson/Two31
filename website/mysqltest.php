<?php
	
	$mysqli = new mysqli("localhost", "two31", "DmquVrtM6ADNWSrq", "two31");
	if($mysqli->connect_errno){
		echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
		exit();
	}
	echo $mysqli->host_info . "\n";
	
	echo '<br>';
	
	$res = $mysqli->query("SELECT * FROM highscore");
	$res2 = $res->fetch_all();
	for($i=0; $i<count($res2); $i++){
		var_dump($res2[$i]);
		echo '<br>';
	}
	
?>