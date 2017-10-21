String comdata = "";
int correct = 0; //表示数据正确与否
int on = 1; //表示open的状态

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
 while(Serial.available() > 0)
 {
  char sig = char(Serial.read());
  comdata += sig;
  delay(2);
  if(sig == '\r')
  {
    correct = 1;
    }
  
  }

  if(correct == 1)
  {
    dataProcess(comdata);
    comdata = "";
    correct = 0;
    }
  
}


void dataProcess(String data)
{
  if(data == NULL)
     sendData(0);
   char type = data.charAt(0);
   data = data.substring(0,data.length()-1);
   /*
   if(type == 'V' && data.length() == 1)
       Serial.print("SV2.5-HV2.0");
   else if(type == 'O' && data.charAt(1)=='1'&&data.length()==2)
     open();
   else if(type=='C'&&data.length()==1)
      close(); 
   else if(type=='S'&&data.length()==2)
     setSpeed(data.charAt(1));
     */
  if(type=='T' || type=='t')
    sendFrameData(data,type);
  else 
     sendData(0);     
  }

void sendFrameData(String data,char type)
{
  int normal;
  if(on == 0)
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

void sendData(int sig)
{
  if(sig == 0)
    Serial.write((char)7);
   else
   {
    Serial.print("SV2.5-HV2.0");
    Serial.write("\r");
    }
    
  }
  
