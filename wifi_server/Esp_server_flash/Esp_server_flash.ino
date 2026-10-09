#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "createBitmap.h"

const char* ssid = "Emmes pixel";
const char* password = "Emman********";

WebServer server(80);
File uploadFile;
BitMap showProcess=BitMap();//display process and displaying object
// 1. This function handles the final HTTP response after the stream ends
void handleUploadResponse() {
  server.send(200, "text/plain", "File uploaded and saved to flash successfully!");
}

// 2. This function below help processes incoming binary data chunks sequentially
void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  
  if (upload.status == UPLOAD_FILE_START) {
    // We will save all uploaded data into a file named "/uploaded_data.bin"
    String filename = "/"+upload.filename; 
    Serial.printf("Starting file upload: %s\n", filename.c_str());
    
    // Open file in write mode ('w'), which creates the file or overwrites it
    uploadFile = LittleFS.open(filename, "w");
    if (!uploadFile) {
      Serial.println("Failed to open file for writing!");
    }
  } 
  else if (upload.status == UPLOAD_FILE_WRITE) {
    // Write the current data chunk to the flash memory
    if (uploadFile) {
      size_t bytesWritten = uploadFile.write(upload.buf, upload.currentSize);
      if (bytesWritten != upload.currentSize) {
        Serial.println("Warning: Chunk write mismatch!");
      }
    }
  } 
  else if (upload.status == UPLOAD_FILE_END) {
    // Close the file safely once the upload finishes
    if (uploadFile) {
      uploadFile.close();
      Serial.printf("Upload complete. Total size: %u bytes\n", upload.totalSize);
    }
  }
}
//3. the function below called to handle viewing of file saved on the saver with error handling for 
void handleViewFile() {

  // 1. Check if the "name" argument was provided in the URL
  if (!server.hasArg("name")) {
    server.send(400, "text/plain", "Bad Request: Missing 'name' parameter.");
    return;
  }

  // 2. Read the filename and prepend a '/' for LittleFS
  String filename = "/" + server.arg("name");
  // Check if the file exists in flash memory
  if (!LittleFS.exists(filename)) {
    server.send(404, "text/plain", "File Not Found. Upload a file first!");
    return;
  }

  // Open the file in read mode
  File file = LittleFS.open(filename, "r");
  if (!file) {
    server.send(500, "text/plain", "Failed to open file for reading.");
    return;
  }

  // streamFile automatically handles sending the content in chunks
  // We use "application/octet-stream" so it downloads as a binary file
  server.streamFile(file, "application/octet-stream");
  file.close();
}

//3. handles file deletion

  // 1. Function to handle incoming plain text
void handleReceiveText() {
  // Check if the request is a POST request
    if (server.method() != HTTP_POST) {
      server.send(405, "text/plain", "Method Not Allowed");
      return;
    }

    // Check if the content type is text/plain
    // (Optional, but good practice for API validation)
    if (server.header("Content-Type") != "text/plain") {
      // Note: To read headers, you must call server.collectHeaders() in setup.
    }

    // Read the raw text body from the request
    String receivedText = server.arg("plain");

    // Print the text to the Serial Monitor
    Serial.println("--- New Text Received ---");
    Serial.print("Content: ");
    Serial.println('/'+receivedText);
    Serial.printf("Length: %d characters\n", receivedText.length());
    Serial.println("-------------------------");

    bool result =LittleFS.remove("/"+receivedText);
    result? server.send(200, "text/plain", "File succesfully deleted"):server.send(404, "text/plain", "File Not Found. Upload a file first!");
    
}

//4 function handles plain text with no savings

void setup() {
  Serial.begin(115200);
  showProcess.init();
  // Initialize LittleFS
  if (!LittleFS.begin(true)) { // true enables auto-formating if it fails to mount
    Serial.println("LittleFS Mount Failed");
    return;
  }
  Serial.println("LittleFS mounted successfully.");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("conecting....");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // the route to both the final response handler and the streaming chunk handler
  server.on("/upload", HTTP_POST, handleUploadResponse, handleFileUpload);//route object creation for client uploading or posting 
  server.on("/view", HTTP_GET, handleViewFile);//route object creation for client downloading/streaming/getting
  server.on("/delete", HTTP_POST, handleReceiveText);//route for plain text delete command

  server.begin();
}

bool tryShow=true;
void loop() {
  if (LittleFS.exists("/star_hex.txt") && showOnce) {
    bool result=showProcess.loadBitmapFromFS("/star_hex.txt");
    showOnce=!result;
  }
  server.handleClient();
}
