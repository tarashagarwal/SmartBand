/*
   RS 3
   D4 5
   D5 6
   D6 7
   D7 9
*/
#include <DHT11.h>
#include<LiquidCrystal.h>
#include<VirtualWire.h>


char *devicePassword="B1";


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(3, A4, 5, 6, 7, 9);




//pin variables here
const int tempSensor = A5;
const int vibrator=10;
const int buzzer=11;
const int led=13;

//button setup
const int button1 = 12;
const int button2 = 2;



//paedometer variables here

const int xpin=A1;
const int ypin=A2;
const int zpin=A7;
float threshhold=80.0;
float xval[100]={0};
float yval[100]={0};
float zval[100]={0};
volatile float xavg;
volatile float yavg;
volatile float zavg;
volatile int steps,flag=0;
volatile long int countSteps=0;

//program variables here

 float temp = 0, humi = 0;
int mode = 1; //0 for normal,1 for silent,2 from meeting
 boolean mode_flag = false;
 String voice="";
 boolean pedo_flag=false;
 int buzzVolume=150;
 int vibrVolume=200;
 int output=buzzer;
boolean trans_flag=true;
boolean delay_flag=false;


const int transPin=8;

//program functions declaration
char profile(int);
void extProfile(int);
void shout(int);
void alarm(void);
void calibrate();

void pciSetup(byte pin)
{
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

DHT11 dht11(tempSensor); //setting up temperature sensor pin


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);


  //setting up buttons
  pinMode(A3,OUTPUT);
  pinMode(4,OUTPUT);
 digitalWrite(A3,LOW);
digitalWrite(4, HIGH);

//  pciSetup(button1);
//  pciSetup(button2);

  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  
 
//setting up output devices
pinMode(buzzer,OUTPUT);
pinMode(vibrator,OUTPUT);
pinMode(led,OUTPUT);

analogWrite(buzzer,255);
analogWrite(vibrator,255);
analogWrite(led,255);

pinMode(transPin,OUTPUT);

//setting up accelerometers pins
//pinMode(xpin,INPUT);
//pinMode(ypin,INPUT);
//pinMode(zpin,INPUT);

//setting up transmitter
vw_set_ptt_inverted(true); //
vw_set_tx_pin(transPin);
vw_setup(4000);
// speed of data transfer Kbps

  //displaying setting up text
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setting up");
  for (int i = 0; i < 10; i++)
  {
    lcd.print(".");
    delay(300);
  }
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calbrtin Devices");
  calibrate();
  lcd.setCursor(0,1);

  for (int i = 0; i < 10; i++)
  {
    lcd.print(".");
    delay(300);
  }
   lcd.clear();



}


/*************************************************************************Main loop here*************************************************************/
void loop()
{
//transmission code here

if(trans_flag==true)
{
  analogWrite(vibrator,255);
vw_send((uint8_t *)devicePassword, strlen(devicePassword));
analogWrite(vibrator,255);


vw_wait_tx(); // Wait until the whole message is gone
}
  
  char c;
  while(Serial.available()) //if text is available through bluetooth
  {
    delay(10);//makes things stable
    
    if(c=Serial.read()!='#')
    voice+=c;
    else {
      break;
    }
  }

  if(voice.length()>0)
  {
    Serial.println(voice);
    
  }
  
int e = dht11.read(humi,temp);


  
  lcd.setCursor(0, 0);
  lcd.print("TEMP:");
  lcd.setCursor(5,0);
  lcd.print((int)temp);
  lcd.print("\337C");
  lcd.setCursor(11,0);
  lcd.print("TRA:");
  if(trans_flag==true)
  lcd.print("A");
  else
  lcd.print("D");
  
  //Serial.print(temp);
  delay(100);
  lcd.setCursor(0,1);
  lcd.print("HUMI:");
  lcd.print((int)humi);
  lcd.print("%");
  lcd.setCursor(11,1);
  lcd.print("MOD:");
  //Serial.println("Here");
  lcd.print(profile(mode));
  if(delay_flag==true)
  {
    delay_flag=false;
   
  delay(500);
   lcd.clear();
  }
/***********************************************************************************No proble below this*******************************************************/
if(mode_flag==true)
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mode Set To:");
  lcd.setCursor(0,1);
  extProfile(mode);
  shout(output);
   lcd.clear();
  mode_flag=false;
 }
  

if(digitalRead(button2)==HIGH)
{
     delay(300);
if(digitalRead(button1)==HIGH)
{
  
  Serial.println("Mode Changed");
  mode_flag=true;
  //change mode
    switch(mode)
  {
    case 1:{mode=2;output=vibrator;}
    break;
    case 2:{mode=3;output=led;}
    break;
    case 3:{mode=1;output=buzzer;}
    break;
    default:Serial.println("Error Mode 4");
  }
  delay(200);
  
}
else
{
  //if only button 2 is pressed

   pedo_flag=true;
} 
  
}
    
    
  if(digitalRead(button1)==HIGH)
  {
     delay(300);
if(digitalRead(button2)==HIGH)
{
  mode_flag=true;
  //change mode
  switch(mode)
  {
    case 1:{mode=2;output=vibrator;}
    break;
    case 2:{mode=3;output=led;}
    break;
    case 3:{mode=1;output=buzzer;}
    break;
    default:Serial.println("Error Mode 4");
  }
}
else
{
  //if only button 1 is pressed
  Serial.println("Button 1 is pressed");

  if(trans_flag==true)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Transmission");
    lcd.setCursor(0,1);
    lcd.print("Stopped");
    shout(output);
    trans_flag=false;
    lcd.clear();
  }
  else 
  {
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Transmission");
    lcd.setCursor(0,1);
    lcd.print("Resumed");
    shout(output);
    trans_flag=true;
    lcd.clear();
  }


  
 
delay(200);
  
} 
 }

/******************************** checked by tarash agarwal  no problem below this*********************************/


  if(pedo_flag==true)
  {
    //pedometer begins
    Serial.println("Paedometer Begins");
    lcd.print("   PAEDOMETER   ");
     delay(1000);
     countSteps=0;
     lcd.clear();
while(1)
{
    
    lcd.setCursor(0,0);
    lcd.print("   PAEDOMETER   ");
    delay(100);
     lcd.setCursor(0,1);
     lcd.print("   Limit=");
     lcd.print(countSteps);
     delay(100);

     if(digitalRead(button1)==HIGH)
     {
      delay(300);
      if(digitalRead(button2)==HIGH)
      {
        break;
      }
      else if(countSteps>0)
      {
        countSteps-=10;;
      }
      else
      break;
     }

     if(digitalRead(button2)==HIGH)
     {
      delay(300);
      if(digitalRead(button1)==HIGH)
      {
        break;
      }
      else
      {
        countSteps+=10;
      }
     }
     
     
}
    
     

   while(1)
    {
      if(countSteps<=0)
      break;
      lcd.clear();
      lcd.setCursor(0,0);
       lcd.print("Start walking");
     lcd.setCursor(0,1);
     lcd.print("No of Steps=");
     lcd.print(steps);
     
      int acc=0;
  float totvect[100]={0};
  float totave[100]={0};
   //float sum1,sum2,sum3=0;
  float xaccl[100]={0};
  float yaccl[100]={0};
  float zaccl[100]={0};
  // float x,y,z;
   
  
for (int i=0;i<100;i++)
{
xaccl[i]=float(analogRead(xpin));
delay(10);
if(digitalRead(button1)==HIGH)
break;

//delay(100);
//x=sum1/100.0;

//Serial.println(xavg);


yaccl[i]=float(analogRead(ypin));
delay(10);

//sum2=yaccl[i]+sum2;

//y=sum2/100.0;

//Serial.println(yavg);
//delay(100);

zaccl[i]=float(analogRead(zpin));
delay(10);

//sum3=zaccl[i]+sum3;
//z=sum3/100;


 totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)) + ((zval[i] - zavg)*(zval[i] - zavg)));
 totave[i] = (totvect[i] + totvect[i-1]) / 2 ;
 //acc=acc+totave[i];
// Serial.println(totave[i]);
 delay(200);
Serial.println(steps);
//cal steps 
if (totave[i]>threshhold && flag==0)
{
  steps=steps+1;
  
   lcd.setCursor(0,0);
       lcd.print("Start walking");
     lcd.setCursor(0,1);
     lcd.print("No of Steps=");
     lcd.print(steps);
  flag=1;
 
 
}
 else if (totave[i] > threshhold && flag==1)
{
//do nothing 
}
  if (totave[i] <threshhold  && flag==1)
  {flag=0;}
 // Serial.println('\n');
  //Serial.print("steps=");
  //Serial.println(steps);

  if(steps>countSteps)
  break;
  
}
//float tim=acc/100;
//Serial.println(tim);
 delay(500);
// stepcal(totave);

if(steps>=countSteps)
{
  while(1)
  {
alarm();
steps=0;
if(digitalRead(button1)==HIGH)
break;
  }
  break;
}
else if(digitalRead(button1)==HIGH)
{
  steps=0;
  break;
}
      
    }
    
    pedo_flag=false;
    delay(200);
    delay_flag=true;
  }
}



//profile function defined here
char profile(int mode)
{
  switch (mode)
  {
    case 1: return 'N';
      break;
    case 2: return 'S';
      break;
    case 3: return 'M';
      break;

  }
}

void extProfile(int mode)
{
  switch (mode)
  {
    case 1: lcd.print("Normal");
      break;
    case 2: lcd.print("Silent");
      break;
    case 3: lcd.print("Meeting");
      break;

  }
}

void shout(int output)
{
  //Serial.println("hEY");
  if(output==led){
  digitalWrite(output,LOW);
  delay(500);
  analogWrite(output,255);
  delay(500);
  }
  else if(output==buzzer)
  {
    analogWrite(output,255-buzzVolume);
  delay(500);
  analogWrite(output,255);
  delay(500);
  }
   else if(output==vibrator)
  {
    analogWrite(output,255-vibrVolume);
  delay(500);
  analogWrite(output,255);
  delay(500);
  }
}

void calibrate()

{  float sum=0;
  float sum1=0;
  float sum2=0;
for (int i=0;i<100;i++)
{
xval[i]=float(analogRead(xpin));

sum=xval[i]+sum;
}
delay(100);
xavg=sum/100.0;

Serial.println(xavg);

for (int j=0;j<100;j++)
{
xval[j]=float(analogRead(xpin));

sum1=xval[j]+sum1;
}
yavg=sum1/100.0;

Serial.println(yavg);
delay(100);
for (int i=0;i<100;i++)
{
zval[i]=float(analogRead(zpin));

sum2=zval[i]+sum2;
}
zavg=sum2/100.0;
delay(100);
Serial.println(zavg);

//digitalWrite(13,LOW);

}

void alarm()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Target Achieved");
  lcd.setCursor(0,1);
  lcd.print("press & hold B1");
  shout(output);
 
}


ISR (PCINT0_vect)
{
  
 countSteps++;
}



ISR (PCINT1_vect) 
{
 countSteps--;
}

