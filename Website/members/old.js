function toggletable(){
	if(document.getElementById("tabletogglebutton").value == "Hide Table Results (plots are at the bottom)"){
		document.getElementById("tableresult").style.display="none";
		document.getElementById("tabletogglebutton").value = "Show Table Results";
	}
	else if(document.getElementById("tabletogglebutton").value == "Show Table Results"){
		document.getElementById("tableresult").style.display="block";
		document.getElementById("tabletogglebutton").value = "Hide Table Results (plots are at the bottom)";
	}
}
