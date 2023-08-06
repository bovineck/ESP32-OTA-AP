# ESP32-OTA-AP
ESP32 code to allow the device to be an AP and update it's own code "over the air" or OTA

Download the zip file, unzip and then load ESP32-OTA-AP into the Arduino IDE. You can alter some variables here such as the SSID and PASSWORD of the Access Point, as well as it's IP address. Defaults are:
ssid = "ESP32-Access-Point"
password = "12345678"
AP IP: 192.168.19.63
page login: admin
page password: admin

The other two files are:

1. mymain.h which is like a "normal" Arduino IDE ino sketch file and it is here that you load your code for the ESP32
2. tjquery.h which is a very large file needed for the AP to handle the upload (from https://github.com/italocjs/ESP32_OTA_APMODE)

The process to use these files is as follows:

a) change mymain.h to be your project code - as found here it is just a "fake multitasking" blinky
b) change any details of the AP as per above (e.g. AP IP)
c) compile and upload as usual
d) switch your WiFi to the new AP (default: "ESP32-Access-Point"), enter AP password (default: "12345678")
e) make a change to main.h so you can recognise the new code
f) navigate your browser to the IP address (default: 192.168.19.63)
g) enter the login (default: admin) and password (default: admin)
h) upload the new code and enjoy!
