/*
   OTA via an AP (ESP32 version)
   This little wonder will set up it's own AP
   according to the code below - then you can
   upload and change the code by logging into
   the AP (find it on the WiFi and connect to
   it via the IP coded below)
   To change the code, see the relevant parts
   below, including MyVariables,  setup() and
   dostuff().

   You need the bin file to upload, then copy
   it to the ESP32 via the html page it shows
   when you login. See the blog below for all
   the code and the instructions.

      OneCircuit Wed 02 Aug 2023 18:40:22 AEST
      https://www.youtube.com/@onecircuit-as
      https://onecircuit.blogspot.com/
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

// alter your variables in main.h
#include "mymain.h"

// separate file for jquery
#include "tjquery.h"

// what to call your device as an AP
const char* ssid = "ESP32-Access-Point";
const char* password = "12345678"; // must be 8 characters

WebServer server(80);

/* Style */
String style =
  "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
  "input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
  "#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
  "#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
  "form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
  ".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

// Login page - change credentials here
String loginIndex =
  "<form name=loginForm>"
  "<h1>ESP32 Login</h1>"
  "<input name=userid placeholder='User ID'> "
  "<input name=pwd placeholder=Password type=Password> "
  "<input type=submit onclick=check(this.form) class=btn value=Login></form>"
  "<script>"
  "function check(form) {"
  "if(form.userid.value=='admin' && form.pwd.value=='admin')"
  "{window.open('/serverIndex')}"
  "else"
  "{alert('Error Password or Username')}"
  "}"
  "</script>" +
  style;

// Server Index Page
String serverIndex =
  "<script src='/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
  "<label id='file-input' for='file'>   Choose file...</label>"
  "<input type='submit' class=btn value='Update'>"
  "<br><br>"
  "<div id='prg'></div>"
  "<br><div id='prgbar'><div id='bar'></div></div><br></form>"
  "<script>"
  "function sub(obj){"
  "var fileName = obj.value.split('\\\\');"
  "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
  "};"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  "$.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "$('#bar').css('width',Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!') "
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>" +
  style;

void onJavaScript(void) {
  server.setContentLength(jquery_min_js_v3_2_1_gz_len);
  server.sendHeader(F("Content-Encoding"), F("gzip"));
  server.send_P(200, "text/javascript", jquery_min_js_v3_2_1_gz, jquery_min_js_v3_2_1_gz_len);
}

void setup(void)
{

  // alter your setup code in main.h
  MySetup();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  // Wait 200 ms for AP_START
  delay(200);
  //change to your own IP Access Point
  IPAddress Ip(192, 168, 19, 63);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Ip, Ip, NMask);

/*  if (!MDNS.begin(host))
  { // http://esp32.local
    while (1)
    {
      delay(1000);
    }
  }
*/
  server.on("/jquery.min.js", HTTP_GET, onJavaScript);

  server.on("/", HTTP_GET, []()
  {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []()
  {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  server.on(
    "/update", HTTP_POST, []()
  {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  },
  []()
  {
    HTTPUpload &upload = server.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN))
      { // start with max available size
        Update.printError(Serial);
      }
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
      {
        Update.printError(Serial);
      }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
      if (Update.end(true))
      {
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      }
      else
      {
        Update.printError(Serial);
      }
    }
  });
  server.begin();

}

void loop(void)
{
  server.handleClient();
  delay(1);

  // alter your loop code in main.h
  MyLoop();
}
