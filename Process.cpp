#include"basic.h"

unsigned int node_cnt,demand_cnt,edge_cnt,deploy_cost;
unsigned int cost_sum=0;
bool valid_scheme=true;
unsigned int predef_deep=0;

set<unsigned int> fix_service_set;
map<unsigned int, Service*> node2service;

set<Service*> assigned_service;
set<Demand*> assigned_demand;
set<Demand*> unassigned_demand;

vector<Demand*> demand_vector;
vector<Service*> service_vector;
vector<struct node*> node_vector;

connect_list* waiting_connect_head;

void Process::init_graph(char * topo[], int line_num)
{
    sscanf(topo[0],"%d %d %d",&node_cnt,&edge_cnt,&demand_cnt);
    sscanf(topo[2],"%d",&deploy_cost);

    for(unsigned int i=0; i<node_cnt; i++)
    {
        node_vector.push_back(new struct node(i));
    }
    unsigned int start_node,stop_node,bandwidth,length;
    struct edge *new_edge_one,*new_edge_two,*head;
    for(unsigned int i=4; i<4+edge_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d %d",&start_node,&stop_node,&bandwidth,&length);
        new_edge_one=new edge(length,start_node,stop_node,bandwidth);
        head=node_vector[start_node]->edge_head;
        node_vector[start_node]->edge_head=insert_edge(head,new_edge_one);

        new_edge_two=new edge(length,stop_node,start_node,bandwidth);
        head=node_vector[stop_node]->edge_head;
        node_vector[stop_node]->edge_head=insert_edge(head,new_edge_two);
        new_edge_two->brother=new_edge_one;
        new_edge_one->brother=new_edge_two;
    }

    unsigned int demand_index,index,demand;
    for(unsigned int i=5+edge_cnt; i<5+edge_cnt+demand_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d",&demand_index,&index,&demand);
        demand_vector.push_back(new Demand(demand_index,index,demand));

        node_vector[index]->attached_demand=demand_index;
    }
}

inline edge* insert_edge(edge* head, edge* new_node)
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
        edge* parent_node=head;

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

void Process::pre_process(void)
{
    for(unsigned int i=0; i<demand_cnt; i++)
    {
        Demand* demand=demand_vector[i];
        struct node* node=node_vector[demand->attached_node];
        unsigned int out_bandwidth_sum=0;
        unsigned int out_cost_sum=0;
        unsigned int now_bandwidth;
        edge* now_edge=node->edge_head;
        while(now_edge)
        {
            now_bandwidth=now_edge->edge_bandwidth;
            if(now_bandwidth+out_bandwidth_sum >= demand->demand)
            {
                out_cost_sum+=(now_edge->length)*(demand->demand-out_bandwidth_sum);
                out_bandwidth_sum=demand->demand;
                break;
            }
            else
            {
                out_bandwidth_sum+=now_bandwidth;
                out_cost_sum+=now_bandwidth*now_edge->length;
            }
            now_edge=now_edge->next;
        }
        if(out_bandwidth_sum < demand->demand || out_cost_sum>deploy_cost)
        {
            fix_service_set.insert(demand->attached_node);
        }
    }
}

void Process::search_connect(void)
{
    for(unsigned int i=0; i<demand_cnt; i++)
    {
        Demand* demand=demand_vector[i];
        demand->search_connect(predef_deep);
    }
}

void Process::find_scheme(void)
{
    //init deamd
    unassigned_demand.clear();
    assigned_demand.clear();
    for(unsigned int i=0; i<demand_cnt; i++)
    {
        Demand* demand=demand_vector[i];
        demand->demand=demand->test_demand;
        demand->service_cnt=0;
        unassigned_demand.insert(demand);
    }

    assigned_service.clear();
    set<unsigned int>::iterator service_index_iter;
    for(service_index_iter=fix_service_set.begin(); service_index_iter!=fix_service_set.end(); service_index_iter++)
    {
        Service* service=node2service[*service_index_iter];
        assigned_service.insert(service);
        service->update_service_cnt();
    }

    //init service
    for(unsigned int i=0; i<demand_cnt; i++)
    {
        Demand* demand=demand_vector[i];
        if(demand->service_cnt) {continue;}

        Service* service=node2service[demand->rand_service()];
        assigned_service.insert(service);
        service->update_service_cnt();
    }
    //init connect list

    set<Service*>::iterator service_iter;
    for(service_iter=assigned_service.begin(),service_iter!=assigned_service.end();service_iter++)
    {

    }
    //init edge


    while(!unassigned_demand.empty())
    {

    }
    cout<<endl<<endl<<endl<<"  GOOD JOB !!!!"<<endl<<endl;
    cout<<"find one cost:"<<endl;
}

string Process::scheme2string(void)
{
//    string out_string="";
//
//    set<unsigned int>::iterator service_iter;
//    for(service_iter=assigned_service.begin(); service_iter!=assigned_service.end(); service_iter++)
//    {
//
//        Service* service=node2service[*service_iter];
////        print_connet_list(service->fix_connect_head);
////        cout<<endl<<endl<<service->to_string()<<endl;
//        out_string=out_string.append(service->to_string());
//
//    }
//    return out_string;
}

