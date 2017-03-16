#include"basic.h"

//class Demand
//{
//public:
//
//public:
//	unsigned int index;
//	unsigned int attached_service;
//	unsigned int demand;
//	unsigned int tmp_demand;
//	queue<Connect*> connect_queue;
//};

bool Demand::redistribute(unsigned int next_cost){
//    print_connect_list();
    return false;
}

void Demand::print_connect_list(void){
    connect_list* now_connect_node=connect_head;
    while(now_connect_node){
        Connect* now_connect=now_connect_node->connect;
        cout<<now_connect->service_index<<"->"<<now_connect->demand_index<<": ";
        cout<<now_connect->length<<" "<<now_connect->bandwidth<<endl;
        now_connect_node=now_connect_node->next;
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



