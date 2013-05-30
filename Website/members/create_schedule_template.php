//This file is the template for the PHP script that crontab will call to run a scheduled data collection.
//
//This template should be created by prepending the following line:
//<?php $table={table name}; $etime={end time};	// User defined, retrieved from frontpage POSTed to "schedule.php"

if(preg_match('/^\d{4}-\d{2}-\d{2}\s\d{1,2}:\d{1,2}:\d{1,2}$/',$etime)){
	$ETIME = strtotime($etime);
} else{
	exit("Invalid time period");
}

$directory = '/var/www/html/ece/projects/power/members';
include($directory.'/pinclude/devicelist.php');

if(in_array($table, $devices)){
	$tbl = $table;
} else{
	exit("Wrong device name");
}

include($directory.'/../device_call.php');

	while(time() <= $ETIME){
		$output = device_call("$tbl"); // "device_call" function in "device_call.php" gets data from the device and stores it into a database


		if($output != "success"){	//On success, "device_call.php" will return "success". If not, then an error occured or the
			sleep(9);
		}
	}
	
	unlink(__FILE__);
?>