#include<time.h>
String comdata = "";  
boolean sig = false; 
int state = 1; //表示open的状态int state=0; 
//int isopen = 0;//表示之前没有打开，没有经历发送self数据,因为按逻辑写的话，发送数据时，当输入C，data这时是回车，会输出不必要的fail。
int cnt=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
} 
//发送的正确的数据
String self_right[2] = {"t3588CD3CFC7F1818F000\r","t03D82A00000000000000\r"};
int right = 2;
 
void loop() {
  // put your main code here, to run repeatedly:
  if (sig) 
  {
   // Serial.println(comdata);
    comdataProcessing(comdata);
    comdata="";
    sig=false;
  }
  cnt++;
  delay(10);
  srand((unsigned)time(NULL));
  if(cnt%100==0)
  {
    cnt-=100;
    if(state == 0)
    {
      String temp = "";
      int index = rand() % right;
      Serial.print(self_right[index]);
    }
  
    } 
}
void serialEvent() 
{
  while (Serial.available()) 
  {
    char recChar=(char)Serial.read();
    comdata+=recChar;
    if (recChar=='\r') 
    {
      sig=true;
    }
  }
}
void comdataProcessing(String data)
{
  if(data==NULL||data.length()==0)
  {
    sendData(0);
    }
     
  char type=data.charAt(0);
  data = data.substring(0,data.length()-1);
  if(type=='V')
  {
   //sendData(1);
   //Serial.println("SV2.5-HV2.0");
    Serial.print("SV2.5-HV2.0\r");
  }
  else if(type=='O'&&data.charAt(1)=='1')
  {
    open();
  }
  else if(type=='C')
  {
    close();
  }
  else if(type=='S')
  {
    setSpeed(data.charAt(1));
  }
else if(type=='T' || type=='t')
{
sendFrameData(data,type);
}
  else 
  {
       sendData(0);
  }
}

void sendSelfData()
{
  
  }

 //change to 6
void sendFrameData(String data,char type)
{
  int normal;
  if(state == 1)
  {
    sendData(0);
    return;
    }
   if(type == 'T')
      normal = 11;
   else
      normal = 6;

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
      if(len < normal || canLen < 0 || canLen >8 || len != normal + canLen*2)
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
void sendData(int sig)
{
  if(sig == 0){
    //Serial.println("Fail!");
    Serial.write((char)7);
    }
   else
   {
    //Serial.println("Suceed!");
    Serial.write("\r");
    }
}
