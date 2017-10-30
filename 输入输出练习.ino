String comdata = "";
void setup()
{
  Serial.begin(9600);
  for(int i = 2; i <= 13; i++) pinMode(i, OUTPUT);
}

void loop()
{
  while (Serial.available() > 0)
  {
    comdata += int(Serial.read()) - '0';
    delay(2);
  }
  if(comdata.length() > 0)
  {
    for(int i = 0; i < comdata.length(); i++)
    {
      if(comdata[i]=='0'||comdata[i]=='1')
      {
        digitalWrite(i + 2, comdata[i] - '0');
        Serial.print("Pin ");
        Serial.print(i + 2);
        Serial.print(" is ");
        Serial.println(comdata[i]);
        //System.out.println(messageID);
      }
    }
    comdata = "";
  }
}












//接收代码：
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3,2);
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(9,OUTPUT);
}

void loop() {
  while(mySerial.available() > 0){
    char c = mySerial.read();
    Serial.write(c);
  }
}



//发送代码：
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,2);

bool a=0;

int b=0;

void setup() {

mySerial.begin (9600);

Serial.begin(9600);

pinMode(9,OUTPUT);

}

void loop() {

if(a==0)

{

digitalWrite(9,HIGH);

Serial.println ("pin9 is HIGH");

mySerial.println("pin9 is HIGH");

}

else

{

digitalWrite(9,LOW);

Serial.println ("pin9 is LOW");

mySerial.println("pin9 is LOW");

}

b++;

a=b%2;

delay(1000);

}









