#include"basic.h"

unsigned int node_cnt;
unsigned int demand_cnt;
unsigned int edge_cnt;
unsigned int deploy_cost;

unsigned int cost_sum=0;
unsigned int wast_cost=0;
bool valid_scheme=true;

unsigned int predef_deep=0;

set<unsigned int> fix_service_set;
set<unsigned int> assigned_service;
set<unsigned int> unassigned_service;
set<unsigned int> assigned_demand;
set<unsigned int> unassigned_demand;

vector<Demand*> demand_vector;
vector<Service*> service_vector;
vector<struct node*> node_vector;

connect_list* waiting_connect_head;

map<unsigned int, Service*> node2service;

vector<signed int> orignal_demand;

//var for pre process
unsigned int pre_fix_cnt=0;
unsigned int next_fix_cnt=0;
string pre_fix_str="";

unsigned int max_length=0;
void Process::init_graph(char * topo[], int line_num)
{
    sscanf(topo[0],"%d %d %d",&node_cnt,&edge_cnt,&demand_cnt);
    sscanf(topo[2],"%d",&deploy_cost);

    for(unsigned int i=0; i<node_cnt; i++)
    {
        node_vector.push_back(new struct node(i));
    }

    unsigned int start_node;
    unsigned int stop_node;
    unsigned int bandwidth;
    unsigned int length;

    //init service info: edge
    for(unsigned int i=4; i<4+edge_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d %d",&start_node,&stop_node,&bandwidth,&length);

        edge* new_edge_one;
        edge* new_edge_two;
        edge* head;

        new_edge_one=new edge(length,start_node,stop_node,bandwidth);
        head=node_vector[start_node]->edge_head;
        node_vector[start_node]->edge_head=insert_edge(head,new_edge_one);

        new_edge_two=new edge(length,stop_node,start_node,bandwidth);
        head=node_vector[stop_node]->edge_head;
        node_vector[stop_node]->edge_head=insert_edge(head,new_edge_two);
        new_edge_two->brother=new_edge_one;
        new_edge_one->brother=new_edge_two;
    }

    unsigned int demand_index;
    unsigned int index;
    unsigned int demand;

    //init demand info
    //init service demand connect info
    for(unsigned int i=5+edge_cnt; i<5+edge_cnt+demand_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d",&demand_index,&index,&demand);
        demand_vector.push_back(new Demand(demand_index,index,demand));

        orignal_demand.push_back(demand);

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

//        cout<<stop_node<<":"<<endl;

        edge* now_edge=node->edge_head;
        while(now_edge)
        {
            now_bandwidth=now_edge->edge_bandwidth;

//            cout<<"edge:"<<start_node<<"->"<<stop_node<<":"<<" bandwidth:"<<now_bandwidth<<" length:"<<now_edge->length<<endl;
            if(now_bandwidth+out_bandwidth_sum >= demand->demand)
            {
                out_cost_sum+=(now_edge->length)*(demand->demand-out_bandwidth_sum);
                out_bandwidth_sum=demand->demand;
//                cout<<"cost:"<<out_cost_sum<<" bandwidth:"<<out_bandwidth_sum<<endl;
                break;
            }
            else
            {
                out_bandwidth_sum+=now_bandwidth;
                out_cost_sum+=now_bandwidth*now_edge->length;
            }
//            cout<<"cost:"<<out_cost_sum<<" bandwidth:"<<out_bandwidth_sum<<endl;
            now_edge=now_edge->next;
        }
        if(out_bandwidth_sum < demand->demand || out_cost_sum>deploy_cost)
        {
//            cout<<endl;
//            cout<<endl;
//            cout<<"demand:"<<i<<" fix ser:"<<stop_node<<"cost:"<<(float)deploy_cost/demand->demand<<" deep:0"<<endl;
//            cout<<endl;
//            cout<<endl;

            pre_fix_cnt++;
            cost_sum+=deploy_cost;
            string connect_str="\n";
            connect_str=connect_str.append(SSTR(demand->attached_node));
            connect_str=connect_str.append(" ");
            connect_str=connect_str.append(SSTR(demand->index));
            connect_str=connect_str.append(" ");
            connect_str=connect_str.append(SSTR(demand->demand));
            pre_fix_str.append(connect_str);

            demand->demand=0;
            demand->test_demand=0;
            fix_service_set.insert(demand->attached_node);
            assigned_demand.insert(i);

        }else{
            unassigned_demand.insert(i);
        }
    }
}

void Process::search_connect(void)
{
    set<unsigned int>::iterator demand_iter;
    for(demand_iter=unassigned_demand.begin(); demand_iter!=unassigned_demand.end(); demand_iter++){
        Demand* demand=demand_vector[*demand_iter];
        demand->search_connect(predef_deep,false);
    }
    for(demand_iter=assigned_demand.begin(); demand_iter!=assigned_demand.end(); demand_iter++){
        Demand* demand=demand_vector[*demand_iter];
        demand->search_connect(predef_deep,true);
    }
}

bool redistribut(unsigned int min_cost)
{
    connect_list* list_head=waiting_connect_head;
    connect_list* list_node;
    Connect* now_connect;

//    cout<<"waiting list:"<<endl;
//    print_connet_list(waiting_connect_head);

    bool change_flg=false;

    while(waiting_connect_head)
    {
        if(waiting_connect_head->connect->length <= min_cost)
        {
            list_head=waiting_connect_head;
            waiting_connect_head=waiting_connect_head->bigger;

            list_node=list_head;
            while(list_node)
            {
                now_connect=list_node->connect;
                now_connect->best_connect_bandwidth=min(now_connect->connect_bandwidth,demand_vector[now_connect->demand_index]->demand);
                if(now_connect->best_connect_bandwidth)
                {
                    now_connect->fix_connect();
                    Service* service=node2service[now_connect->node_index];
                    service->fix_connect_head=insert_connect(service->fix_connect_head,list_node);

//                    cout<<"fix connect:"<<now_connect->to_string()<<endl;
                    change_flg=true;
                }
               list_node=list_node->next;
            }
        }
        else{break;}
    }
    return change_flg;
}


    //for unassigned service  get average cost
    //for assigned service if the small than min average cost
    //for assigned demand if average cost too big  redistribute
        //in this process we have to
        //fix connect reovke_connect caculate tmp bandwidth caculate bandwidth
        //if there is a tmp connect fix -> update tmp bandwidth
        //if there is a connect fix -> update bandwidth

void Process::find_scheme(void)
{
    while(!unassigned_demand.empty())
    {
    //get service to fix
        //find the min
        float min_average_cost=10000;
        unsigned int min_service_index=0;
        unsigned int max_service_bandwidth=0;
        float tmp_average_cost=10000;
        set<unsigned int>::iterator service_iter;
        for(service_iter=unassigned_service.begin(); service_iter!=unassigned_service.end(); ++service_iter)
        {
            Service* service=node2service[*service_iter];
            //caculate average cost
            tmp_average_cost=service->get_average_cost();
            if(tmp_average_cost<min_average_cost)
            {
                min_average_cost=tmp_average_cost;
                min_service_index=(*service_iter);
            }
//            float value_e=(service->bandwidth_sum-max_service_bandwidth)+(min_average_cost-tmp_average_cost)*50;
//            if(value_e)
//            {
//                min_average_cost=tmp_average_cost;
//                min_service_index=(*service_iter);
//                max_service_bandwidth=service->bandwidth_sum;
//            }
//            if(service->bandwidth_sum < max_service_bandwidth)
//            {
//                if(tmp_average_cost<min_average_cost-0.2)
//                {
//                    min_average_cost=tmp_average_cost;
//                    min_service_index=(*service_iter);
//                    max_service_bandwidth=service->bandwidth_sum;
//                }
//            }else{
//                if(tmp_average_cost<min_average_cost+0.2){
//                    min_average_cost=tmp_average_cost;
//                    min_service_index=(*service_iter);
//                    max_service_bandwidth=service->bandwidth_sum;
//                }
//            }

        }

        Service* service=node2service[min_service_index];
//        cout<<"servic "<<min_service_index<<": min cost="<<min_average_cost<<" deepth="<<service->best_deepth<<" band sum="<<service->bandwidth_sum<<endl;

//        cout<<"wait connect:"<<endl;
//        print_connet_list(waiting_connect_head);

        //redistribution
        if(!redistribut(min_average_cost)){
            Service* service=node2service[min_service_index];
//            cout<<"   fix service "<<service->index<<endl;
            service->fix_service();
        }
//        cout<<"unassigned demand:"<<endl;
//        set<unsigned int>::iterator demand_iter;
//        for(demand_iter=unassigned_demand.begin(); demand_iter!=unassigned_demand.end(); demand_iter++)
//        {
//            Demand* demand=demand_vector[*demand_iter];
//            cout<<"dem "<<demand->index<<" remain "<<demand->demand<<endl;
//        }
    }
    cout<<endl<<endl<<endl<<"  GOOD JOB !!!!"<<endl<<endl;
}

string Process::scheme2string(void)
{
    string out_string="";

    set<unsigned int>::iterator service_iter;
    for(service_iter=assigned_service.begin(); service_iter!=assigned_service.end(); service_iter++)
    {

        Service* service=node2service[*service_iter];
//        print_connet_list(service->fix_connect_head);
//        cout<<endl<<endl<<service->to_string()<<endl;
        out_string=out_string.append(service->to_string());

    }
    return out_string;
}

