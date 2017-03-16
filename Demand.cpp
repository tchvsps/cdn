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
    return false;
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
