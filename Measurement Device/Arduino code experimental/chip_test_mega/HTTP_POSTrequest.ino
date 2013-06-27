String POST = "GET /projects/power/arduino_DBconnect.php?data=";

void HTTP_POSTrequest(String data){
  lineCount = 0;
  done = false;
  if (client.connect(site, 80)) {
    Serial.println("connected");
    client.print(POST);
    client.print(data);
    client.print("&&name=");
    client.print(deviceName);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(site);
    client.println();
  } 
  else {
    Serial.println("connection failed");
  }
  
  while (!done){
    while (client.available()) {
      char inChar = client.read();
      currLine += inChar;
      Serial.write(inChar);
      if (inChar == '\n'){
        lineCount ++;
        currLine = "";
      }
    }


    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      done = true;
    }
  }

  return;
}

