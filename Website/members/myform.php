<?php session_start(); ?>
<html>
<head>
<title>hello, <?php echo $_POST[element_2_1];?> !</title>
<link rel="stylesheet" type="text/css" href="../css/indextest.css">
<link rel="stylesheet" type="text/css" href="../css/schedule.css">
</head>
<body>

			<div class="topbar">
			
				
				
			</div>
			
<div id="main">
<?php

$ed = $_SESSION['end_date'];
$et = $_SESSION['end_time'];

echo "hello " . $_POST[element_2_1] . ", an email has been sent to " . $_POST[element_1]. ". thanks for your cooperation. you will be redirected back to the main page in 10 seconds.";

$to = $_POST[element_1];
$subject = "Scheduled Data Collection";
$body = "Hello " . $_POST[element_2_1] . "!" . "\n\nYour data collection has been successfully scheduled. Please follow the instructions below to view and retrieve your data:"
. "\n\nAccording to our records, your data collection is scheduled to end on " . $ed . " at " . $et . ". \n\nAfter this time, you can view the collected data by clicking here: \nHTTP://www.ece.rochester.edu/projects/power/members/livepreview.php" . "\n\nIf the link above is not clickable, try copying and pasting the link directly into your Internet Explorer browser. \n\nPlease contact Professor Jones if you are experiencing any problems with the system."


;

if (mail($to,$subject,$body)){
}
else{
}
?>

</div>
</body>

<script type="text/javascript" language="javascript">
window.onload =function(){
setTimeout(function(){window.location.href="http://www.ece.rochester.edu/projects/power/indextest.html";},5000);
}
</script>


</html>