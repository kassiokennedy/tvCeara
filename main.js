//-----------Tabs----------------------------------------------
/*
function openCity(evt, cityName) {
  var i, tabcontent, tablinks
  tabcontent = document.getElementsByClassName('tabcontent')
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = 'none'
  }
  tablinks = document.getElementsByClassName('tablinks')
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(' active', '')
  }
  document.getElementById(cityName).style.display = 'block'
  evt.currentTarget.className += ' active'
}
*/
//-----------Tabs----------------------------------------------
/*
var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 100)
function loadData(url, callback){
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function(){
   if(this.readyState == 4 && this.status == 200){
   callback.apply(xhttp);
   }
  };
  xhttp.open("GET", url, true);
  xhttp.send();
  }
  function updateData(){
   document.getElementById("data").innerHTML = this.responseText;
  }
  */

var attempt = 3 // Variable to count number of attempts.
// Below function Executes on click of login button.
function validate() {
  //e.preventDefault()
  var username = document.getElementById('username').value
  var password = document.getElementById('password').value
  if (username == 'funtelc' && password == '1234') {
    document.location.href = 'index.html'
    alert('Login successfully')
    // window.location = 'a.html' // Redirecting to other page.
    //document.location.href = 'a.html'
  } else {
    attempt-- // Decrementing by one.
    alert('You have left ' + attempt + ' attempt;')
    // Disabling fields after 3 attempts.
    if (attempt == 0) {
      document.getElementById('username').disabled = true
      document.getElementById('password').disabled = true
      document.getElementById('submit').disabled = true
    }
  }
}
//----------------cClock-----------------------------------
function startTime() {
  const today = new Date()
  let h = today.getHours()
  let m = today.getMinutes()
  let s = today.getSeconds()
  m = checkTime(m)
  s = checkTime(s)
  document.getElementById('txt').innerHTML = h + ':' + m
  setTimeout(startTime, 1000)
}

function checkTime(i) {
  if (i < 10) {
    i = '0' + i
  } // add zero in front of numbers < 10
  return i
}
