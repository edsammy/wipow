// When the user clicks continue, this function inspects all 
// the check boxes and generates a string with a '1' if the box is checked 
// and a '0' if the box is unchecked. This string is decoded by the Arduino
// and only the checked data is gathered by the MAXIM chip.
$(function(){
  $('#btn').click(function () {
    // Commands are prepended with device name, ex. "cogen$11000..."
    var comm = "$";
    var device = $("select option:selected").val().toLowerCase();
    $('.selection').each(function () {
        if ($(this).prop('checked') == true) {
            comm = comm + "1";
        } else {
            comm = comm + "0";
        }
    });
    //alert(comm);
    var request = $.ajax({
      url: "commands.php",
      type: "POST",
      data: {arduino : comm, device: device},
      dataType: "text"
    });
    $("#lean_overlay").fadeOut(200);
    $("#userSelect").css({"display":"none"});
    document.action = "live.php";
  });
}); 

//// Various selection shortcuts ////
// Select all
$(function(){
  $('.selAll').on('click', function() {
    if ($(this).prop('checked') == true) {
      $('.selection').prop('checked', true);
    }
    else {
      $('.selection').prop('checked', false);
    }
  });
 }); 
 
 // Selected voltages
 $(function(){
  $('.selVoltage').on('click', function() {
    if ($(this).prop('checked') == true) {
      $('.voltage').prop('checked', true);
    }
    else {
      $('.voltage').prop('checked', false);
    }
  });
 }); 
 
 // Select currents
  $(function(){
  $('.selCurrent').on('click', function() {
    if ($(this).prop('checked') == true) {
      $('.current').prop('checked', true);
    }
    else {
      $('.current').prop('checked', false);
    }
  });
 }); 
 
 // Select power
  $(function(){
  $('.selPower').on('click', function() {
    if ($(this).prop('checked') == true) {
      $('.power').prop('checked', true);
    }
    else {
      $('.power').prop('checked', false);
    }
  });
 }); 
 
 // Select voltage harmonics
  $(function(){
  $('.selVoltageHarmonics').on('click', function() {
    if ($(this).prop('checked') == true) {
      $('.voltageHarmonics').prop('checked', true);
    }
    else {
      $('.voltageHarmonics').prop('checked', false);
    }
  });
 }); 
 
  // Select current harmonics
  $(function(){
  $('.selCurrentHarmonics').on('click', function() {
    if ($(this).prop('checked') == true) {
      $('.currentHarmonics').prop('checked', true);
    }
    else {
      $('.currentHarmonics').prop('checked', false);
    }
  });
 }); 
 
   // Select current harmonics
  $(function(){
  $('.selOther').on('click', function() {
    if ($(this).prop('checked') == true) {
      $('.other').find(':checkbox').prop('checked', true);
    }
    else {
      $('.other').find(':checkbox').prop('checked', false);
    }
  });
 });