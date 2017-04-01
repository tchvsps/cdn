#include"basic.h"
inline connect_list* insert_connect_head(connect_list* head, connect_list* new_head)
{
    new_head->bigger=NULL;
    if(!head){return new_head;}
    if(head->connect->length > new_head->connect->length)
    {
        new_head->bigger=head;
        return new_head;
    }

    connect_list* now_head=head;
    while(now_head->bigger)
    {
        if(now_head->bigger->connect->length >= new_head->connect->length)
        {
            new_head->bigger=now_head->bigger;
            now_head->bigger=new_head;
            return head;
        }

        now_head=now_head->bigger;
    }
    now_head->bigger=new_head;
    return head;
}

void Service::update_service_cnt(void)
{
    set<unsigned int>::iterator demand_iter;
    for(demand_iter=demand_set.begin(); demand_iter!=demand_set.end(); demand_iter++)
    {
        Demand* demand=demand_vector[*demand_iter];
        demand->service_cnt++;
    }
}

void Service::init_connect(void)
{
    for()
}


void Service::print_connect(void)
{
    cout<<"service "<<index<<"'s connect: ";
    print_connet_list(connect_head);
    cout<<endl;
}

string Service::to_string(void)
{
    string service_string="";
//    connect_list* list_head;
//    connect_list* list_node;
//    cost_sum+=deploy_cost;
//
//    list_head=fix_connect_head;
//    while(list_head)
//    {
//        list_node=list_head;
//        while(list_node)
//        {
//            if(list_node->connect->best_connect_bandwidth)
//            {
//                service_string=service_string.append(list_node->connect->to_string_f());
//                next_fix_cnt++;
//
//                cost_sum+=list_node->connect->best_connect_bandwidth*list_node->connect->length;
//                if(list_node->connect->length > max_length)
//                    max_length=list_node->connect->length;
//            }
//
//            list_node=list_node->next;
//        }
//
//        list_head=list_head->bigger;
//    }
    return service_string;
}
