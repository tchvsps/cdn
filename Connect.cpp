#include"basic.h"


void update_edge_bandwidth(struct edge* edge, int bandwidth)
{
    edge->edge_bandwidth-=bandwidth;
    if(edge->edge_bandwidth>10000)
    {
        cout<<"ERROR"<<endl;
    }

    Connect* connect;
    for(unsigned int i=0; i<edge->connect_vector.size(); i++)
    {
        connect=edge->connect_vector[i];
        connect->connect_bandwidth=min(connect->connect_bandwidth,edge->edge_bandwidth);
    }
}


void Connect::fix_connect(void)
{
//    cout<<"best bandwidth:"<<best_connect_bandwidth<<endl;
    demand_vector[demand_index]->demand-=best_connect_bandwidth;

    if(demand_vector[demand_index]->demand == 0)
    {
        assigned_demand.insert(demand_vector[demand_index]);
        if(best_connect_bandwidth)
        {
            unassigned_demand.erase(unassigned_demand.find(demand_vector[demand_index]));
            demand_vector[demand_index]->assigned=true;
        }

    }
    //update the edge tmp_bandwidth
    struct search_node* stop_node;
    struct search_node* start_node;

    stop_node=leaf_node;
    start_node=stop_node->parent;

//    cout<<"fix connect:"<<this->to_string()<<endl;
    while(start_node){
        update_edge_bandwidth(stop_node->edge,best_connect_bandwidth);

        stop_node=start_node;
        start_node=start_node->parent;
    }
}

void Connect::edge_tmp_init(void)
{
    struct search_node* stop_node;
    struct search_node* start_node;

    stop_node=leaf_node;
    start_node=stop_node->parent;
    while(start_node){
        stop_node->edge->edge_bandwidth=stop_node->edge->tmp_edge_bandwidth;

        stop_node=start_node;
        start_node=start_node->parent;
    }
}

string Connect::to_string(void)
{
//    string connect_string="\n";
    string connect_string="";

    struct search_node* stop_node;
    struct search_node* start_node;

    stop_node=leaf_node;
    start_node=stop_node->parent;
    while(start_node){
        connect_string=connect_string.append(SSTR(stop_node->index));
        connect_string=connect_string.append(" ");
        stop_node=start_node;
        start_node=start_node->parent;
    }
    connect_string=connect_string.append(SSTR(stop_node->index));
    connect_string=connect_string.append(" -> ");

    connect_string=connect_string.append(SSTR(node_vector[stop_node->index]->attached_demand));
    connect_string=connect_string.append(" ");

//    connect_string=connect_string.append(SSTR(length));

//    connect_string=connect_string.append(SSTR(bandwidth));
    connect_string=connect_string.append(" ");
    connect_string=connect_string.append(SSTR(best_connect_bandwidth));
    return connect_string;
}

string Connect::to_string_f(void)
{
//    demand_vector[demand_index]->test_demand-=best_connect_bandwidth;
//    if(demand_vector[demand_index]->test_demand < 0)
//    {
//        cout<<"ERROR CONNECT DEMAND"<<endl;
//        cout<<this->to_string();
//        cout<<endl;
//    }
//    string connect_string="\n";
//    struct search_node* stop_node;
//    struct search_node* start_node;
//
//    stop_node=leaf_node;
//    start_node=stop_node->parent;
//    while(start_node){
//        connect_string=connect_string.append(SSTR(stop_node->index));
//        connect_string=connect_string.append(" ");
//        stop_node->edge->test_edge_bandwidth-=best_connect_bandwidth;
//        stop_node->edge->wast_band+=best_connect_bandwidth;
//
//        unsigned int wast_bandwidth=min(stop_node->edge->wast_band,stop_node->edge->brother->wast_band);
//        if(wast_bandwidth)
//        {
//            wast_cost+=(stop_node->edge->length)*wast_bandwidth*2;
//            stop_node->edge->wast_band-=wast_bandwidth;
//            stop_node->edge->brother->wast_band-=wast_bandwidth;
//        }
//
//        if(stop_node->edge->test_edge_bandwidth<0)
//        {
//            valid_scheme=false;
//            cout<<"ERROR CONNECT EDGE"<<endl;
//            cout<<this->to_string()<<endl;
//            cout<<stop_node->edge->start_node<<"->"<<stop_node->edge->stop_node<<endl;
//            cout<<endl;
//        }
//
//        stop_node=start_node;
//        start_node=start_node->parent;
//    }
//    connect_string=connect_string.append(SSTR(stop_node->index));
//    connect_string=connect_string.append(" ");
//
//    connect_string=connect_string.append(SSTR(node_vector[stop_node->index]->attached_demand));
//    connect_string=connect_string.append(" ");
//
//    connect_string=connect_string.append(SSTR(best_connect_bandwidth));
//    return connect_string;
}

