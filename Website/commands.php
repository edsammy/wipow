<?php
  $directory = getcwd();
  include($directory.'/members/pinclude/devicelist.php');

  if(isset($_POST['arduino']) && !empty($_POST['arduino'])) {
    $data = $_POST['arduino'];
  }
  $device = substr($data, 0, strpos($data, "$")); 
  $data = substr($data, strpos($data, "$"), strlen($data));
  $data = $data."\n";
  $myFile = "tmp/".$device."_commands.txt";
  file_put_contents($myFile, $data);
?>