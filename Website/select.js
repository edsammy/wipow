// When the user clicks continue, this function inspects all 
// the check boxes and generates a string with a '1' if the box is checked 
// and a '0' if the box is unchecked. This string is decoded by the Arduino
// and only the checked data is gathered by the MAXIM chip.

$(function(){
  $('#btn').click(function () {
    // Commands are prepended with device name, ex. "cogen$11000..."
    var comm = $("select option:selected").val().toLowerCase() + "$";
    $('#a :checkbox,#b :checkbox ,#c :checkbox, #other :checkbox').each(function () {
        if ($(this).prop('checked') == true) {
            comm = comm + "1";
        } else {
            comm = comm + "0";
        }
    });
    //alert(name);
    var request = $.ajax({
      url: "commands.php",
      type: "POST",
      data: {arduino : comm},
      dataType: "text"
    });
    $("#lean_overlay").fadeOut(200);
    $("#userSelect").css({"display":"none"});
  });
}); 

//// Various selection shortcuts ////
// Select all
$(function(){
  $('.selAll').on('click', function() {
    if ($(this).prop('checked') == true) {
      $('#a :checkbox,#b :checkbox ,#c :checkbox, #other :checkbox').prop('checked', true);
    }
    else {
      $('#a :checkbox,#b :checkbox ,#c :checkbox, #other :checkbox').prop('checked', false);
    }
  });
 }); 
 
 // Selected voltages
 $(function(){
  $('.selVoltage, .voltage').on('click', function() {
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
  $('.selCurrent, .current').on('click', function() {
    if ($(this).prop('checked') == true) {
      $('.current').prop('checked', true);
    }
    else {
      $('.current').prop('checked', false);
    }
  });
 }); 
 
 // Select power (realpower, reacpower, appapower, powerfactor)
  $(function(){
  $('.selPower').on('click', function() {
    if ($(this).prop('checked') == true) {
      $('.realpower').prop('checked', true);
      $('.reacpower').prop('checked', true);
      $('.appapower').prop('checked', true);
      $('.powerfactor').prop('checked', true);
    }
    else {
      $('.realpower').prop('checked', false);
      $('.reacpower').prop('checked', false);
      $('.appapower').prop('checked', false);
      $('.powerfactor').prop('checked', false);
    }
  });
 }); 
 
 // If user clicks one measurment for one phase then select same measurement for all phases
 $(function(){
  $(':checkbox').on('click', function() {
    var boxes = $(this).attr('class');
    if ($(this).prop('checked') == true) {
      $('.' + boxes).prop('checked', true);
    }
    else {
      $('.' + boxes).prop('checked', false);
    }
  });
 });
 

 // Select voltage harmonics
  $(function(){
  $('.selVoltageHarmonics').on('click', function() {
    if ($(this).prop('checked') == true) {
      $("[class^='voltageHarmonics']").prop('checked', true);
    }
    else {
      $("[class^='voltageHarmonics']").prop('checked', false);
    }
  });
 }); 
 
  // Select current harmonics
  $(function(){
  $('.selCurrentHarmonics').on('click', function() {
    if ($(this).prop('checked') == true) {
      $("[class^='currentHarmonics']").prop('checked', true);
    }
    else {
      $("[class^='currentHarmonics']").prop('checked', false);
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