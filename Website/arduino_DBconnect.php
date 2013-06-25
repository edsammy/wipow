<?php
  // Read and parse the incoming csv data
  $arduinoName = $_GET["name"];
  $arduinoData = $_GET["data"];
  $delim = ",";
  //echo $arduinoData;
  $dataParsed = explode($delim, $arduinoData);
  
  // Connect to the DB
  $directory = getcwd();
  //include($directory.'/members/pinclude/arduino_insert.php');
  //$conn = mysql_connect(DBH, DBU, DBP) or exit(mysql_error());
	//mysql_select_db(DBN, $conn);
  echo arudinoName;
  echo arduinoData;
  
?>