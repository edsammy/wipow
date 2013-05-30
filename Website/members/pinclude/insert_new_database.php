<?php
$table = "cogen";
include("./arduino_insert.php");

$conn = mysql_connect(DBH, DBU, DBP) or exit(mysql_error());
mysql_select_db(DBN, $conn);

$insert_query = 'create table cogen (recorded timestamp not null, temp decimal(7,3), freq decimal(7,3), volta decimal(8,3), voltb decimal(8,3), voltc decimal(8,3), curra decimal(8,3), currb decimal(8,3), currc decimal(8,3), currn decimal(8,3), reala decimal(8,3), realb decimal(8,3), realc decimal(8,3), reaca decimal(8,3), reacb decimal(8,3), reacc decimal(8,3), appaa decimal(8,3), appab decimal(8,3), appac decimal(8,3), pfa decimal(4,3), pfb decimal(4,3), pfc decimal(4,3), pft decimal(4,3), phaseab  decimal(6,3), phaseac decimal(6,3), thda decimal(8,3), cha1 decimal(8,3), cha2 decimal(8,3), cha3 decimal(8,3), cha4 decimal(8,3), cha5 decimal(8,3), cha6 decimal(8,3), cha7 decimal(8,3), cha8 decimal(8,3), cha9 decimal(8,3), cha10 decimal(8,3), cha11 decimal(8,3), vha1 decimal(8,3), vha2 decimal(8,3), vha3 decimal(8,3), vha4 decimal(8,3), vha5 decimal(8,3), vha6 decimal(8,3), vha7 decimal(8,3), vha8 decimal(8,3), vha9 decimal(8,3), vha10 decimal(8,3), vha11 decimal(8,3), thdb decimal(8,3), chb1 decimal(8,3), chb2 decimal(8,3), chb3 decimal(8,3), chb4 decimal(8,3), chb5 decimal(8,3), chb6 decimal(8,3), chb7 decimal(8,3), chb8 decimal(8,3), chb9 decimal(8,3), chb10 decimal(8,3), chb11 decimal(8,3), vhb1 decimal(8,3), vhb2 decimal(8,3), vhb3 decimal(8,3), vhb4 decimal(8,3), vhb5 decimal(8,3), vhb6 decimal(8,3), vhb7 decimal(8,3), vhb8 decimal(8,3), vhb9 decimal(8,3), vhb10 decimal(8,3), vhb11 decimal(8,3), thdc decimal(8,3), chc1 decimal(8,3), chc2 decimal(8,3), chc3 decimal(8,3), chc4 decimal(8,3), chc5 decimal(8,3), chc6 decimal(8,3), chc7 decimal(8,3), chc8 decimal(8,3), chc9 decimal(8,3), chc10 decimal(8,3), chc11 decimal(8,3), vhc1 decimal(8,3), vhc2 decimal(8,3), vhc3 decimal(8,3), vhc4 decimal(8,3), vhc5 decimal(8,3), vhc6 decimal(8,3), vhc7 decimal(8,3), vhc8 decimal(8,3), vhc9 decimal(8,3), vhc10 decimal(8,3), vhc11 decimal(8,3), primary key(recorded));';

$result = mysql_query($insert_query, $conn); 
echo $result;
?>
