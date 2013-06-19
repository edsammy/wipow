<html>
	<head>
	<title>congratulations sir!</title>
	<link rel="stylesheet" href="../misc.css">
	</head>
	<body>
	hello

	<?php 
// Creates a PHP script that calls the function in "device_call.php" to retrieve data. Also creates a crontab to run that script at the specified time.

echo "<!DOCTYPE html>";
if (isset($_POST['schedule'])) {
	unset($_POST['schedule']);
	$directory = getcwd();
	include($directory.'/pinclude/devicelist.php');

	if(isset($_REQUEST["startdatecollect"]) && preg_match('/^\d{4}-\d{2}-\d{2}$/',$_REQUEST["startdatecollect"])){$Sdate = $_REQUEST["startdatecollect"];}else{exit("<script language=\"JavaScript\">alert(\"Invalid Start Date\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["SHOURC"])){$Shour = $_POST['SHOURC'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid Start Hour\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["SMINUTEC"])){$Sminute = $_POST['SMINUTEC'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid Start Minute\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["SSECONDC"])){$Ssecond = $_POST['SSECONDC'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid Start Second\");self.close();</script>");}
	if(isset($_REQUEST["enddatecollect"]) && preg_match('/^\d{4}-\d{2}-\d{2}$/',$_REQUEST["enddatecollect"])){$Edate = $_REQUEST["enddatecollect"];}else{exit("<script language=\"JavaScript\">alert(\"Invalid End Date\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["EHOURC"])){$Ehour = $_POST['EHOURC'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid End Hour\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["EMINUTEC"])){$Eminute = $_POST['EMINUTEC'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid End Minute\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["ESECONDC"])){$Esecond = $_POST['ESECONDC'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid End Second\");self.close();</script>");}
	


/*	$Sdate = isset($_REQUEST["startdatecollect"]) ? $_REQUEST["startdatecollect"] : "";
	$Shour = $_POST['SHOURC'];
	$Sminute = $_POST['SMINUTEC'];
	$Ssecond = $_POST['SSECONDC'];
	$Edate = isset($_REQUEST["enddatecollect"]) ? $_REQUEST["enddatecollect"] : "";
	$Ehour = $_POST['EHOURC'];
	$Eminute = $_POST['EMINUTEC'];
	$Esecond = $_POST['ESECONDC'];
	$TBL = $_POST['SITE'];
	$tbl = strtolower($TBL);
*/

	$TBL = $_POST['SITE'];
	$tbl_posted = strtolower($TBL);

	if(in_array($tbl_posted, $devices)){
		$tbl = $tbl_posted;
	} else{
		exit("Wrong device name");
	}
	$Shour = $Shour *1;
	$Sminute = $Sminute *1;
	$Ssecond = $Ssecond *1;
	$Ehour = $Ehour *1;
	$Eminute = $Eminute *1;
	$Esecond = $Esecond *1;
	$SWHEN = "$Sdate $Shour:$Sminute:$Ssecond";
	$EWHEN = "$Edate $Ehour:$Eminute:$Esecond";
	
	$minuteadded = time() + 60;
	$now = date("Y-m-d H:i:s", $minuteadded);	// Adding a minute since cron's smallest unit is minute (the soonest a schedule will start is at the next minute)

	if(strtotime($SWHEN) <= $minuteadded ){$SWHEN = $now;}
	if(strtotime($EWHEN) <= strtotime($SWHEN)){exit("<script language=\"JavaScript\">alert(\"Invalid selected period!\");self.close();</script>");}

	$Sday = date('d',strtotime($SWHEN));
	$Smonth = date('m',strtotime($SWHEN));
	$Syear = date('Y',strtotime($SWHEN));
	$Shour = date('H',strtotime($SWHEN));
	$Sminute = date('i',strtotime($SWHEN));


	do{$PID = rand();}
	while(file_exists($directory."/../tmp/$PID.php"));
	
	//$schedule = $directory."/schedules/$PID.php";
	$schedule = $directory."/../tmp/$PID.php";
	
	system("echo '<?php \$table=\"$tbl\"; \$etime=\"$EWHEN\";' | cat - ".$directory."/create_schedule_template.php > ".$directory."/../tmp/scheduletmp && mv ".$directory."/../tmp/scheduletmp $schedule", $output1);
	//echo "While attempting to create collection schedule task program: ".$output1;
	//echo "<br>";
	
	//passthru("crontab -l > ".$_SERVER['DOCUMENT_ROOT']."/tmp/mytmpcron; echo \"$Sminute $Shour $Sday $Smonth * php $schedule\n\" >> ".$_SERVER['DOCUMENT_ROOT']."/tmp/mytmpcron; crontab ".$_SERVER['DOCUMENT_ROOT']."/tmp/mytmpcron;", $output2);	
	$atSWHEN = $Shour.":".$Sminute." ".$Sday.".".$Smonth.".".$Syear;
	system("echo 'php $schedule' | at $atSWHEN", $output2);
	//echo "While attempting to create scheduled collection: ".$output2;
	//echo "<br>";

	//unlink($directory."/tmp/mytmpcron");

	if (!$output1 && !$output2){
		echo "Data collection for $TBL was scheduled successfully!<br>";
		echo "Start time: ".$Syear."-".$Smonth."-".$Sday." $Shour:$Sminute<br>";
		echo "End   time: ".$Edate." $Ehour:$Eminute<br>";
		echo "You may now close this window.";
	} else{
		echo "Data collection for $TBL was not scheduled successfully.<br>";
		echo "Please close this window and retry scheduling, or if still unsuccessful, contact administrator at kfung2@u.rochester.edu";
	}
}	
echo "</html>";
?>

	</body>
</html>