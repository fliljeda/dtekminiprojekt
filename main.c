#include <pic32mx.h>
#include <stdint.h>
#include "header.h"

#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9


#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200


char textbuffer[4][16];
//Name for highscore
char first[6];
char second[6];
char third[6];
//score for highscore
int firstscore;
int secondscore;
int thirdscore;
//current players name
char currentplayer[] = "______";
//last players name
char lastplayer[] = "______";
//used for cycling through possible characters in name
char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
//the timer
int time = 0;
//RNG CARRIES
int m_z = 1;
int m_w = 1;
//length of game
int lengthofgame = 30000; //30 second game
//number of leds (moles)
int ledcount = 0;
int ledmax = 25;
//The currentscore and current average score
int currentscore = 0;
int currentavg = 0;

static const uint8_t const font[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};

const uint8_t const icon[] = {
	255, 255, 255, 255, 255, 255, 127, 187,
	68, 95, 170, 93, 163, 215, 175, 95,
	175, 95, 175, 95, 223, 111, 175, 247,
	59, 237, 242, 254, 171, 254, 1, 255,
	255, 255, 15, 211, 109, 58, 253, 8,
	178, 77, 58, 199, 122, 197, 242, 173,
	242, 237, 186, 215, 40, 215, 41, 214,
	35, 175, 91, 212, 63, 234, 149, 111,
	171, 84, 253, 252, 254, 253, 126, 184,
	195, 52, 201, 22, 225, 27, 196, 19,
	165, 74, 36, 146, 72, 162, 85, 8,
	226, 25, 166, 80, 167, 216, 167, 88,
	106, 149, 161, 95, 135, 91, 175, 87,
	142, 123, 134, 127, 134, 121, 134, 121,
	132, 59, 192, 27, 164, 74, 177, 70,
	184, 69, 186, 69, 254, 80, 175, 217,
};

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

void delayms(int ms){
	int i;
	ms = ms*8000;
	for(i = ms; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 0x01));
	return SPI2BUF;
}

void display_init() {
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	delay(10);
	DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
	delay(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
	delay(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}


/* Interrupt Service Routine */
void user_isr( void )
{
  IFS(0) &= ~0x100; //reset flag
  time++;
}

void timer_init(){
  //Interrupt
  IPCSET(2) = 0x001f; // set priority 7 and sub to 3
  IFSCLR(0) = 0x00100; //clear possible flag
  IECSET(0) = 0x00100; //enable timer interrupt
  enable_interrupt();
  TRISD = (TRISD | 0x00fe0); // prepare input
  PR2 = 40000; //When to send flag
  T2CON = 0x0; //Reset settings
  TMR2 = 0x0; // Reset timer
  T2CONSET = 0x8010;
}

void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

void display_image(int x, const uint8_t *data) {
	int i, j;
	
	for(i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));
		
		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
		
		for(j = 0; j < 32; j++)
			spi_send_recv(~data[i*32 + j]);
	}
}

void display_update() {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

void led_init(){
	TRISE = TRISE & ~0x8; //LED pin 29:26
}

//LIGHTS THE LED at SPECIFIED NUMBER
void light_led(int nr){
	int i = 1;
	int lednr = 1;
	for(i; i < nr; i++){
		lednr = 2 * lednr;
	}
	PORTE = PORTE | lednr;
}

//SHUTS THE SPECIFIED LIGHT, SPECIFIED WITH NUMBER
void shut_led(int nr){
	int i = 1;
	int lednr = 1;
	for(i; i < nr; i++){
		lednr = 2 * lednr;
	}
	PORTE = PORTE & ~lednr;
}

void buttons_init(){
	TRISD = TRISD | 0xe0; //BTN 4,3,2
	TRISF = TRISF | 0x2; //BTN 1
}

//RETURNS 1 IF THAT BUTTONS IS PRESSED
int getbtn(int nr){
	if(nr == 1){
		return (PORTF >> 1) & 0x1;
	}else{
		return (PORTD >> (nr+3)) & 0x1;
	}
}

//ONE is first quarter, second second, etc
void stringPrinter(char one[], char two[], char three[], char four[]){
	display_string(0, one);
	display_string(1, two);
	display_string(2, three);
	display_string(3, four);
	display_update();
}

char checkdigit(int i){
	if(i == 9){
		return '9';
	}else if(i == 8){
		return '8';
	}else if(i == 7){
		return '7';
	}else if(i == 6){
		return '6';
	}else if(i == 5){
		return '5';
	}else if(i == 4){
		return '4';
	}else if(i == 3){
		return '3';
	}else if(i == 2){
		return '2';
	}else if(i == 1){
		return '1';
	}else if(i == 0){
		return '0';
	}
}

//MAKES and int to a string
void intToString(int value, char *s){
	s[4] = checkdigit(value%10);
	value /= 10;
	s[3] = checkdigit(value%10);
	value /= 10;
	s[2] = checkdigit(value%10);
	value /= 10;
	s[1] = checkdigit(value%10);
	value /= 10;
	s[0] = checkdigit(value%10);
}

//RESET PLAYERNAME
void resetCurrentPlayer(){
	int i;
	for(i=0;i<6;i++){
		currentplayer[i] = '_';
	}
}

//CHOOSES THE NAME
int choosename(){	
	resetCurrentPlayer();
	int i = 0;
	for(i; i<6; i++){
		if(i == 0){
			currentplayer[i] = 'A';
		}else{
			currentplayer[i] = currentplayer[i-1];
		}
		int currentchar = 0;
		stringPrinter(currentplayer, "", "", "(L)(R)(E)(B)");
		//WHILE NEXTBUTTON IS NOT PRESSED
		while(!getbtn(2)){
			if(getbtn(4)){
				while(getbtn(4));
				currentchar--;
				if(currentchar < 0){
					currentchar = 26; //size of alphabet array
				}
				currentplayer[i] = alphabet[currentchar];
				stringPrinter(currentplayer, "", "", "(L)(R)(E)(B)");
			}else if(getbtn(3)){
				while(getbtn(3));
				currentchar++;
				if(currentchar > 26){
					currentchar = 0;
				}
				currentplayer[i] = alphabet[currentchar];
				stringPrinter(currentplayer, "", "", "(L)(R)(E)(B)");
			}else if(getbtn(1)){
				while(getbtn(1));
				i = 6;
				return 0;
			}
		}
		while(getbtn(2));//wait for btn2 to be released
	}
	return 1;
}

void setFirst(char player[6], int score){
	setSecond(first, firstscore);
	firstscore = score;
	int i;
    for(i = 0;i < 6; i++){
    	first[i] = player[i];
    }
}

void setSecond(char player[6], int score){
	setThird(second, secondscore);
	secondscore = score;
	int i;
    for(i = 0;i < 6; i++){
    	second[i] = player[i];
    }
}

void setThird(char player[6], int score){
	thirdscore = score;
	int i;
    for(i = 0;i < 6; i++){
    	third[i] = player[i];
    }
}

//Uses the timer to delay
void timerdelay(int n){
	int timemark = time;
	while(time - timemark < n);
}

void startDelay(){
	int timemark = time;
	char delaydisp[] = "GAME STARTS IN:";

	stringPrinter(delaydisp, "", "      3       ", "");
	timerdelay(1000);

	stringPrinter(delaydisp, "", "      2       ", "");
	timerdelay(1000);

	stringPrinter(delaydisp, "", "      1       ", "");
	timerdelay(1000);
}

//Multiple with carry, created by George Marsagliaa
unsigned int rng(){
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

int nextRngInt(int i){
	return rng()%i;
}

//DOES THIS LED
void doThisLed(int nextled, int starttime){
	int ledscore = 1000;
	//wait for the perfect opportunity to light the eld
	timerdelay(starttime - time);
	light_led(nextled);
	while(getbtn(nextled)){ //wait for button to be released
		//if held down to long it shouldnt interrupt the game
		if(time-starttime > 1000){
			ledcount++;
			currentavg = currentscore/ledcount;
			shut_led(nextled);
			return;
		}
	}
	//the real waiting loop, breaks if 1000ms passes or presses the correct
	//button
	while(!getbtn(nextled)){
		if(time-starttime > 1000){
			ledcount++;
			currentavg = currentscore/ledcount;
			shut_led(nextled);
			return;
		}
	}
	shut_led(nextled);
	ledscore = ledscore - (time-starttime);
	currentscore += ledscore;
	ledcount++;
	currentavg = currentscore/ledcount;
}

//FORMATS FOR INGAME SCORE DISPLAY-- n is for where to start putting chars
void gameScoreFormat(int n, int times, char *s, int score){
	char stringscore[times];
	intToString(score, stringscore);
	int i;
	for(i = n; i<n+times; i++){
		s[i] = stringscore[i-n];
	}
}
//FORMATS FOR INGAME SCORE DISPLAY-- n is for where to start putting chars
void gameNameFormat(int n, int times, char *s, char *name){
	int i;
	for(i = n; i<n+times; i++){
		s[i] = name[i-n];
	}
}

void lastScore(){
	char lastscore[15] = "    00000      ";
	char lastplayername[15] = "   ______:    ";
	gameScoreFormat(4,5, lastscore, currentscore);
	gameNameFormat(3,6, lastplayername, lastplayer);
	stringPrinter(" Last score", lastplayername, lastscore, "( )( )( )(B)");
	while(!getbtn(1));
	while(getbtn(1));
}

//STARTS THE GAME
void startGame(){
	currentscore = 0;
	ledcount = 0;
	int gamestart = time; //start of game
	char score[15] = "    00000      ";
	char avg[15] = "    00000      ";
	stringPrinter("CURRENT SCORE:", score, "AVERAGE SCORE:", avg);
	while(ledcount < ledmax){
		int nextled = nextRngInt(4) +1;
		int nexttime = (time + (nextRngInt(2)+1)*500)-(ledcount*10*(nextRngInt(3)+1));
		doThisLed(nextled, nexttime);
		gameScoreFormat(4, 5, score, currentscore);
		gameScoreFormat(4, 5, avg, currentavg);
		stringPrinter("CURRENT SCORE:", score, "AVERAGE SCORE:", avg);
	}
	if(currentscore > firstscore){
		setFirst(currentplayer, currentscore);
	}else if(currentscore > secondscore){
		setSecond(currentplayer, currentscore);
	}else if(currentscore > thirdscore){
		setThird(currentplayer, currentscore);
	}
}

void showScore(){
	int timestart = time;
	char finalscore[15] = "    00000      ";
	gameScoreFormat(4, 5,finalscore, currentscore);
	stringPrinter(" FINAL SCORE", "", finalscore, "");
	while(time - timestart < 4000);
}

//THE PLAY FUNCTION
void play(){
	//CHoose name returns 1 if name has been selected
	if(choosename()){
		startDelay(); //gives the play a bit of time before game starts
		startGame();
		showScore(); //shows score at end of game
		int i;
		for(i = 0; i <6; i++){
			lastplayer[i] = currentplayer[i];
		}
	}
}

//THE HIGHSCORE SCREEN
void highscore(){
	//Format
	char dispfirst[14] = "1       -     ";
	char dispsecond[14] = "2       -     ";
	char dispthird[14] = "3       -     ";
	//put the name in 3:7 
	int i = 2;
	for(i; i < 8; i++){
		dispfirst[i] = first[i-2];
		dispsecond[i] = second[i-2];
		dispthird[i] = third[i-2];
	}
	i+=1;
	char firstscorestring[5];
	char secondscorestring[5];
	char thirdscorestring[5];
	//Nothing is displayed if it has 0 in score
	if(firstscore != 0){
		intToString(firstscore, firstscorestring);
	}
	if(secondscore != 0){
		intToString(secondscore, secondscorestring);
	}
	if(thirdscore != 0){
		intToString(thirdscore, thirdscorestring);
	}
	//sets the score at position 9:13
	for(i; i<14; i++){
		dispfirst[i] = firstscorestring[i-9];
		dispsecond[i] = secondscorestring[i-9];
		dispthird[i] = thirdscorestring[i-9];
	}

	//pressing back
	while(!getbtn(1)){
		stringPrinter(dispfirst, dispsecond, dispthird, "( )( )( )(B)"); 
	}
	while(getbtn(1));
}

//THE STARTMENU
void startmenu(){
	while(1){
		char playbutton[] = "Play        "; //DIFFERENT BUTTONS
		char highscorebutton[] = "Highscore   ";
		char lastscorebutton[] = "Last score  ";
		int cursor = 0;						// WHERE THE CURSOR IS 0indexed with 0 at top
		int cursorcolumn = 11; //where the < will be in the string
		char cursorchar = '<';
		playbutton[cursorcolumn] = cursorchar;
		stringPrinter(playbutton, highscorebutton, lastscorebutton,  "(U)(D)(G)( )");
		while(!getbtn(2)){					// BUTTON 2 
			if(getbtn(4)){
				while(getbtn(4));
				cursor -= 1;
				if(cursor < 0){
					cursor = 2;
				}
			}else if(getbtn(3)){
				while(getbtn(3));
				cursor += 1;
				if(cursor > 2){
					cursor = 0;
				}
			}
			if(cursor == 0){
				playbutton[cursorcolumn] = cursorchar;
				lastscorebutton[cursorcolumn] = ' ';
				highscorebutton[cursorcolumn] = ' ';
			}else if(cursor == 1){
				playbutton[cursorcolumn] = ' ';
				lastscorebutton[cursorcolumn] = ' ';
				highscorebutton[cursorcolumn] = cursorchar;
			}else if(cursor == 2){
				playbutton[cursorcolumn] = ' ';
				highscorebutton[cursorcolumn] = ' ';
				lastscorebutton[cursorcolumn] = cursorchar;
			}
			stringPrinter(playbutton, highscorebutton, lastscorebutton,  "(U)(D)(G)( )");
		}
		while(getbtn(2)); //wait for btn2 release
		if(cursor == 0){
			play();
		}else if(cursor == 1){
			highscore();
		}else if(cursor == 2){
			lastScore();
		}
	}
}

int main(void) {
	/* Set up peripheral bus clock */
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	
	/* Clear SPIROV*/
	SPI2STATCLR &= ~0x40;
	/* Set CKP = 1, MSTEN = 1; */
        SPI2CON |= 0x60;
	
	/* Turn on SPI */
	SPI2CONSET = 0x8000;
	
	char* highscore = "HIGHSCORE";
	timer_init();
	display_init();
	buttons_init();
	led_init();
	
	startmenu();
   // int i = 0;
   // uint8_t myicon[128];
   // for(i; i < 128; i++){
   // 	myicon[i] = 0;
   // }
	//display_image(96, icon);
	return 0;
}

