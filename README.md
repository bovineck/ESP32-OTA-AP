# Remote-ESP32: Simplify IoT Projects with OTA Updates and Web Control

Remote-ESP32 is a versatile IoT framework that empowers ESP32 projects with seamless sensor integration, OTA updates, and web-based control. The Remote-ESP32.ino file configures an ESP32 as an access point (AP), offering OTA update capabilities through a built-in web server. This framework harnesses the ESP32's dual-core architecture, enabling multitasking between OTA handling and custom application logic.

### Key Features:

* Sensor Integration: Effortlessly incorporate various sensors into your projects, such as a soil moisture sensor for real-time monitoring.
* Web-Based Control: Access the user-friendly web interface to monitor sensor data, control devices remotely, and trigger actions.
* OTA Updates: Ensure secure Over-The-Air (OTA) firmware updates with password-protected web interfaces.
* Update, delete, download the web interface, style.css file and any other project files.
* Efficient Multitasking: Leverage the ESP32's dual-core architecture for parallel execution of OTA update handling and your unique application logic.

### How to Use:

1. Download and unzip the repository.
2. Open Remote-ESP32.ino in the Arduino IDE.
3. Modify variables (e.g., SSID, PASSWORD) to match your network settings.
4. Edit your application-specific code in mymain.h (separate tab on the IDE)
5. Edit html and style files to suit
6. Upload the code to your ESP32 board.

## File Structure:

![File Structure](/filestructure.png)

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**data/**: Contains SPIFFS Files

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**main.html**: User interface for your IoT project, featuring buttons, inputs, and readouts.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**upload.html**: Dedicated page for uploading new firmware with a password field and file selection.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**access.html**: HTML file for accessing the LittleFS file partition.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**style.css**: Styling page to customize fonts, colors, and formatting.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**avatar.png**: Example image file for the HTML pages.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Remote-ESP32.ino**: Core 0 sketch with OTA setup, handler routines, and links to mymain.h.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**mymain.h**: Core 1 application-specific code with variables, setup, and loop logic. Utilize the ESP32's second core for multitasking.



Explore more at:

[OneCircuit Blog](https://onecircuit.blogspot.com/)

[OneCircuit YouTube Channel](https://youtube.com/@onecircuit-as)

Feel free to customize this framework for your unique IoT projects and streamline development with OTA capabilities and user-friendly web control.




