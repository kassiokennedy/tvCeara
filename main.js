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
