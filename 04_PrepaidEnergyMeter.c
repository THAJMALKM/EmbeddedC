#include<pic.h>
#include"pic_lcd8.h
#include"pic_serial.h"
#define relay RC0

unsigned char a[8],i,eb=0,unit=0;
unsigned char bal,k,val;
bit m,u;

void init();
void enter();
void Mobile_send();

void interrupt serial()
{
	if(RCIF)
	{   
        RCIF=0;
		a[i]=RCREG;
        if(a[0]=='*')i++;
        else i=0;
	}
    if(INTF==1)  //INTCON.1
    {
	eb++;
    INTF=0;
    }	
} 

void main()
{
	TRISC=0X80;     
	TRISD=0X00;
    TRISB=0XFF; 
	Lcd8_Init();
	Serial_Init(9600);relay=1;
	Lcd8_Display(0x80,"GSM BASED WIRLS ",16);
	Lcd8_Display(0xC0,"  ENERGY BILLING",16);
	Delay(65000);Delay(65000);
    init();
    bal=EEPROM_READ(0X10);
    GIE=1;  //INTCON.7
    PEIE=1; //INTCON.6
	INTE=1;  //INTCON.4
    Lcd8_Display(0x80,"Cnt:    Unt:    ",16);
	Lcd8_Display(0xC0,"Bal:     -LdCON-",16);
	Receive(1);
	while(1)
	{
	   Lcd8_Decimal2(0x84,eb);
	   Lcd8_Decimal3(0x8C,unit);
	   Lcd8_Decimal3(0xc4,bal);
	   if(m==0)     {Lcd8_Display(0xC9,"-LdCON-",7);}
	   else if(m==1){Lcd8_Display(0xC9,"-LdCUT-",7);}
	   if(eb>=5)
	   {   
		   eb=0;
		   unit++;
		   bal=bal-2;
	   }
	   if(i>4)
	   {
		   Receive(0);						
		   if     (a[1]=='1'&&a[2]=='R'&&a[3]=='E'&&a[4]=='Q')        {k=1;Mobile_send();}
    	   else if(a[1]=='1'&&a[2]=='2'&&a[3]=='1'&&a[4]=='N'&&bal>0) {m=0;relay=1;      }
		   else if(a[1]=='1'&&a[2]=='2'&&a[3]=='1'&&a[4]=='F')        {m=1;relay=0;      }
           else if(a[1]=='C') 
           {
               Lcd8_Display(0xC0,"Recharged ......",16);
               val=(((a[2]-0x30)*100)+((a[3]-0X30)*10)+(a[4]-0x30));  Delay(5000);
               if((val+bal)<255) bal=bal+val;   
	           EEPROM_WRITE(0x10,bal);
	           bal=EEPROM_READ(0X10);
               Delay(65000);
               m=0;relay=1;
               Lcd8_Display(0xC0,"Bal:     -LdCON-",16);
           }
           else if(a[1]=='1'&&a[2]=='F'&&a[3]=='C'&&a[4]=='T')
           {
               Lcd8_Display(0x80,"FACTORY         ",16);
               Lcd8_Display(0xC0,"  RESET...Done  ",16);
               eb=0;unit=0;bal=0;   
	           EEPROM_WRITE(0x10,bal);
	           bal=EEPROM_READ(0X10);
               Delay(65000);
               Lcd8_Display(0x80,"Cnt:    Unt:    ",16);
	           Lcd8_Display(0xC0,"Bal:     -LdCON-",16);
           }
           i=0;
		   Receive(1);
	   }
       if     (bal<20&&u==0){u=1;k=2;Mobile_send();}
       else if(bal>20)      {u=0;                  }
       if     (bal==0)      {m=1;relay=0;          }
	}
}


void enter()
{
	Serial_Out(0x0d);
	Serial_Out(0x0a);
}

void init()
{
    Lcd8_Display(0x80,"GSM Initializing",16);
    Lcd8_Display(0xc0,">>Please Wait...",16);
	Serial_Conout("AT",2);
	Serial_Out(0x0d);Serial_Out(0x0a);Delay(65000);Delay(65000);
	Serial_Conout("AT+CMGF=1",9);
	Serial_Out(0x0d);Serial_Out(0x0a);Delay(65000);Delay(65000);
    Serial_Conout("AT+CNMI=2,2,0,0,0",17);
	Serial_Out(0x0d);Serial_Out(0x0a);Delay(65000);Delay(65000);
	Delay(65000);Delay(65000);
    Lcd8_Display(0x80,"Initializing Ovr",16);	
}

void Mobile_send()
{
	Receive(0);
	//init();Delay(65000); 
    Lcd8_Display(0x80,"Msg Snding...   ",16);
	Lcd8_Display(0xc0,"                ",16);
	Serial_Conout("AT+CMGS=",8);     		                     		
	Serial_Out('"');
    Serial_Conout("9539714361",10); Delay(5000);		
	Serial_Out('"');Delay(60000);
	Serial_Out(0x0d);Serial_Out(0x0a);	
   	Delay(60000);Delay(60000);
    if(k==1)
    {
		Serial_Conout("--Electric Bill Summery--",25);Delay(5000);
		Serial_Out(0x0d);Serial_Out(0x0a);Delay(300);
		Serial_Out(0x0d);Serial_Out(0x0a);Delay(300);
	    Serial_Conout("Consumer No:15360001",20);     Delay(5000);
		Serial_Out(0x0d);Serial_Out(0x0a);Delay(300);
		Serial_Out(0x0d);Serial_Out(0x0a);Delay(300);
		Serial_Conout("Unit    : ",10);        Delay(5000);
	    Serial_Out(unit%1000/100+0x30);   Delay(300);
		Serial_Out(unit%100/10+0X30);     Delay(300);
		Serial_Out(unit%10+0X30);         Delay(300);
	    Serial_Out(0x0d);Serial_Out(0x0a);Delay(300);	
	
		Serial_Conout("Amount  : ",10);     Delay(5000);	     
		Serial_Out(bal%1000/100+0x30);    Delay(300);
		Serial_Out(bal%100/10+0X30);      Delay(300);
	    Serial_Out(bal%10+0X30);          Delay(300);
		Serial_Out(0x0d);Serial_Out(0x0a);Delay(300);
		Serial_Out(0x0d);Serial_Out(0x0a);Delay(300);
	
		Serial_Conout("---End of text Message---",25);Delay(5000);
    }
    else if(k==2)
    {
        Serial_Conout("Consumer No:15360001",20);     Delay(300);
        Serial_Out(0x0d);Serial_Out(0x0a);            Delay(300);
        Serial_Out(0x0d);Serial_Out(0x0a);            Delay(300);
    	Serial_Conout("----Low balance-----",20);     Delay(300);
        Serial_Out(0x0d);Serial_Out(0x0a);            Delay(300);
        Serial_Conout("Please Recharge your",20);     Delay(300);
        Serial_Out(0x0d);Serial_Out(0x0a);            Delay(300);
        Serial_Conout("  system to avoid   ",20);     Delay(300);
        Serial_Out(0x0d);Serial_Out(0x0a);            Delay(300);
        Serial_Conout("   disconnection... ",20);     Delay(300);
        Serial_Out(0x0d);Serial_Out(0x0a);            Delay(300);
 
    }
    Serial_Out(0x1a);
	Delay(50000);Delay(50000); Delay(50000);Delay(50000); 
	//Serial_Out(0x0d);Serial_Out(0x1a);
	Lcd8_Display(0x80,"sending over....",16);
	Delay(50000);Delay(50000);k=0;
	Lcd8_Display(0x80,"                ",16);
    Lcd8_Display(0xc0,"                ",16);
    Lcd8_Display(0x80,"Cnt:    Unt:    ",16);
	Lcd8_Display(0xC0,"Bil:            ",16);
    Receive(1);
}
