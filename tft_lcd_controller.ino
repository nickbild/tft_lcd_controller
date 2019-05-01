int data0 = 2;
int data1 = 3;
int data2 = 4;
int data3 = 5;
int data4 = 6;
int data5 = 7;
int data6 = 8;
int data7 = 9;

int dc = 10;            // This clocks flip flop. Reads data0 for value. Set LOW to send high edge to FF.
int clk = 11;           // Set LOW to give a high edge.
int clk_lcd_only = A0;  // Set LOW to give a high edge.
int cs1 = 12;           // This clocks flip flop. Reads data0 for value. Set LOW to send high edge to FF.
int cs2 = A5;           // This clocks flip flop. Reads data0 for value. Set LOW to send high edge to FF.
int srld = 13;          // Set LOW to read data into SR.

void setup() {
  pinMode(data0, OUTPUT);
  pinMode(data1, OUTPUT);
  pinMode(data2, OUTPUT);
  pinMode(data3, OUTPUT);
  pinMode(data4, OUTPUT);
  pinMode(data5, OUTPUT);
  pinMode(data6, OUTPUT);
  pinMode(data7, OUTPUT);

  pinMode(dc, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(clk_lcd_only, OUTPUT);
  pinMode(cs1, OUTPUT);
  pinMode(cs2, OUTPUT);
  pinMode(srld, OUTPUT);

  // Default states -- addresses not selected.
  digitalWrite(dc, HIGH);
  digitalWrite(clk, HIGH);
  digitalWrite(clk_lcd_only, HIGH);
  digitalWrite(cs1, HIGH);
  digitalWrite(cs2, HIGH);
  digitalWrite(srld, HIGH);

  // Disable both screens.
  digitalWrite(data0, HIGH);
  digitalWrite(cs1, LOW);
  digitalWrite(cs1, HIGH);
  digitalWrite(cs2, LOW);
  digitalWrite(cs2, HIGH);

  // Set DC high.
  digitalWrite(data0, HIGH);
  digitalWrite(dc, LOW);
  digitalWrite(dc, HIGH);

  // HW reset.
  //  digitalWrite(reset, HIGH);
  //  delay(100);
  //  digitalWrite(reset, LOW);
  //  delay(100);
  //  digitalWrite(reset, HIGH);
  //  delay(200);

  initDisplay(cs1);
  initDisplay(cs2);
}

void loop() {
  drawBackground(cs1);
  drawBackground(cs2);

  delay(2000);
}

void writeCommand(uint8_t data, int cs) {
  // Set DC low.
  digitalWrite(data0, LOW);
  digitalWrite(dc, LOW);
  digitalWrite(dc, HIGH);

  writeData(data, cs);

  // Set DC high.
  digitalWrite(data0, HIGH);
  digitalWrite(dc, LOW);
  digitalWrite(dc, HIGH);
}

void writeData(uint8_t data, int cs) {
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

void writeData16(uint16_t data, int cs) {
  String highBits = "";
  for (int bit=15; bit>=8; bit--) {
    highBits += bitRead(data, bit);
  }
  uint8_t highBits_i = strtol( highBits.c_str(), NULL, 2 );

  writeData(highBits_i, cs);

  String lowBits = "";
  for (int bit=7; bit>=0; bit--) {
    lowBits += bitRead(data, bit);
  }
  uint8_t lowBits_i = strtol( lowBits.c_str(), NULL, 2 );

  writeData(lowBits_i, cs);
}

void setRectangle(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, int cs) {
  // Column address set.
  writeCommand(0x2A, cs);
  writeData16(x1, cs);
  writeData16(x2, cs);

  // Row address set.
  writeCommand(0x2B, cs);
  writeData16(y1, cs);
  writeData16(y2, cs);

  // RAM write.
  writeCommand(0x2C, cs);
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color, int cs) {
  // Column address set.
  writeCommand(0x2A, cs);
  writeData16(x, cs);
  writeData16(x, cs);

  // Row address set.
  writeCommand(0x2B, cs);
  writeData16(y, cs);
  writeData16(y, cs);

  // RAM write.
  writeCommand(0x2C, cs);
  writeData16(color, cs);
}

void drawBackground(int cs) {
  setRectangle(0, 239, 0, 50, cs);
  for (int i=0; i<12240; i++) {
    writeData16(0x0000, cs);
  }
  setRectangle(0, 239, 51, 101, cs);
  for (int i=0; i<12240; i++) {
    writeData16(0x0000, cs);
  }
  setRectangle(0, 239, 102, 153, cs);
  for (int i=0; i<12480; i++) {
    writeData16(0x0000, cs);
  }
  setRectangle(0, 239, 154, 205, cs);
  for (int i=0; i<12480; i++) {
    writeData16(0x0000, cs);
  }
  setRectangle(0, 239, 206, 257, cs);
  for (int i=0; i<12480; i++) {
    writeData16(0x0000, cs);
  }
  setRectangle(0, 239, 258, 309, cs);
  for (int i=0; i<12480; i++) {
    writeData16(0x0000, cs);
  }
  setRectangle(0, 239, 309, 319, cs);
  for (int i=0; i<2640; i++) {
    writeData16(0x0000, cs);
  }

  setRectangle(50, 100, 200, 250, cs);
  for (int i=0; i<2640; i++) {
    writeData16(0xEC00, cs);
  }

  drawPixel(10, 187, 0xFFFF, cs);
  drawPixel(20, 88, 0xFFFF, cs);
  drawPixel(30, 32, 0xFFFF, cs);
  drawPixel(40, 310, 0xFFFF, cs);
  drawPixel(50, 290, 0xFFFF, cs);
  drawPixel(60, 180, 0xFFFF, cs);
  drawPixel(70, 130, 0xFFFF, cs);
  drawPixel(80, 30, 0xFFFF, cs);
  drawPixel(90, 210, 0xFFFF, cs);
  drawPixel(100, 190, 0xFFFF, cs);
  drawPixel(1110, 105, 0xFFFF, cs);
  drawPixel(120, 89, 0xFFFF, cs);
  drawPixel(130, 4, 0xFFFF, cs);
  drawPixel(140, 100, 0xFFFF, cs);
  drawPixel(150, 219, 0xFFFF, cs);
  drawPixel(160, 145, 0xFFFF, cs);
  drawPixel(170, 45, 0xFFFF, cs);
  drawPixel(180, 286, 0xFFFF, cs);
  drawPixel(190, 300, 0xFFFF, cs);
  drawPixel(200, 200, 0xFFFF, cs);
  drawPixel(210, 100, 0xFFFF, cs);
  drawPixel(220, 50, 0xFFFF, cs);
  drawPixel(230, 60, 0xFFFF, cs);
  drawPixel(114, 80, 0xFFFF, cs);
  drawPixel(45, 70, 0xFFFF, cs);
  drawPixel(207, 50, 0xFFFF, cs);
  drawPixel(66, 40, 0xFFFF, cs);
  drawPixel(42, 30, 0xFFFF, cs);
  drawPixel(154, 20, 0xFFFF, cs);
  drawPixel(222, 10, 0xFFFF, cs);

}

void initDisplay(int cs) {
  // SW Reset.
  writeCommand(0x01, cs);
  delay(150);

  // 0xEF, 3, 0x03, 0x80, 0x02,
  writeCommand(0xEF, cs);
  writeData(0x03, cs);
  writeData(0x80, cs);
  writeData(0x02, cs);

  // 0xCF, 3, 0x00, 0xC1, 0x30,
  writeCommand(0xCF, cs);
  writeData(0x00, cs);
  writeData(0xC1, cs);
  writeData(0x30, cs);

  // 0xED, 4, 0x64, 0x03, 0x12, 0x81,
  writeCommand(0xED, cs);
  writeData(0x64, cs);
  writeData(0x03, cs);
  writeData(0x12, cs);
  writeData(0x81, cs);

  // 0xE8, 3, 0x85, 0x00, 0x78,
  writeCommand(0xE8, cs);
  writeData(0x85, cs);
  writeData(0x00, cs);
  writeData(0x78, cs);

  // 0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  writeCommand(0xCB, cs);
  writeData(0x39, cs);
  writeData(0x2C, cs);
  writeData(0x00, cs);
  writeData(0x34, cs);
  writeData(0x02, cs);

  // 0xF7, 1, 0x20,
  writeCommand(0xF7, cs);
  writeData(0x20, cs);

  // 0xEA, 2, 0x00, 0x00,
  writeCommand(0xEA, cs);
  writeData(0x00, cs);
  writeData(0x00, cs);

  // ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
  writeCommand(0xC0, cs);
  writeData(0x23, cs);

  // ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  writeCommand(0xC1, cs);
  writeData(0x10, cs);

  // ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
  writeCommand(0xC5, cs);
  writeData(0x3E, cs);
  writeData(0x28, cs);

  // ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
  writeCommand(0xC7, cs);
  writeData(0x86, cs);

  // ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  writeCommand(0x36, cs);
  writeData(0x48, cs);

  // ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  writeCommand(0x37, cs);
  writeData(0x00, cs);

  // ILI9341_PIXFMT  , 1, 0x55,
  writeCommand(0x3A, cs);
  writeData(0x55, cs);

  // ILI9341_FRMCTR1 , 2, 0x00, 0x18,
  writeCommand(0xB1, cs);
  writeData(0x00, cs);
  writeData(0x18, cs);

  // ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  writeCommand(0x86, cs);
  writeData(0x08, cs);
  writeData(0x82, cs);
  writeData(0x27, cs);

  // 0xF2, 1, 0x00,                         // 3Gamma Function Disable
  writeCommand(0xF2, cs);
  writeData(0x00, cs);

  // ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
  writeCommand(0x26, cs);
  writeData(0x01, cs);

  // ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
  //   0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  writeCommand(0xE0, cs);
  writeData(0x0F, cs);
  writeData(0x31, cs);
  writeData(0x2B, cs);
  writeData(0x0C, cs);
  writeData(0x0E, cs);
  writeData(0x08, cs);
  writeData(0x4E, cs);
  writeData(0xF1, cs);
  writeData(0x37, cs);
  writeData(0x07, cs);
  writeData(0x10, cs);
  writeData(0x03, cs);
  writeData(0x0E, cs);
  writeData(0x09, cs);
  writeData(0x00, cs);

  // ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
  //   0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  writeCommand(0xE1, cs);
  writeData(0x00, cs);
  writeData(0x0E, cs);
  writeData(0x14, cs);
  writeData(0x03, cs);
  writeData(0x11, cs);
  writeData(0x07, cs);
  writeData(0x31, cs);
  writeData(0xC1, cs);
  writeData(0x48, cs);
  writeData(0x08, cs);
  writeData(0x0F, cs);
  writeData(0x0C, cs);
  writeData(0x31, cs);
  writeData(0x36, cs);
  writeData(0x0F, cs);

  // ILI9341_SLPOUT  , 0x80,                // Exit Sleep
  writeCommand(0x11, cs);
  delay(150);

  // ILI9341_DISPON  , 0x80,                // Display on
  writeCommand(0x29, cs);

  // 0x00                                   // End of list
  writeCommand(0x00, cs);

}
