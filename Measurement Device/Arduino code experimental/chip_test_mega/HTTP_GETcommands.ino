
void HTTP_GETcommands(String request){
  lineCount = 0;
  done = false;
  if (client.connect(site, 80)) {
    Serial.println("connected");
    client.print(request);
    client.print(deviceName);
    client.print("_commands.txt");
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
        if (find_text("$", currLine) == 0){
          user_input = currLine.substring(1);
        }
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
