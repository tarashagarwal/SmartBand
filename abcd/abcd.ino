//#include<GSM.h>
//#include<SoftwareSerial.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,4,5,6,7);
char RcvdMsg[200] = "";
int RcvdCheck = 0;
int RcvdConf = 0;
int index = 0;
int RcvdEnd = 0;
char MsgMob[15];
char MsgTxt[50];
int MsgLength = 0;

void Response()//checks the compatablity of GSM 
{
int count = 0;
Serial.println();
while(1)
{
if(Serial.available())
{
char data =Serial.read();
if(data == 'K'){Serial.println("OK");break;}
if(data == 'R'){Serial.println("GSM Not Working");break;}
}
count++;
delay(10);
if(count == 1000){Serial.println("GSM not Found");break;}
}
}

void setup()
{

lcd.begin(16, 2);
lcd.print("WAITING...");
delay(2000);
Serial.begin(9600);
delay(1000);
//Serial.print("ATE0\r");
//Response();
Serial.print("AT\r");
Response();
//Serial.print("AT+CMGF=1\r");
//Response();
//Serial.print("AT+CNMI=1,2,0,0,0\r");
//Response();
//lcd.begin(16, 2);
pinMode(13,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
}
//<strong>void loop()</strong>
void loop()
{
//lcd.setCursor(0, 0);
if(Serial.available())
{
char data = Serial.read();
if(data == '+'){RcvdCheck = 1;}
if((data == 'C') && (RcvdCheck == 1)){RcvdCheck = 2;}
if((data == 'M') && (RcvdCheck == 2)){RcvdCheck = 3;}
if((data == 'T') && (RcvdCheck == 3)){RcvdCheck = 4;}
if(RcvdCheck == 4){RcvdConf = 1; RcvdCheck = 0;}

//****************************************************************************************************************************************************************************

if(RcvdConf == 1)
{
if(data == '\n'){RcvdEnd++;}
if(RcvdEnd == 3){RcvdEnd = 0;}
RcvdMsg[index] = data;
index++;
if(RcvdEnd == 2){RcvdConf = 0;MsgLength = index-2;index = 0;}
if(RcvdConf == 0)
{
//Serial.print("Mobile Number is: ");
//Serial.println(RcvdMsg);
for(int x = 4;x &17;x++)
{
  
MsgMob[x-4] = RcvdMsg[x];
//Serial.print(MsgMob[x-4]);
}
Serial.println();
Serial.print("Message Text: ");
for(int x = 46;x &MsgLength;x++)
{
MsgTxt[x-46] = RcvdMsg[x];
Serial.print(MsgTxt[x-46]);
/*if(Serial.available())
{
 char d=Serial.read();
  lcd.print(d);
}*/
//lcd.print(MsgTxt[x-46]);
}
Serial.println();
if(MsgTxt[0]=='s' && MsgTxt[1]=='t' && MsgTxt[2]=='a')
{
Serial.print("led start");
digitalWrite(13,HIGH); 
digitalWrite(8,LOW);
digitalWrite(9,HIGH);
digitalWrite(10,LOW);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("forward");
delay(1000);
}
else if(MsgTxt[0]=='s' && MsgTxt[1]=='t' && MsgTxt[2]=='o')
{
Serial.print("led stop");
digitalWrite(13,LOW); 
digitalWrite(8,LOW);
digitalWrite(9,LOW);
digitalWrite(10,LOW);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("STOP");
delay(1000);
}
else if(MsgTxt[0]=='b' && MsgTxt[1]=='a' && MsgTxt[2]=='c')
{
Serial.print("led back");
digitalWrite(13,LOW); 
digitalWrite(8,HIGH);
digitalWrite(9,LOW);
digitalWrite(10,HIGH);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Moving Back");
delay(1000);
}
else if(MsgTxt[0]=='r' && MsgTxt[1]=='i' && MsgTxt[2]=='g')
{
Serial.print("led right");
digitalWrite(13,HIGH); 
digitalWrite(8,LOW);
digitalWrite(9,LOW);
digitalWrite(10,LOW);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Moving Right");
delay(1000);
}
else if(MsgTxt[0]=='l' && MsgTxt[1]=='e' && MsgTxt[2]=='f')
{
Serial.print("led left");
analogWrite(13,LOW); 
digitalWrite(8,LOW);
digitalWrite(9,HIGH);
digitalWrite(10,LOW);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Moving Left");
delay(1000);
}
//Serial.println();
}
}
}
Serial.flush();
}

