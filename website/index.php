<?php
	
	$mysqli = new mysqli("localhost", "two31", "DmquVrtM6ADNWSrq", "two31");
	if($mysqli->connect_errno){
		echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
		exit();
	}else{
		$res = $mysqli->query("SELECT * FROM highscore ORDER BY level DESC, enemies DESC, secrets DESC, time ASC, bullets ASC, damage ASC, health ASC, armor ASC, date DESC LIMIT 10");
		$highscoreList = $res->fetch_all();
	}
	
?>
<!DOCTYPE html>
<html>
<head>
	
	<style>
		table{
			border-collapse: collapse;
		}
		table td{
			border:1px solid #000;
			padding:5px;
		}
	</style>
	
</head>
<body>
	
	<table>
		<thead>
			<td>Level:</td>
			<td>Username:</td>
			<td>Enemies killed:</td>
			<td>Secrets found:</td>
			<td>Time:</td>
			<td>Bullets fired:</td>
			<td>Damage taken:</td>
			<td>Health lost:</td>
			<td>Armor lost:</td>
			<td>Date:</td>
		</thead>
		<?php
			for($i=0; $i<count($highscoreList); $i++){
				
				$highscoreList[$i][4] = 71.123456;
				
				$secs = floor($highscoreList[$i][4]);
				$milli = (int) (($highscoreList[$i][4] - $secs) * 1000);
				$hours = floor($secs / 3600);
				$minutes = (($secs / 60) % 60);
				$seconds = $secs % 60;
				
				$hours = "".$hours;
				if(strlen($hours) != 2){
					$hours = $hours."0";
				}
				
				$minutes = "".$minutes;
				if(strlen($minutes) != 2){
					$minutes = $minutes."0";
				}
				
				$seconds = "".$seconds;
				if(strlen($seconds) != 2){
					$seconds = $seconds."0";
				}
				
				$milli = "".$milli;
				if(strlen($milli) != 3){
					$milli = $milli."0";
				}
				if(strlen($milli) != 3){
					$milli = $milli."0";
				}
				
				echo '<tr>';
				echo '<td>'.$highscoreList[$i][0].'</td>';
				echo '<td>'.$highscoreList[$i][1].'</td>';
				echo '<td>'.$highscoreList[$i][2].'%</td>';
				echo '<td>'.$highscoreList[$i][3].'%</td>';
				echo '<td>'.$hours.':'.$minutes.':'.$seconds.':'.$milli.'</td>';
				echo '<td>'.$highscoreList[$i][5].'</td>';
				echo '<td>'.$highscoreList[$i][6].'</td>';
				echo '<td>'.$highscoreList[$i][7].'</td>';
				echo '<td>'.$highscoreList[$i][8].'</td>';
				echo '<td>'.$highscoreList[$i][9].'</td>';
				echo '</tr>';
			}
		?>
	</table>
	
</body>
</html>