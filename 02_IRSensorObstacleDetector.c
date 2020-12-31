/*********************************************
*Program for IR sensor obstacle detector
*When obstacle present, input will be high and LED should blink alternatively
*Else half LED should be blinking	
*********************************************/

#include<reg52.h>

void msdelay(unsigned int t);

void main(void)
{
	unsigned int a;
	while(1)
	{
		a = P1 & 0x01;	/* Check status of sensor, get data from P1^0 only */
		if(a == 0x01)
		{
			P2 = 0x55;	/* P2 port is set as 0101 0101 */
			msdelay(100);
			P2 = 0xAA;	/* P2 port is set as 1010 1010 */
			msdelay(100);
		}
		else
		{
			P2 = 0x0F;
			msdelay(100);
			P2 = 0xF0;
			msdelay;
		}
		
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