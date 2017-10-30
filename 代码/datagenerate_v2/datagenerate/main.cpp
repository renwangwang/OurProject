#include <iostream>
#include <cstdio>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <time.h>
#include <math.h>
#include <cstdlib>

#define PI 3.1415925635897932


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

ifstream in("powertrain1.txt");
ofstream out("powertrain_data.txt");
ofstream out1("powertrain_phy.txt");

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

unsigned long long calculate(double a,double b,double left,double right,int cnt)
{
    unsigned long long x;
    /*
    srand((unsigned)time(NULL));
    long l = (left - b)/a;
    long r = (right - b)/a;
    x = (rand()%(r-l+1))+l;
    */
    long l = (left - b)/a;
    long r = (right - b)/a;

    if(r - l +1 >= 100)
    {
        double temp = (PI/40) * cnt;
        temp = sin(temp);
        int y = (int)(temp * (r-l+1) + l);
        x = y;
        out1<<x<<" ";
        cout<<x<<endl;
    }
    else
    {
        x = (rand()%(r-l+1))+l;
        out1<<x<<" ";
    }

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
        start0 += 8;
        start1 += 8;
    }

}

void writeMessage(char message[],int start,int leng,char type,double a,double b,double left,double right,int length)
{
    unsigned long long area;

    int totalCount = 0;

    while(totalCount <= 20)
    {
    area = calculate(a,b,left,right,totalCount);

    int index[8] = {0,8,16,24,32,40,48,56};
    //计算area的位数，如果小于leng，则高位补0
    int numCnt = getBitNum(area);

    if(type == '1')
    {
        int p = 0;
        int st = start;
        while(p < numCnt)
        {
            message[st] = ((area>>p)&1) + '0';
            st++;
            p++;
        }
        while(p < leng)
        {
            message[st] = '0';
            st++;
            p++;
        }
    }
    if(type == '0')
    {
        int st;
        int row = start/8;
        int temp = start - index[row] + 1;
        int p = 0;
        int flag = 0;
        int flag0 = 0;
        if (temp < leng)
        {
            int q = leng - temp;
            int q0 = q/8;
            int a0 = q - 8 * q0;
            if(a0 == 0)
                st = index[row + q0];
            else
                st = index[row + q0 + 1] + 8 - a0;
            q0 = q0 + 1;
            while(a0 > 0)
            {
                flag0 = 1;
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
                    if(flag0 == 1)
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
                    if(random == 0)
                        flag0 = 1;
                    q0--;
                    if(flag == 1)
                        break;

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
                            /*
                            if(st % 8 == 0)
                            {
                                st = st -16;
                                q0--;
                            }
                            //modify
                            if(st == start )
                                break;
                            message[st] = '0';
                            st++;
                            */

                            if(st % 8 == 0)
                            {
                                st = st -16;
                                message[st] = '0';
                                if(st==start)
                                    break;
                                st++;
                                q0--;
                            }

                            while(st % 8 != 0)
                            {
                                message[st] = '0';
                                if(st == start)
                                    break;
                                st++;
                            }


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
                cout<<"-------------------------"<<endl;
                cout<<"the input is wrong."<<endl;
            }
        }
    }

    long l = (left - b)/a;
    long r = (right - b)/a;

    if(r - l >= 100)
         totalCount++;
    else
        totalCount = 21;
    sendout(message,length);
    out<<endl;
    out1<<endl;
    }
}




int main()
{

    srand((unsigned)time(NULL));//time(0)
    string line;
    char message[64];

    int id,length,dataCount;

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

                out<<'t';


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
                            out1<<"*"<<id<<endl;
                            //输出id
                            transTo(id,3);
                            c = i;
                            cout<<id<<endl;
                            cout<<"************"<<endl;
                        }
                        if(blank == 3)
                        {
                            d = i;
                            stringstream ss2;
                            ss2<<line.at(c+1);
                            ss2>>length;
                            //输出长度
                            transTo(length,1);
                        }
                    }
                    if(i == len -1)
                    {
                        stringstream ss;
                        ss<<line.substr(d+1,i-d);
                        ss>>dataCount;
                    }

                }
                cnt++;
            }
            else
            {
                for(int i = 0; i < 64;i++)
                   message[i] = '0';
                out<<endl;
                out1<<endl;
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
                if(dataCount != 0)
                    writeMessage(message,start,leng,type,a,b,left,right,length);

            }



        }
        //输出最后一组数据
        //sendout(message,length);
        //out<<endl;
    }

    return 0;
}
