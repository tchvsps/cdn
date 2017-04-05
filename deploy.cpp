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

void init_set(int num);
void dpos(void);

extern unsigned int node_cnt,demand_cnt,edge_cnt,deploy_cost,demand_sum;
set<unsigned int> service_set;
extern unsigned int last_service_size;
set<unsigned int> best_service_set;
int best_cost=10000000;
string flow2string(void);

void prepare_for_creat(void);
void update_set();

unsigned int search_cnt=0;
int valid_cnt=0;
int dyna_service_num;

int init_service_num;
bool update_big=true;

time_t t1;

#define dim 1000
extern char gbest[dim];

int array2cost(char* arr);

void zkw_test(char * topo[], int line_num);


#include "zkw.h"
extern unsigned int node_cnt;
extern unsigned int demand_cnt;
void init_set(int num);
extern set<unsigned int> service_set;
void init_set(int num);

void zkw_test(char*topo[], int line_num)
{
    MCMF_ZKW zkw;
    zkw.Init(node_cnt,node_cnt+1);
    zkw.init_graph(topo,line_num);

    set<unsigned int>::iterator set_iter;
    for(set_iter=service_set.begin(); set_iter!=service_set.end(); set_iter++)
    {
        zkw.add(*set_iter,node_cnt+1,INF,0);
    }
    cout<<"ZKW TEST"<<endl;
    zkw.Zkw_Flow();
    cout<<endl<<zkw.ans<<endl;
}

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
    t1=time(NULL);

//    srand(time(NULL));
    srand(0);
    init_graph(topo,line_num);


    prepare_for_creat();

    init_set(demand_cnt);

    cout<<"SPAF TEST:"<<endl;
    last_service_size=0;
    init_service(service_set,last_service_size);
    cout<<MCMF()<<endl;
    init_service(service_set,last_service_size);
    cout<<MCMF()<<endl;
    init_service(service_set,last_service_size);
    cout<<MCMF()-service_set.size()*deploy_cost<<endl;

    cout<<"zkw TEST:"<<endl;

    zkw_test(topo,line_num);
    cout<<""<<endl;
    dpos();
//
//    init_service_num=demand_cnt*0.5;
//    init_set(init_service_num);
//    while(true)
//    {
//        init_service(service_set,last_service_size);
//        int cost=MCMF();
//        search_cnt++;
//
//        if(cost>0)
//        {
//            cout<<"this:"<<cost<<"best:"<<best_cost<<endl<<endl;
//            valid_cnt++;
//            if(cost<best_cost)
//            {
//                best_service_set.clear();
//                set<unsigned int>::iterator set_iter;
//                for(set_iter=service_set.begin();set_iter!=service_set.end();set_iter++)
//                {
//                    best_service_set.insert(*set_iter);
//                }
//                best_cost=cost;
//            }
//            update_set();
//        }
//        if(cost<0){
//            cout<<"INVALID"<<endl;
//            if(update_big){
//                update_big=false;
//            }
//            init_set(init_service_num);
//        }
//        if(time(NULL)-t1>88)
//            break;
//    }
//
//    if(best_cost==10000000)
//    {
//        cout<<"NOT FIND"<<endl;
//        return;
//    }

    int final_best=array2cost(gbest);
    string out_string=flow2string();
    cout<<endl<<out_string<<endl;
    cout<<endl<<endl<<"final best:"<<final_best<<endl;
    write_result(out_string.c_str(), filename);
}
