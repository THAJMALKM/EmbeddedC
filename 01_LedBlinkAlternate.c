/*********************************************
*Program to blink LEDs alternatively	
*********************************************/

#include<reg52.h>

void msdelay(unsigned int t);

void main(void)
{
	while(1)
	{
		P2 = 0x55;	/* P2 port is set as 0101 0101 */
		msdelay(100);
		P2 = 0xAA;	/* P2 port is set as 1010 1010 */
		msdelay(100);
	}
}

void msdelay(unsigned int t)
{
	unsigned int i,j;
	
	for(i=o; i<t; i++
	{
		for(j=0; j<1275; j++)	/* Set this to necessary value */
	}
	
}