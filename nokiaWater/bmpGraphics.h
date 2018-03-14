// ------ WEATHER
static const unsigned char PROGMEM moon_bmp[] =
{ B01110000,
  B11001000,
  B00100100,
  B00100100,
  B01000100,
  B10001000,
  B01110000};


// ------ GHOSTS
static const unsigned char PROGMEM ghost_right_bmp[] =
{ B00011110,
  B00010001,
  B00100101,
  B00100001,
  B01000110,
  B11111000};

static const unsigned char PROGMEM ghost_left_bmp[] =
{ B01111000,
  B10001000,
  B10100100,
  B10000100,
  B01100010,
  B00011111};



// ------ MONSTERS
// wilted plant
static const unsigned char PROGMEM mon_wilt_bmp[] =
{ B00111100, B00000000,
  B01100110, B00000000,
  B01110010, B00000000,
  B11110001, B00000000,
  B11100001, B00000000,
  B01100001, B00000000,
  B00011111, B11100000,
  B01110001, B00111000,
  B11100000, B00001110,
  B11000000, B00000110 };
  
// shared stem
static const unsigned char PROGMEM mon_stem_bmp[] =
{ B11100000,
  B01000000,
  B01000000,
  B01000000};

// shared bulb
static const unsigned char PROGMEM mon_bulb_bmp[] =
{ B00100000,
  B01110000,
  B11111000,
  B11111000,
  B01110000,
  B00100000};

// ----- Death Stalk

// seed, all seeds are 7x6
 static const unsigned char PROGMEM mon_seed_1_bmp[] =
{ B00000000,
  B00000000,
  B00111000,
  B01101100,
  B00101000,
  B01010100};
  
static const unsigned char PROGMEM mon_leaf_1_bmp[] =
{ B10000000, B00000000, B00100000,
  B01111110, B00001111, B11000000,
  B01111111, B00011111, B11000000,
  B00111111, B11111111, B10000000,
  B00011110, B00001111, B00000000 };

//segments
 static const unsigned char PROGMEM mon_seg_1_bmp[] =
{ B00111000,
  B01010100};

// flowers: 11 x 10
static const unsigned char PROGMEM mon_flower_1_bmp[] =
{ B00000100, B00000000,
  B00001010, B00000000,
  B00011011, B00000000,
  B00101110, B10000000,
  B01010101, B01000000,
  B00111111, B10000000,
  B00101010, B10000000,
  B00110101, B10000000,
  B00011011, B00000000,
  B00100100, B10000000 };

// fill for skill flower 8 x 10
static const unsigned char PROGMEM mon_flower_fill_1_bmp[] =
{ B00000000,
  B00000100,
  B00000100,
  B00000000,
  B00000000,
  B00000000,
  B00010101,
  B00001010,
  B00000100,
  B00000000 };


 // ----- Desert Wheat

// seed, all seeds are 7x6
 static const unsigned char PROGMEM mon_seed_2_bmp[] =
{ B00101000,
  B00010000,
  B00010000,
  B00101000,
  B00101000,
  B00010000 };

// leaf 15 x 4
static const unsigned char PROGMEM mon_leaf_2_bmp[] =
{ B11000000, B00000110,
  B11100000, B00001110,
  B01110000, B00011100,
  B00011111, B11110000 };

//segments 7 x 2
 static const unsigned char PROGMEM mon_seg_2_bmp[] =
{ B10010010,
  B01010100 };

// wheat flower: 11 x 5
static const unsigned char PROGMEM mon_flower_2_bmp[] =
{ B00110101, B10000000,
  B00001010, B00000000,
  B00000100, B11000000,
  B10010101, B00100000,
  B00001110, B00000000 };

// fill is a single pixel entered manully in the draw loop

 // ----- Towering Shroom

// seed, all seeds are 7x6
 static const unsigned char PROGMEM mon_seed_3_bmp[] =
{ B00000000,
  B00010000,
  B00101000,
  B01000100,
  B01111100,
  B00010000 };

//leaf 7 x 2
 static const unsigned char PROGMEM mon_leaf_3_bmp[] =
{ B10000010,
  B01000100 };


//segments 3 x 2
 static const unsigned char PROGMEM mon_seg_3_bmp[] =
{ B11100000,
  B01000000 };

// mushroom: 11x6
static const unsigned char PROGMEM mon_flower_3_bmp[] =
{ B00011111, B00000000,
  B00110011, B10000000,
  B01111001, B01000000,
  B01111001, B01000000,
  B10110011, B10100000,
  B11111111, B11100000 };

static const unsigned char PROGMEM mon_flower_fill_3_bmp[] =
{ B00000000, B00000000,
  B00001100, B00000000,
  B00000110, B10000000,
  B00000110, B10000000,
  B01001100, B01000000,
  B00000000, B00000000 };



 // ----- Billzardy Snowshrooms

// seed, all seeds are 7x6
 static const unsigned char PROGMEM mon_seed_4_bmp[] =
{ B00010000,
  B00101000,
  B01000100,
  B00101000,
  B00010000,
  B00010000 };


// leaf 15 x 13
static const unsigned char PROGMEM mon_leaf_4_bmp[] =
{ B10000000, B00000010,
  B10000000, B00000010,
  B11000000, B00000110,
  B11000000, B00000110,
  B11100000, B00001110,
  B01110000, B00011100,
  B01111100, B01111100,
  B00111110, B11111000,
  B00011000, B00110000,
  B00110000, B00011000,
  B00110000, B00011000,
  B00100000, B00001000,
  B01100000, B00001100 };


// 9 x 4
static const unsigned char PROGMEM mon_seg_4_bmp[] =
{ B00001000, B00000000,
  B01001001, B00000000,
  B10111110, B10000000,
  B01101011, B00000000 };

// segments for mon 4 have fills manually coded as 2 pixels

// blizardy puff flower: 11 x 9
static const unsigned char PROGMEM mon_flower_4_bmp[] =
{ B00110000, B00000000,
  B01001000, B00000000,
  B01001000, B00000000,
  B00110100, B00000000,
  B00000100, B11000000,
  B01100101, B00100000,
  B10010101, B00100000,
  B10011110, B11000000,
  B01100100, B00000000 };

// blizardy puff flower fill: 11 x 9
static const unsigned char PROGMEM mon_flower_fill_4_bmp[] =
{ B00000000, B00000000,
  B00110000, B00000000,
  B00110000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B11000000,
  B01100000, B11000000,
  B01100000, B00000000,
  B00000000, B00000000 };

