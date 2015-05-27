// Bluetooth receiver, display any received messages

void Bluetooth() {
  //Serial.println("Bluetooth");
  //Serial1.println("HC-05");
  content = "";
  
  while(Serial1.available()) 
  {
      character = Serial1.read();
      content.concat(character);
      Serial.println(character);
      //content[sizeof(content)]=character;
      delay(50);
  }
  
  if (content.length() >0) 
  {
    
    Serial.println(content.length());
    Serial.println(content);
    //int led = content.toInt();  // use atoi to convert the string to an int
    
          SenseMeLEDMatrix.setTextSize(1);
          SenseMeLEDMatrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
          SenseMeLEDMatrix.setTextColor(LED_ON);

          int len=content.length();
          for (int8_t x=0; x>=-((len+1)*6); x--) {
            SenseMeLEDMatrix.clear();
            SenseMeLEDMatrix.setCursor(x,0);
            SenseMeLEDMatrix.print(content);
            SenseMeLEDMatrix.writeDisplay();
            delay(100);
          }
          
          SenseMeLEDMatrix.clear();
          SenseMeLEDMatrix.writeDisplay();
          

    content = "";
    //delay(10); 
  }
  
}
