<?php
  $myFile = "test.txt";
  $data = "Arduino has landed!";
  echo file_put_contents($myFile, $data);
  
?>