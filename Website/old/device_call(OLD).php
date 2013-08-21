<?php
function device_call($device){

$directory = '/var/www/html/ece/projects/power';
include($directory.'/members/pinclude/devicelist.php');

if(in_array($device, $devices)){	//$devices is the whitelist from devicelist.php
	$include = $directory.'/members/pinclude/arduino_insert.php';
	$table = $device;
	if(file_exists($include)){
		include($include); //Defines: DBU, DBP, DBH, DBN, Arduino_IP for the specified device
	} else{ return "No record of device"; }
} else{
	return "Wrong device name";
}

if(!preg_match('/^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$/', Arduino_IP)){
	return "IP address does not match";
}

$flag = $directory.'/tmp/flags/arduino_'.$device.'.running';
$tmp_data = $directory."/tmp/tmp_".$device.".data";

	if (file_exists("$flag")){
		return "Data gathering from ".$device." is already underway";
	}
	
	$fp = fopen($flag,"w") or exit("Error in creating flag file");
	if (flock($fp, LOCK_EX)){	// Lock flag file so others can't intefer while device is being accessed
		$time_now = date("Y-m-d H:i:s");
		fwrite($fp, "$time_now");
		
		//passthru("wget --no-http-keep-alive --timeout=30 --tries=1 -O \"$tmp_data\" ".Arduino_IP);
		system("wget --no-http-keep-alive --timeout=60 --tries=1 -O \"$tmp_data\" ".Arduino_IP);	// Calling device and storing the data in $tmp_data
		
		if(!file_exists("$tmp_data")){	// Error handling for unsuccessful wget
			fclose($fp);
			unlink($flag);
			return "Unable to retrieve data from ".$device;
		}

		$conn = mysql_connect(DBH, DBU, DBP) or exit(mysql_error());
		mysql_select_db(DBN, $conn);

		$query = file_get_contents($tmp_data);

		$escaped_query = mysql_real_escape_string($query);
		
		$result = mysql_query($escaped_query, $conn); // Store retrieved data into database
		
		while(sleep(2)!=0){};	// Wait time for device to disconnect itself TODO: improve device code to lower/remove this overhead
		unlink($tmp_data);
		fclose($fp);		// Delete flag and temporary data file
		unlink($flag);
		if($result){
			return "success";
		} else{
			return "failure";
		}
	}
	else{	// Flag file was unable to be locked. Something is wrong with the flag file, requires admin attention
		fclose($fp);
		unlink($flag);
		unlink($tmp_data);
		return "Could not lock ".$device." flag file";
	}
}
?>
