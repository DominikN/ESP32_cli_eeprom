# ESP32_CLI_eeprom

[![GitHub stars](https://img.shields.io/github/stars/dominikn/ESP32_CLI_eeprom?style=social)](https://github.com/DominikN/ESP32_CLI_eeprom/stargazers/)

[![Build firmware](https://github.com/DominikN/ESP32_CLI_eeprom/actions/workflows/build.yml/badge.svg)](https://github.com/DominikN/ESP32_CLI_eeprom/actions/workflows/build.yml)
[![GitHub license](https://img.shields.io/github/license/dominikn/ESP32_CLI_eeprom.svg)](https://github.com/dominikn/ESP32_CLI_eeprom/blob/master/LICENSE)

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
- **[Husarnet](https://github.com/husarnet/husarnet)** - Open source, peer-to-peer Virtual LAN (VPN) network thanks to which you can access ESP32 both from LAN network and through the internet, without static IP adressess, setting port forwarding on your router etc.


To run the project follow these steps:

**1. Flashing ESP32**

Clone the repo and open it using [Visual Studio Code](https://code.visualstudio.com/) with [PlatformIO extension](https://platformio.org/install/ide?install=vscode) installed.

Connect ESP32 with your laptop using USB cable, and execute:

```bash
# Build project
$ pio run

# Upload firmware
$ pio run --target upload
```

**2. Get Husarnet join code**

- Register at https://app.husarnet.com/
- Click **Create network** button, name it (eg. `mynet`), and click **Create** button
- Click **Add element** button and go to the `join code` tab
- Copy your join code (looking like `fc94:b01d:1803:8dd8:b293:5c7d:7639:932a/xxxxxxxxxxxxxxxxxxxxxxxxx`) and save it for later

**3. Open serial monitor in Arduino IDE:**

Click the **"PlatformIO: Serial Monitor"** button and access ESP32 CLI.

**4. Save Wi-Fi network credentials in ESP32 non-volatile memory by typing in the Serial Monitor:**

`con_wifi -ssid "<YOUR_WIFI_NAME>" -pass "<YOUR_WIFI_PASS>"`

where in `<YOUR_WIFI_NAME>` and `<YOUR_WIFI_PASS>` place your Wi-Fi credentials.

**5. Save Husarnet network credentials in ESP32 non-volatile memory by typing in the Serial Monitor:**

`hnet_join XXXXXXXXXXXXXXXXXXXXXX myesp32"`

where in `XXXXXXXXXXXXXXXXXXXXXX` is husarnet join code from point 2.

**6. Reset ESP32, on the serial monitor you should see that is trying to connect to Wi-Fi and Husarnet network.**

**7. Install Husarnet on your laptop and add it to the same network as ESP32**

- open Linux terminal and type this line to install Husarnet: `$ curl https://install.husarnet.com/install.sh | sudo bash`
- connect your Linux computer to Husarnet network by executing this command: `$ sudo husarnet join XXXXXXXXXXXXXXXXXXXXXX mycomputer"`, where in `XXXXXXXXXXXXXXXXXXXXXX` is husarnet join code from point 2.

> Other instalation methods and other platforms are described in: https://husarnet.com/docs/

**8. Control your ESP32 through CLI over the Internet:**

- open Linux terminal  and type this line to connect to ESP32: `$ socat - tcp:myesp32:8001`
- type `set_led 1` to turn on LED connected to pin 16.

Enjoy! :)
