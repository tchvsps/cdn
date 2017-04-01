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

float Service::get_average_cost(void)
{
    //init tmp_demand
    set<unsigned int>::iterator demand_iter;
    for(demand_iter=unassigned_demand.begin(); demand_iter!=unassigned_demand.end(); demand_iter++)
    {
        Demand* demand=demand_vector[*demand_iter];
        demand->tmp_demand=demand->demand;
    }
    //init tmp_connect_bandwidth
    connect_list* list_head;
    connect_list* list_node;
    list_head=connect_head;
    while(list_head)
    {
        list_node=list_head;
        while(list_node)
        {
            list_node->connect->tmp_connect_bandwidth=list_node->connect->connect_bandwidth;
            list_node->connect->edge_tmp_init();
            list_node=list_node->next;
        }

        list_head=list_head->bigger;
    }

    unsigned int cost_sum=deploy_cost;
    unsigned int bandwidth_sum=0;
    unsigned int tmp_bandwidth_sum=0;

    float min_average_cost=1000;
    float tmp_average_cost=1000;

    struct connect_list* head=connect_head;
    struct connect_list* node;
    struct connect_list* last_head=connect_head;

//    cout<<endl<<"  service "<<index<<endl;
//    print_connet_list(connect_head);

    while(head)
    {
        if(head->connect->length > min_average_cost){
            this->bandwidth_sum=bandwidth_sum;
            this->best_deepth=last_head->connect->length;
            return min_average_cost;
        }

        node=head;
        tmp_bandwidth_sum=0;
        while(node)
        {
            Connect* connect=node->connect;


//            cout<<"tmp:"<<connect->tmp_connect_bandwidth<<" dem:"<<demand_vector[connect->demand_index]->tmp_demand<<endl;
            connect->best_connect_bandwidth=min(connect->tmp_connect_bandwidth,demand_vector[connect->demand_index]->tmp_demand);
            if(connect->best_connect_bandwidth)
            {
                tmp_bandwidth_sum+=connect->best_connect_bandwidth;
//                cout<<connect->to_string()<<endl;
                node->connect->tmp_fix_connect();
            }
//            cout<<"tmp:"<<connect->tmp_connect_bandwidth<<" dem:"<<demand_vector[connect->demand_index]->tmp_demand<<endl;

            node=node->next;
        }
        if(bandwidth_sum+tmp_bandwidth_sum==0){tmp_average_cost=1000;}
        else{tmp_average_cost=(float)(cost_sum+tmp_bandwidth_sum*head->connect->length)/(tmp_bandwidth_sum+bandwidth_sum);}

        if(tmp_average_cost<=min_average_cost){
            bandwidth_sum+=tmp_bandwidth_sum;
            cost_sum+=tmp_bandwidth_sum*head->connect->length;
            min_average_cost=tmp_average_cost;
//            cout<<"band sum:"<<bandwidth_sum<<" cost sum:"<<cost_sum<<" ave cost:"<<min_average_cost<<endl;

        }else{
            this->bandwidth_sum=bandwidth_sum;
            this->best_deepth=last_head->connect->length;
            return min_average_cost;
        }


        last_head=head;
        head=head->bigger;
    }

    if(head){
        this->bandwidth_sum=bandwidth_sum;
        this->best_deepth=last_head->connect->length;
    }else{
        this->bandwidth_sum=0;
        this->best_deepth=0;
    }

    return min_average_cost;
}

void Service::fix_service(void)
{
    //update assigned service
    assigned_service.insert(index);
    unassigned_service.erase(unassigned_service.find(index));

    connect_list* list_head;
    connect_list* list_node;

    while(connect_head)
    {
        list_head=connect_head;
        connect_head=connect_head->bigger;

        if(list_head->connect->length<=best_deepth)
        {
            //add to fix connect
            list_head->bigger=fix_connect_head;
            fix_connect_head=list_head;

            //fix connect
            list_node=list_head;
            while(list_node)
            {
                if(list_node->connect->best_connect_bandwidth)
                {
                    list_node->connect->fix_connect();
                }

                list_node=list_node->next;
            }
        }
        else
        {
            //add to wait connet list
            waiting_connect_head=insert_connect_head(waiting_connect_head,list_head);
        }
    }
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
    connect_list* list_head;
    connect_list* list_node;
    cost_sum+=deploy_cost;

    list_head=fix_connect_head;
    while(list_head)
    {
        list_node=list_head;
        while(list_node)
        {
            if(list_node->connect->best_connect_bandwidth)
            {
                service_string=service_string.append(list_node->connect->to_string_f());
                next_fix_cnt++;

                cost_sum+=list_node->connect->best_connect_bandwidth*list_node->connect->length;
                if(list_node->connect->length > max_length)
                    max_length=list_node->connect->length;
            }

            list_node=list_node->next;
        }

        list_head=list_head->bigger;
    }
    return service_string;
}
