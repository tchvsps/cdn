#include"basic.h"
//class Connect
//{
//public:
//
//public:
//	unsigned int demand_index;
//	unsigned int service_index;
//	unsigned int length;
//	unsigned int bandwidth;
//	Tree_node* leaf_node;
//};
//
//typedef struct edge_list
//{
//	unsigned int length;
//	unsigned int start_node;
//	unsigned int stop_node;
//	struct edge_list* next;
//}edge_list;

void Connect::tmp_fix_connect(void)
{
    demand_vector[demand_index]->tmp_demand-=bandwidth;
    leaf_node->update_tmp_bandwidth(bandwidth);
}

void Connect::fix_connect(void)
{
    demand_vector[demand_index]->demand-=bandwidth;

    if(demand_vector[demand_index]->demand == 0)
    {
        assigned_demand.insert(demand_index);
        demand_vector[demand_index]->assigned=true;

        set<unsigned int>::iterator demand_iter;
        for(demand_iter=unassigned_demand.begin(); demand_iter!=unassigned_demand.end(); ++demand_iter)
        {
            if(*demand_iter == demand_index)
            {
                unassigned_demand.erase(demand_iter);
            }
        }
    }
    leaf_node->update_static_bandwidth(bandwidth);

}

string Connect::scheme2string(void)
{
    string connect_string="";
    string node_string="";
    Tree_node* now_node=leaf_node;
    while(now_node)
    {
        node_string=SSTR(now_node->index);
        node_string.append(" ");
        connect_string=node_string.append(connect_string);
        now_node=now_node->parent;
    }
    connect_string.append(SSTR(demand_index));

    connect_string.append(" ");
    connect_string.append(SSTR(bandwidth));
    connect_string=string("\n").append(connect_string);

    return connect_string;
}

string Connect::scheme2string_f(void)
{
    string connect_string="";
    string node_string="";

    //move demand
    orignal_demand[demand_index]-=bandwidth;
    if(orignal_demand[demand_index]<0){
        valid_scheme=false;
    }

    Tree_node* now_node=leaf_node;
    while(now_node)
    {
        if(now_node->parent)
        {
            orignal_bandwidth[ {now_node->parent->index,now_node->index}]-=bandwidth;
            if(orignal_bandwidth[ {now_node->parent->index,now_node->index} ]<0){
                valid_scheme=false;
            }
        }
        node_string=SSTR(now_node->index);
        node_string.append(" ");
        connect_string=node_string.append(connect_string);
        now_node=now_node->parent;
    }
    connect_string.append(SSTR(demand_index));

    connect_string.append(" ");
    connect_string.append(SSTR(bandwidth));
    connect_string=string("\n").append(connect_string);

    return connect_string;
}
