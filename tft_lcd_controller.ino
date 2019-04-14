int data0 = 2;
int data1 = 3;
int data2 = 4;
int data3 = 5;
int data4 = 6;
int data5 = 7;
int data6 = 8;
int data7 = 9;

int dc = 10; // This clocks flip flop. Reads data0 for value. Set LOW to send high edge to FF.
int cs = 11; // This clocks flip flop. Reads data0 for value. Set LOW to send high edge to FF.
int clk = 12; // Set LOW to give a high edge.
int srld = 13; // Set LOW to read data into SR.
int clk_lcd_only = A0; // Set LOW to give a high edge.

void setup() {
  pinMode(dc, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(clk_lcd_only, OUTPUT);
  pinMode(srld, OUTPUT);

  pinMode(data0, OUTPUT);
  pinMode(data1, OUTPUT);
  pinMode(data2, OUTPUT);
  pinMode(data3, OUTPUT);
  pinMode(data4, OUTPUT);
  pinMode(data5, OUTPUT);
  pinMode(data6, OUTPUT);
  pinMode(data7, OUTPUT);

  Serial.begin(9600);

  initDisplay();
}

void loop_test() {
  pinMode(A5, INPUT);
  
  digitalWrite(clk_lcd_only, LOW);

  Serial.println("DC low");

  delay(2000);

  digitalWrite(clk_lcd_only, HIGH);

  Serial.println("DC high");

  delay(2000);
}

void loop() {
//  setRectangle(0, 239, 0, 319);
//  for (int i=0; i<3200; i++) {
//    writeData16(0x0000);
//  }

  drawSprite(50, 51, 0x0102);
  drawSprite(51, 50, 0x0000);
  drawSprite(52, 50, 0x0000);
  drawSprite(50, 51, 0x0000);
  drawSprite(51, 51, 0x0000);
  drawSprite(52, 51, 0x0000);
  drawSprite(50, 52, 0x0000);
  drawSprite(51, 52, 0x0000);
  drawSprite(52, 52, 0x0000);
//  
//  drawSprite(150, 150, 0x00FF);
//
//  delay(5000);
//
//  setRectangle(0, 19, 0, 19);
//  for (int i=0; i<398; i++) {
//    writeData16(0xFF00);
//  }

  delay(2000);
}

void writeCommand(uint8_t data) {
  // Set DC low.
  digitalWrite(data0, LOW);
  digitalWrite(dc, LOW);
  digitalWrite(dc, HIGH);

  writeData(data);
  
  // Set DC high.
  digitalWrite(data0, HIGH);
  digitalWrite(dc, LOW);
  digitalWrite(dc, HIGH);
}

void writeData(uint8_t data) {
  // Read data into SR.
  // lda #$(data) - sta $(address)
  digitalWrite(data0, bitRead(data, 0));
  digitalWrite(data1, bitRead(data, 1));
  digitalWrite(data2, bitRead(data, 2));
  digitalWrite(data3, bitRead(data, 3));
  digitalWrite(data4, bitRead(data, 4));
  digitalWrite(data5, bitRead(data, 5));
  digitalWrite(data6, bitRead(data, 6));
  digitalWrite(data7, bitRead(data, 7));

  digitalWrite(srld, LOW);
  digitalWrite(srld, HIGH);

  // Hold CS low.
  // lda #$00 - sta $(address)
  digitalWrite(data0, LOW);
  digitalWrite(cs, LOW);
  digitalWrite(cs, HIGH);

  // Clock data from SR to LCD.

  // Get that first bit.
  // sta $(address)
  digitalWrite(clk_lcd_only, LOW);
  digitalWrite(clk_lcd_only, HIGH);

  // Clock SR and LCD together for remaining bits.
  // sta $(address)
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);

  // Hold CS high.
  // lda #$01 - sta $(address)
  digitalWrite(data0, HIGH);
  digitalWrite(cs, LOW);
  digitalWrite(cs, HIGH);
}




void writeDataNoCS(uint8_t data) {
  // Read data into SR.
  // lda #$(data) - sta $(address)
  digitalWrite(data0, bitRead(data, 0));
  digitalWrite(data1, bitRead(data, 1));
  digitalWrite(data2, bitRead(data, 2));
  digitalWrite(data3, bitRead(data, 3));
  digitalWrite(data4, bitRead(data, 4));
  digitalWrite(data5, bitRead(data, 5));
  digitalWrite(data6, bitRead(data, 6));
  digitalWrite(data7, bitRead(data, 7));

  digitalWrite(srld, LOW);
  digitalWrite(srld, HIGH);

  // Clock data from SR to LCD.

  // Get that first bit.
  // sta $(address)
  digitalWrite(clk_lcd_only, LOW);
  digitalWrite(clk_lcd_only, HIGH);

  // Clock SR and LCD together for remaining bits.
  // sta $(address)
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
  digitalWrite(clk, HIGH);
}






void writeData16(uint16_t data) {
  // Hold CS low.
  // lda #$00 - sta $(address)
  digitalWrite(data0, LOW);
  digitalWrite(cs, LOW);
  digitalWrite(cs, HIGH);
  
  String highBits = "";
  for (int bit=15; bit>=8; bit--) {
    highBits += bitRead(data, bit);
  }
  uint8_t highBits_i = strtol( highBits.c_str(), NULL, 2 );

//  writeDataNoCS(highBits_i);
  writeData(highBits_i);

  String lowBits = "";
  for (int bit=7; bit>=0; bit--) {
    lowBits += bitRead(data, bit);
  }
  uint8_t lowBits_i = strtol( lowBits.c_str(), NULL, 2 );
  
//  writeDataNoCS(lowBits_i);
  writeData(lowBits_i);

  // Hold CS high.
  // lda #$01 - sta $(address)
  digitalWrite(data0, HIGH);
  digitalWrite(cs, LOW);
  digitalWrite(cs, HIGH);
  
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color) {
  // Column address set.
  writeCommand(0x2A);
  writeData16(x);
  writeData16(x);

  // Row address set.
  writeCommand(0x2B);
  writeData16(y);
  writeData16(y);

  // RAM write.
  writeCommand(0x2C);
  writeData16(color);
}


void setRectangle(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2) {
  // Column address set.
  writeCommand(0x2A);
  writeData16(x1);
  writeData16(x2);

  // Row address set.
  writeCommand(0x2B);
  writeData16(y1);
  writeData16(y2);

  // RAM write.
  writeCommand(0x2C);
}

void drawSprite(uint16_t x, uint16_t y, uint16_t color) {
  drawPixel(x, y, color);
  drawPixel(x+1, y, color);
  drawPixel(x+2, y, color);
  drawPixel(x+3, y, color);
  drawPixel(x+4, y, color);
  drawPixel(x+5, y, color);
  drawPixel(x+6, y, color);

  drawPixel(x+6, y-2, color);
  drawPixel(x+6, y-1, color);
  drawPixel(x+6, y+1, color);
  drawPixel(x+6, y+2, color);
  
  drawPixel(x+7, y, color);
  drawPixel(x+8, y, color);
  drawPixel(x+9, y, color);
  drawPixel(x+10, y, color);
  drawPixel(x+11, y, color);
  drawPixel(x+12, y, color);
  drawPixel(x+13, y, color);
  drawPixel(x+14, y, color);
}

void initDisplay() {
  // Default states -- addresses not selected.
  digitalWrite(dc, HIGH);
  digitalWrite(clk, HIGH);
  digitalWrite(clk_lcd_only, HIGH);
  digitalWrite(cs, HIGH);
  digitalWrite(srld, HIGH);

  // Set DC high.
  digitalWrite(data0, HIGH);
  digitalWrite(dc, LOW);
  digitalWrite(dc, HIGH);
  
//  digitalWrite(reset, HIGH);
//  delay(100);
//  digitalWrite(reset, LOW);
//  delay(100);
//  digitalWrite(reset, HIGH);
//  delay(200);

  // SW Reset.
  writeCommand(0x01);
  delay(150);

  // 0xEF, 3, 0x03, 0x80, 0x02,
  writeCommand(0xEF);
  writeData(0x03);
  writeData(0x80);
  writeData(0x02);

  // 0xCF, 3, 0x00, 0xC1, 0x30,
  writeCommand(0xCF);
  writeData(0x00);
  writeData(0xC1);
  writeData(0x30);

  // 0xED, 4, 0x64, 0x03, 0x12, 0x81,
  writeCommand(0xED);
  writeData(0x64);
  writeData(0x03);
  writeData(0x12);
  writeData(0x81);

  // 0xE8, 3, 0x85, 0x00, 0x78,
  writeCommand(0xE8);
  writeData(0x85);
  writeData(0x00);
  writeData(0x78);

  // 0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  writeCommand(0xCB);
  writeData(0x39);
  writeData(0x2C);
  writeData(0x00);
  writeData(0x34);
  writeData(0x02);

  // 0xF7, 1, 0x20,
  writeCommand(0xF7);
  writeData(0x20);

  // 0xEA, 2, 0x00, 0x00,
  writeCommand(0xEA);
  writeData(0x00);
  writeData(0x00);

  // ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
  writeCommand(0xC0);
  writeData(0x23);

  // ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  writeCommand(0xC1);
  writeData(0x10);

  // ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
  writeCommand(0xC5);
  writeData(0x3E);
  writeData(0x28);

  // ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
  writeCommand(0xC7);
  writeData(0x86);

  // ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  writeCommand(0x36);
  writeData(0x48);

  // ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  writeCommand(0x37);
  writeData(0x00);

  // ILI9341_PIXFMT  , 1, 0x55,
  writeCommand(0x3A);
  writeData(0x55);

  // ILI9341_FRMCTR1 , 2, 0x00, 0x18,
  writeCommand(0xB1);
  writeData(0x00);
  writeData(0x18);

  // ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  writeCommand(0x86);
  writeData(0x08);
  writeData(0x82);
  writeData(0x27);

  // 0xF2, 1, 0x00,                         // 3Gamma Function Disable
  writeCommand(0xF2);
  writeData(0x00);

  // ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
  writeCommand(0x26);
  writeData(0x01);

  // ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
  //   0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  writeCommand(0xE0);
  writeData(0x0F);
  writeData(0x31);
  writeData(0x2B);
  writeData(0x0C);
  writeData(0x0E);
  writeData(0x08);
  writeData(0x4E);
  writeData(0xF1);
  writeData(0x37);
  writeData(0x07);
  writeData(0x10);
  writeData(0x03);
  writeData(0x0E);
  writeData(0x09);
  writeData(0x00);

  // ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
  //   0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  writeCommand(0xE1);
  writeData(0x00);
  writeData(0x0E);
  writeData(0x14);
  writeData(0x03);
  writeData(0x11);
  writeData(0x07);
  writeData(0x31);
  writeData(0xC1);
  writeData(0x48);
  writeData(0x08);
  writeData(0x0F);
  writeData(0x0C);
  writeData(0x31);
  writeData(0x36);
  writeData(0x0F);

  // ILI9341_SLPOUT  , 0x80,                // Exit Sleep
  writeCommand(0x11);
  delay(150);

  // ILI9341_DISPON  , 0x80,                // Display on
  writeCommand(0x29);

  // 0x00                                   // End of list
  writeCommand(0x00);

}
