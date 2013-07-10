
<?php
  $directory = getcwd();
  include($directory.'/members/pinclude/devicelist.php');

  if(isset($_POST['arduino']) && !empty($_POST['arduino'])) {
    $data = $_POST['arduino'];
  }
  $device = substr($data, 0, strpos($data, "$")); 
  $data = substr($data, strpos($data, "$"), strlen($data));
  $data = $data."\n";
  
  $arduinoFile = "tmp/".$device."_commands.txt";
//  $usersFile = "tmp/".$device."_users.txt";
//  $users = file_get_contents($usersFile);
  
//  if ($users != 0){
//    $data = "$11111111111111111111111111111111111111111111111111111111\n";
//  }
  
//  $users += 1;
  
//  file_put_contents($usersFile, $users);
  file_put_contents($arduinoFile, $data);

  
?>