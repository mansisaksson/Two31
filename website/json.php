<?php
	
	$input = file_get_contents("php://input");
	
	if(isset($input)){
		$decoded = json_decode(stripslashes($input), TRUE);
		if(!is_null($decoded)){
			$decoded = $decoded['highscore'];
			
			$mysqli = new mysqli("localhost", "two31", "DmquVrtM6ADNWSrq", "two31");
			if(!$mysqli->connect_errno){
				$today = getdate();
				$time = $today['year']."-".$today['mon']."-".$today['mday']." ".$today['hours'].":".$today['minutes'].":".$today['seconds'];
				
			
				
				if($stmt = $mysqli->prepare("INSERT INTO highscore VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")){
					if(!$stmt->bind_param("ssssssssss", $decoded[0], $decoded[1], $decoded[2], $decoded[3], $decoded[4], $decoded[5], $decoded[6], $decoded[7], $decoded[8], $time)){
						$response['status'] = "Bind params failed: (" . $mysqli->errno . ") " . $mysqli->error;
					}
					if(!$stmt->execute()){
						$response['status'] = var_export($decoded, true);
						//$response['status'] = "[".$decoded[0].", ".$decoded[1]."]execute failed: (" . $mysqli->errno . ") " . $mysqli->error;
					}
					$stmt->close();
				
					if($stmt2 = $mysqli->prepare("SELECT * FROM highscore WHERE level = ? ORDER BY enemies DESC, secrets DESC, time ASC, bullets ASC, damage ASC, health ASC, armor ASC, date DESC LIMIT 10")){
						$stmt2->bind_param("s", $decoded[0]);
						$stmt2->execute();
						$stmt2->bind_result($level, $name, $kills, $secrets, $time2, $bullets, $damage, $bla, $bla2, $date);
						
						$response['rank'] = -1;
						
						while($stmt2->fetch()){
							$response['highscore_'.($i+1)] = array($name, $kills, $secrets, $time2, $bullets, $damage, 0, 0);
						}
						
						$response['highscore_0'] = array($decoded[1], $decoded[2], $decoded[3], $decoded[4], $decoded[5], $decoded[6], $decoded[7], $decoded[8]);
						$stmt2->close();
						
					}else{
						$response['status'] = "error: failed to retrieve top 10 list";
					}
					
				}else{
					$response['status'] = "Prepare failed: (" . $mysqli->errno . ") " . $mysqli->error;
				}
				
			}else{
				$response['status'] = "error: cant connect mysqli";
			}
			
		}else{
			$response['status'] = "error: wrong json";
		}
		
	}else{
		$response['status'] = "error: no json";
	}
	
	$encoded = json_encode($response);
	header('Content-type: application/json');
	exit($encoded);
	
?>