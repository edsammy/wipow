<?php
  $myFile = "commands.txt";
  if(isset($_POST['arduino']) && !empty($_POST['arduino'])) {
    $data = $_POST['arduino'];
  }
  if(strlen($data) == 57){
    file_put_contents($myFile, $data);
  }
  else{
    file_put_contents($myFile, $data);
  }
  
?>