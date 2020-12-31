#include<reg52.h>
void msdelay(unsigned int);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);

#define lcd P2	//define LCD port
sbit rs = P1^0;
sbit rw = P1^1;
sbit en = P1^2;

void main(void)
{
	unsigned char str[] = "TEST LCD";
	unsigned char i;
	lcd_cmd(0x38); // initializing LCD
	msdelay(10);
	lcd_cmd(0x01);	// Clear display
	msdelay(10);
	lcd_cmd(0x0E);	// Display ON cursor blinking
	msdelay(10);
	lcd_cmd(0x82);	// Setting cursor position
	msdelay(10);
	
	while(str[i] != '\0')
	{
		lcd_data(str[i]);
		msdelay(100);
		i++;
	}
	i=0;

}

void msdelay(unsigned int t)
{
	unsigned int i,j;
	for(i = 0; i < t; i++)
	{
		for(j = 0; j < 1275; j++);
	}
}

void lcd_cmd(unsigned char c)
{
	lcd = c;
	rs = 0;	// selecting command register of LCD
	rw = 0;	// selecting write option
	en = 1;
	msdelay(10);
	en = 0;
}

void lcd_data(unsigned char d)
{
	lcd = d;
	rs = 1;	// selecting data register of LCD
	rw = 0;
	en = 1;
	msdelay(10);
	en = 0;
}