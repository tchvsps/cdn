#include "deploy.h"
#include <stdio.h>

#include<iostream>
#include<algorithm>
#include<string>
#include<sstream>
#include<set>
#include<vector>
#include<stack>
#include<map>
#include<queue>
#include<deque>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<ctime>
#include<functional>
using namespace std;



//你要完成的功能总入口
int MCMF(void);
void init_graph(char * topo[], int line_num);
void init_service(set<unsigned int> service_set, unsigned int last_service_size);

extern unsigned int node_cnt,demand_cnt,edge_cnt,deploy_cost,demand_sum;
set<unsigned int> service_set;
set<unsigned int> best_service_set;
int best_cost=10000000;
string flow2string(void);




void prepare_for_creat(void);
void create_set(int);

unsigned int search_cnt=0;
int valid_cnt=0;
int dyna_service_num;

time_t t1;
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
    t1=time(NULL);
    srand(time(NULL));
    init_graph(topo,line_num);

    prepare_for_creat();

    dyna_service_num=demand_cnt*0.4;

    unsigned int last_service_size=0;
    while(true)
    {
        create_set(dyna_service_num);
        init_service(service_set,last_service_size);
        last_service_size=service_set.size();

        search_cnt++;
        if(search_cnt%10==0){
            switch(valid_cnt){
                case 10:
                    dyna_service_num=dyna_service_num*0.8;
                    break;
                case 9:
                    dyna_service_num-=2;
                    break;
                case 8:
                case 7:
                    dyna_service_num--;
                    break;
                case 0:
                    dyna_service_num*1.2;
                    break;
                case 1:
                    dyna_service_num+=2;
                    break;
                case 2:
                case 3:
                    dyna_service_num+=1;
                    break;
            }
//            if(dyna_service_num>demand_cnt)
//                dyna_service_num=demand_cnt;
//            if(dyna_service_num<0)
//                dyna_service_num=0;

            valid_cnt=0;
            cout<<"dyna servic:"<<dyna_service_num<<endl;
        }
        int cost=MCMF();

        if(cost>0)
        {
            cout<<"this:"<<cost<<"best:"<<best_cost<<endl<<endl;
            valid_cnt++;
            if(cost<best_cost)
            {

                best_service_set.clear();
                set<unsigned int>::iterator set_iter;
                for(set_iter=service_set.begin();set_iter!=service_set.end();set_iter++)
                {
                    best_service_set.insert(*set_iter);
                }
                best_cost=cost;
            }

        }

        if(time(NULL)-t1>80)
            break;
    }

    init_service(best_service_set,last_service_size);
    last_service_size=best_service_set.size();
    MCMF();
    string out_string=flow2string();
    cout<<endl<<out_string<<endl;
    cout<<endl<<endl<<"final best:"<<best_cost<<endl;
    write_result(out_string.c_str(), filename);
}
