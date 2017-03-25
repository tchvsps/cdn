#include"basic.h"
//class Process{
//public:
//	int init_graph(char * topo[], int line_num);
//	unsigned int get_search_deep(void);
//
//public:
//

//};
//
//Process::init_graph(char * topo[], int line_num){
//
//}

bool valid_scheme=true;

unsigned int service_cnt;
unsigned int demand_cnt;
unsigned int edge_cnt;
unsigned int deploy_cost;

unsigned int cost_sum;

set<unsigned int> assigned_service;
set<unsigned int> unassigned_service;
set<unsigned int> assigned_demand;
set<unsigned int> unassigned_demand;

vector<Demand*> demand_vector;
vector<Service*> service_vector;

unsigned int max_search_deep;
unsigned int max_add_deep;
unsigned int predef_deep;

map<pair<unsigned int, unsigned int>, signed int> static_bandwidth;
map<pair<unsigned int, unsigned int>, signed int> tmp_bandwidth;

map<pair<unsigned int, unsigned int>, signed int> orignal_bandwidth;
vector<signed int> orignal_demand;
connect_list* waiting_connect_head=NULL;

void Process::init_graph(char * topo[], int line_num)
{
    sscanf(topo[0],"%d %d %d",&service_cnt,&edge_cnt,&demand_cnt);
    sscanf(topo[2],"%d",&deploy_cost);

    for(unsigned int i=0; i<service_cnt; i++)
    {
        service_vector.push_back(new Service(i));
    }

    unsigned int start_node;
    unsigned int stop_node;
    unsigned int bandwidth;
    unsigned int length;

    //init service info: edge
    for(unsigned int i=4; i<4+edge_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d %d",&start_node,&stop_node,&bandwidth,&length);
//        cout<<topo[i];
        edge_list* new_edge;
        edge_list* head;

        new_edge=new edge_list(length,start_node,stop_node);
        head=service_vector[start_node]->edge_head;
        service_vector[start_node]->edge_head=insert_edge(head,new_edge);

        new_edge=new edge_list(length,stop_node,start_node);
        head=service_vector[stop_node]->edge_head;
        service_vector[stop_node]->edge_head=insert_edge(head,new_edge);

        static_bandwidth.insert(make_pair(make_pair(start_node, stop_node), bandwidth));
        static_bandwidth.insert(make_pair(make_pair(stop_node,start_node), bandwidth));

    }

    unsigned int demand_index;
    unsigned int service_index;
    unsigned int demand;

    //init demand info
    //init service demand connect info
    for(unsigned int i=5+edge_cnt; i<5+edge_cnt+demand_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d",&demand_index,&service_index,&demand);
//        cout<<topo[i];
        demand_vector.push_back(new Demand(demand_index,service_index,demand));
        orignal_demand.push_back(demand);

        service_vector[service_index]->attached_demand=demand_index;
    }
//    demand_vector[0]->print_demand();
    orignal_bandwidth=static_bandwidth;

}

void Process::init_process(void)
{

    //init unassigned
    for(unsigned int i=0; i<service_cnt; i++)
    {
        unassigned_service.insert(i);
    }
    for(unsigned int i=0; i<demand_cnt; i++)
    {
        unassigned_demand.insert(i);
    }

    //init service search_list
    for(unsigned int i=0; i<service_cnt; i++)
    {
        Tree_node* new_tree_node=new Tree_node(i,service_vector[i]->edge_head);
        new_tree_node->search_deep=new_tree_node->bfs_position->length;


        service_vector[i]->search_head=new_tree_node;
        service_vector[i]->jude_new_node(new_tree_node);
    }
}
inline edge_list* insert_edge(edge_list* head, edge_list* new_node)
{
    new_node->next=NULL;

    if(!head)
    {
        head=new_node;
    }
    else if(head->length >= new_node->length)
    {
        new_node->next=head;
        head=new_node;
    }
    else
    {
        edge_list* parent_node=head;

        while(parent_node->next)
        {
            if(parent_node->next->length >= new_node->length)
            {
                new_node->next=parent_node->next;
                break;
            }
            else
            {
                parent_node=parent_node->next;
            }
        }
        parent_node->next=new_node;
    }
    return head;
}

void Process::get_search_deep(void)
{

    unsigned int tmp_search_deep=1000;
    set<unsigned int>::iterator demand_iter;
    unsigned int demand;

    for(demand_iter=unassigned_demand.begin(); demand_iter!=unassigned_demand.end(); ++demand_iter)
    {
        demand=(demand_vector[*demand_iter])->demand;
        if(deploy_cost/demand<tmp_search_deep)
        {
            tmp_search_deep=deploy_cost/demand;
        }
    }
    max_search_deep=tmp_search_deep+1;
}

void Process::pre_process(void)
{
    for(unsigned int i=0; i<demand_cnt; i++)
    {
        Demand* demand=demand_vector[i];
        Service* service=service_vector[demand->attached_service];
        unsigned int out_bandwidth_sum=0;
        unsigned int out_cost_sum=0;
        unsigned int start_node;
        unsigned int stop_node;
        unsigned int now_bandwidth;

        stop_node=service->index;

        cout<<stop_node<<":"<<endl;

        edge_list* now_edge=service->edge_head;
        while(now_edge)
        {
            start_node=now_edge->stop_node;
            now_bandwidth=static_bandwidth[ {start_node,stop_node}];

            cout<<"edge:"<<start_node<<"->"<<stop_node<<":"<<" bandwidth:"<<now_bandwidth<<" length:"<<now_edge->length<<endl;
            if(now_bandwidth+out_bandwidth_sum >= demand->demand)
            {
                out_cost_sum+=(now_edge->length)*(demand->demand-out_bandwidth_sum);
                out_bandwidth_sum=demand->demand;
                cout<<"cost:"<<out_cost_sum<<" bandwidth:"<<out_bandwidth_sum<<endl;
                break;
            }
            else
            {
                out_bandwidth_sum+=now_bandwidth;
                out_cost_sum+=now_bandwidth*now_edge->length;
            }
            cout<<"cost:"<<out_cost_sum<<" bandwidth:"<<out_bandwidth_sum<<endl;
            now_edge=now_edge->next;
        }
        if(out_bandwidth_sum < demand->demand || out_cost_sum>deploy_cost)
        {
            //fix this service
            service->best_deepth=0;
            cout<<"demand:"<<i<<" fix ser:"<<stop_node<<" cost:"<<(float)deploy_cost/demand->demand<<" deep:0"<<endl;
            service->fix_service();
        }

    }
}

bool redistribut(unsigned int min_cost)
{
    connect_list* now_connect_node=waiting_connect_head;
    Connect* now_connect;

//    cout<<"waiting list:"<<endl;
//    while(now_connect_node){
//        now_connect=now_connect_node->connect;
//        cout<<now_connect->service_index<<"->"<<now_connect->demand_index<<": ";
//        cout<<now_connect->length<<" "<<now_connect->bandwidth;
//        cout<<now_connect->scheme2string()<<endl;
//
//        now_connect_node=now_connect_node->next;
//    }

    bool change_flg=false;

    while(waiting_connect_head)
    {
        now_connect_node=waiting_connect_head;
        waiting_connect_head=waiting_connect_head->next;

        now_connect=now_connect_node->connect;
        if(now_connect->length < min_cost)
        {
            now_connect->bandwidth=now_connect->leaf_node->get_max_bandwidth_f(demand_vector[now_connect->demand_index]->demand);
            if(now_connect->bandwidth)
            {
                now_connect->fix_connect();
                change_flg=true;
            }
        }
    }

    return change_flg;
}

void Process::find_scheme(void)
{
    while(!unassigned_demand.empty())
    {
        float min_average_cost=10000;
        unsigned int min_service_index;
        float tmp_average_cost;

        set<unsigned int>::iterator service_iter;
        for(service_iter=assigned_service.begin(); service_iter!=assigned_service.end(); ++service_iter)
        {
            Service* service=service_vector[*service_iter];
            service->add_connect();
        }

        for(service_iter=unassigned_service.begin(); service_iter!=unassigned_service.end(); ++service_iter)
        {
            Service* service=service_vector[*service_iter];

            service->search_connect();
            tmp_average_cost=service->get_average_cost();
            if(tmp_average_cost<min_average_cost)
            {
                if(false)
                {
                    ;
                }
                else
                {
                    min_average_cost=tmp_average_cost;
                    min_service_index=(*service_iter);
                }
            }
        }

        bool redistribu_flg=redistribut(min_average_cost);
        if(!redistribu_flg)
        {
            Service* service=service_vector[min_service_index];
            cout<<"fix ser:"<<min_service_index<<" cost:"<<min_average_cost<<" deep:"<<service_vector[min_service_index]->best_deepth<<endl;
            service->fix_service();
            max_search_deep+=1;
            if(max_search_deep>predef_deep)
            {
                max_search_deep=predef_deep;

                max_add_deep+=1;
                if(max_add_deep>predef_deep+2)
                {
                    max_add_deep=predef_deep+2;
                }
            }
            else
            {
                max_add_deep=max_search_deep;
            }
        }
        cout<<"search deep:"<<max_search_deep<<endl;
    }
    cout<<endl<<endl<<endl<<"  GOOD JOB !!!!"<<endl<<endl;
}

string Process::scheme2string_f(void)
{
    string out_string="";
    unsigned int connect_cnt=0;

    set<unsigned int>::iterator service_iter;
    string service_string="";
    for(service_iter=assigned_service.begin(); service_iter!=assigned_service.end(); ++service_iter)
    {
        Service* service=service_vector[*service_iter];
        service_string=service->scheme2string_f();

        connect_cnt+=service->connect_queue.size();
        out_string=out_string.append(service_string);
    }

    string connect_cnt_string=SSTR(connect_cnt);
    connect_cnt_string.append("\n");
    out_string=connect_cnt_string.append(out_string);

    return out_string;
}

void print_map(map<pair<unsigned int, unsigned int>, signed int> tmp_map)
{
    unsigned int remander=tmp_map.size()%5;
    unsigned int quoitent=tmp_map.size()/5;
    map<pair<unsigned int, unsigned int>, signed int>::iterator bandwidth_map_iter;

    bandwidth_map_iter=tmp_map.begin();
    for(unsigned int i=0; i<remander; i++)
    {
        cout<<(bandwidth_map_iter->first).first<<","<<(bandwidth_map_iter->first).second<<":" <<(bandwidth_map_iter->second)<<"; ";
        ++bandwidth_map_iter;
    }
    if(remander)
    {
        cout<<endl;
    }

    for(unsigned int i=0; i<quoitent; i++)
    {
        for(unsigned int j=0; j<5; j++)
        {
            cout<<(bandwidth_map_iter->first).first<<","<<(bandwidth_map_iter->first).second<<":" <<(bandwidth_map_iter->second)<<"; ";
            ++bandwidth_map_iter;
        }
        cout<<endl;
    }
    if(bandwidth_map_iter==tmp_map.end())
    {
        cout<<"OK"<<endl;
    }
    else
    {
        cout<<"BAD"<<endl;
    }
}

void print_test_bandwidth(void)
{
    cout<<"bandwidth:"<<endl;
    print_map(orignal_bandwidth);
}

void print_static_bandwidth(void)
{
    cout<<"bandwidth:"<<endl;

    print_map(static_bandwidth);
}

void print_tmp_bandwidth(void)
{

    cout<<"tmp bandwidth:"<<endl;
    print_map(tmp_bandwidth);
}

void print_test_demand(void)
{
    cout<<"demand:";
    for(unsigned int i=0; i<demand_cnt; i++)
    {
        cout<<" "<<orignal_demand[i];
    }
    cout<<endl;
}
void demand_vaild_detect(void)
{
    for(unsigned int i=0; i<demand_cnt; i++)
    {
        if(orignal_demand[i]!=0)
        {
            valid_scheme=false;
        }
    }
}


