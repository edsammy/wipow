void parseHTML(){
  if (client.connect(site, 80)) {
    Serial.println("connected");
    client.println(get);
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
        if (lineCount == 13){
          user_input = currLine;
        }
        lineCount ++;
        currLine = "";
      }
    }


    if (!client.connected()) {
      Serial.println();
      Serial.println(user_input);
      Serial.println("disconnecting.");
      client.stop();
      done = true;
    }
  }
  
  user_input.toCharArray(charBuffer, total_functs);
  for (int i=0; i<total_functs - 1; i++){
    commands[i] = (charBuffer[i]) - 48;
  }
  return;
}

int find_text(String needle, String haystack) {
  int foundpos = -1;
  for (int i = 0; (i < haystack.length() - needle.length()); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}
