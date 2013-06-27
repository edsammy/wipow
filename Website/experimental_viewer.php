<?php
  $table = "cogen";
  $directory = getcwd();
  include($directory.'/members/pinclude/arduino_read.php');	// Defines: DBH, DBU, DBP, DBN. For reading only.
  
  $now = date("Y-m-d H:i:s");
  $conn = mysql_connect(DBH, DBU, DBP) or die("error connecting to database: ".mysql_error());
  mysql_select_db(DBN, $conn);

  $tbl=mysql_real_escape_string($tbl);
  // Select the latest data (where timestamp is more recent than the time when this script is called)
  $query = "SELECT * FROM cogen;";


  $result = mysql_query($query, $conn) or die("error2");

  if (!mysql_num_rows($result)){echo "error"; exit("error3");}

  ob_clean();

  // echo data out for "live.php"
  while ($row = mysql_fetch_assoc($result)){
    foreach ($row as $name => $val) {
      if ($val == null){echo $name.'|NA!';}
      else{echo $val.',';}
    }	
  }
 ?>