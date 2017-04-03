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
string flow2string(void);

int valid_cnt=0;
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
    srand(0);
    init_graph(topo,line_num);

    unsigned int last_service_size=0;
    while(true)
    {
        service_set.clear();
        for(unsigned int i=0; i<demand_cnt*3; i++)
        {
            service_set.insert(rand()%node_cnt);
        }
        init_service(service_set,last_service_size);
        last_service_size=service_set.size();

        int cost=MCMF();
        valid_cnt++;

        if(cost>0)
        {
            cout<<"valid:"<<cost<<endl;
        }
        if(valid_cnt>0)
            break;

    }

    string out_string=flow2string();
//    cout<<endl<<out_string<<endl;
    write_result(out_string.c_str(), filename);
}
