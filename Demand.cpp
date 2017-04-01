#include"basic.h"

inline connect_list* insert_connect(connect_list* head,connect_list* new_node)
{
    new_node->next=NULL;
    new_node->bigger=NULL;

    if(!head)
    {
        return new_node;
    }

    if(head->connect->length > new_node->connect->length)
    {
        new_node->bigger=head;
        return new_node;
    }
    if(head->connect->length == new_node->connect->length)
    {
        new_node->next=head;
        new_node->bigger=head->bigger;
        return new_node;
    }
    connect_list* now_node=head;
    while(now_node->bigger)
    {

        if(now_node->bigger->connect->length == new_node->connect->length)
        {
            new_node->next=now_node->bigger;
            new_node->bigger=now_node->bigger->bigger;
            now_node->bigger=new_node;
            return head;
        }

        if(now_node->bigger->connect->length > new_node->connect->length){
            new_node->bigger=now_node->bigger->bigger;
            now_node->bigger=new_node;
            return head;
        }

        if(now_node->bigger->connect->length < new_node->connect->length)
        {
            now_node=now_node->bigger;
        }
    }

    now_node->bigger=new_node;
    return head;
}



void print_connet_list(connect_list* head){
    connect_list* node;
    while(head){
        cout<<"len="<<head->connect->length<<" :"<<endl;

        node=head;
        while(node){
            if(node->connect->connect_bandwidth){
                cout<<node->connect->to_string()<<endl;
            }
            node=node->next;
        }

        head=head->bigger;
    }
}

void Demand::print_demand(void){
//    for(unsigned int i=0; i<demand_cnt; i++){
//        printf(" %2d",i);
//    }
//    cout<<endl;

    for(unsigned int i=0; i<demand_cnt; i++){
        printf(" %2d",demand_vector[i]->demand);
    }
    cout<<endl;
    for(unsigned int i=0; i<demand_cnt; i++){
        printf(" %2d",demand_vector[i]->tmp_demand);
    }
    cout<<endl;
}

bool is_searched(search_node* leaf_node, unsigned int new_node_index){

    while(leaf_node){
        if(leaf_node->index == new_node_index){return true;}
        leaf_node=leaf_node->parent;
    }
    return false;
}
void Demand::search_connect(unsigned int deepth, bool is_assigned)
{
    queue<search_node*> search_queue;

    struct search_node* leaf_node=new search_node(attached_node,demand);
    search_queue.push(leaf_node);
    Connect* new_connect=new Connect(index,attached_node,0,demand,leaf_node);

//    cout<<"new connect:";
//    cout<<new_connect->to_string()<<endl;
    //add to service
    Service* service;
    if(unassigned_service.find(new_connect->node_index)==unassigned_service.end())
    {
        service=new Service(new_connect->node_index);
        node2service[new_connect->node_index]=service;
        unassigned_service.insert(new_connect->node_index);
    }
    else{
        service=node2service[new_connect->node_index];
    }
    connect_list* new_connect_list;

    new_connect_list=new connect_list(new_connect);

    if(is_assigned){
        waiting_connect_head=insert_connect(waiting_connect_head,new_connect_list);
        service->demand_set.insert(new_connect->demand_index);
    }else{
        service->connect_head=insert_connect(service->connect_head,new_connect_list);
        service->demand_set.insert(new_connect->demand_index);
    }


    //add to  demand
    new_connect_list=new connect_list(new_connect);
    Demand* demand=demand_vector[new_connect->demand_index];
    demand->connect_head=insert_connect(demand->connect_head,new_connect_list);

    demand->service_set.insert(new_connect->node_index);



    while(!search_queue.empty()){
        struct search_node* search_node=search_queue.front();
        search_queue.pop();

        edge* edge_node=node_vector[search_node->index]->edge_head;
        while(edge_node){

            if(search_node->deepth+edge_node->length > deepth)
            {
                break;
            }
            else{
                //jude if searched
                if(!is_searched(search_node,edge_node->stop_node))
                {
                    //create new node

                    struct search_node* leaf_node=new struct search_node(edge_node->stop_node, edge_node->length+search_node->deepth, edge_node,search_node,min(edge_node->edge_bandwidth,search_node->bandwidth));
                    search_queue.push(leaf_node);

                    //create new connect
                    Connect* new_connect=new Connect(index,edge_node->stop_node,leaf_node->deepth,leaf_node->bandwidth,leaf_node);

//                    cout<<"new connect:";
//                    cout<<new_connect->to_string();

                    //add to service
                    Service* service;
                    if(unassigned_service.find(new_connect->node_index)==unassigned_service.end())
                    {
                        service=new Service(new_connect->node_index);
                        node2service[new_connect->node_index]=service;
                        unassigned_service.insert(new_connect->node_index);
                    }
                    else{
                        service=node2service[new_connect->node_index];
                    }
                    connect_list* new_connect_list;

                    new_connect_list=new connect_list(new_connect);

                    if(is_assigned){
                        waiting_connect_head=insert_connect(waiting_connect_head,new_connect_list);
                        service->demand_set.insert(new_connect->demand_index);
                    }else{
                        service->connect_head=insert_connect(service->connect_head,new_connect_list);
                        service->demand_set.insert(new_connect->demand_index);
                    }

                    //add to  demand
                    new_connect_list=new connect_list(new_connect);
                    Demand* demand=demand_vector[new_connect->demand_index];
                    demand->connect_head=insert_connect(demand->connect_head,new_connect_list);

                    demand->service_set.insert(new_connect->node_index);

                    //add to edge
                    struct search_node* start_node;
                    struct search_node* stop_node;
                    stop_node=new_connect->leaf_node;
                    start_node=stop_node->parent;
                    while(start_node){
                        struct edge* edge=stop_node->edge;
                        edge->connect_vector.push_back(new_connect);

                        stop_node=start_node;
                        start_node=start_node->parent;
                    }
                }
            }

            edge_node=edge_node->next;
        }
    }
}


void Demand::print_connect(void)
{
    cout<<"demand "<<index<<"'s connect: ";
    print_connet_list(connect_head);
    cout<<endl;
}


