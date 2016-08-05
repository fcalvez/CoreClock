#include "Global.h"
#include "Particle.h"
#include "SparkTime.h"
#include "Adafruit_LEDBackpack.h"
#include "spr_numbers.h"
#include "sprites.h"

// x screen size
#define MTX 24
// y screen size
#define MTY 8
// number of matrix composing the screen
#define NB_MATRIX MTX/8
// global brightness
#define LUM 1

// define matrixes
Adafruit_LEDBackpack matrix[NB_MATRIX];

// for time updates
UDP UDPClient;
SparkTime rtc;

// --- Graph lib ----

void clearScreen() {
	for (int m = 0; m<NB_MATRIX; m++) {
		matrix[m].clear();
	}
}

void showScreen() {
	for (int m = 0; m<NB_MATRIX; m++) {
		matrix[m].writeDisplay();
	}
}

// set or clear pixel
void setPixel(uint8_t x, uint8_t y, bool on) {
	if (x <0 || x >= MTX || y<0 || y >= MTY)
		return;

	uint8_t mx = x >> 3;
	uint8_t dx = x & 7;
	y = MTY - y - 1;

	if (on)
		if (y == 0)
			matrix[mx].displaybuffer[dx] |= 128;
		else
			matrix[mx].displaybuffer[dx] |= 1 << (y - 1);
	else
		if (y == 0)
			matrix[mx].displaybuffer[dx] &= ~(128);
		else
			matrix[mx].displaybuffer[dx] &= ~(1 << (y - 1));
}

/* more draw functions
void fillBox(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool on) {
int x;
for(;y0<=y1;y0++)
for(x=x0;x<=x1;x++)
setPixel(x,y0,on);
}

void drawBox(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool on) {
drawLine(x0,y0,x1,y0,on);
drawLine(x0,y1,x1,y1,on);
drawLine(x0,y0,x0,y1,on);
drawLine(x1,y0,x1,y1,on);
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool on) {
int16_t steep = abs(y1 - y0) > abs(x1 - x0);
if (steep) {
swap(x0, y0);
swap(x1, y1);
}

if (x0 > x1) {
swap(x0, x1);
swap(y0, y1);
}

int16_t dx, dy;
dx = x1 - x0;
dy = abs(y1 - y0);

int16_t err = dx / 2;
int16_t ystep;

if (y0 < y1) {
ystep = 1;
} else {
ystep = -1;
}

for (; x0<=x1; x0++) {
if (steep) {
setPixel(y0, x0, on);
} else {
setPixel(x0, y0, on);
}
err -= dy;
if (err < 0) {
y0 += ystep;
err += dx;
}
}
}
*/

// draw a sprite on screen buffer

void setSprite(uint8_t x0, uint8_t y0, uint8_t spriteIndex, uint8_t tx, uint8_t ty) {
	uint8_t sprite_line;
	for (uint8_t y = 0; y<ty; y++)
	{
		sprite_line = getSpriteLine(spriteIndex, y);
		for (uint8_t x = 0; x<tx; x++)
		{
			// setpixel takes care of writing data on the matrixes
			setPixel(x0 + tx - x - 1, y0 + y, sprite_line & (1 << x));
		}
	}
}

/* --- Tests ---
void test_draw0(){

matrix[0].displaybuffer[7] = 128;
matrix[0].displaybuffer[6] = 1;
matrix[0].displaybuffer[5] = 2;
matrix[0].displaybuffer[4] = 4;
matrix[0].displaybuffer[3] = 8;
matrix[0].displaybuffer[2] = 16;
matrix[0].displaybuffer[1] = 32;
matrix[0].displaybuffer[0] = 64;

showScreen();
}

void test_draw1() {
uint8_t x;
uint8_t y;

for(y=0; y<MTY; y++) {
for(x=0; x<MTX; x++) {
setPixel(x,y,true);
showScreen();
delay(25);
}
}
for(y=0; y<MTY; y++) {
for(x=0; x<MTX; x++) {
setPixel(x,y,false);
showScreen();
delay(25);
}
}
}

void test_draw2() {
uint8_t t;

for(t=0; t<8; t++){
drawBox(t,t,MTX-t-1,MTY-t-1,true);
showScreen();
delay(100);
}

for(t=0; t<8; t++){
drawBox(t,t,MTX-t-1,MTY-t-1,false);
showScreen();
delay(100);
}
}

void test_draw3() {
for(int j=0; j<21; j++){
for(int i=0; i<10; i++) {
clearScreen();
setSprite(j, 0, i, 4, 7);
showScreen();
delay(250);
}
}
}
*/

// first hour digit position

static const uint8_t chifh1x = 1;
static const uint8_t chifh1y = 1;
// second hour digit position
static const uint8_t chifh2x = 6;
static const uint8_t chifh2y = 1;
// first minute digit position
static const uint8_t chifm1x = 13;
static const uint8_t chifm1y = 1;
// second minute digit position
static const uint8_t chifm2x = 18;
static const uint8_t chifm2y = 1;
// separator position
static const uint8_t pointx = 11;
static const uint8_t pointy1 = 2;
static const uint8_t pointy2 = 6;
// seconds indicator
static const uint8_t pointsx = 23;

void drawTime() {
	unsigned long currentTime;
	uint8_t h, m, s;
	uint8_t c1, c2, c3, c4;
	int8_t spos;

	// get current time
	currentTime = rtc.now();
	h = rtc.hour(currentTime);
	m = rtc.minute(currentTime);
	s = rtc.second(currentTime);

	// extract digits
	c1 = h / 10;
	c2 = h % 10;
	c3 = m / 10;
	c4 = m % 10;

	// clear screen, even if it's should not be necessary
	clearScreen();

	// hour (first digit only drawn if ncessary)
	if (c1 > 0)
		setSprite(chifh1x, chifh1y, c1, 4, 8);
	else
		setSprite(chifh1x, chifh1y, 999, 4, 8);
	// second hour digit
	setSprite(chifh2x, chifh2y, c2, 4, 8);
	// minutes first digit
	setSprite(chifm1x, chifm1y, c3, 4, 8);
	// minutes second digit
	setSprite(chifm2x, chifm2y, c4, 4, 8);

	// separator
	setPixel(pointx, pointy1, true);
	setPixel(pointx, pointy2, true);

	// secondes
	spos = s * 9 / 61 - 1;
	setPixel(pointsx, MTY - spos - 1, true);

	// display it
	showScreen();
}

void setup() {
	// setup display
	matrix[0] = Adafruit_LEDBackpack();
	matrix[0].begin(0x74);
	matrix[0].setBrightness(LUM);
	matrix[1] = Adafruit_LEDBackpack();
	matrix[1].begin(0x72);
	matrix[1].setBrightness(LUM);
	matrix[2] = Adafruit_LEDBackpack();
	matrix[2].begin(0x71);
	matrix[2].setBrightness(LUM);
	clearScreen();
	showScreen();

	// setup time
	rtc.begin(&UDPClient, "north-america.pool.ntp.org");
	rtc.setTimeZone(-5); // gmt offset
}

void loop() {
	drawTime();
	delay(1000);
}

