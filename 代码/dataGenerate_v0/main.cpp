#include <iostream>
#include <cstdio>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <time.h>
#include <math.h>
#include <cstdlib>


using namespace std;

/*
struct data
{
    int id,length,cnt;
    char lable;
    sub_data sub[32];
}data[1024];

struct sub_data
{
    int start,length;
    double a,b,left,right;
    char type,sig;
};
*/

ifstream in("comfort1.txt");
ofstream out("comfort_data.txt");

void transTo(int data, int num)
{
     for(int i = num -1; i>=0;i--)
     {
         char c;
         int number = (data>>(i*4))&15;
         if(number >= 0 && number <= 9)
            out<<number;
         if(number >= 10)
         {
             c = 'A' + number - 10;
             out<<c;
         }

     }
}

unsigned long long calculate(double a,double b,double left,double right)
{
    unsigned long long x;
    srand((unsigned)time(NULL));
    long l = (left - b)/a;
    long r = (right - b)/a;
    x = (rand()%(r-l+1))+l;
    return x;
}

int getBitNum(unsigned long long area)
{
    int cnt = 1;
    while(area / 2 != 0)
    {
        cnt++;
        area = area / 2;
    }
    return cnt;
}

void writeMessage(char message[],int start,int leng,char type,double a,double b,double left,double right)
{
    unsigned long long area;
    area = calculate(a,b,left,right);

    int index[8] = {0,8,16,24,32,40,48,56};
    //计算area的位数，如果小于leng，则高位补0
    int numCnt = getBitNum(area);

    if(type == '1')
    {
        int st;
        int row = start/8;
        int temp = start - index[row] + 1;

        if(temp >= leng)
        {
            int p,st;
            if(numCnt < leng)
            {
                p = 0;
                st = start - leng +1;
                while(numCnt)
                {
                    message[st] = ((area>>p)&1) + '0';
                    st++;
                    numCnt--;
                    p++;
                }
                while(st<= start)
                {
                    message[st] = '0';
                    st++;

                }
            }
            else if(numCnt == leng)
            {
                p = 0;
                st = start - leng +1;
                while(st <= start)
                {
                    message[st] = ((area>>p)&1) + '0';
                    st++;
                    p++;
                }
            }
            else
            {
                cout<<"the input is wrong."<<endl;
            }
        }
        else
        {
            int q = leng - temp;
            int q0 = q/8;
            int a0 = q - 8 * q0;
            if(a0 == 0)
                st = index[row - q0];
            else
                st = index[row - q0 - 1] + 8 - a0;
            int p = 0;
            while(p < numCnt)
            {
                message[st] = ((area>>p)&1) + '0';
                    st++;
                    p++;
            }
            while(st <= start)
            {
                message[st] = '0';
                    st++;
            }
        }
    }
    if(type == '0')
    {
        int st;
        int row = start/8;
        int temp = start - index[row] + 1;
        int p = 0;
        int flag = 0;
        if (temp < leng)
        {
            int q = leng - temp;
            int q0 = q/8;
            int a0 = q - 8 * q0;
            if(a0 == 0)
                st = index[row + q0];
            else
                st = index[row + q0 + 1] + 8 - a0;
            while(a0 > 0)
            {
                if(p < numCnt)
                {
                    message[st] = ((area>>p)&1) + '0';
                    p++;
                    a0--;
                    st++;
                }
                else
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                while(q0)
                {
                    st = st - 16;
                    int random = 8;
                    while(random)
                    {
                        if(p < numCnt)
                        {
                            message[st] = ((area>>p)&1) + '0';
                            p++;
                            random--;
                            st++;
                        }
                        else
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if(flag == 1)
                        break;
                    q0--;
                }
            }
            if(flag == 1)
            {
                if(numCnt < leng)
                {
                    if(q0 == 0)
                    {
                        while(st <= start)
                        {
                            message[st] = '0';
                            st++;
                        }
                    }
                    else
                    {
                        while(q0)
                        {

                            if(st % 8 == 0)
                            {
                                st = st -16;
                                q0--;
                            }
                            if(st == start )
                                break;
                            message[st] = '0';
                            st++;



                        }
                    }
                }
            }



        }
        else
        {
            int p,st;
            if(numCnt < leng)
            {
                p = 0;
                st = start - leng +1;
                while(numCnt)
                {
                    message[st] = ((area>>p)&1) + '0';
                    st++;
                    numCnt--;
                    p++;
                }
                while(st<= start)
                {
                    message[st] = '0';
                    st++;

                }
            }
            else if(numCnt == leng)
            {
                p = 0;
                st = start - leng +1;
                while(st <= start)
                {
                    message[st] = ((area>>p)&1) + '0';
                    st++;
                    p++;
                }
            }
            else
            {
                cout<<"the input is wrong."<<endl;
            }
        }
    }
}

void print(char message[],int cnt,int start)
{
    int data[4];
    int number = 0;
         while(cnt > 0)
          {
              data[cnt-1] = message[start] - '0';
              cnt--;
              start--;
          }
          for(int j = 0; j < 4;j++)
          {
              number += data[j]*(pow(2.0,j));
          }
          if(number >= 0 && number <= 9)
            out<<number;
          if(number >= 10)
          {
              char c = 'A' + number -10;
              out<<c;
          }
}

void sendout(char message[],int length)
{
    int start0 = 7;
    int start1 = 3;
    for(int i = 0; i < length;i++)
    {
        print(message,4,start0);
        print(message,4,start1);
        start0 += 12;
        start1 += 12;
    }

}


int main()
{

    string line;
    char message[64];

    int id,length;

    int start,leng;
    double a,b,left,right;
    char type,sig;

    int cnt = 0;

    if(in)
    {
        while(getline(in,line))
        {
            int len = line.length();

            if(line.at(0) == 'B')
            {
                //帧的格式
                if(cnt > 0)
                {
                    //根据上一组数据的全部信息得到的message，根据length输出can信息
                    sendout(message,length);
                    out<<endl;
                }

                out<<'t';

                for(int i = 0; i < 64;i++)
                   message[i] = 0;
                int c,d;
                int blank = 0;
                //将ID和长度输出
                for(int i = 0; i < len;i++)
                {

                    if(line.at(i) == ' ')
                    {
                        blank++;
                        if(blank == 2)
                        {
                            stringstream ss1;
                            ss1<<line.substr(2,i-2);
                            ss1>>id;
                            //输出id
                            transTo(id,3);
                            c = i;
                        }
                        if(blank == 3)
                        {
                            //d = i;
                            stringstream ss2;
                            ss2<<line.at(c+1);
                            ss2>>length;
                            //输出长度
                            transTo(length,1);
                        }
                    }
                }
                cnt++;
            }
            else
            {
                int blank = 0;
                int c,d;
                for(int i = 0; i < len;i++)
                {
                    if(line.at(i) == ' ')
                    {
                        blank++;
                        if(blank == 1)
                        {
                            c = i;
                            stringstream ss3;
                            ss3<<line.substr(0,i);
                            ss3>>start;
                        }
                        if(blank == 2)
                        {
                            stringstream ss4;
                            ss4<<line.substr(c+1,i-c-1);
                            ss4>>leng;
                        }
                        if(blank == 3)
                        {
                            type = line.at(i-1);
                            d = i;
                        }
                        if(blank == 4)
                        {
                            stringstream ss5;
                            ss5<<line.substr(d+1,i-d-1);
                            ss5>>a;
                            d = i;
                        }
                        if(blank == 5)
                        {
                            stringstream ss6;
                            ss6<<line.substr(d+1,i-d-1);
                            ss6>>b;
                            d = i;
                        }
                        if(blank == 6)
                        {
                            stringstream ss7;
                            ss7<<line.substr(d+1,i-d-1);
                            ss7>>left;
                            d = i;
                        }

                    }
                    if(i == len-1)
                    {
                            stringstream ss8;
                            ss8<<line.substr(d+1,i-d);
                            ss8>>right;
                    }

                }
                //填写message
                    writeMessage(message,start,leng,type,a,b,left,right);

            }

        }
        //输出最后一组数据
        sendout(message,length);
        out<<endl;
    }

    return 0;
}
