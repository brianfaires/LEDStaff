#ifdef USING_SD
void InitSD() {
  SD.begin(SS);
  #ifdef USING_IMAGES
    rootSD = SD.open("/");// TO DO: Is this necessary with file.vwf()?
  #endif
}

uint16_t curImgHeight;
uint16_t curImgWidth;

#ifdef USING_IMAGES
void NextImage() {
  uint32_t t1 = millis();

  leds(0, NUM_LEDS-1) = CRGB::Black;
  if(curFile) curFile.close();
  curFile = rootSD.openNextFile();
  if(!curFile) {
    rootSD.close();
    rootSD = SD.open("/");
    curFile = rootSD.openNextFile();
  }

  char tempName[5];
  curFile.getName(tempName, 5);
  if(tempName[0] == 'A' && tempName[1] == 'c' && tempName[2] == 'c')
    NextImage();
  
  
  curImgHeight = curFile.read() << 8;
  curImgHeight += curFile.read();
  curImgWidth = curFile.read() << 8;
  curImgWidth += curFile.read();

  #ifdef USING_SERIAL
    char tempName[14];
    curFile.getName(tempName, 14);
    Serial.println(tempName);
    Serial.println(curImgHeight);
    Serial.println(curImgWidth);
  #endif
}

void StreamImage() {
  if(curFile.available() < 3*curImgHeight) {
    curFile.seek(4);
    #ifdef USING_SERIAL
      Serial.println("    Refresh");
    #endif
  }

  if(curImgHeight > NUM_LEDS) {
    curFile.read(leds, 3*NUM_LEDS);
    curFile.seek(curFile.position() + 3*(curImgHeight - NUM_LEDS));
  }
  else {
    curFile.read(leds, 3*curImgHeight);
  }
}

#endif
#endif
