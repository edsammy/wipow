<?php

class simpleCMS{
var $host;
var $username;
var $password;
var $table;

public function display_public(){}
public function display_admin(){
return <<<ADMIN_FORM

}
public function write(){}
public function connect(){

mysql_connect($this->host,$this->username,$this->password) or die('could not connect. ' . mysql_error());
mysql_select_db($this->table)or die("could not select database. " . mysql_error());}


private function buildDB(){
$sql = <<<MySQL_QUERY
	CREATE TABLE IF NOT EXISTS testDB(
	title VARCHAR(150),
	bodytext TEXT,
	created VARCHAR(100)
	)
	MySQL_QUERY;
	
	return mysql_query($sql);

}


}
?>