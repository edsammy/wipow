<?php
// TODO: better error handling (see "livepull.php")
// TODO: include harmonics?
// TODO: Change live graphing library? Maybe change to highcharts? (www.highcharts.com) It has a better API.

$directory = getcwd();
include($directory.'/members/pinclude/devicelist.php');

if (isset($_POST['submit'])) {
	unset($_POST['submit']);
	
	$TBL = $_POST['SITE'];
	$tbl_posted = strtolower($TBL);	// $tbl is the device

	if(in_array($tbl_posted, $devices)){
		$tbl = $tbl_posted;
	} else{
		exit("Wrong device name");
	}
}
?>

<!DOCTYPE html>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<title>Live View for <?php echo $tbl?></title>
		<link href="live.css" rel="stylesheet" type="text/css" />
		<!--[if lte IE 8]><script language="javascript" type="text/javascript" src="flot/excanvas.min.js"></script><![endif]-->
		<script language="javascript" type="text/javascript" src="http://code.jquery.com/jquery-1.9.1.min.js"></script>
		<script language="javascript" type="text/javascript" src="flot/jquery.flot.js"></script>
		<script language="javascript" type="text/javascript" src="flot/jquery.flot.time.js"></script>
		<script language="javascript" type="text/javascript" src="flot/jquery.flot.selection.js"></script>
		
		<script type="text/javascript">
			var table = "<?php echo $tbl?>";
			//	alert("table java is: "+table);
			$(document).ready(function(){
				var inputs = document.getElementById("livetable").getElementsByTagName("input");
				var color = 0;
				var y_units = {						// List of y-axes for different scales units
					volta: 1, voltb: 1, voltc: 1,
					curra: 2, currb: 2, currc: 2, currn: 2,
					freq: 3,
					reala: 4, realb: 4, realc: 4,
					reaca: 5, reacb: 5, reacc: 5,
					appaa: 6, appab: 6, appac: 6,
					pfa: 7, pfb: 7, pfc: 7, pft: 7,
					phaseab: 8, phaseac: 8,
					vha1: 1, vha2: 1, vha3: 1, vha4: 1, vha5:1,
					vhb1: 1, vhb2: 1, vhb3: 1, vhb4: 1, vhb5:1,
					vhc1: 1, vhc2: 1, vhc3: 1, vhc4: 1, vhc5:1,
					cha1: 2, cha2: 2, cha3: 2, cha4: 2, cha5:2,
					chb1: 2, chb2: 2, chb3: 2, chb4: 2, chb5:2,
					chc1: 2, chc2: 2, chc3: 2, chc4: 2, chc5:2,
					thda: 9, thdb: 9, thdc: 9
				}
				var alreadyFetched = {};  // Keeps track of which measurement is already in the alldata[] array.
				var seriesID = 0;	// Keeps track of the index of each measurement in the alldata[] array.
				var alldata = []; 	// An array of objects. Each object in this array corresponds to a measurement.
									// Each object has its own array of data points for flot. (options as well)
									// Example: alldata[0] = {label: "Frequency" , data : [[time1,value1],[time2,value2]]};
									//          alldata[1] = {label: "Voltage A" , data : [[time1,value1],[time2,value2]]};
									// Remember: Flot data points only takes in NUMBERS, NOT strings.
				var plotdata = []; // The array of data series to be plotted
				function voltageformat(v, axis){return v.toFixed(axis.tickDecimals) + "V";}
				function currentformat(v, axis){return v.toFixed(axis.tickDecimals) + "A";}
				function frequencyformat(v, axis){return v.toFixed(axis.tickDecimals) + "Hz";}
				function wattformat(v, axis){return v.toFixed(axis.tickDecimals) + "W";}
				function reactiveformat(v, axis){return v.toFixed(axis.tickDecimals) + "VAR";}
				function voltampformat(v, axis){return v.toFixed(axis.tickDecimals) + "VA";}
				function pfformat(v, axis){return v.toFixed(axis.tickDecimals) + "(PF)";}
				function angleformat(v, axis){return v.toFixed(axis.tickDecimals) + "&deg;";}
				function thdnformat(v, axis){return v.toFixed(axis.tickDecimals);}

				var options = {
					xaxis:{
						mode: "time", timeformat: "%H:%M"
					},
					selection: {mode: "x"},
					grid: { hoverable: true, mouseActiveRadius: 5},
					legend: {container: "#legend"},
					series: {
						points:{show: true, radius: 2},
						lines:{show: true},
					},
					yaxes: [	{tickFormatter: voltageformat},
										{tickFormatter: currentformat},
										{tickFormatter: frequencyformat},
										{tickFormatter: wattformat},
										{tickFormatter: reactiveformat},
										{tickFormatter: voltampformat},
										{tickFormatter: pfformat},
										{tickFormatter: angleformat},
										{tickFormatter: thdnformat},
									]
				};

				var overview_options = {
					series: {
						lines: {
							show: true,
							lineWidth: 1
						},
						shadowSize: 0
					},
					legend: {show: false},
					xaxis: {
						ticks: [],
						mode: "time"
					},
					yaxis: {
						show: false,
						ticks: [],
						autoscaleMargin: 0.1
					},
					selection: {
						mode: "x"
					},
          yaxes:[
									{tickFormatter: voltageformat},
                  {tickFormatter: currentformat},
                  {tickFormatter: frequencyformat},
                  {tickFormatter: wattformat},
                  {tickFormatter: reactiveformat},
                  {tickFormatter: voltampformat},
                  {tickFormatter: pfformat},
                  {tickFormatter: angleformat},
									{tickFormatter: thdnformat},
               	]

				}
			
				function full_labeler(shortname){
					switch(shortname){
						case "freq": return "Frequency"; break;
						case "pft": return "Total Power Factor"; break;
						case "currn": return "Neutral Current"; break;
						case "phaseab": return "Phasor Angle B to A"; break;
						case "phaseac": return "Phasor Angle C to A"; break;
						case "volta": return "Voltage A"; break;
						case "volta": return "Voltage A"; break;
						case "volta": return "Voltage A"; break;
						case "curra": return "Current A"; break;
						case "reala": return "Real Power A"; break;
						case "reaca": return "Reactive Power A"; break;
						case "appaa": return "Apparent Power A"; break;
						case "pfa": return "Power Factor A"; break;
						case "voltb": return "Voltage B"; break;
						case "currb": return "Current B"; break;
						case "realb": return "Real Power B"; break;
						case "reacb": return "Reactive Power B"; break;
						case "appab": return "Apparent Power B"; break;
						case "pfb": return "Power Factor B"; break;
						case "voltc": return "Voltage C"; break;
						case "currc": return "Current C"; break;
						case "realc": return "Real Power C"; break;
						case "reacc": return "Reactive Power C"; break;
						case "appac": return "Apparent Power C"; break;
						case "pfc": return "Power Factor C"; break;
						case "vha1": return "Voltage A 1st Harmonic"; break;
						case "vha2": return "Voltage A 2nd Harmonic"; break;
						case "vha3": return "Voltage A 3rd Harmonic"; break;
						case "vha4": return "Voltage A 4th Harmonic"; break;
						case "vha5": return "Voltage A 5th Harmonic"; break;
						case "vhb1": return "Voltage B 1st Harmonic"; break;
						case "vhb2": return "Voltage B 2nd Harmonic"; break;
						case "vhb3": return "Voltage B 3rd Harmonic"; break;
						case "vhb4": return "Voltage B 4th Harmonic"; break;
						case "vhb5": return "Voltage B 5th Harmonic"; break;
						case "vhc1": return "Voltage C 1st Harmonic"; break;
						case "vhc2": return "Voltage C 2nd Harmonic"; break;
						case "vhc3": return "Voltage C 3rd Harmonic"; break;
						case "vhc4": return "Voltage C 4th Harmonic"; break;
						case "vhc5": return "Voltage C 5th Harmonic"; break;
						case "cha1": return "Current A 1st Harmonic"; break;
						case "cha2": return "Current A 2nd Harmonic"; break;
						case "cha3": return "Current A 3rd Harmonic"; break;
						case "cha4": return "Current A 4th Harmonic"; break;
						case "cha5": return "Current A 5th Harmonic"; break;
						case "chb1": return "Current B 1st Harmonic"; break;
						case "chb2": return "Current B 2nd Harmonic"; break;
						case "chb3": return "Current B 3rd Harmonic"; break;
						case "chb4": return "Current B 4th Harmonic"; break;
						case "chb5": return "Current B 5th Harmonic"; break;
						case "chc1": return "Current C 1st Harmonic"; break;
						case "chc2": return "Current C 2nd Harmonic"; break;
						case "chc3": return "Current C 3rd Harmonic"; break;
						case "chc4": return "Current C 4th Harmonic"; break;
						case "chc5": return "Current C 5th Harmonic"; break;
						case "thda": return "Current A THDN"; break;
						case "thdb": return "Current B THDN"; break;
						case "thdc": return "Current C THDN"; break;
						default: return shortname;
					}
				}

				var plot = $.plot("#placeholder", plotdata, options);
				var overview = $.plot("#overview", plotdata, overview_options);

				function showTooltip(x, y, contents) {
					$("<div id='tooltip'>" + contents + "</div>").css({
						position: "absolute",
						display: "none",
						top: y + 5,
						left: x + 5,
						border: "1px solid #fdd",
						padding: "2px",
						"background-color": "#fee",
						"font-size": "12px",
						opacity: 0.80
					}).appendTo("body").fadeIn(200);
				}

				var previousPoint = null;
				$("#placeholder").bind("plothover", function (event, pos, item) {
					if (item) {
						if (previousPoint != item.dataIndex) {

							previousPoint = item.dataIndex;

							$("#tooltip").remove();
							var y = item.datapoint[1],
									x = item.datapoint[0];
							x = new Date(x);
							var time = x.getHours() + ":" + x.getMinutes() + ":" + x.getSeconds();
							showTooltip(item.pageX, item.pageY,
							    "&nbsp;&nbsp;&nbsp;" + time + "<br>" + item.series.label + " = " + y);
							}
						} else {
							$("#tooltip").remove();
						previousPoint = null;            
						}
			
				});

				$("#placeholder").bind("plotselected", function (event, ranges) {
					plot = $.plot("#placeholder", plotdata, $.extend(true, {}, options, {
						xaxis: {
							min: ranges.xaxis.from,
							max: ranges.xaxis.to
						}
					}));
					overview.setSelection(ranges, true);
				});

				$("#overview").bind("plotselected", function (event, ranges) {
					plot.setSelection(ranges);
				});

				$(".selection").bind("click", function (event) {
						var selectedname = event.target.name;
						var selected = event.target.checked;
						if(selected == true){
							plotdata.push(alldata[alreadyFetched[selectedname]]);
						}
						else{
							for (var i=0; i<plotdata.length; i++){
								if((plotdata[i].label == full_labeler(selectedname)) && (selected == false)){
									plotdata.splice(i,1);
									i--;
								}
							}
						}
						plot = $.plot("#placeholder", plotdata, options);
						overview = $.plot("#overview", plotdata, overview_options);
				});

				$("#pausegraph").bind("click", function (event) {
						if(event.target.checked == false){
							plotdata = []; // Resetting the data to be plotted array
							for (var i=0; i<inputs.length; i++){
								if(inputs[i].type == "checkbox" && inputs[i].checked == true && alreadyFetched.hasOwnProperty(inputs[i].name)){
									plotdata.push(alldata[alreadyFetched[inputs[i].name]]);
								}
							}
							plot = $.plot("#placeholder", plotdata, options);
							overview = $.plot("#overview", plotdata, overview_options);
						}
				});

			function refreshdata(){
			//ajax calling "livepull.php" which in turn calls function in "device_call.php" to get data from the device
				var request = $.ajax({
					url: "livepull.php",
					type: "POST",
					data: {tbl : table},
					timeout: 70000,
					dataType: "text"
				});
				request.done(function(msg) { // The data coming in is in the form "name|value!name|value!..."
							     // "name" is the measurement name, as well as the name of the div id that the value goes into.
								 // This is done because the server's PHP does not support JSON format(which would make data transfer much easier).
					//alert(msg);	//For debugging. Shows what data is coming in from livepull.php
					if(msg == "IP address does not match"){
						alert("IP address in file does not correspond to the device. Please close this window and contact administrator @ kfung2@u.rocheser.edu");
					}
					else{
					var resultlist = msg.split('!');
					var timestamp = new Date();
					timestamp = timestamp.getTime();

					for(var i=0;i<resultlist.length;i++){
						var individual = resultlist[i].split('|');

						if(individual[0] == "recorded"){							// This implementation assumes that the actual timestamp (from database) is first in the data coming in. Otherwise, flot will plot the rest of the results with the time that this function gets called (as set when timestamp variable was initialized).
							timestamp = new Date(individual[1].replace(/-/g,"/"));
							timestamp = timestamp.getTime();
							document.getElementById(individual[0]).innerHTML = individual[1];
						}
						else if(document.getElementById(individual[0])){
							document.getElementById(individual[0]).innerHTML=individual[1];

							if (!alreadyFetched.hasOwnProperty(individual[0])) {
								alreadyFetched[individual[0]] = seriesID;
								seriesID++;
								color++;
								alldata.push({label:full_labeler(individual[0]), data: [[timestamp, parseFloat(individual[1])]], yaxis:y_units[individual[0]], color: color});
							}
							else{
								alldata[alreadyFetched[individual[0]]].data.push([timestamp, parseFloat(individual[1])]);
							}
						}
					}
					
					if(document.getElementById("pausegraph").checked != true){
						plotdata = []; // Resetting the data to be plotted array
						for (var i=0; i<inputs.length; i++){
							if(inputs[i].type == "checkbox" && inputs[i].checked == true && alreadyFetched.hasOwnProperty(inputs[i].name)){
								plotdata.push(alldata[alreadyFetched[inputs[i].name]]);
							}
						}
						plot = $.plot("#placeholder", plotdata, options);
						overview = $.plot("#overview", plotdata, overview_options);
					}
					}
				});
				request.fail(function(jqXHR, textStatus) {
					alert( "Request failed: " + textStatus );
				});
				
			}
			$(document).ajaxComplete(function(){
				// When ajax completes, call again. (this is non-recursive)
				refreshdata();
			});
			$(document).ajaxError(function(event, jqxhr, settings, exception) {
				alert("ajax error: "+exception);
			});

	$("#footer").prepend("Flot " + $.plot.version + " &ndash; ");

	refreshdata();
});
		</script>
	</head>
	
	<body>
	<h1>&nbsp;&nbsp;You are now reading data from: <?php echo $tbl?></h1>
	<br><em class='warn'>*Check the box next to a measurement to see it on the chart</em>
	&nbsp;&nbsp;<a href='./definitions.html' target='_blank'>Click here to see definitions for each measurement</a>
		<table id="livetable">
				<tr>
					<td>
						<fieldset id="phaseboxa">
						<legend><b>Phase A</b></legend>
						<table>
							<tr><td class="centered"><input class="selection" type="checkbox" name="volta"/></td>   <td>Voltage</td>		<td><div class="value" id="volta"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="curra"/></td>  <td>Current</td>		<td><div class="value" id="curra"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="reala"/></td>  <td>Real Power</td>		<td><div class="value" id="reala"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="reaca"/></td>  <td>Reactive Power</td>	<td><div class="value" id="reaca"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="appaa"/></td>  <td>Apparent Power</td>	<td><div class="value" id="appaa"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="pfa"/></td>  <td>Power Factor</td>	<td><div class="value" id="pfa"></div></td>	</tr>
						</table>
						</fieldset>
					</td>
					
					<td>
						<fieldset id="phaseboxb">
						<legend><b>Phase B</b></legend>
						<table>
							<tr><td class="centered"><input class="selection" type="checkbox" name="voltb"/></td>  <td>Voltage</td>		<td><div class="value" id="voltb"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="currb"/></td>  <td>Current</td>		<td><div class="value" id="currb"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="realb"/></td>  <td>Real Power</td>		<td><div class="value" id="realb"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="reacb"/></td>  <td>Reactive Power</td>	<td><div class="value" id="reacb"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="appab"/></td>  <td>Apparent Power</td>	<td><div class="value" id="appab"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="pfb"/></td>  <td>Power Factor</td>	<td><div class="value" id="pfb"></div></td>		</tr>
						</table>
						</fieldset>
					</td>
					
					<td>
						<fieldset id="phaseboxc">
						<legend><b>Phase C</b></legend>
						<table>
							<tr><td class="centered"><input class="selection" type="checkbox" name="voltc"/></td>  <td>Voltage</td>		<td><div class="value" id="voltc"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="currc"/></td>  <td>Current</td>		<td><div class="value" id="currc"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="realc"/></td>  <td>Real Power</td>		<td><div class="value" id="realc"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="reacc"/></td>  <td>Reactive Power</td>	<td><div class="value" id="reacc"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="appac"/></td>  <td>Apparent Power</td>	<td><div class="value" id="appac"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="pfc"/></td>  <td>Power Factor</td>	<td><div class="value" id="pfc"></div></td>		</tr>
						</table>
						</fieldset>
					</td>
					
					<td>
						<fieldset>
						<legend><b>Other Values</b></legend>
						<table>
							<tr><td class="centered"><input class="selection" type="checkbox" name="freq"/></td>  <td>Frequency</td>					<td><div class="value" id="freq"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="pft"/></td>  <td>Total Power Factor</td>			<td><div class="value" id="pft"></div></td>		</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="currn"/></td>  <td>Neutral Current</td>			<td><div class="value" id="currn"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="phaseab"/></td>  <td>Phasor Angle from B to A</td>	<td><div class="value" id="phaseab"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="phaseac"/></td>  <td>Phasor Angle from C to A</td>	<td><div class="value" id="phaseac"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="temp"/></td>  <td>Temperature</td>	<td><div class="value" id="temp"></div></td>	</tr>
						</table>
						</fieldset>
					</td>
				</tr>
				<tr>
					<td>
						<fieldset id="phaseboxa">
						<legend><b>Phase A Harmonics</b></legend>
						<table>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vha1"/></td>  <td>Voltage 1st</td>	<td><div class="value" id="vha1"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vha2"/></td>  <td>Voltage 2nd</td>	<td><div class="value" id="vha2"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vha3"/></td>  <td>Voltage 3rd</td>	<td><div class="value" id="vha3"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vha4"/></td>  <td>Voltage 4th</td>	<td><div class="value" id="vha4"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vha5"/></td>  <td>Voltage 5th</td>	<td><div class="value" id="vha5"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="thda"/></td>  <td>Current THDN</td>	<td><div class="value" id="thda"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="cha1"/></td>  <td>Current 1st</td>	<td><div class="value" id="cha1"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="cha2"/></td>  <td>Current 2nd</td>	<td><div class="value" id="cha2"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="cha3"/></td>  <td>Current 3rd</td>	<td><div class="value" id="cha3"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="cha4"/></td>  <td>Current 4th</td>	<td><div class="value" id="cha4"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="cha5"/></td>  <td>Current 5th</td>	<td><div class="value" id="cha5"></div></td>
						</table>
						</fieldset>
					</td>
					
					<td>
						<fieldset id="phaseboxb">
						<legend><b>Phase B Harmonics</b></legend>
						<table>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vhb1"/></td>  <td>Voltage 1st</td>	<td><div class="value" id="vhb1"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vhb2"/></td>  <td>Voltage 2nd</td>	<td><div class="value" id="vhb2"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vhb3"/></td>  <td>Voltage 3rd</td>	<td><div class="value" id="vhb3"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vhb4"/></td>  <td>Voltage 4th</td>	<td><div class="value" id="vhb4"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vhb5"/></td>  <td>Voltage 5th</td>	<td><div class="value" id="vhb5"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="thdb"/></td>  <td>Current THDN</td>	<td><div class="value" id="thdb"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="chb1"/></td>  <td>Current 1st</td>	<td><div class="value" id="chb1"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="chb2"/></td>  <td>Current 2nd</td>	<td><div class="value" id="chb2"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="chb3"/></td>  <td>Current 3rd</td>	<td><div class="value" id="chb3"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="chb4"/></td>  <td>Current 4th</td>	<td><div class="value" id="chb4"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="chb5"/></td>  <td>Current 5th</td>	<td><div class="value" id="chb5"></div></td>	</tr>
						</table>
						</fieldset>
					</td>
					
					<td>
						<fieldset id="phaseboxc">
						<legend><b>Phase C Harmonics</b></legend>
						<table>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vhc1"/></td>  <td>Voltage 1st</td>	<td><div class="value" id="vhc1"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vhc2"/></td>  <td>Voltage 2nd</td>	<td><div class="value" id="vhc2"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vhc3"/></td>  <td>Voltage 3rd</td>	<td><div class="value" id="vhc3"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vhc4"/></td>  <td>Voltage 4th</td>	<td><div class="value" id="vhc4"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="vhc5"/></td>  <td>Voltage 5th</td>	<td><div class="value" id="vhc5"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="thdc"/></td>  <td>Current THDN</td>	<td><div class="value" id="thdc"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="chc1"/></td>  <td>Current 1st</td>	<td><div class="value" id="chc1"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="chc2"/></td>  <td>Current 2nd</td>	<td><div class="value" id="chc2"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="chc3"/></td>  <td>Current 3rd</td>	<td><div class="value" id="chc3"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="chc4"/></td>  <td>Current 4th</td>	<td><div class="value" id="chc4"></div></td>	</tr>
							<tr><td class="centered"><input class="selection" type="checkbox" name="chc5"/></td>  <td>Current 5th</td>	<td><div class="value" id="chc5"></div></td>	</tr>
						</table>
						</fieldset>
					</td>

					<td>
						<div id="latest">Latest data at:&nbsp;<div id="recorded"><br></div></div>
					</td>
				</tr>
				</table>
		
				<table><tr><td class="plotcontainer"><div id="placeholder" style="width:850px;height:300px"></div></td>  <td><div id="legend"></div></td>  <td><div id="pause"><input type="checkbox" id="pausegraph"/>Pause the graph</div></td></tr>
							 <tr><td class="plotcontainer"><div id="overview" style="width:850px;height:100px"></div></td></tr>
				</table>
				<div id="footer">Copyright &copy; 2007 - 2013 IOLA and Ole Laursen</div>

				
	<div id="currenttime"><b>Current Time: </b><iframe src="http://free.timeanddate.com/clock/i3ik54db/n421/fn14/fc03f/tct/pct/tt0/th1/ta1/tb4" frameborder="0" width="210" height="38" allowTransparency="true"></iframe></div>
	</body>
</html>
