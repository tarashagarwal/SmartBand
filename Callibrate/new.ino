/***************************************************************Variables Defined Here*****************************************************/
//axis variables
const int X=A1;
const int Y=A2;
const int Z=A3;
const int GND=4;
const int VCC=2;


//compVariables
int avgX;
int avgY;
int avgZ;

//callibration array
int x[10];
int y[10];
int z[10];

//number of observations while callibrating
const int ob=10;
const int obTime=1;
/***************************************************************Functions declaration here**************************************************/
int average(int []);
void calibrate();
/*******************************************************************************************************************************************/




void setup() {
Serial.begin(9600);
 pinMode(X,INPUT);
 pinMode(Y,INPUT);
 pinMode(Z,INPUT);
 pinMode(GND,OUTPUT);
 pinMode(VCC,OUTPUT);

 digitalWrite(VCC,HIGH);
 digitalWrite(GND,LOW);
 

calibrate();
 
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++main loop here++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void loop() {
 
  Serial.print(avgX);
  Serial.print("   ");
  Serial.print(analogRead(X));
  Serial.print("   ");
  Serial.print(avgY);
  Serial.print("   ");
  Serial.print(analogRead(Y));
  Serial.print("   ");
  Serial.print(avgZ);
  Serial.print("   ");
  Serial.print(analogRead(Z));
  Serial.println();
}


/************************************************************Function Definitions here*****************************************/
void calibrate()
{
  
  for(int i=0;i<ob;i++)
  {
    delay(obTime*1000);
    x[i]=analogRead(X);
    y[i]=analogRead(Y);
    z[i]=analogRead(Z);
 }
 avgX=average(x);
 avgY=average(y);
 avgZ=average(z);
  
}

int average(int a[])
{
 int sum=0;
 for(int i=0;i<ob;i++)
 {
   sum+=a[i];
 }
 return sum/ob;
}



