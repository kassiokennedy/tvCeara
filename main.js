const updateTemp = document.querySelector('#section1 .div2')

setInterval(displayUpdateTemp, 1000)

function displayUpdateTemp() {
  document.getElementsByClassName('div2').innerHTML = updateTemp
}
