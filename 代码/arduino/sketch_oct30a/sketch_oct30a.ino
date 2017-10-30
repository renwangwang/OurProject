#include<time.h>
String comdata = "";
boolean sig = false; //表示是否接受到数据
int state = 1; //表示open的状态int state=0;
//int isopen = 0;//表示之前没有打开，没有经历发送self数据,因为按逻辑写的话，发送数据时，当输入C，data这时是回车，会输出不必要的fail。
int cnt=0;//设置延时
int dataCnt = 0;//当前第几个数据
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
}
//发送的正确的数据
//数组可以根据需要测的功能等变化
String self_right[39] = {"t42B80000000000000000\r",
"t42B81400000000000000\r",
"t42B82800000000000000\r",
"t42B83B00000000000000\r",
"t42B84F00000000000000\r",
"t42B86100000000000000\r",
"t42B87400000000000000\r",
"t42B88500000000000000\r",
"t42B89600000000000000\r",
"t42B8A600000000000000\r",
"t42B8B500000000000000\r",
"t42B8C200000000000000\r",
"t42B8CF00000000000000\r",
"t42B8DA00000000000000\r",
"t42B8E400000000000000\r",
"t42B8EC00000000000000\r",
"t42B8F300000000000000\r",
"t42B828F8000000000000\r",
"t42B828FC000000000000\r",
"t42B828FF000000000000\r",
"t42B828FC000000000000\r",
"t42B828F8000000000000\r",
"t42B8F300000000000000\r",
"t42B8EC00000000000000\r",
"t42B8E400000000000000\r",
"t42B8DA00000000000000\r",
"t42B8CF00000000000000\r",
"t42B8C200000000000000\r",
"t42B8B500000000000000\r",
"t42B8A600000000000000\r",
"t42B89600000000000000\r",
"t42B88500000000000000\r",
"t42B87400000000000000\r",
"t42B86100000000000000\r",
"t42B84F00000000000000\r",
"t42B83B00000000000000\r",
"t42B82800000000000000\r",
"t42B81400000000000000\r",
"t42B80000000000000000\r"};
int right = 39;


void loop()
{
    // put your main code here, to run repeatedly:
    //如果接收到数据，则处理相应命令
    if (sig)
    {
        // Serial.println(comdata);
        comdataProcessing(comdata);
        comdata="";
        sig=false;
    }

    cnt++;
    delay(10);

    //cnt为1000时，表示1秒发送1条数据
    if(cnt%100==0)
    {
        cnt-=100;
        if(state == 0)
        {
          /*
            //随机输出can信息
            int index = rand() % right;
            Serial.print(self_right[index]);
            */
            //按序输出
            
            if(dataCnt > 39)
                dataCnt-=40;
            Serial.print(self_right[dataCnt]);
            dataCnt++;
            
        }

    }
}

//中断，接收数据
void serialEvent()
{
    while (Serial.available())
    {
        //设置随机，每次接受到的信号不一样
        srand((unsigned)time(0));
        char recChar=(char)Serial.read();
        comdata+=recChar;
        if (recChar=='\r')
        {
            sig=true;
        }
    }
}

//具体处理can信息
void comdataProcessing(String data)
{
    if(data==NULL||data.length()==0)
    {
        sendData(0);
    }

    //得到信息类型，判断信号正确与否
    char type=data.charAt(0);
    data = data.substring(0,data.length()-1);
    //具体信息
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

//change to 5
void sendFrameData(String data,char type)
{
    int normal;
    if(state == 1)
    {
        sendData(0);
        return;
    }
    //设置信号出现前已经占的位数
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

//设置速率是否成功
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

//返回数据
void sendData(int sig)
{
    if(sig == 0)
    {
        //Serial.println("Fail!");
        Serial.write((char)7);
    }
    else
    {
        //Serial.println("Suceed!");
        Serial.write("\r");
    }
}

