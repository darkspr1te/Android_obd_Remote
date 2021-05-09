#include <WiFiClient.h>
#include <WebServer.h>

WebServer server(80);
File fsUploadFile;

void  handleButton() {
  String message = "<h>Buttons Test</h>";
  for (uint8_t i = 2; i < 35; i++) {
  message += "<a href='/button?b="+ String(i)+ "'>button "+buttonname(i)+"</a><br>"; //String(i)
  }
  if (server.argName(0) == "b" ) {
  HU_button(server.arg(0).toInt());
  message += "<br> button "+server.arg(0)+" Pressed <br>";
  }
  server.send(200, "text/plain", message);
}

void ServerPages() {
  server.on("/root/format", HTTP_GET, []() {
    server.send(200, "text/plain", "Formating File System");
    FILESYSTEM.format();
  });
  server.on("/list", HTTP_GET, handleFileList);
  //load editor
  server.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
  server.on("/button", handleButton);
  //create file
  //server.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  //server.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  server.on("/edit", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  }, handleFileUpload);

  //called when the url is not defined here
  //use it to load content from FILESYSTEM
  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
}

bool handleFileRead(String path) {
  if (path.endsWith("/")) {
    path += "index.htm";
  }
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (exists(pathWithGz) || exists(path)) {
    if (exists(pathWithGz)) {
      path += ".gz";
    }
    File file = FILESYSTEM.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void websrv() {
    server.handleClient();
}

void setupWebServer() {
 ServerPages();
 server.begin();
}

String getContentType(String filename) {
  if (server.hasArg("download")) {
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  } else if (filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}

bool exists(String path){
  bool found = false;
  File file = FILESYSTEM.open(path, "r");
  if(!file.isDirectory()){
    found = true;
  }
  file.close();
  return found;
}

void handleFileList() {
  if (!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }
  String path = server.arg("dir");
  File root = FILESYSTEM.open(path);
  path = String();
  String output = "[";
  if(root.isDirectory()){
      File file = root.openNextFile();
      while(file){
          if (output != "[") {
            output += ',';
          }
          output += "{\"type\":\"";
          output += (file.isDirectory()) ? "dir" : "file";
          output += "\",\"name\":\"";
          output += String(file.name()).substring(1);
          output += "\"}";
          file = root.openNextFile();
      }
  }
  output += "]";
  server.send(200, "text/json", output);
}

void handleFileUpload() {
  if (server.uri() != "/edit") {
    return;
  }
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    fsUploadFile = FILESYSTEM.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
  }
}

String buttonname(byte name) {
  switch (name) {
        case 2:
        return "Menu";
        break;
        case 3:
        return "Menu UP";
        break;
        case 4:
        return "Menu DOWN";
        break;
        case 7:
        return "OK";
        break;
        case 8:
        return "ESC";
        break;
        case 16:
        return "Mode";
        break;
        case 17:
        return "Source";
        break;
        case 18:
        return "Seek +";
        break;
        case 19:
        return "Seek -";
        break;
        case 20:
        return "Volume +";
        break;
        case 21:
        return "Volume -";
        break;
        case 22:
        return "Mute";
        break;
        case 23:
        return "Memo +";
        break;
        case 24:
        return "Memo -";
        break;
        case 35:
        return "BT";
        break;
  }
  return "??";
}
