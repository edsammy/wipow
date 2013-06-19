<?php
include('../calendar/classes/tc_calendar.php');	// Required for the interactive Calendar.
						// TODO: change this to use jquery? Something else that also has time?
?>
<!DOCTYPE html> 
<html>
	<head>
		<title>Wireless Power Monitor version 1.0</title>
		<link href="../calendar/calendar.css" rel="stylesheet" type="text/css" />
		<link rel="shortcut icon" href="../powericon.ico">
		<link href="index.css" rel="stylesheet" type="text/css" />
		<script language="javascript" src="../calendar/calendar.js"></script>
		<script language="javascript" src="index.js"></script>
		
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
	</head>
	
	<body>
		<h1>&nbsp;&nbsp;&nbsp;&nbsp;Data Display From Wireless Power Monitor Devices&nbsp;</h1>
				
		<form id="bigform" name="myform" onsubmit="return OnSubmitForm();" method="post" target="_blank">
		<div id="deviceselect">
			<a href="../definitions.html" target="_new" onclick="javascript:void window.open('../definitions.html','1371063848022','width=700,height=320,toolbar=0,menubar=0,location=0,status=0,scrollbars=0,resizable=1,left=0,top=0');return false;">Click here to see definitions for each measurement</a><br><br>
			<b>Please select device to monitor:</b>
			<select name="SITE" size=1>
				<option>Hopeman</option>
				<option>Cogen</option>
			</select>
		</div>
		
		
		<div class="viewchoice">
		<fieldset>
			<legend id="bigboxtitle"><b>Would you like to view LIVE data or view RECORDED data?</b></legend>
			<input type="submit" name="submit" onclick="document.pressed=this.value" value="Go Live!" /><em class="warn">&nbsp;&nbsp;* Live display shows all values in tabular form by default.</em>
			<br>
			&nbsp;&nbsp;or
			<br>
			<input type="button" onclick="showtimefield()" value="View recorded data" /><em>&nbsp;&nbsp;* Will have to select time period.</em>
			<br>
			&nbsp;&nbsp;or
			<br>
			<input type="button" onclick="showtimefield2()" value="Schedule Data Collection" /><em>&nbsp;&nbsp;* Will have to select time period.</em>
		</fieldset>
		</div>
		
		
		<div id="timefield">
			<img id="arrows" border="0" src="../Down_Arrow_Icon.png" width="30" height="30">
			<fieldset>
			<legend id="bigboxtitle"><b>Please enter the time period of the recorded data you want to view:</b><br><em>* To select all data in database, keep start and end times equal (default).</em></legend>
			<table>
					<tr><td></td><td></td><td class="centered">Hour</td><td class="centered">Minute</td><td class="centered">Second</td></tr>
					<tr><td nowrap>Start Date/Time: </td>
						<td>
							<?php
							  $myCalendar = new tc_calendar("startdate", true);
							  $myCalendar->setIcon("../calendar/images/iconCalendar.gif");
							  $myCalendar->setDate(date('d'), date('m'), date('Y'));
							  $myCalendar->setPath("../calendar/");
							  $myCalendar->setYearInterval(2012, 2015);
							  $myCalendar->dateAllow('2012-01-01', date('Y').'-'.date('m').'-'.date('d'));
							  $myCalendar->setOnChange("myChanged('test')");
							  $myCalendar->writeScript();
							?>
						</td>
						<td>
							<select name='SHOUR' size=1>
							<?php $hournow = date('G');
									for($i=0; $i<24; $i++){ 
										if($i == $hournow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
						</td>
						<td>
							<select name='SMINUTE' size=1>
							<?php $minnow = date('i');
									for($i=0; $i<60; $i++){ 
										if($i == $minnow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
							</td>
						<td>
							<select name='SSECOND' size=1>
							<?php $secnow = date('s');
									for($i=0; $i<60; $i++){ 
										if($i == $secnow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
						</td>
					</tr>
					
					<tr><td nowrap>End Date/Time: </td>
						<td>
							<?php
							  $myCalendar = new tc_calendar("enddate", true);
							  $myCalendar->setIcon("../calendar/images/iconCalendar.gif");
							  $myCalendar->setDate(date('d'), date('m'), date('Y'));
							  $myCalendar->setPath("../calendar/");
							  $myCalendar->setYearInterval(2012, 2015);
							  $myCalendar->dateAllow('2012-01-01', date('Y').'-'.date('m').'-'.date('d'));
							  $myCalendar->setOnChange("myChanged('test')");
							  $myCalendar->writeScript();
							?>
						</td>
						<td>
							<select name='EHOUR' size=1>
							<?php $hournow = date('G');
									for($i=0; $i<24; $i++){ 
										if($i == $hournow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
						</td>
						<td>
							<select name='EMINUTE' size=1>
							<?php $minnow = date('i');
									for($i=0; $i<60; $i++){ 
										if($i == $minnow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
							</td>
						<td>
							<select name='ESECOND' size=1>
							<?php $secnow = date('s');
									for($i=0; $i<60; $i++){ 
										if($i == $secnow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
						</td>
					</tr>
			</table>
			
			
			<b>Now choose to either</b>
			<br>
			<input type="submit" name="submit" onclick="document.pressed=this.value" value="Get All Data" /><em class="warn">&nbsp;&nbsp;* This will show all measurements from the selected time period (Table only). Might be slow for long time periods.</em>
			<br>
			&nbsp;&nbsp;or
			<br>
			<input type="button" onclick="showselectfield()" value="Manually select your measurements" />
			</fieldset>
		</div>
		
		<div id="timefield2">
			<img id="arrows" border="0" src="../Down_Arrow_Icon.png" width="30" height="30">
			<fieldset>
			<legend id="bigboxtitle"><b>Please enter the time period for scheduled data collection:</b><br></legend>
			<table>
					<tr><td></td><td></td><td class="centered">Hour</td><td class="centered">Minute</td><td class="centered">Second</td></tr>
					<tr><td nowrap>Start Date/Time: </td>
						<td>
							<?php
							  $myCalendar = new tc_calendar("startdatecollect", true);
							  $myCalendar->setIcon("../calendar/images/iconCalendar.gif");
							  $myCalendar->setDate(date('d'), date('m'), date('Y'));
							  $myCalendar->setPath("../calendar/");
							  $myCalendar->setYearInterval(date('Y'), date('Y'));
							  $myCalendar->dateAllow(date('Y').'-'.date('m').'-'.date('d'), date('Y').'-12-31');
							  $myCalendar->setOnChange("myChanged('test')");
							  $myCalendar->writeScript();
							?>
						</td>
						<td>
							<select name='SHOURC' size=1>
							<?php $hournow = date('G');
									for($i=0; $i<24; $i++){ 
										if($i == $hournow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
						</td>
						<td>
							<select name='SMINUTEC' size=1>
							<?php $minnow = date('i');
									for($i=0; $i<60; $i++){ 
										if($i == $minnow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
							</td>
						<td>
							<select name='SSECONDC' size=1>
							<?php $secnow = date('s');
									for($i=0; $i<60; $i++){ 
										if($i == $secnow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
						</td>
					</tr>
					
					
					
					
					<tr><td nowrap>End Date/Time: </td>
						<td>
							<?php
							  $myCalendar = new tc_calendar("enddatecollect", true);
							  $myCalendar->setIcon("../calendar/images/iconCalendar.gif");
							  $myCalendar->setDate(date('d'), date('m'), date('Y'));
							  $myCalendar->setPath("../calendar/");
							  $myCalendar->setYearInterval(date('Y'), date('Y'));
							  $myCalendar->dateAllow(date('Y').'-'.date('m').'-'.date('d'), date('Y').'-12-31');
							  $myCalendar->setOnChange("myChanged('test')");
							  $myCalendar->writeScript();
							?>
						</td>
						<td>
							<select name='EHOURC' size=1>
							<?php $hournow = date('G');
									for($i=0; $i<24; $i++){ 
										if(($i-1) == $hournow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
						</td>
						<td>
							<select name='EMINUTEC' size=1>
							<?php $minnow = date('i');
									for($i=0; $i<60; $i++){ 
										if($i == $minnow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
							</td>
						<td>
							<select name='ESECONDC' size=1>
							<?php $secnow = date('s');
									for($i=0; $i<60; $i++){ 
										if($i == $secnow){echo "<option selected>";}else{echo "<option>";} 
										echo str_pad($i,2,'0',STR_PAD_LEFT)."</option>"; 
									} 
							?>
							</select>
						</td>
					</tr>
					
					
					<tr><td class="repeat">Repeat?: </td>
						<td class="repeat">
						<select name='REPEAT' size=1>
							<option value="none">none</option>
							<option value="1">1</option>
							<option value="2">2</option>
							<option value="3">3</option>
						</select> &nbsp;&nbsp;time(s) a&nbsp;&nbsp;
						
						<select name="REPEAT" size=1>
							<option value="day">day</option>
							<option value="week">week</option>
							<option value="fortnight">fortnight</option>
						</td>
						
					</tr>
					
			</table>
			
			
			
			
			
			
	
			<input type="submit" name="schedule" id="schedule" onclick="document.pressed=this.value" value="Submit Data Collection Request">
			<script type="text/javascript">	
			
			
			document.getElementById("schedule").onclick = function()
			{
			var EHOUR = document.getElementsByName("EHOURC")[0].value;
			var SHOUR = document.getElementsByName("SHOURC")[0].value;
			var ABS = Math.abs(EHOUR - SHOUR);
			var sdate = document.getElementsByName("startdatecollect_day")[0].value;
			var edate = document.getElementsByName("enddatecollect_day")[0].value;
			
			
			if((ABS <6) && (edate == sdate))
			{document.pressed=this.value;}
			
			else{alert("ERROR: maximum collection interval is 6 hours");return false;}
			}
			
		</script>	

			
			</input>
			
			
			
			
			
			
			
			</fieldset>
			
		

			
			
		</div>

		
		
		
		
		<div id="selectfield">
		<img id="arrows" border="0" src="../Down_Arrow_Icon.png" width="30" height="30">
		<fieldset>
			<legend id="bigboxtitle"><b>Please enter your selection criteria below</b>&nbsp;(checked values will be displayed)</legend>
			
			<div class="combinecheckbox">
				<input type="checkbox" name="all" onClick="selectall()"/>&nbsp;Select everything!&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="checkbox" name="allp" onClick="selectallplots()"/>&nbsp;Select ALL PLOTS&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="checkbox" name="allt" onClick="selectalltables()"/>&nbsp;Select ALL TABLES&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="checkbox" name="combine"/>&nbsp;Combine plots of similar measurements (Example: Voltage A, B, and C will be on the same plot)
			</div>
			<table>
				<tr>
					<td>
						<fieldset id="phaseboxa">
						<legend><b>Phase A</b></legend>
						<table>
							<tr><td class="centered">&nbsp;Plot&nbsp;</td><td class="centered">Table</td></tr>
							<tr><td class="centered"><input type="checkbox" name="pvolta"/></td>  <td class="centered"><input type="checkbox" name="tvolta"/></td>  <td>Voltage</td></tr>
							<tr><td class="centered"><input type="checkbox" name="pcurra"/></td>  <td class="centered"><input type="checkbox" name="tcurra"/></td>  <td>Current</td></tr>
							<tr><td class="centered"><input type="checkbox" name="preala"/></td>  <td class="centered"><input type="checkbox" name="treala"/></td>  <td>Real Power</td></tr>
							<tr><td class="centered"><input type="checkbox" name="preaca"/></td>  <td class="centered"><input type="checkbox" name="treaca"/></td>  <td>Reactive Power</td></tr>
							<tr><td class="centered"><input type="checkbox" name="pappaa"/></td>  <td class="centered"><input type="checkbox" name="tappaa"/></td>  <td>Apparent Power</td></tr>
							<tr><td class="centered"><input type="checkbox" name="ppfa"/></td>    <td class="centered"><input type="checkbox" name="tpfa"/></td>  <td>Power Factor</td></tr>
						</table>
						<div class="phaseallcheck"><input type="checkbox" name="alla" onClick="selectalla()"/> All of Phase A</div>
						</fieldset>
					</td>
					
					<td>
						<fieldset id="phaseboxb">
						<legend><b>Phase B</b></legend>
						<table>
							<tr><td class="centered">&nbsp;Plot&nbsp;</td><td class="centered">Table</td></tr>
							<tr><td class="centered"><input type="checkbox" name="pvoltb"/></td>  <td class="centered"><input type="checkbox" name="tvoltb"/></td>  <td>Voltage</td></tr>
							<tr><td class="centered"><input type="checkbox" name="pcurrb"/></td>  <td class="centered"><input type="checkbox" name="tcurrb"/></td>  <td>Current</td></tr>
							<tr><td class="centered"><input type="checkbox" name="prealb"/></td>  <td class="centered"><input type="checkbox" name="trealb"/></td>  <td>Real Power</td></tr>
							<tr><td class="centered"><input type="checkbox" name="preacb"/></td>  <td class="centered"><input type="checkbox" name="treacb"/></td>  <td>Reactive Power</td></tr>
							<tr><td class="centered"><input type="checkbox" name="pappab"/></td>  <td class="centered"><input type="checkbox" name="tappab"/></td>  <td>Apparent Power</td></tr>
							<tr><td class="centered"><input type="checkbox" name="ppfb"/></td>    <td class="centered"><input type="checkbox" name="tpfb"/></td>  <td>Power Factor</td></tr>
						</table>
						<div class="phaseallcheck"><input type="checkbox" name="allb" onClick="selectallb()"/> All of Phase B</div>
						</fieldset>
					</td>
					
					<td>
						<fieldset id="phaseboxc">
						<legend><b>Phase C</b></legend>
						<table>
							<tr><td class="centered">&nbsp;Plot&nbsp;</td><td class="centered">Table</td></tr>
							<tr><td class="centered"><input type="checkbox" name="pvoltc"/></td>  <td class="centered"><input type="checkbox" name="tvoltc"/></td>  <td>Voltage</td></tr>
							<tr><td class="centered"><input type="checkbox" name="pcurrc"/></td>  <td class="centered"><input type="checkbox" name="tcurrc"/></td>  <td>Current</td></tr>
							<tr><td class="centered"><input type="checkbox" name="prealc"/></td>  <td class="centered"><input type="checkbox" name="trealc"/></td>  <td>Real Power</td></tr>
							<tr><td class="centered"><input type="checkbox" name="preacc"/></td>  <td class="centered"><input type="checkbox" name="treacc"/></td>  <td>Reactive Power</td></tr>
							<tr><td class="centered"><input type="checkbox" name="pappac"/></td>  <td class="centered"><input type="checkbox" name="tappac"/></td>  <td>Apparent Power</td></tr>
							<tr><td class="centered"><input type="checkbox" name="ppfc"/></td>    <td class="centered"><input type="checkbox" name="tpfc"/></td>  <td>Power Factor</td></tr>
						</table>
						<div class="phaseallcheck"><input type="checkbox" name="allc" onClick="selectallc()"/> All of Phase C</div>
						</fieldset>
					</td>
					
					<td>
						<fieldset>
						<legend><b>Other Values</b></legend>
							<table>
								<tr><td class="centered">&nbsp;Plot&nbsp;</td><td class="centered">Table</td></tr>
								<tr><td class="centered"><input type="checkbox" name="pfreq"/></td>     <td class="centered"><input type="checkbox" name="tfreq"/></td>     <td>Frequency</td></tr>
								<tr><td class="centered"><input type="checkbox" name="ppft"/></td>      <td class="centered"><input type="checkbox" name="tpft"/></td>      <td>Total Power Factor</td></tr>
								<tr><td class="centered"><input type="checkbox" name="pcurrn"/></td>    <td class="centered"><input type="checkbox" name="tcurrn"/></td>    <td>Neutral Current</td></tr>
								<tr><td class="centered"><input type="checkbox" name="pphaseab"/></td>  <td class="centered"><input type="checkbox" name="tphaseab"/></td>  <td>Phasor Angle from B to A</td></tr>
								<tr><td class="centered"><input type="checkbox" name="pphaseac"/></td>  <td class="centered"><input type="checkbox" name="tphaseac"/></td>  <td>Phasor Angle from C to A</td></tr>
							</table>
							<div class="phaseallcheck"><input type="checkbox" name="allo" onClick="selectallo()"/> All of Other Values</div>
						</fieldset>
					</td>
				</tr>
				</table>
				<table>
					<tr>
					<td>
						<fieldset id="phaseboxa">
						<legend><b>Phase A Harmonics</b></legend>
							<table>
								<tr><td class="centered">&nbsp;Plot&nbsp;</td><td class="centered">Table</td></tr>
								<tr><td class="centered"><input type="checkbox" name="pvha"/></td>     <td class="centered"><input type="checkbox" name="tvha"/></td>     <td class="atext">Voltage A Harmonics up to Harmonic:</td> <td><select name='vha' size=1><?php for($i=1; $i<6; $i++){ echo "<option>$i</option>"; } ?></select></td></tr>
								<tr><td class="centered"><input type="checkbox" name="pcha"/></td>     <td class="centered"><input type="checkbox" name="tcha"/></td>     <td class="atext">Current A Harmonics up to Harmonic:</td> <td><select name='cha' size=1><?php for($i=1; $i<6; $i++){ echo "<option>$i</option>"; } ?></select></td></tr>
								<tr><td class="centered"><input type="checkbox" name="pthda"/></td>  <td class="centered"><input type="checkbox" name="tthda"/></td>  <td class="atext">Total Harmonic Distortion A</td></tr>
							</table>
							<div class="harmallcheck"><input type="checkbox" name="allharma" onClick="selectallharma()"/> All of Phase A Harmonics</div>
						</fieldset>
					</td>
					<td>
						<fieldset id="phaseboxb">
						<legend><b>Phase B Harmonics</b></legend>
							<table>
								<tr><td class="centered">&nbsp;Plot&nbsp;</td><td class="centered">Table</td></tr>
								<tr><td class="centered"><input type="checkbox" name="pvhb"/></td>     <td class="centered"><input type="checkbox" name="tvhb"/></td>     <td class="btext">Voltage B Harmonics up to Harmonic:</td> <td><select name='vhb' size=1><?php for($i=1; $i<6; $i++){ echo "<option>$i</option>"; } ?></select></td></tr>
								<tr><td class="centered"><input type="checkbox" name="pchb"/></td>     <td class="centered"><input type="checkbox" name="tchb"/></td>     <td class="btext">Current B Harmonics up to Harmonic:</td> <td><select name='chb' size=1><?php for($i=1; $i<6; $i++){ echo "<option>$i</option>"; } ?></select></td></tr>
								<tr><td class="centered"><input type="checkbox" name="pthdb"/></td>  <td class="centered"><input type="checkbox" name="tthdb"/></td>  <td class="btext">Total Harmonic Distortion B</td></tr>
							</table>
							<div class="harmallcheck"><input type="checkbox" name="allharmb" onClick="selectallharmb()"/> All of Phase B Harmonics</div>
						</fieldset>
					</td>
					<td>
						<fieldset id="phaseboxc">
						<legend><b>Phase C Harmonics</b></legend>
							<table>
								<tr><td class="centered">&nbsp;Plot&nbsp;</td><td class="centered">Table</td></tr>
								<tr><td class="centered"><input type="checkbox" name="pvhc"/></td>     <td class="centered"><input type="checkbox" name="tvhc"/></td>     <td class="ctext">Voltage C Harmonics up to Harmonic:</td> <td><select name='vhc' size=1><?php for($i=1; $i<6; $i++){ echo "<option>$i</option>"; } ?></select></td></tr>
								<tr><td class="centered"><input type="checkbox" name="pchc"/></td>     <td class="centered"><input type="checkbox" name="tchc"/></td>     <td class="ctext">Current C Harmonics up to Harmonic:</td> <td><select name='chc' size=1><?php for($i=1; $i<6; $i++){ echo "<option>$i</option>"; } ?></select></td></tr>
								<tr><td class="centered"><input type="checkbox" name="pthdc"/></td>  <td class="centered"><input type="checkbox" name="tthdc"/></td>  <td class="ctext">Total Harmonic Distortion C</td></tr>
							</table>
							<div class="harmallcheck"><input type="checkbox" name="allharmc" onClick="selectallharmc()"/> All of Phase C Harmonics</div>
						</fieldset>
					</td>
					</tr>
				</table>
				
				
				<input id="oldbutton" type="submit" name="submit" onclick="document.pressed=this.value" value="Submit Recorded Data Request" /><em>&nbsp;&nbsp;* Make sure time period is reasonable!</em>
		</fieldset>
		</div>
		</form>
	</body>
	
	
</html>
