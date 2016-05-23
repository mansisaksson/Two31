<?php
	if(isset($_REQUEST['json'])){
		$decoded = json_decode(stripslashes($_REQUEST['json']), TRUE);
		if(!is_null($decoded)){
			$response = array(
				array("you", 0, 0, 0.0f, 0, 0, 0, 0),
				array("player1", 0, 0, 0.0f, 0, 0, 0, 0),
				array("player2", 0, 0, 0.0f, 0, 0, 0, 0),
				array("player3", 0, 0, 0.0f, 0, 0, 0, 0),
				array("player4", 0, 0, 0.0f, 0, 0, 0, 0),
				array("player5", 0, 0, 0.0f, 0, 0, 0, 0),
				array("player6", 0, 0, 0.0f, 0, 0, 0, 0),
				array("player7", 0, 0, 0.0f, 0, 0, 0, 0),
				array("player8", 0, 0, 0.0f, 0, 0, 0, 0),
				array("player9", 0, 0, 0.0f, 0, 0, 0, 0),
				array("player10", 0, 0, 0.0f, 0, 0, 0, 0)
				);
		}
	}else{
		exit();
	}
	
	$encoded = json_encode($response);
	header('Content-type: application/json');
	exit($encoded);
	
?>