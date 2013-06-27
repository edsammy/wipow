<?php
  // Array of recognized device names
  $directory = '/var/www/html/ece/projects/power';
  include($directory.'/members/pinclude/devicelist.php');

  // Read and parse the incoming csv data
  $device = $_GET["name"];
  $data = $_GET["data"];
  $delim = ",";
  
  if(in_array($device, $devices)){
    $table = $device;
    include($directory.'/members/pinclude/arduino_insert.php');
  }
  else {
    echo "Specified device not found!";
  }
  
  // Connect to the DB
  $conn = mysql_connect(DBH, DBU, DBP) or die("error connecting to database: ".mysql_error());
	mysql_select_db(DBN, $conn);
  $query = "insert into $table (freq,volta,voltb,voltc,curra,currb,currc,currn,reala,realb,realc,reaca,reacb,reacc,appaa,appab,appac,pfa,pfb,pfc,pft,phaseab,phaseac,thda,cha1,cha2,cha3,cha4,cha5,vha1,vha2,vha3,vha4,vha5,thdb,chb1,chb2,chb3,chb4,chb5,vhb1,vhb2,vhb3,vhb4,vhb5,thdc,chc1,chc2,chc3,chc4,chc5,vhc1,vhc2,vhc3,vhc4,vhc5) values($data);";
	$escaped_query = mysql_real_escape_string($query);	
	$result = mysql_query($escaped_query, $conn); // Store retrieved data into database
  // Confirm the data has been entered in DB
  if($result){
    echo "OK";
  } 
  else {
    echo "ERROR";
  }
  
?>