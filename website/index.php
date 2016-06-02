<?php
	
	$mysqli = new mysqli("localhost", "two31", "DmquVrtM6ADNWSrq", "two31");
	if($mysqli->connect_errno){
		echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
		exit();
	}else{
		$res = $mysqli->query("SELECT * FROM highscore ORDER BY time ASC, level DESC, enemies DESC, secrets DESC, bullets ASC, damage ASC, health ASC, armor ASC, date DESC LIMIT 10");
		$highscoreList = $res->fetch_all();
	}
	
?>
<!DOCTYPE html>
<html lang="en">
<head>
	
	<link rel="icon" href="logo.png">
	<title>Two31</title>
	
	<!-- https://www.google.com/fonts/specimen/Open+Sans -->
	<link href='https://fonts.googleapis.com/css?family=Open+Sans' rel='stylesheet' type='text/css'>
	
	<style>
		html, body{
			margin:0px;
			padding:0px;
			width:100%;
			height:100%;
			font-family: 'Open Sans', sans-serif;
		}
		body{
			background-image:url("banner.png");
			background-repeat: no-repeat;
			background-position: center-x;
			background-color:#000;
		}
		table{
			border-collapse: collapse;
			margin:auto;
		}
		table td{
			border:1px solid #000;
			padding:10px;
		}
	</style>
	
</head>
<body>
	
	<div style="width:50%; margin:auto; margin-top:200px; height:600px; color:#fff; font-size:40px; text-align:center; background-color:rgba(0, 0, 0, 0.3); padding:0px 20px 0px 20px;">
		<h1>Two31</h1>
		Two:31 is a fast paced, Doom inspired FPS with a retro feeling to it and a dark Lovecraftian theme.
		 Earth is but a shell of its former self.
		 Humanity have emptied almost all resources on earth and tainted all of its water.
		 To survive, humanity needs to get water elsewhere.
	</div>
	
	<div style="color:#FFF; font-size:24px; margin-top:800px; width:100%;">
		
		<div style="clear:both; width:1300px; margin:auto;">
			<div style="float:left; width:550px; padding:50px; padding-top:50px;">
				<iframe width="550" height="310" src="https://www.youtube.com/embed/E_lL7c6DogI" frameborder="0" allowfullscreen></iframe>
				<br><br><br><br><br><br><br><br>
			</div>
			<div style="float:left; width:550px; padding-left:100px;">
				<img src="poster.png" width="50%" height="50%">
			</div>
		</div>
		
		<div style="width:100%; overflow:hidden; background-color:#FFF; margin-bottom:100px;">
		<div style="clear:both; width:1300px; margin:auto; padding-top:100px;">
			<div style="float:left; width:550px; padding-left:100px;">
				<img src="head.png" width="50%" height="50%" style="float:left;">
				<img src="imp.png" width="50%" height="50%" style="float:left;">
			</div>
			<div style="float:left; width:550px; padding:50px; padding-top:120px; color:#000;">
				You play as a mercenary, hired by a company that owns a water harvesting base.
				 A distress signal have been received from said base which is located on one of Neptune’s moons – Triton.
				 Something has gone terribly wrong, you must go to Triton and make sure that the problem is handled.
				 The base is needed. Without the water.
				 Humanity will perish.
				<br><br><br><br><br><br><br>
			</div>
		</div>
		</div>
		
		<div style="clear:both; width:1300px; margin:auto;">
			<div style="float:left; width:550px; padding:50px; padding-top:120px;">
				The game is made in Unreal Engine 4 by 17 students at the University of Skövde.
				 We all feel that the retro FPS genre have been forgotten and we’re trying to bring it back because it’s something that’s missing in today’s market.
				 “We’re not trying to out doom Doom, but we’ll try to out doom any other title that has been released in the last 10 years”
				<br><br><br><br><br><br><br>
			</div>
			<div style="float:left; width:550px; padding-left:100px;">
				<img src="logo.png">
			</div>
		</div>
		
	</div>
	
	<div style="color:#000; clear:both; width:100%; margin-top:100px; padding-top:100px; padding-bottom:100px; text-align:center; background-color:#FFF; overflow:hidden;">
		<h1>Highscore</h1>
		<table>
			<thead style="font-weight:bold;">
				<td>Rank</td>
				<!-- <td>Level</td> -->
				<td>Username</td>
				<td>Enemies killed</td>
				<td>Secrets found</td>
				<td>Time</td>
				<td>Bullets fired</td>
				<td>Damage taken</td>
				<td>Health lost</td>
				<td>Armor lost</td>
				<td>Date</td>
			</thead>
			<?php
				for($i=0; $i<count($highscoreList); $i++){
					echo '<tr><td>'.($i+1).'</td>';
					//echo '<td>'.$highscoreList[$i][0+1].'</td>';
					echo '<td>'.$highscoreList[$i][1+1].'</td>';
					echo '<td>'.$highscoreList[$i][2+1].'%</td>';
					echo '<td>'.$highscoreList[$i][3+1].'%</td>';
					echo '<td>'.$highscoreList[$i][4+1].'</td>';
					echo '<td>'.$highscoreList[$i][5+1].'</td>';
					echo '<td>'.$highscoreList[$i][6+1].'</td>';
					echo '<td>'.$highscoreList[$i][7+1].'</td>';
					echo '<td>'.$highscoreList[$i][8+1].'</td>';
					echo '<td>'.$highscoreList[$i][9+1].'</td>';
					echo '</tr>';
				}
			?>
		</table>
	</div>
	
</body>
</html>