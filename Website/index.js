function OnSubmitForm(){	// Function called when a submit button on frontpage is pressed. Directs to corresponding page.
	if(document.pressed == 'Go Live!')
		{document.myform.action = "live.php";}
	else if(document.pressed == 'Submit Recorded Data Request')
		{document.myform.action = "old.php";}
	else if(document.pressed == 'Get All Data')
		{
			var inputs = document.getElementById("selectfield").getElementsByTagName("input");
				for (var i=0; i<inputs.length; i++){
					if(inputs[i].type == "checkbox" && inputs[i].name.indexOf('t') == 0){inputs[i].checked = true;}
				}
				document.myform.combine.checked = false;
				document.myform.vha.selectedIndex = 4;
				document.myform.cha.selectedIndex = 4;
				document.myform.vhb.selectedIndex = 4;
				document.myform.chb.selectedIndex = 4;
				document.myform.vhc.selectedIndex = 4;
				document.myform.chc.selectedIndex = 4;
				document.myform.allt.checked = true;

			document.myform.action = "old.php";
		}
	else if(document.pressed == 'Submit Data Collection Request')
		{document.myform.action = "schedule.php"}
	return true;
} 

function showtimefield(){ // Recorded data time form
		document.getElementById("timefield").style.display="block";
		document.getElementById("timefield2").style.display="none";
}

function showtimefield2(){ // Schedule time form
		document.getElementById("timefield2").style.display="block";
		document.getElementById("timefield").style.display="none";
		document.getElementById("selectfield").style.display="none";
}

function showselectfield(){ // Recorded data measurement manual select form
		document.getElementById("selectfield").style.display="block";
		document.getElementById("timefield2").style.display="none";
}

function selectall(){	// Function called by the "Select everything!" checkbox in the manual select form
	var inputs = document.getElementById("selectfield").getElementsByTagName("input");
	if(document.myform.all.checked == true){
		for (var i=0; i<inputs.length; i++){
			if(inputs[i].type == "checkbox"){inputs[i].checked = true;}
		}
		document.myform.combine.checked = false;
		document.myform.vha.selectedIndex = 4;
		document.myform.cha.selectedIndex = 4;
		document.myform.vhb.selectedIndex = 4;
		document.myform.chb.selectedIndex = 4;
		document.myform.vhc.selectedIndex = 4;
		document.myform.chc.selectedIndex = 4;
	}
	else if(document.myform.all.checked == false){
		for (var i=0; i<inputs.length; i++){
			if(inputs[i].type == "checkbox"){inputs[i].checked = false;}
			document.myform.vha.selectedIndex = 0;
			document.myform.cha.selectedIndex = 0;
			document.myform.vhb.selectedIndex = 0;
			document.myform.chb.selectedIndex = 0;
			document.myform.vhc.selectedIndex = 0;
			document.myform.chc.selectedIndex = 0;
		}
	}
}

function selectallplots(){ // Function called by the "Select all plots" checkbox in the manual select form
	var inputs = document.getElementById("selectfield").getElementsByTagName("input");
	if(document.myform.allp.checked == true){
		for (var i=0; i<inputs.length; i++){
			if(inputs[i].type == "checkbox" && inputs[i].name.indexOf('p') == 0){inputs[i].checked = true;}
		}
		document.myform.combine.checked = false;
		document.myform.vha.selectedIndex = 4;
		document.myform.cha.selectedIndex = 4;
		document.myform.vhb.selectedIndex = 4;
		document.myform.chb.selectedIndex = 4;
		document.myform.vhc.selectedIndex = 4;
		document.myform.chc.selectedIndex = 4;
	}
	else if(document.myform.allp.checked == false){
		for (var i=0; i<inputs.length; i++){
			if(inputs[i].type == "checkbox" && inputs[i].name.indexOf('p') == 0){inputs[i].checked = false;}
			document.myform.vha.selectedIndex = 0;
			document.myform.cha.selectedIndex = 0;
			document.myform.vhb.selectedIndex = 0;
			document.myform.chb.selectedIndex = 0;
			document.myform.vhc.selectedIndex = 0;
			document.myform.chc.selectedIndex = 0;
		}
	}
}

function selectalltables(){ // Function called by the "Select all tables" checkbox in the manual select form
	var inputs = document.getElementById("selectfield").getElementsByTagName("input");
	if(document.myform.allt.checked == true){
		for (var i=0; i<inputs.length; i++){
			if(inputs[i].type == "checkbox" && inputs[i].name.indexOf('t') == 0){inputs[i].checked = true;}
		}
		document.myform.combine.checked = false;
		document.myform.vha.selectedIndex = 4;
		document.myform.cha.selectedIndex = 4;
		document.myform.vhb.selectedIndex = 4;
		document.myform.chb.selectedIndex = 4;
		document.myform.vhc.selectedIndex = 4;
		document.myform.chc.selectedIndex = 4;
	}
	else if(document.myform.allt.checked == false){
		for (var i=0; i<inputs.length; i++){
			if(inputs[i].type == "checkbox" && inputs[i].name.indexOf('t') == 0){inputs[i].checked = false;}
			document.myform.vha.selectedIndex = 0;
			document.myform.cha.selectedIndex = 0;
			document.myform.vhb.selectedIndex = 0;
			document.myform.chb.selectedIndex = 0;
			document.myform.vhc.selectedIndex = 0;
			document.myform.chc.selectedIndex = 0;
		}
	}
}
			
function selectalla(){ // Function to select everything of Phase A in the manual select form
	if(document.myform.alla.checked == true)
	{
		document.myform.pvolta.checked = true;
		document.myform.pcurra.checked = true;
		document.myform.preala.checked = true;
		document.myform.preaca.checked = true;
		document.myform.pappaa.checked = true;
		document.myform.ppfa.checked = true;
		document.myform.tvolta.checked = true;
		document.myform.tcurra.checked = true;
		document.myform.treala.checked = true;
		document.myform.treaca.checked = true;
		document.myform.tappaa.checked = true;
		document.myform.tpfa.checked = true;
	}
	else if(document.myform.alla.checked == false)
	{
		document.myform.pvolta.checked = false;
		document.myform.pcurra.checked = false;
		document.myform.preala.checked = false;
		document.myform.preaca.checked = false;
		document.myform.pappaa.checked = false;
		document.myform.ppfa.checked = false;
		document.myform.tvolta.checked = false;
		document.myform.tcurra.checked = false;
		document.myform.treala.checked = false;
		document.myform.treaca.checked = false;
		document.myform.tappaa.checked = false;
		document.myform.tpfa.checked = false;
	}
} 
			
			
function selectallb(){
				if(document.myform.allb.checked == true)
				{
					document.myform.pvoltb.checked = true;
					document.myform.pcurrb.checked = true;
					document.myform.prealb.checked = true;
					document.myform.preacb.checked = true;
					document.myform.pappab.checked = true;
					document.myform.ppfb.checked = true;
					document.myform.tvoltb.checked = true;
					document.myform.tcurrb.checked = true;
					document.myform.trealb.checked = true;
					document.myform.treacb.checked = true;
					document.myform.tappab.checked = true;
					document.myform.tpfb.checked = true;
				}
				else if(document.myform.allb.checked == false)
				{
					document.myform.pvoltb.checked = false;
					document.myform.pcurrb.checked = false;
					document.myform.prealb.checked = false;
					document.myform.preacb.checked = false;
					document.myform.pappab.checked = false;
					document.myform.ppfb.checked = false;
					document.myform.tvoltb.checked = false;
					document.myform.tcurrb.checked = false;
					document.myform.trealb.checked = false;
					document.myform.treacb.checked = false;
					document.myform.tappab.checked = false;
					document.myform.tpfb.checked = false;
				}
			}
			
			
function selectallc(){
				if(document.myform.allc.checked == true)
				{
					document.myform.pvoltc.checked = true;
					document.myform.pcurrc.checked = true;
					document.myform.prealc.checked = true;
					document.myform.preacc.checked = true;
					document.myform.pappac.checked = true;
					document.myform.ppfc.checked = true;
					document.myform.tvoltc.checked = true;
					document.myform.tcurrc.checked = true;
					document.myform.trealc.checked = true;
					document.myform.treacc.checked = true;
					document.myform.tappac.checked = true;
					document.myform.tpfc.checked = true;
				}
				else if(document.myform.allc.checked == false)
				{
					document.myform.pvoltc.checked = false;
					document.myform.pcurrc.checked = false;
					document.myform.prealc.checked = false;
					document.myform.preacc.checked = false;
					document.myform.pappac.checked = false;
					document.myform.ppfc.checked = false;
					document.myform.tvoltc.checked = false;
					document.myform.tcurrc.checked = false;
					document.myform.trealc.checked = false;
					document.myform.treacc.checked = false;
					document.myform.tappac.checked = false;
					document.myform.tpfc.checked = false;
				}
}


function selectallharma(){
	if(document.myform.allharma.checked == true)
	{
		document.myform.pvha.checked = true;
		document.myform.pcha.checked = true;
		document.myform.pthda.checked = true;
		document.myform.tvha.checked = true;
		document.myform.tcha.checked = true;
		document.myform.tthda.checked = true;

	}
	else if(document.myform.allharma.checked == false)
	{
		document.myform.pvha.checked = false;
		document.myform.pcha.checked = false;
		document.myform.pthda.checked = false;
		document.myform.tvha.checked = false;
		document.myform.tcha.checked = false;
		document.myform.tthda.checked = false;

	}
}

function selectallharmb(){
	if(document.myform.allharmb.checked == true)
	{
		document.myform.pvhb.checked = true;
		document.myform.pchb.checked = true;
		document.myform.pthdb.checked = true;
		document.myform.tvhb.checked = true;
		document.myform.tchb.checked = true;
		document.myform.tthdb.checked = true;

	}
	else if(document.myform.allharmb.checked == false)
	{
		document.myform.pvhb.checked = false;
		document.myform.pchb.checked = false;
		document.myform.pthdb.checked = false;
		document.myform.tvhb.checked = false;
		document.myform.tchb.checked = false;
		document.myform.tthdb.checked = false;

	}
}

function selectallharmc(){
	if(document.myform.allharmc.checked == true)
	{
		document.myform.pvhc.checked = true;
		document.myform.pchc.checked = true;
		document.myform.pthdc.checked = true;
		document.myform.tvhc.checked = true;
		document.myform.tchc.checked = true;
		document.myform.tthdc.checked = true;

	}
	else if(document.myform.allharmc.checked == false)
	{
		document.myform.pvhc.checked = false;
		document.myform.pchc.checked = false;
		document.myform.pthdc.checked = false;
		document.myform.tvhc.checked = false;
		document.myform.tchc.checked = false;
		document.myform.tthdc.checked = false;

	}
} 

function selectallo(){
	if(document.myform.allo.checked == true)
	{
		document.myform.pfreq.checked = true;
		document.myform.ppft.checked = true;
		document.myform.pcurrn.checked = true;
		document.myform.pphaseab.checked = true;
		document.myform.pphaseac.checked = true;
		document.myform.tfreq.checked = true;
		document.myform.tpft.checked = true;
		document.myform.tcurrn.checked = true;
		document.myform.tphaseab.checked = true;
		document.myform.tphaseac.checked = true;

	}
	else if(document.myform.allo.checked == false)
	{
		document.myform.pfreq.checked = false;
		document.myform.ppft.checked = false;
		document.myform.pcurrn.checked = false;
		document.myform.pphaseab.checked = false;
		document.myform.pphaseac.checked = false;
		document.myform.tfreq.checked = false;
		document.myform.tpft.checked = false;
		document.myform.tcurrn.checked = false;
		document.myform.tphaseab.checked = false;
		document.myform.tphaseac.checked = false;

	}
}

