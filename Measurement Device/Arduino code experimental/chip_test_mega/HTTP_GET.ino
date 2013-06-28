
void HTTP_GET(String request){
  lineCount = 0;
  done = false;
  if (client.connect(site, 80)) {
    #ifdef DEBUG
      Serial.println("connected");
    #endif
    if (request == GETcommands){
      client.print(request);
      client.print(deviceName);
      client.print("_commands.txt");
    }
    
    else if (request == GETdata){
      client.print(GETdata);
      client.print(data);
      client.print("&&name=");
      client.print(deviceName);
    }
    
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(site);
    client.println();
  } 
  #ifdef DEBUG
    else {
      Serial.println("connection failed");
    }
  #endif
  
  while (!done){
    while (client.available()) {
      char inChar = client.read();
      currLine += inChar;
      #ifdef DEBUG
        Serial.write(inChar);
      #endif
      if (inChar == '\n'){ 
        if (request == GETcommands){
          user_input = currLine.substring(1);
        }
        else if (request == GETdata){
          if (find_text("OK", currLine) != 0){
            error_log = currLine;
          }
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
