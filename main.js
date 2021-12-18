const updateTemp = document.querySelector('#section1 .tabcontent')

setInterval(displayUpdateTemp, 4000)

function displayUpdateTemp() {
  document.getElementsByClassName('div2').innerHTML = updateTemp
}

window.setInterval(function () {
  reloadIFrame()
}, 100)

function reloadIFrame() {
  console.log('reloading..')
  document.getElementById('sup1').contentWindow.location.reload()
  var t = setTimeout(refreshIFrame, 300)
}
//-----------Tabs----------------------------------------------
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
//-----------Tabs----------------------------------------------
