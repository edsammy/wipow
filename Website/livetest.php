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
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<title>Live View for <?php echo $tbl?></title>
	
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
						mode: "time", timeformat: "%H:%M:%S"
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
      // Function below is used to implement user counting feature so no one loses the data they want to see while in go live mode
/*      window.onunload = window.onbeforeunload=function(){
        var done = false;
        if (done){ 
          return; 
        }
        done = true;
        var request = $.ajax({
        url: "php.php",
        type: "POST",
        data: {device : table},
        dataType: "text"
        });
      }
*/
	</script>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<title></title>
		<link rel="stylesheet" type="text/css" href="css/newlive.css"/>
		
	
	<link rel="stylesheet" type="text/css" href="css/tables-min.css"/>
	<link href="css/indextest.css" rel="stylesheet" type="text/css" />
	</head>
	
	<body>
		
		
			<div class="topbar">
			
				<h1><?php
				
				$text = file_get_contents('tmp/'.$tbl.'_commands.txt');
	/*			$text = $text+0;
				$text = (string)$strings;
				  $text = substr($text, 1, strlen($text));
			
			$text = $text+0;
				$text = (string)$text;
				*/
				  $text = substr($text, 1, strlen($text));
				 
				$text = str_split($text);
				$textnew = $textnew.$text[2].$text[5].$text[9].$text[12].$text[15].$text[18].$text[30].$text[31].$text[32].$text[33].$text[34].$text[35].$text[36].$text[37].$text[38].$text[39].$text[40];
				

				$text = $textnew;
				?>&nbsp;&nbsp;&nbsp;&nbsp;Reading Live Data From: <?php echo $tbl ?>&nbsp;</h1>
				
			</div>
			
			<script language="javascript">
				window.onload = function(){
				
				document.getElementById('loading').innerHTML ="retrieving data, please wait...<span style='margin-top:50px'><img src='loader.blue.gif' style='width:30px'></span>";
				document.getElementById('closedisplay').style.display="none";
				document.getElementById('liveright').style.display="none"; 
				document.getElementById('flot1').style.display="none";
				document.getElementById('flot').style.display="none";
				setInterval(function (){
				if(document.getElementById('volta').innerHTML !="")
				{
				 document.getElementById('loading').innerHTML="loading complete! click on values below to graph:";
				 document.getElementById('closedisplay').style.display="inline";
				 document.getElementById('liveright').style.display="inline";
				 document.getElementById('loadinginfo').style.display="none";
				 document.getElementById('flot1').style.display="inline";
				 document.getElementById('flot').style.display="inline";
				}
				}, 100);
				
				var strings = '<?php echo $text ?>';
			var tablestart = document.getElementById('livetable');
			var tablechild = tablestart.getElementsByTagName('tr');
			var poop = document.getElementById('volta');
			
			
			 
			for (var i=0;i<strings.length;i++)
			{
			if (strings[i] == 0){
			tablechild[i+1].style.display="none";}
			
			}
			
			
			
			
			
		
			
				}			
			
			</script>
			
			<script language="javascript">
			
			function bold(ids){
			if(document.getElementById(ids).style.fontWeight !="bold") {
			document.getElementById(ids).style.fontWeight="bold";}
			else
			{document.getElementById(ids).style.fontWeight="";}
			}
			</script>
				
				
				
		<br>		
		<section id="mainlive">
		
		<div id="liveleft">
		<div id="loading"></div><br>
		<div id="loadinginfo">selecting all harmonic values may result in increased loading time. <br><a href="indextest.html" style="text-decoration:none" id="visitors">click here</a> to try again with fewer parameters. </div>
		<span id="closedisplay">
			<table class="pure-table" id="livetable">
		
				<thead>
						<th></th>
						<th class="value" style="font-weight:bold;">&nbsp;Phase A&nbsp;&nbsp;</th>
						<th class="value1" style="font-weight:bold" >&nbsp;Phase B&nbsp;&nbsp;</th>
						<th class="value2" style="font-weight:bold">&nbsp;Phase C&nbsp;&nbsp;</th>
						
	</thead>
	
				</tbody>
				<tr id="voltage">
					<th>Voltage</th>
					<th><input class="selection" type="checkbox" id="voltacheck" name="volta" /><label for="voltacheck" ><div class="value" id="volta" style="display:inline-block" onclick="bold(this.id);"></div></label></th>
					
					<th><input class="selection" type="checkbox" id="voltbcheck" name="voltb"/><label for="voltbcheck"><div class="value1" id="voltb" style="display:inline-block" onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="voltccheck" name="voltc"/><label for="voltccheck"><div class="value2" id="voltc" style="display:inline-block" onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="current">
					<th>Current</th>
					<th><input class="selection" type="checkbox" id="curracheck" name="curra"/><label for="curracheck"><div class="value" id="curra" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="currbcheck" name="currb"/><label for="currbcheck"><div class="value1" id="currb" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="currccheck" name="currc"/><label for="currccheck"><div class="value2" id="currc" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="realpower">
					<th>Real Power</th>
					<th><input class="selection" type="checkbox" id="realacheck" name="reala"/><label for="realacheck"><div class="value" id="reala" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="realbcheck" name="realb"/><label for="realbcheck"><div class="value1" id="realb" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="realccheck" name="realc"/><label for="realccheck"><div class="value2" id="realc" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="reactivepower">
					<th>Reactive Power</th>
					<th><input class="selection" type="checkbox" id="reacacheck" name="reaca"/><label for="reacacheck"><div class="value" id="reaca" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="reacbcheck" name="reacb"/><label for="reacbcheck"><div class="value1" id="reacb" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="reacccheck" name="reacc"/><label for="reacccheck"><div class="value2" id="reacc" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="apparentpower">
					<th>Apparent Power</th>
					<th><input class="selection" type="checkbox" id="appaacheck" name="appaa"/><label for="appaacheck"><div class="value" id="appaa" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="appabcheck" name="appab"/><label for="appabcheck"><div class="value1" id="appab" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="appaccheck" name="appac"/><label for="appaccheck"><div class="value2" id="appac" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="powerfactor" >
					<th>Power Factor</th>
					<th><input class="selection" type="checkbox" id="pfacheck"name="pfa"/><label for="pfacheck"><div class="value" id="pfa" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="pfbcheck"name="pfb"/><label for="pfbcheck"><div class="value1" id="pfb" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="pfccheck"name="pfc"/><label for="pfccheck"><div class="value2" id="pfc" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="voltage1" >
					<th>Voltage 1st</th>
					<th><input class="selection" type="checkbox" id="vha1check"name="vha1"/><label for="vha1check"><div class="value" id="vha1" style="display:inline-block"onclick="bold(this.id);"></div></th>
					<th><input class="selection" type="checkbox" id="vhb1check"name="vhb1"/><label for="vhb1check"><div class="value1" id="vhb1" style="display:inline-block"onclick="bold(this.id);"></div></th>
					<th><input class="selection" type="checkbox" id="vhc1check" name="vhc1"/><label for="vhc1check"><div class="value2" id="vhc1" style="display:inline-block"onclick="bold(this.id);"></div></th>
				</tr>
				<tr id="voltage2" >
					<th>Voltage 2nd</th>
					<th><input class="selection" type="checkbox" id="vha2check" name="vha2"/><label for="vha2check"><div class="value" id="vha2" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="vhb2check" name="vhb2"/><label for="vhb2check"><div class="value1" id="vhb2" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="vhc2check" name="vhc2"/><label for="vhc2check"><div class="value2" id="vhc2" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="voltage3" >
					<th>Voltage 3rd</th>
					<th><input class="selection" type="checkbox" id="vha3check" name="vha3"/><label for="vha3check"><div class="value" id="vha3" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="vhb3check" name="vhb3"/><label for="vhb3check"><div class="value1" id="vhb3" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="vhc3check" name="vhc3"/><label for="vhc3check"><div class="value2" id="vhc3" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="voltage4" >
					<th>Voltage 4th</th>
					<th><input class="selection" type="checkbox" id="vha4check"name="vha4"/><label for="vha4check"><div class="value" id="vha4" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="vhb4check"name="vhb4"/><label for="vhb4check"><div class="value1" id="vhb4" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="vhc4check"name="vhc4"/><label for="vhc4check"><div class="value2" id="vhc4" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="voltage5" >
					<th>Voltage 5th</th>
					<th><input class="selection" type="checkbox" id="vha5check"name="vha5"/><label for="vha5check"><div class="value" id="vha5" style="display:inline-block"onclick="bold(this.id);"></div></th>
					<th><input class="selection" type="checkbox" id="vhb5check"name="vhb5"/><label for="vhb5check"><div class="value1" id="vhb5" style="display:inline-block"onclick="bold(this.id);"></div></th>
					<th><input class="selection" type="checkbox" id="vhc5check"name="vhc5"/><label for="vhc5check"><div class="value2" id="vhc5" style="display:inline-block"onclick="bold(this.id);"></div></th>
				</tr>
				<tr id="currentthdn" >
					<th>Current THDN</th>
					<th><input class="selection" type="checkbox" id="thdacheck" name="thda"/><label for="thdacheck"><div class="value" id="thda" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="thdbcheck" name="thdb"/><label for="thdbcheck"><div class="value1" id="thdb" style="display:inline-blockonclick="bold(this.id);"div></label></th>
					<th><input class="selection" type="checkbox" id="thdccheck" name="thdc"/><label for="thdccheck"><div class="value2" id="thdc" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="current1" >
					<th>Current 1st</th>
					<th><input class="selection" type="checkbox" id="cha1check"name="cha1"/><label for="cha1check"><div class="value" id="cha1" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="chb1check"name="chb1"/><label for="chb1check"><div class="value1" id="chb1" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="chc1check"name="chc1"/><label for="chc1check"><div class="value2" id="chc1" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="current2" >
					<th>Current 2nd</th>
					<th><input class="selection" type="checkbox" id="cha2check"name="cha2"/><label for="cha2check"><div class="value" id="cha2" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="chb2check"name="chb2"/><label for="chb2check"><div class="value1" id="chb2" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="chc2check"name="chc2"/><label for="chc2check"><div class="value2" id="chc2" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="current3" >
					<th>Current 3rd</th>
					<th><input class="selection" type="checkbox" id="cha3check"name="cha3"/><label for="cha3check"><div class="value" id="cha3" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="chb3check"name="chb3"/><label for="chb3check"><div class="value1" id="chb3" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="chc3check"name="chc3"/><label for="chc3check"><div class="value2" id="chc3" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="current4" >
					<th>Current 4th</th>
					<th><input class="selection" type="checkbox" id="cha4check"name="cha4"/><label for="cha4check"><div class="value" id="cha4" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="chb4check" name="chb4"/><label for="chb4check"><div class="value1" id="chb4" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
					<th><input class="selection" type="checkbox" id="chc4check" name="chc4"/><label for="chc4check"><div class="value2" id="chc4" style="display:inline-block"onclick="bold(this.id);"></div></label></th>
				</tr>
				<tr id="current5" >
					<th>Current 5th</th>
					<th><input class="selection" type="checkbox" id="cha5check"name="cha5"/><label for="cha5check"><div class="value" id="cha5" style="display:inline-block"onclick="bold(this.id);"></div></th>
					<th><input class="selection" type="checkbox" id="chb5check"name="chb5"/><label for="chb5check"><div class="value1" id="chb5" style="display:inline-block"onclick="bold(this.id);"></div></th>
					<th><input class="selection" type="checkbox" id="chc5check"name="chc5"/><label for="chc5check"><div class="value2" id="chc5" style="display:inline-block"onclick="bold(this.id);"></div></th>
				</tr>
				
			</tbody>
			</table>
			<!-- close display -->
			
			<!-- main table -->
		
	
						
						<br>
						<table class="pure-table">
							<tr><input class="selection" type="checkbox" name="freq"/>  <td>Frequency</td>					<td><div class="value3"style="font-weight:bold;" id="freq"></div></td>	</tr>
							<tr><input class="selection" type="checkbox" name="pft"/>  <td>Total Power Factor</td>			<td><div class="value3"style="font-weight:bold;" id="pft"></div></td>		</tr>
							<tr><input class="selection" type="checkbox" name="currn"/>  <td>Neutral Current</td>			<td><div class="value3"style="font-weight:bold;" id="currn"></div></td>	</tr>
							<tr><input class="selection" type="checkbox" name="phaseab"/>  <td>Phasor Angle from B to A</td>	<td><div class="value3" id="phaseab" style="font-weight:bold;"></div></td>	</tr>
							<tr><input class="selection" type="checkbox" name="phaseac"/>  <td>Phasor Angle from C to A&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>	<td><div class="value3" style="font-weight:bold;"id="phaseac"></div></td>	</tr>
							
						</table>
				
						
					<br>
					
						</span>	
					<br>
				</tr>
			</div><!--liveleft -->
		<div id="liveright">
				<table><tr><td class="plotcontainer" ><div id="placeholder" style="width:700px;height:300px;"></div></td>    <td></td></tr>
							 <tr><td class="plotcontainer"><div id="overview" style="width:700px;height:150px"></div></td></tr>
							 <td><div id="legend"></div></td>
				</table>
				
</div>
<div id="flot">graph created with flot.js, a JavaScript plotting library for jQuery. <a href="https://github.com/flot/flot/blob/master/API.md" style="text-decoration:none;">view documentation here</a> </div>
<div id="flot1"><div id="latest">Latest data at:&nbsp;<span id="recorded" style="color:rgb(8,106,135);"></span>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" id="pausegraph" class="poo"/>&nbsp;Pause graph</div></div>

		</section>
		<div class="footer">
	<div class="footerborder"></div>
		<div class="footer_contents">University of Rochester, Department of Electrical & Computer Engineering  &nbsp;&nbsp;&nbsp;&#169; 2012 - 2013
		</div>
	</div>	
	</body>
</html>
