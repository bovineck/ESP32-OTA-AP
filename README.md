# ESP32-OTA-AP: Simplify IoT Projects with OTA Updates and Web Control

ESP32-OTA-AP is a versatile IoT framework that empowers ESP32 projects with seamless sensor integration, OTA updates, and web-based control. The ESP32-OTA-AP.ino file configures an ESP32 as an access point (AP), offering OTA update capabilities through a built-in web server. This framework harnesses the ESP32's dual-core architecture, enabling multitasking between OTA handling and custom application logic.

### Key Features:

* Sensor Integration: Effortlessly incorporate various sensors into your projects, such as a soil moisture sensor for real-time monitoring.
* Web-Based Control: Access the user-friendly web interface to monitor sensor data, control devices remotely, and trigger actions.
* OTA Updates: Ensure secure Over-The-Air (OTA) firmware updates with password-protected web interfaces.
* Efficient Multitasking: Leverage the ESP32's dual-core architecture for parallel execution of OTA update handling and your unique application logic.

### How to Use:

1. Download and unzip the repository.
1. Open ESP32-OTA-AP.ino in the Arduino IDE.
1. Modify variables (e.g., SSID, PASSWORD) to match your network settings.
1. Edit your application-specific code in mymain.h.
1. Upload the code to your ESP32 board.

## File Structure:



**data/**: Contains SPIFFS Files

**main.html**: User interface for your IoT project, featuring buttons, inputs, and readouts.

**upload.html**: Dedicated page for uploading new firmware with a password field and file selection.

**style.css**: Styling page to customize fonts, colors, and formatting.

**ESP32-OTA-AP.ino**: Core 0 sketch with OTA setup, handler routines, and links to mymain.h.

**mymain.h**: Core 1 application-specific code with variables, setup, and loop logic. Utilize the ESP32's second core for multitasking.



Explore more at:

[OneCircuit Blog](https://onecircuit.blogspot.com/)

[OneCircuit YouTube Channel](https://youtube.com/@onecircuit-as)

Feel free to customize this framework for your unique IoT projects and streamline development with OTA capabilities and user-friendly web control.




