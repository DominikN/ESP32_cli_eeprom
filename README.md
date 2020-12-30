# ESP32-CLI-eeprom

**_Communicate with ESP32 over the internet using command line interface to save network credentials in the non-volatile memory and control LED. Written using Arduino framework._**

Command line interface is in many cases the most universal and comfortable way to control things. With CLI you can control your things over the terminal application and in easy way integrate it with your desktop/mobile application.

In this simple project I will demonstrate:

- How to **use CLI to write network credentials to non-volatile memory**. You will be able to change Wi-Fi network credentials without reprogramming your board.
- How to **connect over the internet to your ESP32** through a Linux terminal
- How to **control LED over the Internet through CLI**


In the project we use a few interesing tools:

- **[SimpleCLI library](https://github.com/spacehuhn/SimpleCLI)** - 
to easily create a command line interface. You don't need to play with parsing strings. That library do that (and much more) for you in a few lines.
- **[Preferences library](https://github.com/espressif/arduino-esp32/blob/master/libraries/Preferences/src/Preferences.h)** - the most elegant way that I have found to interface to non-volatile/EEPROM/Flash memory during run-time from user code. We use that library to store network credentials in the memory and read them after power-on to connent to the network.
- **[Husarnet](https://github.com/husarnet/arduino-esp32)** - a Virtual LAN network thanks to which you can access ESP32 both from LAN network and through the internet, without static IP adressess, setting port forwarding on your router etc.


To run the project, open Arduino IDE and follow these steps:

**1. Install Husarnet package for ESP32:**

- open `File -> Preferences`
- in a field **Additional Board Manager URLs** add this link: `https://github.com/husarnet/arduino-esp32/releases/download/1.0.4-1/package_esp32_index.json`
- open `Tools -> Board: ... -> Boards Manager ...`
- Search for `esp32-husarnet by Husarion`
- Click Install button

**2. Select ESP32 dev board:**

- open `Tools -> Board`
- select **_ESP32 Dev Module_** under "ESP32 Arduino (Husarnet)" section

**3. Install SimpleCLI library:**

- open `Tools -> Manage Libraries...`
- search for `SimpleCLI`
- click install button

**4. Program ESP32 board:**

- Open **ESP32_cli_eeprom.ino** project
- upload project to your ESP32 board.

**5. Get Husarnet join code**

- Register at https://app.husarnet.com/
- Click **Create network** button, name it (eg. `mynet`), and click **Create** button
- Click **Add element** button and go to the `join code` tab
- Copy your join code containging 22 characters `XXXXXXXXXXXXXXXXXXXXXX` and save it for later

**6. Open serial monitor in Arduino IDE:**

- open `Tools -> Serial Monitor`
- change baudrate to "115200 baud"
- change line ending to "Both NL & CR"

**7. Save Wi-Fi network credentials in ESP32 non-volatile memory by typing in the Serial Monitor:**

`conwifi -ssid "<YOUR_WIFI_NAME>" -pass "<YOUR_WIFI_PASS>"`

where in `<YOUR_WIFI_NAME>` and `<YOUR_WIFI_PASS>` place your Wi-Fi credentials.

**8. Save Husarnet network credentials in ESP32 non-volatile memory by typing in the Serial Monitor:**

`husarnet_join XXXXXXXXXXXXXXXXXXXXXX myesp32"`

where in `XXXXXXXXXXXXXXXXXXXXXX` is husarnet join code from point 5.

**9. Reset ESP32, on the serial monitor you should see that is trying to connect to Wi-Fi and Husarnet network.

**10. Install Husarnet on your Linux computer and add it to the same network as ESP32**

- open Linux terminal and type this line to install Husarnet: `$ curl https://install.husarnet.com/install.sh | sudo bash`
- connect your Linux computer to Husarnet network by executing this command: `$ sudo husarnet join XXXXXXXXXXXXXXXXXXXXXX mycomputer"`, where in `XXXXXXXXXXXXXXXXXXXXXX` is husarnet join code from point 5.

**11. Control your ESP32 through CLI over the Internet:**

- open Linux terminal  and type this line to connect to ESP32: `$ socat - tcp:myesp32:8001`
- type `setled 1` to turn on LED connected to pin 16.

Enjoy! :)
