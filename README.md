# ESP32 Wi-Fi File Server & OLED Bitmap Display

A simple ESP32-based HTTP server that supports file uploads, downloads, deletion, and displaying image bitmaps on an onboard OLED. It works alongside Python utilities for converting images into hexadecimal bitmap data and transferring files over Wi-Fi.

## Features

* **HTTP Server:** Handles client requests using route handlers.
* **File Upload:** Receives files from a PC and stores them in ESP32 LittleFS.
* **File Download:** Retrieves stored files through an HTTP endpoint.
* **File Deletion:** Deletes selected files from the server.
* **Image-to-Bitmap Converter:** Converts images into hexadecimal bitmap text using Python.
* **OLED Display:** Reads the uploaded bitmap text and displays the image on a 128×64 OLED.
* **Basic Rate Limiting:** Tracks individual visitors using a C structure containing information such as IP address, request count, and timing data to enforce request limits.

## How It Works

1. A Python script converts an image into hexadecimal bitmap data and saves it as a `.txt` file.
2. A Python client uploads the generated file to the ESP32 over Wi-Fi.
3. The ESP32 receives and stores the file in LittleFS.
4. The server reads the bitmap data and displays the image on its onboard OLED.
5. Clients can also download or delete stored files through the available HTTP routes.

## HTTP Routes

| Route     | Method | Purpose                          |
| --------- | ------ | -------------------------------- |
| `/upload` | POST   | Upload a file                    |
| `/view`   | GET    | Download or stream a stored file |
| `/delete` | POST   | Delete a stored file             |

## Project Structure

* `Esp_server_flash/` — ESP32 server implementation.
* `Bitmap.py` — Image-to-hexadecimal-bitmap conversion utility.
* `client.py` — Python client for interacting with the server.
* `getBytes.py` — Python byte-processing utility.
* `practice.py` — Additional development and testing.
* `Screenshots/` — Project screenshots and test results.

## Requirements

* ESP32 development board
* 128×64 SSD1306 OLED display
* Arduino IDE and required ESP32/OLED libraries
* Python 3
* PC and ESP32 connected to the same Wi-Fi network

## Getting Started

1. Open the ESP32 project in Arduino IDE.
2. Configure your Wi-Fi credentials and upload the firmware.
3. Connect the PC to the same network and identify the ESP32's IP address.
4. Install any required Python dependencies.
5. Run the Python client to convert, upload, download, or delete files.
6. Upload a bitmap text file and verify the image on the OLED.

## What I Learned

This project helped me understand HTTP routing and request handlers, binary data transfer, file handling with LittleFS, Python–ESP32 communication, OLED bitmap rendering, and basic client request rate limiting in C.

## Project Status

Functional prototype — file operations, bitmap conversion and transfer, and OLED image display have been implemented and tested.
