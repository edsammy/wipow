<?php
// TODO: better way of displaying table/graphs? Progressively display them or have tabs?
// TODO: find another way of letting users download data and all graphs without using system zip (don't want to call the zip program on the server needlessly).

// ----From davidwalsh.name/create-zip-php  ****UNIVERSITY SERVER'S PHP DOES NOT HAVE THE ZIP EXTENSION
/* creates a compressed zip file */
/*function create_zip($files = array(),$destination = '',$overwrite = false) {
	echo $destination."<br>";
	//if the zip file already exists and overwrite is false, return false
	if(file_exists($destination) && !$overwrite) { return false; }
	//vars
	$valid_files = array();
	//if files were passed in...
	if(is_array($files)) {
		//cycle through each file
		foreach($files as $file) {
			//make sure the file exists
			if(file_exists($file)) {
				$valid_files[] = $file;
			}
		}
	}
	print_r($valid_files);
	//if we have good files...
	if(count($valid_files)) {
		echo 'we have valid files<br>';
		echo 'still alive before<br>';
		//create the archive
		$zip = new ZipArchive();
		echo 'still alive after <br>';
		if($zip->open($destination,$overwrite ? ZIPARCHIVE::OVERWRITE : ZIPARCHIVE::CREATE) !=== true) {
			echo 'cannot open zip file to write in<br>';
			return false;
		}
		//add the files
		foreach($valid_files as $file) {
			echo 'adding files to zip<br>';
			$zip->addFile($file,$file);
		}
		//debug
		echo 'The zip archive contains ',$zip->numFiles,' files with a status of ',$zip->status;
		
		//close the zip -- done!
		$zip->close();
		
		if(file_exists($destination)){echo 'true';}else{echo 'false';}
		//check to make sure the file exists
		return file_exists($destination);
	}
	else
	{
		echo 'we have no valid files<br>';
		return false;
	}
}
*/

// =====================> Section 1:
//
//   Getting POSTed data from form and checking them against a whitelist or expected-format
//
echo "<!DOCTYPE html> <head><script language='javascript' src='old.js'></script>";
echo '<link href="old.css" rel="stylesheet" type="text/css" /></head>';

if (isset($_POST['submit'])) {
	unset($_POST['submit']);
	$directory = getcwd();
	include($directory.'/pinclude/devicelist.php');
	$htmlvalues = array("on");
	
	if(isset($_REQUEST["startdate"]) && preg_match('/^\d{4}-\d{2}-\d{2}$/',$_REQUEST["startdate"])){$Sdate = $_REQUEST["startdate"];}else{exit("<script language=\"JavaScript\">alert(\"Invalid Start Date\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["SHOUR"])){$Shour = $_POST['SHOUR'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid Start Hour\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["SMINUTE"])){$Sminute = $_POST['SMINUTE'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid Start Minute\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["SSECOND"])){$Ssecond = $_POST['SSECOND'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid Start Second\");self.close();</script>");}
	if(isset($_REQUEST["enddate"]) && preg_match('/^\d{4}-\d{2}-\d{2}$/',$_REQUEST["enddate"])){$Edate = $_REQUEST["enddate"];}else{exit("<script language=\"JavaScript\">alert(\"Invalid End Date\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["EHOUR"])){$Ehour = $_POST['EHOUR'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid End Hour\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["EMINUTE"])){$Eminute = $_POST['EMINUTE'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid End Minute\");self.close();</script>");}
	if(preg_match('/^\d{2}$/',$_POST["ESECOND"])){$Esecond = $_POST['ESECOND'];}else{exit("<script language=\"JavaScript\">alert(\"Invalid End Second\");self.close();</script>");}
	if(in_array($_POST['pvolta'], $htmlvalues)){$PvoltA = $_POST['pvolta'];}
	if(in_array($_POST['pcurra'], $htmlvalues)){$PcurrA = $_POST['pcurra'];}
	if(in_array($_POST['preala'], $htmlvalues)){$PrealA = $_POST['preala'];}
	if(in_array($_POST['preaca'], $htmlvalues)){$PreacA = $_POST['preaca'];}
	if(in_array($_POST['pappaa'], $htmlvalues)){$PappaA = $_POST['pappaa'];}
	if(in_array($_POST['ppfa'], $htmlvalues)){$PpfA = $_POST['ppfa'];}
	if(in_array($_POST['tvolta'], $htmlvalues)){$TvoltA = $_POST['tvolta'];}
	if(in_array($_POST['tcurra'], $htmlvalues)){$TcurrA = $_POST['tcurra'];}
	if(in_array($_POST['treala'], $htmlvalues)){$TrealA = $_POST['treala'];}
	if(in_array($_POST['treaca'], $htmlvalues)){$TreacA = $_POST['treaca'];}
	if(in_array($_POST['tappaa'], $htmlvalues)){$TappaA = $_POST['tappaa'];}
	if(in_array($_POST['tpfa'], $htmlvalues)){$TpfA = $_POST['tpfa'];}
	if(in_array($_POST['pvoltb'], $htmlvalues)){$PvoltB = $_POST['pvoltb'];}
	if(in_array($_POST['pcurrb'], $htmlvalues)){$PcurrB = $_POST['pcurrb'];}
	if(in_array($_POST['prealb'], $htmlvalues)){$PrealB = $_POST['prealb'];}
	if(in_array($_POST['preacb'], $htmlvalues)){$PreacB = $_POST['preacb'];}
	if(in_array($_POST['pappab'], $htmlvalues)){$PappaB = $_POST['pappab'];}
	if(in_array($_POST['ppfb'], $htmlvalues)){$PpfB = $_POST['ppfb'];}
	if(in_array($_POST['tvoltb'], $htmlvalues)){$TvoltB = $_POST['tvoltb'];}
	if(in_array($_POST['tcurrb'], $htmlvalues)){$TcurrB = $_POST['tcurrb'];}
	if(in_array($_POST['trealb'], $htmlvalues)){$TrealB = $_POST['trealb'];}
	if(in_array($_POST['treacb'], $htmlvalues)){$TreacB = $_POST['treacb'];}
	if(in_array($_POST['tappab'], $htmlvalues)){$TappaB = $_POST['tappab'];}
	if(in_array($_POST['tpfb'], $htmlvalues)){$TpfB = $_POST['tpfb'];}
	if(in_array($_POST['pvoltc'], $htmlvalues)){$PvoltC = $_POST['pvoltc'];}
	if(in_array($_POST['pcurrc'], $htmlvalues)){$PcurrC = $_POST['pcurrc'];}
	if(in_array($_POST['prealc'], $htmlvalues)){$PrealC = $_POST['prealc'];}
	if(in_array($_POST['preacc'], $htmlvalues)){$PreacC = $_POST['preacc'];}
	if(in_array($_POST['pappac'], $htmlvalues)){$PappaC = $_POST['pappac'];}
	if(in_array($_POST['ppfc'], $htmlvalues)){$PpfC = $_POST['ppfc'];}
	if(in_array($_POST['tvoltc'], $htmlvalues)){$TvoltC = $_POST['tvoltc'];}
	if(in_array($_POST['tcurrc'], $htmlvalues)){$TcurrC = $_POST['tcurrc'];}
	if(in_array($_POST['trealc'], $htmlvalues)){$TrealC = $_POST['trealc'];}
	if(in_array($_POST['treacc'], $htmlvalues)){$TreacC = $_POST['treacc'];}
	if(in_array($_POST['tappac'], $htmlvalues)){$TappaC = $_POST['tappac'];}
	if(in_array($_POST['tpfc'], $htmlvalues)){$TpfC = $_POST['tpfc'];}
	if(in_array($_POST['pfreq'], $htmlvalues)){$Pfreq = $_POST['pfreq'];}
	if(in_array($_POST['ppft'], $htmlvalues)){$PpfT = $_POST['ppft'];}
	if(in_array($_POST['pcurrn'], $htmlvalues)){$PcurrN = $_POST['pcurrn'];}
	if(in_array($_POST['pphaseab'], $htmlvalues)){$PphaseAB = $_POST['pphaseab'];}
	if(in_array($_POST['pphaseac'], $htmlvalues)){$PphaseAC = $_POST['pphaseac'];}
	if(in_array($_POST['tfreq'], $htmlvalues)){$Tfreq = $_POST['tfreq'];}
	if(in_array($_POST['tpft'], $htmlvalues)){$TpfT = $_POST['tpft'];}
	if(in_array($_POST['tcurrn'], $htmlvalues)){$TcurrN = $_POST['tcurrn'];}
	if(in_array($_POST['tphaseab'], $htmlvalues)){$TphaseAB = $_POST['tphaseab'];}
	if(in_array($_POST['tphaseac'], $htmlvalues)){$TphaseAC = $_POST['tphaseac'];}
	if(in_array($_POST['pvha'], $htmlvalues)){$PvhA = $_POST['pvha'];}
	if(in_array($_POST['pcha'], $htmlvalues)){$PchA = $_POST['pcha'];}
	if(in_array($_POST['pthda'], $htmlvalues)){$PthdA = $_POST['pthda'];}
	if(in_array($_POST['tvha'], $htmlvalues)){$TvhA = $_POST['tvha'];}
	if(in_array($_POST['tcha'], $htmlvalues)){$TchA = $_POST['tcha'];}
	if(in_array($_POST['tthda'], $htmlvalues)){$TthdA = $_POST['tthda'];}
	if(preg_match('/^\d{1,2}$/',$_POST["vha"])){$NvhA = $_POST['vha'];}
	if(preg_match('/^\d{1,2}$/',$_POST["cha"])){$NchA = $_POST['cha'];}
	if(in_array($_POST['pvhb'], $htmlvalues)){$PvhB = $_POST['pvhb'];}
	if(in_array($_POST['pchb'], $htmlvalues)){$PchB = $_POST['pchb'];}
	if(in_array($_POST['pthdb'], $htmlvalues)){$PthdB = $_POST['pthdb'];}
	if(in_array($_POST['tvhb'], $htmlvalues)){$TvhB = $_POST['tvhb'];}
	if(in_array($_POST['tchb'], $htmlvalues)){$TchB = $_POST['tchb'];}
	if(in_array($_POST['tthdb'], $htmlvalues)){$TthdB = $_POST['tthdb'];}
	if(preg_match('/^\d{1,2}$/',$_POST["vhb"])){$NvhB = $_POST['vhb'];}
	if(preg_match('/^\d{1,2}$/',$_POST["chb"])){$NchB = $_POST['chb'];}
	if(in_array($_POST['pvhc'], $htmlvalues)){$PvhC = $_POST['pvhc'];}
	if(in_array($_POST['pchc'], $htmlvalues)){$PchC = $_POST['pchc'];}
	if(in_array($_POST['pthdc'], $htmlvalues)){$PthdC = $_POST['pthdc'];}
	if(in_array($_POST['tvhc'], $htmlvalues)){$TvhC = $_POST['tvhc'];}
	if(in_array($_POST['tchc'], $htmlvalues)){$TchC = $_POST['tchc'];}
	if(in_array($_POST['tthdc'], $htmlvalues)){$TthdC = $_POST['tthdc'];}
	if(preg_match('/^\d{1,2}$/',$_POST["vhc"])){$NvhC = $_POST['vhc'];}
	if(preg_match('/^\d{1,2}$/',$_POST["chc"])){$NchC = $_POST['chc'];}
	$TBL = $_POST['SITE'];
	$tbl_posted = strtolower($TBL);

	if(in_array($tbl_posted, $devices)){
		$tbl = $tbl_posted;
		$table = $tbl_posted;
	} else{
		exit("Wrong device name");
	}
	$Shour = $Shour *1;
	$Sminute = $Sminute *1;
	$Ssecond = $Ssecond *1;
	$Ehour = $Ehour *1;
	$Eminute = $Eminute *1;
	$Esecond = $Esecond *1;
	$SWHEN = "$Sdate $Shour:$Sminute:$Ssecond";
	$EWHEN = "$Edate $Ehour:$Eminute:$Esecond";
	//echo $SWHEN."<br>";
	//echo $EWHEN."<br>";
	
	if(strtotime($SWHEN)>strtotime($EWHEN)){die("<script language=\"JavaScript\">alert(\"Invalid time period!\");self.close();</script>");}
	
// ==================> SECTION 2: 
//
//  Temporary data files setup.
// $DNAME is the record file (for download)
// $FNAME is the file that has the formatted data for gnuplot 
//
	//$recordtime = date("y\y.m\m.d\d.H\h.i\m.s\s");
	if(!preg_match('/^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$/', $_SERVER['REMOTE_ADDR'])){
		die("<script language=\"JavaScript\">alert(\"Invalid IP address!\");self.close();</script>");
	}
	$user_ip = $_SERVER['REMOTE_ADDR'];
	do{$PID = rand();}
	while(file_exists($directory."/tmp/$user_ip.$PID.record") or file_exists($directory."/tmp/$user_ip.$PID.dat") );
	
	$DNAME = $directory."/../tmp/$user_ip.$PID.record";
	$FNAME = $directory."/../tmp/$user_ip.$PID.dat";
	//$files_to_zip = array();
	
// ==================> SECTION 3: 
//
//  MySQL query strings and HTML tabular display buildup.
//	
	$Tsql = null;	// MySQl query string for table results
	$Psql = null;	// MySQl query string for plots results
	
	$tablestring = null;	// HTML string for the table
	$recstring = null;	// Data record file string
	$plot_vars = array();	// Array to store what plots to display
	
	if($Pfreq === "on"){$Psql = $Psql.",freq"; $plot_vars[]="Frequency";}		if($Tfreq === "on"){$Tsql = $Tsql.",freq"; $tablestring = $tablestring."<th>Frequency</th>"; $recstring = $recstring."Frequency\t";}
	if($PpfT === "on"){$Psql = $Psql.",pft"; $plot_vars[]="Total Power Factor";}		if($TpfT === "on"){$Tsql = $Tsql.",pft"; $tablestring = $tablestring. "<th>Total Power Factor</th>"; $recstring = $recstring."Total Power Factor\t";}
	if($PcurrN === "on"){$Psql = $Psql.",currn"; $plot_vars[]="Neutral Current";}	if($TcurrN === "on"){$Tsql = $Tsql.",currn"; $tablestring = $tablestring. "<th>Neutral Current</th>"; $recstring = $recstring."Neutral Current\t";}
	if($PphaseAB === "on"){$Psql = $Psql.",phaseab"; $plot_vars[]="Phasor Angle B to A";}if($TphaseAB === "on"){$Tsql = $Tsql.",phaseab"; $tablestring = $tablestring. "<th>Phasor Angle B to A</th>"; $recstring = $recstring."Phasor Angle B to A\t";}
	if($PphaseAC === "on"){$Psql = $Psql.",phaseac"; $plot_vars[]="Phasor Angle C to A";}if($TphaseAC === "on"){$Tsql = $Tsql.",phaseac"; $tablestring = $tablestring. "<th>Phasor Angle C to A</th>"; $recstring = $recstring."Phasor Angle C to A\t";}
	if($PvoltA === "on"){$Psql = $Psql.",volta"; $plot_vars[]="Voltage A";} 	if($TvoltA === "on"){$Tsql = $Tsql.",volta"; $tablestring = $tablestring. "<th>Voltage A</th>"; $recstring = $recstring."Voltage A\t";}
	if($PcurrA === "on"){$Psql = $Psql.",curra"; $plot_vars[]="Current A";} 	if($TcurrA === "on"){$Tsql = $Tsql.",curra"; $tablestring = $tablestring. "<th>Current A</th>"; $recstring = $recstring."Current A\t";}
	if($PrealA === "on"){$Psql = $Psql.",reala"; $plot_vars[]="Real Power A";} 	if($TrealA === "on"){$Tsql = $Tsql.",reala"; $tablestring = $tablestring. "<th>Real Power A</th>"; $recstring = $recstring."Real Power A\t";}
	if($PreacA === "on"){$Psql = $Psql.",reaca"; $plot_vars[]="Reactive Power A";} 	if($TreacA === "on"){$Tsql = $Tsql.",reaca"; $tablestring = $tablestring. "<th>Reactive Power A</th>"; $recstring = $recstring."Reactive Power A\t";}
	if($PappaA === "on"){$Psql = $Psql.",appaa"; $plot_vars[]="Apparent Power A";} 	if($TappaA === "on"){$Tsql = $Tsql.",appaa"; $tablestring = $tablestring. "<th>Apparent Power A</th>"; $recstring = $recstring."Apparent Power A\t";}
	if($PpfA === "on"){$Psql = $Psql.",pfa"; $plot_vars[]="Power Factor A";} 		if($TpfA === "on"){$Tsql = $Tsql.",pfa"; $tablestring = $tablestring. "<th>Power Factor A</th>"; $recstring = $recstring."Power Factor A\t";}
	if($PvoltB === "on"){$Psql = $Psql.",voltb"; $plot_vars[]="Voltage B";} 	if($TvoltB === "on"){$Tsql = $Tsql.",voltb"; $tablestring = $tablestring. "<th>Voltage B</th>"; $recstring = $recstring."Voltage B\t";}
	if($PcurrB === "on"){$Psql = $Psql.",currb"; $plot_vars[]="Current B";} 	if($TcurrB === "on"){$Tsql = $Tsql.",currb"; $tablestring = $tablestring. "<th>Current B</th>"; $recstring = $recstring."Current B\t";}
	if($PrealB === "on"){$Psql = $Psql.",realb"; $plot_vars[]="Real Power B";} 	if($TrealB === "on"){$Tsql = $Tsql.",realb"; $tablestring = $tablestring. "<th>Real Power B</th>"; $recstring = $recstring."Real Power B\t";}
	if($PreacB === "on"){$Psql = $Psql.",reacb"; $plot_vars[]="Reactive Power B";} 	if($TreacB === "on"){$Tsql = $Tsql.",reacb"; $tablestring = $tablestring. "<th>Reactive Power B</th>"; $recstring = $recstring."Reactive Power B\t";}
	if($PappaB === "on"){$Psql = $Psql.",appab"; $plot_vars[]="Apparent Power B";} 	if($TappaB === "on"){$Tsql = $Tsql.",appab"; $tablestring = $tablestring. "<th>Apparent Power B</th>"; $recstring = $recstring."Apparent Power B\t";}
	if($PpfB === "on"){$Psql = $Psql.",pfb"; $plot_vars[]="Power Factor B";} 		if($TpfB === "on"){$Tsql = $Tsql.",pfb"; $tablestring = $tablestring. "<th>Power Factor B</th>"; $recstring = $recstring."Power Factor B\t";}
	if($PvoltC === "on"){$Psql = $Psql.",voltc"; $plot_vars[]="Voltage C";} 	if($TvoltC === "on"){$Tsql = $Tsql.",voltc"; $tablestring = $tablestring. "<th>Voltage C</th>"; $recstring = $recstring."Voltage C\t";}
	if($PcurrC === "on"){$Psql = $Psql.",currc"; $plot_vars[]="Current C";} 	if($TcurrC === "on"){$Tsql = $Tsql.",currc"; $tablestring = $tablestring. "<th>Current C</th>"; $recstring = $recstring."Current C\t";}
	if($PrealC === "on"){$Psql = $Psql.",realc"; $plot_vars[]="Real Power C";} 	if($TrealC === "on"){$Tsql = $Tsql.",realc"; $tablestring = $tablestring. "<th>Real Power C</th>"; $recstring = $recstring."Real Power C\t";}
	if($PreacC === "on"){$Psql = $Psql.",reacc"; $plot_vars[]="Reactive Power C";} 	if($TreacC === "on"){$Tsql = $Tsql.",reacc"; $tablestring = $tablestring. "<th>Reactive Power C</th>"; $recstring = $recstring."Reactive Power C\t";}
	if($PappaC === "on"){$Psql = $Psql.",appac"; $plot_vars[]="Apparent Power C";} 	if($TappaC === "on"){$Tsql = $Tsql.",appac"; $tablestring = $tablestring. "<th>Apparent Power C</th>"; $recstring = $recstring."Apparent Power C\t";}
	if($PpfC === "on"){$Psql = $Psql.",pfc"; $plot_vars[]="Power Factor C";} 		if($TpfC === "on"){$Tsql = $Tsql.",pfc"; $tablestring = $tablestring. "<th>Power Factor C</th>"; $recstring = $recstring."Power Factor C\t";}
	if($PvhA === "on"){for($i=1; $i<=$NvhA; $i++){$Psql = $Psql.",vha$i"; $plot_vars[]="Voltage A Harmonic $i";}}	if($TvhA === "on"){for($i=1; $i<=$NvhA; $i++){$Tsql = $Tsql.",vha$i"; $tablestring = $tablestring. "<th>Voltage A Harmonic $i</th>"; $recstring = $recstring."Voltage A Harmonic $i\t";}}
	if($PchA === "on"){for($i=1; $i<=$NchA; $i++){$Psql = $Psql.",cha$i"; $plot_vars[]="Current A Harmonic $i";}}	if($TchA === "on"){for($i=1; $i<=$NchA; $i++){$Tsql = $Tsql.",cha$i"; $tablestring = $tablestring. "<th>Current A Harmonic $i</th>"; $recstring = $recstring."Current A Harmonic $i\t";}}
	if($PthdA === "on"){$Psql = $Psql.",thda"; $plot_vars[]="Current A Total Harmonic Distortion + Noise";} 		if($TthdA === "on"){$Tsql = $Tsql.",thda"; $tablestring = $tablestring. "<th>Current A THD+N</th>"; $recstring = $recstring."Current A Total Harmonic Distortion + Noise\t";}
	if($PvhB === "on"){for($i=1; $i<=$NvhB; $i++){$Psql = $Psql.",vhb$i"; $plot_vars[]="Voltage B Harmonic $i";}}	if($TvhB === "on"){for($i=1; $i<=$NvhB; $i++){$Tsql = $Tsql.",vhb$i"; $tablestring = $tablestring. "<th>Voltage B Harmonic $i</th>"; $recstring = $recstring."Voltage B Harmonic $i\t";}}
	if($PchB === "on"){for($i=1; $i<=$NchB; $i++){$Psql = $Psql.",chb$i"; $plot_vars[]="Current B Harmonic $i";}}	if($TchB === "on"){for($i=1; $i<=$NchB; $i++){$Tsql = $Tsql.",chb$i"; $tablestring = $tablestring. "<th>Current B Harmonic $i</th>"; $recstring = $recstring."Current B Harmonic $i\t";}}
	if($PthdB === "on"){$Psql = $Psql.",thdb"; $plot_vars[]="Current B Total Harmonic Distortion + Noise";} 		if($TthdB === "on"){$Tsql = $Tsql.",thdb"; $tablestring = $tablestring. "<th>Current B THD+N</th>"; $recstring = $recstring."Current B Total Harmonic Distortion + Noise\t";}
	if($PvhC === "on"){for($i=1; $i<=$NvhC; $i++){$Psql = $Psql.",vhc$i"; $plot_vars[]="Voltage C Harmonic $i";}}	if($TvhC === "on"){for($i=1; $i<=$NvhC; $i++){$Tsql = $Tsql.",vhc$i"; $tablestring = $tablestring. "<th>Voltage C Harmonic $i</th>"; $recstring = $recstring."Voltage C Harmonic $i\t";}}
	if($PchC === "on"){for($i=1; $i<=$NchC; $i++){$Psql = $Psql.",chc$i"; $plot_vars[]="Current C Harmonic $i";}}	if($TchC === "on"){for($i=1; $i<=$NchC; $i++){$Tsql = $Tsql.",chc$i"; $tablestring = $tablestring. "<th>Current C Harmonic $i</th>"; $recstring = $recstring."Current C Harmonic $i\t";}}
	if($PthdC === "on"){$Psql = $Psql.",thdc"; $plot_vars[]="Current C Total Harmonic Distortion + Noise";} 		if($TthdC === "on"){$Tsql = $Tsql.",thdc"; $tablestring = $tablestring. "<th>Current C THD+N</th>"; $recstring = $recstring."Current C Total Harmonic Distortion + Noise\t";}
	
	
// ==================> SECTION 4: 
//
//  Processing and preparing the final query strings and results page format.
//	
	if($Psql === null && $Tsql === null){die("<script language=\"JavaScript\">alert(\"No measurement selected!\");self.close();</script>");}

	$showplot = false;
	$showtabale = false;
	
	if($Tsql != null){
		$Tsql = "SELECT recorded$Tsql FROM $tbl";
		$showtable = true; 
		echo "<input id=\"tabletogglebutton\" type=\"button\" onclick=\"toggletable()\" value=\"Hide Table Results (plots are at the bottom)\" />&nbsp;&nbsp;&nbsp;".PHP_EOL;  // Hide/show table button
		echo "<a href='../tmp/$user_ip.$PID.record'>Download the requested table data here (right click-> save as)</a>";
		//echo '<input type="button" value="Download all requested data" onClick="window.location.href=\'tmp/'.$user_ip.'.'.$PID.'.zip\'"/>'; // Download button
		echo "<div id=\"tableresult\"><table border=1><thead><tr><th>Date<br>Time</th>".$tablestring."</tr></thead><tbody>"; // Start of table
		$recstring = "Index\tTime Recorded\t".$recstring."\n";
		$fp = fopen("$DNAME","w") or die("<script language=\"JavaScript\">alert(\"Error in managing record data file\");self.close();</script>");
		fwrite( $fp, "$recstring");
	}
	
	if($Psql != null){
		$Psql = "SELECT recorded$Psql FROM $tbl";
		$showplot = true;
	}
	
	if ( $SWHEN === $EWHEN ){
		$Psql = "$Psql ORDER BY recorded;";
		$Tsql = "$Tsql ORDER BY recorded DESC;";
	}
	else{
		$Psql = "$Psql WHERE recorded >= '$SWHEN' AND recorded <= '$EWHEN' ORDER BY recorded;";
		$Tsql = "$Tsql WHERE recorded >= '$SWHEN' AND recorded <= '$EWHEN' ORDER BY recorded DESC;";
	}
	
	//echo "Plots:<pre>".$Psql."</pre>";	//Debug lines (print out final MYSQL query statements.)
	//echo "Table:<pre>".$Tsql."</pre>";
	
// ==================> SECTION 5: 
//
//  Connect to and query the database.
//	
	include($directory.'/pinclude/arduino_read.php');
	$conn = mysql_connect(DBH, DBU, DBP) or die("<script language=\"JavaScript\">alert(\"Could Not Connect to Database: ".mysql_error()."\");self.close();</script>");
	mysql_select_db(DBN, $conn);

	
	if($showtable === true){
		$Tresult = mysql_query($Tsql, $conn) or die("<script language=\"JavaScript\">alert(\"Error in MYSQL query for table: ".mysql_error()."\");self.close();</script>");
		$numrecs = 0;
		while ($row = mysql_fetch_row($Tresult)){    
			$numrecs += 1;
			fwrite( $fp, "$numrecs\t");
			echo "<tr>\n";
			foreach ($row as $val) {	// Building the html table as well as the record file that can be downloaded ($DNAME).
				echo "<td>$val</td>\n";
				if ($val == null){fwrite( $fp, "NaN\t");}
				else{fwrite( $fp, "$val\t");}
			}
			echo "</tr>\n";
			fwrite ( $fp, "\n");
		}
		fclose($fp);
		echo '</tbody></table></div>';
	}
	
	if($showplot === true){
		$Presult = mysql_query($Psql, $conn) or die("<script language=\"JavaScript\">alert(\"Error in MYSQL query for plots: ".mysql_error()."\");self.close();</script>");
		$fp = fopen("$FNAME","w") or die("<script language=\"JavaScript\">alert(\"Error in managing plot data file\");self.close();</script>");
		$numrecs = 0;
		$firstdate = null;
		$lastdate = null;
		if ($row = mysql_fetch_row($Presult)){
			$firstdate = $row[0];
			do{					// Building the gnuplot data into the file named $FNAME
				$numrecs += 1;
				fwrite( $fp, "$numrecs ");
				foreach ($row as $val) {
					if ($val == null){fwrite( $fp, "NaN ");}
					else{fwrite( $fp, "$val ");}
				}
				$lastdate = $row[0];
				fwrite ( $fp, "\n");
			}while ($row = mysql_fetch_row($Presult));
		}
		fclose($fp);
		
		$PNAME = $directory."/../tmp/$user_ip.$PID.gnuplot"; // This file is the gnuplot script file
		
		$xtic = (strtotime($lastdate) - strtotime($firstdate))/5;
		$fp = fopen("$PNAME","w") or die("<script language=\"JavaScript\">alert(\"Error in managing gnuplot file\");self.close();</script>");
		fwrite($fp,"set terminal png medium size 1000 400\n");
		fwrite($fp,"set datafile missing 'NaN'\n");
		fwrite($fp,"set xdata time\n");
		fwrite($fp,"set timefmt \"%Y-%m-%d %H:%M:%S\"\n");
		fwrite($fp,"set format x \"%m/%d/%y\\n%H:%M:%S\"\n");
		//fwrite($fp,"set xrange [\"$firstdate\":\"$lastdate\"]\n");
		fwrite($fp,"set xtics $xtic\n");
		foreach($plot_vars as $i => $var){
			if ( $SWHEN != $EWHEN ){
				fwrite($fp,"set title \"Arduino Data for $var at $TBL from $SWHEN to $EWHEN\"\n");
			}
			else{
				fwrite($fp,"set title \"Arduino Data for $var at $TBL\"\n");
			}
			fwrite($fp,"set output \"../tmp/$user_ip.$PID.$var.png\"\n");
			$col = $i + 4;
			fwrite($fp,"plot \"$FNAME\" using 2:$col title '$var' with linespoints\n");
		}
		fwrite($fp,"quit\n");
		fclose($fp);

		echo "<br>";

		ob_start();
		system("gnuplot '$PNAME' 2>&1",$output);
		ob_end_clean();

		unlink($FNAME);
		unlink($PNAME);

	//	chdir('tmp');
	//        system("zip $user_ip.$PID.zip *$PID*"); // Zipping up $DNAME record file and all plots for download
       	//	ob_end_clean();

	//	chdir($directory);
		//$files_to_zip[]=$DNAME;

		foreach($plot_vars as $var){ // HTML for displaying the plots
			//$files_to_zip[]= $directory."/tmp/$user_ip.$PID.$var.png";
			echo "<br>";
			echo "<IMG SRC=\"http://www.ece.rochester.edu/projects/power/tmp/$user_ip.$PID.$var.png\" align=center alt=\"$var Data Plot\">";
		}
	}

	mysql_close($conn);

	//print_r($files_to_zip);

	//$zip_result = create_zip($files_to_zip, $directory.'/tmp/'.$user_ip.'.'.$PID.'.zip');

	//if(!$zip_result){echo "download file not here"; echo "<script language=\"JavaScript\">alert(\"Error in creating downloadable data file\");</script>";}

}

	echo "</html>";
?>
