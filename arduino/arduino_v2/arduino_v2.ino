#include<time.h>
String input = "";  
boolean flag = false; 
int state = 1; //表示open的状态int state=0; 
int isopen = 0;//表示之前没有打开，没有经历发送self数据,因为按逻辑写的话，发送数据时，当输入C，command这时是回车，会输出不必要的fail。

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
} 
//发送的正确的数据
String self_right[13] = {"t3588CD3CFC7F1818F000","t03D82A00000000000000","t42B87F37FF0000000000","t4208FF37FF0000000000",
"t31880200000000000000","t34580000000024000000","t393800000F0F33000000","t3208F9FFFFEF0FC79EFF","t3218FFFF0C0F000F2000",
"t31D80700000000000000","t3608050F0F0FFF000000","t36380F03C00F7F00F0F7","t3648CE60800700000000"};
int right = 13;

void loop() {
  // put your main code here, to run repeatedly:
  if (flag) 
  {
   // Serial.println(input);
    dataProcess(input);
    input="";
    flag=false;
  }
}
void serialEvent() 
{
  while (Serial.available()) 
  {
    char inChar=(char)Serial.read();
    input+=inChar;
    if (inChar=='\r') 
    {
      flag=true;
    }
  }
}
void dataProcess(String command)
{
  if(command==NULL||command.length()==0)
  {
    Serial.println("wangyixiwangxyixi");
    sendData(0);
    }
     
  char type=command.charAt(0);
  command = command.substring(0,command.length()-1);
  if(type=='V'&&command.length()==1)
  {
   sendData(1);
   Serial.println("SV2.5-HV2.0");
   return;
    
  }
  else if(type=='O'&&command.charAt(1)=='1'&&command.length()==2)
  {
    open();
  }
  else if(type=='C'&&command.length()==1)
  {
    close();
  }
  else if(type=='S'&&command.length()==2)
  {
    setSpeed(command.charAt(1));
  }
else if(type=='T' || type=='t')
{
sendFrameData(command,type);
}
  else 
  {
    if(!isopen)
       sendData(0);
     else
        isopen = 0;
  }
   sendSelfData();
}

void sendSelfData()
{
  srand((unsigned)time(NULL));
  while(state == 0)
  {
    int index = rand() % right;
    Serial.println(self_right[index]);
    if(Serial.read() == 'C')
    {
      state = 1;
      sendData(1);
      isopen = 1;
      break;
      }
    } 
  }
void sendFrameData(String data,char type)
{
  int normal;
  if(state == 1)
  {
    sendData(0);
    return;
    }
   if(type == 'T')
      normal = 10;
   else
      normal = 5;

    int len = data.length();
    for(int i = 1; i < len; i++)
    {
      if(!((data[i] >= '0'&& data[i] <= '9') || ( data[i] >= 'A'&&data[i] <= 'F')))
        
        {
          sendData(0);
          return;
          }
       
      }
      int canLen = data.charAt(4)-'0';
      if(len < normal || canLen <= 0 || canLen >8 || len != normal + canLen*2)
      {
        sendData(0);
        return;
        }
        sendData(1);
  }
void setSpeed(char c) 
{
  if(state==1)
  {
    int level=(int)(c-'0');
    if(level<0||level >8)
      sendData(0);
    else
    {
     sendData(1);
    }
  }
  else
  {
    sendData(0);
  }
}

void close() 
{
    if(state==0)
  {
    state=1;
    sendData(1);
  }
  else
  {
   sendData(0);
   
  }
}
void open() 
{
  if(state==1)
  {
    state=0;
    sendData(1);
  }
  else
  {
    sendData(0);
  }
}
void sendData(int flag)
{
  if(flag == 0){
    Serial.println("Fail!");
    Serial.write((char)7);
    }
   else
   {
    Serial.println("Suceed!");
    Serial.write("\r");
    }
}
