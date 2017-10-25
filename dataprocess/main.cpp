#include <iostream>
#include <cstdio>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace std;

struct data
{
    int id,length,cnt;
    char lable;
}data[1024];

struct sub_data
{
    int start,length;
    double a,b,left,right;
    char type,sig;
}sub_data[1024];

int main()
{
    int cnt = 0;

    ifstream in("canmsg-sample.txt");
    ofstream out("canmsg-sample1.txt");

    string line;
    int temp;
    int total = 0;

    if(in)
    {
        while(getline(in,line))
        {
            int blank = 0;
            int len = line.length();
            int end0 = 0;
            if(len>0)
            {
                //cout<<line.at(0)<<endl;;
                if(line.at(0) == 'B')
                {
                    stringstream ss;
                    stringstream ss1;
                    if(cnt > 0)
                    {
                        data[cnt-1].cnt = temp;
                        total += temp;
                    }
                    temp = 0;
                    for(int i = 0; i < len; i++)
                    {
                        if(line.at(i) == ' ')
                        {
                            blank ++;
                            if(blank == 2)
                        {
                            end0 = i;
                            //cout<<line.substr(4,end0-4)<<endl;
                            ss<<line.substr(4,end0-4);
                            ss>>data[cnt].id;
                            //cout<<data[cnt].id<<endl;
                        }
                        }


                        if(line.at(i) == ':')
                        {
                            ss1<<line.substr(i+2,1);
                            ss1>>data[cnt].length;
                            //cout<<data[cnt].length<<endl;
                            break;
                        }

                    }
                    cnt++;
                }
                else
                {
                    int c,d;//取每个值的时候的左右边界
                    int flag = 0;

                    for(int i =0; i < len;i ++)
                    {
                        if(line.at(i) == ':')
                        {
                            c = i;
                        }
                        if(flag == 0 && line.at(i) == '|')
                        {
                            d = i;
                            stringstream ss2;
                            ss2<<line.substr(c+2,d-c-2);
                            ss2>>sub_data[total+temp].start;
                            flag = 1;

                        }
                        if(line.at(i) == '@')
                        {
                            stringstream ss3;
                            ss3<<line.substr(d+1,i-d-1);
                            ss3>>sub_data[total+temp].length;
                            sub_data[total+temp].type = line.at(i+1);
                            sub_data[total+temp].sig = line.at(i+2);
                            //cout<<sub_data[total+temp].length<<endl;
                            //cout<<sub_data[total+temp].type<<endl;
                        }
                        if(line.at(i) == '(')
                            c = i;
                        if(line.at(i) == ',')
                            {
                                d = i;
                                stringstream ss4;
                                ss4<<line.substr(c+1,d-c-1);
                                ss4>>sub_data[total+temp].a;
                            }
                        if(line.at(i) == ')')
                        {
                            stringstream ss5;
                            ss5<<line.substr(d+1,i-d-1);
                            ss5>>sub_data[total+temp].b;
                        }
                        if(line.at(i) == '[')
                            c = i;
                        if(line.at(i) == '|' && flag == 1)
                        {
                            d = i;
                            stringstream ss6;
                            ss6<<line.substr(c+1,d-c-1);
                            ss6>>sub_data[total+temp].left;
                        }
                        if(line.at(i) == ']')
                        {
                            stringstream ss7;
                            ss7<<line.substr(d+1,i-d-1);
                            ss7>>sub_data[total+temp].right;
                            break;
                        }
                    }
                    if(sub_data[total+temp].sig == '+')
                         temp++;

                }
            }
        }
    }


    int tempCnt = 0;
    for(int i = 0; i < cnt;i++)
        data[i].lable = 'B';
    for(int i = 0; i < cnt; i++)
    {
        out<<data[i].lable<<' ';
        out<<data[i].id<<' ';
        out<<data[i].length<<' ';
        out<<data[i].cnt<<endl;
        for(int j = tempCnt; j < tempCnt+data[i].cnt;j++)
        {
            out<<sub_data[j].start<<' ';
            out<<sub_data[j].length<<' ';
            out<<sub_data[j].type<<' ';
            out<<sub_data[j].a<<' ';
            out<<sub_data[j].b<<' ';
            out<<sub_data[j].left<<' ';
            out<<sub_data[j].right<<endl;

        }

        tempCnt+= data[i].cnt;
    }
    return 0;
}
