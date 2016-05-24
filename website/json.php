<?php
	
	$input = file_get_contents("php://input");
	
	if(isset($input)){
		$decoded = json_decode(stripslashes($input), TRUE);
		if(!is_null($decoded)){
			$response['highscore_0'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
			$response['highscore_1'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
			$response['highscore_2'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
			$response['highscore_3'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
			$response['highscore_4'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
			$response['highscore_5'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
			$response['highscore_6'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
			$response['highscore_7'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
			$response['highscore_8'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
			$response['highscore_9'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
			$response['highscore_10'] = array("you", 0, 0, 0.0, 0, 0, 0, 0);
		}
		$response['status'] = "fine";
	}else{
		$response['status'] = "error";
	}
	
	$encoded = json_encode($response);
	header('Content-type: application/json');
	exit($encoded);
	
?>