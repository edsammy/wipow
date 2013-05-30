<?php
// This script is called by "live.php" in an ajax call.
// Data is returned in the format: name|value!name|value...

// TODO: Better error handling: use ajax/http errors
$directory = getcwd();
$now = date("Y-m-d H:i:s");
include($directory.'/members/pinclude/devicelist.php');
	
$tbl_posted = $_POST['tbl'];	// Specifies the device

if(in_array($tbl_posted, $devices)){
	$tbl = $tbl_posted;
	$table = $tbl_posted;
} else{
	exit("Wrong device name");
}

include($directory.'/device_call.php');
$output = device_call("$tbl"); // "device_call" function in "device_call.php" gets data from the device and stores it into a database

if($output != "success"){
	sleep(5);	// "device_call.php" returns success if no fatal error occurs (data is not necessarily retrieved successfully)
}			// If not successful, either device is in use by something else or fatal error. If in use, device takes
			// ~4sec (no harmonics) or ~30sec (harmonics) to finish.


//
//  Connect to and query the database for the latest data.
//	
include($directory.'/members/pinclude/arduino_read.php');	// Defines: DBH, DBU, DBP, DBN. For reading only.

if(!preg_match('/^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$/', Arduino_IP)){
	exit("IP address does not matched".Arduino_IP);
}

$conn = mysql_connect(DBH, DBU, DBP) or die("error connecting to database: ".mysql_error());
mysql_select_db(DBN, $conn);

$tbl=mysql_real_escape_string($tbl);
// Select the latest data (where timestamp is more recent than the time when this script is called)
$query = "SELECT * FROM $tbl WHERE recorded >= '$now' ORDER BY recorded DESC LIMIT 1 OFFSET 0;";


$result = mysql_query($query, $conn) or die("error2");

if (!mysql_num_rows($result)){echo "error"; exit("error3");}

ob_clean();

// echo data out for "live.php"
while ($row = mysql_fetch_assoc($result)){
	foreach ($row as $name => $val) {
		if ($val == null){echo $name.'|NA!';}
		else{echo $name.'|'.$val.'!';}
	}	
}
?>
