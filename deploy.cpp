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
time_t t_start;

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

    zkw.add_service(service_set);
    cout<<"ZKW TEST"<<endl;
    zkw.Zkw_Flow();
    cout<<endl<<zkw.ans<<endl;

    zkw.delete_service();
    zkw.add_service(service_set);
    cout<<"ZKW TEST"<<endl;
    zkw.Zkw_Flow();
    cout<<endl<<zkw.ans<<endl;
}


set<unsigned int > set_from_chen_hang;
int mini_cost_from_chen_hang=100000;
MCMF_ZKW zkw;
extern map<unsigned int,unsigned int> node2demand;
extern map<unsigned int,unsigned int> demand2node;

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
    t1=time(NULL);

    srand(time(NULL));
//    srand(0);
    last_service_size=0;

    init_graph(topo,line_num);
    prepare_for_creat();


    zkw.Init(node_cnt,node_cnt+1);
    zkw.init_graph(topo,line_num);

//    init_set(demand_cnt);
//    init_service(service_set,last_service_size);
//    cout<<"SPFA: "<<MCMF()<<endl;
//
//    zkw.add_service(service_set);
//    zkw.Zkw_Flow();
//    cout<<"ZKW : "<<zkw.ans<<endl;
//    zkw.add_service(service_set);
//    zkw.Zkw_Flow();
//    cout<<"ZKW : "<<zkw.ans<<endl;

//    t_start=time(NULL);
//    for(unsigned int i=0 ; i<100; i++){
//        init_set(demand_cnt);
//        init_service(service_set,last_service_size);
//        cout<<MCMF()<<" ";
//
////        init_set(demand_cnt);
//        zkw.add_service(service_set);
//        zkw.Zkw_Flow();
//        cout<<"zkw :";
//        cout<<zkw.ans<<" "<<zkw.all_demand<<endl;
//
//    }
//    cout<<"start time:"<<t_start<<" stop time:"<<time(NULL)<<" last:"<<time(NULL)-t_start<<endl;
//
//    t_start=time(NULL);
//    for(unsigned int i=0; i<1000; i++)
//    {
//        init_set(demand_cnt);
//        zkw.add_service(service_set);
//        zkw.Zkw_Flow();
////        cout<<"zkw :";
////        cout<<zkw.ans+deploy_cost*service_set.size()<<" "<<zkw.all_demand<<endl<<endl;
//    }
//    cout<<"start time:"<<t_start<<"  stop time:"<<time(NULL)<<" last:"<<time(NULL)-t_start<<endl;
//
//
//    t_start=time(NULL);
//    for(unsigned int i=0 ; i<1000; i++){
//        init_set(demand_cnt);
//
//        init_service(service_set,last_service_size);
//        cout<<"SPAF:";
//        cout<<MCMF()<<endl;
//    }
//    cout<<"start time:"<<t_start<<" stop time:"<<time(NULL)<<" last:"<<time(NULL)-t_start<<endl;
//
//    t_start=time(NULL);
//    for(unsigned int i=0; i<1000; i++)
//    {
//        init_set(demand_cnt);
//        zkw.add_service(service_set);
//        zkw.Zkw_Flow();
//        cout<<"zkw :";
//        cout<<zkw.ans+deploy_cost*service_set.size()<<" "<<zkw.all_demand<<endl<<endl;
//    }
//    cout<<"start time:"<<t_start<<"  stop time:"<<time(NULL)<<" last:"<<time(NULL)-t_start<<endl;


    for(unsigned int i=0; i<demand_cnt*0.5; i++){
        set_from_chen_hang.insert(demand2node[i]);
    }

    zkw.add_service(set_from_chen_hang);
    zkw.Zkw_Flow();
    if(zkw.ans<0){
        mini_cost_from_chen_hang=100000;
    }else{
        mini_cost_from_chen_hang=zkw.ans;
    }

    dpos();

    int final_best=array2cost(gbest);
    string out_string=flow2string();
    cout<<endl<<out_string<<endl;
    cout<<endl<<endl<<"final best:"<<final_best<<endl;
    write_result(out_string.c_str(), filename);
}
