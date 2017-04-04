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

#define N 2000


extern unsigned int node_cnt,demand_cnt,edge_cnt,deploy_cost,demand_sum;
extern set<unsigned int> service_set;

struct Edge
{
    int from,to,cap,flow,cost;
    Edge(int u,int v,int ca,int f,int co):from(u),to(v),cap(ca),flow(f),cost(co){};
};

extern vector<Edge> origal_edges;
extern vector<int> G[N];

unsigned int search_deepth;

struct {
    set<unsigned int> node_set;
    vector<unsigned int> node_array;
    int node_cnt;
    bool assigned;
}demand[400];

struct {
    set<unsigned int> demand_set;
    unsigned int degree_sum;
    void sele_action(void)
    {
        set<unsigned int>::iterator set_iter;
        for(set_iter=demand_set.begin(); set_iter!=demand_set.end(); ++set_iter)
        {
            demand[*set_iter].node_cnt++;
        }
    }
}service[1000];

extern map<unsigned int, unsigned int> demand2node;

void dfs(unsigned int demand_index,unsigned int node_index, unsigned int deepth){
    if(deepth>search_deepth) return;

    demand[demand_index].node_set.insert(node_index);
    service[node_index].demand_set.insert(demand_index);

    for(unsigned int i=0; i<G[node_index].size();i++){
        dfs(demand_index, origal_edges[G[node_index][i]].to, deepth+origal_edges[G[node_index][i]].cost);
    }
}
void service_search(){
    search_deepth=4;
    set<unsigned int>::iterator demand_iter;
    for(unsigned int i=0; i<demand_cnt; i++){
        dfs(i,demand2node[i],0);
        for(demand_iter=demand[i].node_set.begin();demand_iter!=demand[i].node_set.end();demand_iter++){
            demand[i].node_array.push_back(*demand_iter);
        }
    }
}

void caculate_degree_sum(){
    for(unsigned int i=0; i<node_cnt; i++){
        int degee_sum=0;
        for(unsigned int j=0; j<G[i].size();j++){
            degee_sum+= origal_edges[G[i][j]].cap;
        }
        service[i].degree_sum=degee_sum;
    }
}

void prepare_for_creat(void)
{
    service_search();
    caculate_degree_sum();
}
void create_set(int node_num){
    int degee_sum=0;
    service_set.clear();
    bool add_flg;
    for(unsigned int i=0; i<demand_cnt; i++){
        demand[i].node_cnt=0;
        demand[i].assigned=false;
    }

    while(true)
    {
        add_flg=false;
        for(unsigned int i=0; i<demand_cnt; i++)
        {
            if(!demand[i].node_cnt && !demand[i].assigned){
                int new_service_index=demand[i].node_array[random()%demand[i].node_array.size()];
                if(service_set.find(new_service_index)==service_set.end()){
                    demand[i].assigned=true;
                    add_flg=true;
                    service_set.insert(new_service_index);
                    degee_sum+=service[new_service_index].degree_sum;
                    if(degee_sum>demand_sum*1.2 && service_set.size()>=node_num)
                        return;
                }
            }
        }
        if(add_flg)
            return;
        if(degee_sum>demand_sum*1.1 && service_set.size()>=node_num){
            return;
        }
        else
        {
            for(unsigned int i=0; i<demand_cnt; i++)
            {
                demand[i].node_cnt=0;
            }
        }
    }
}
