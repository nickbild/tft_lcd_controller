int dc = 9;
int cs = 10;
int mosi = 11;
int reset = 12;
int clk = 13;

void setup() {
  pinMode(dc, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(mosi, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(reset, OUTPUT);

  initDisplay();
}

void loop() {
  drawPixel(100, 100, 0xFB60);
  drawPixel(50, 50, 0x0000);
  drawPixel(51, 50, 0x0000);
  drawPixel(52, 50, 0x0000);
  drawPixel(53, 50, 0x0000);
  drawPixel(54, 50, 0x0000);
  drawPixel(55, 50, 0x0000);

  delay(5000);
}

void writeCommand(uint8_t data) {
  digitalWrite(dc, LOW);

  for (int bit=7; bit>=0; bit--) {
    digitalWrite(mosi, bitRead(data, bit));
    digitalWrite(clk, HIGH);
    digitalWrite(clk, LOW);
  }

  digitalWrite(dc, HIGH);
}

void writeData(uint8_t data) {
  for (int bit=7; bit>=0; bit--) {
    digitalWrite(mosi, bitRead(data, bit));
    digitalWrite(clk, HIGH);
    digitalWrite(clk, LOW);
  }
}

void writeData16(uint16_t data) {
  for (int bit=15; bit>=0; bit--) {
    digitalWrite(mosi, bitRead(data, bit));
    digitalWrite(clk, HIGH);
    digitalWrite(clk, LOW);
  }
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color) {
  digitalWrite(cs, LOW);

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

  digitalWrite(cs, HIGH);
}

void initDisplay() {
  digitalWrite(dc, HIGH);
  digitalWrite(mosi, LOW);
  digitalWrite(clk, LOW);

  digitalWrite(reset, HIGH);
  delay(100);
  digitalWrite(reset, LOW);
  delay(100);
  digitalWrite(reset, HIGH);
  delay(200);

  digitalWrite(cs, LOW);

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

  digitalWrite(cs, HIGH);
}
