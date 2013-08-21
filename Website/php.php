
<?php
  if(isset($_POST['device']) && !empty($_POST['device'])) {
    $device = $_POST['device'];
  }
  $usersFile = "tmp/".$device."_users.txt";
  $users = file_get_contents($usersFile);
  $users -= 1;
  file_put_contents($usersFile, $users);

?>