#include"basic.h"
//class Tree_node
//{
//public:
//
//public:
//	unsigned int index;
//	unsigned int deepth;
//	Tree_node* parent;
//	edge_list* bfs_position;
//};
//
//typedef struct tree_node_list
//{
//	Tree_node* tree_node;
//	unsigned int length;
//
//	struct tree_node_list* next;
//}tree_node_list;
//

bool Tree_node::is_search(unsigned int node_index){
    Tree_node* tmp_node=parent;

    while(tmp_node){
        if(tmp_node->index == node_index){
            return true;
        }
        tmp_node=tmp_node->parent;
    }
    return false;
}

unsigned int Tree_node::get_max_bandwidth(unsigned int bandwidth){
    Tree_node* start_node;
    Tree_node* stop_node;
    start_node=parent;
    stop_node=this;

    while(start_node){
        unsigned int edge_bandwidth;
        if(tmp_bandwidth.find({ start_node->index, stop_node->index }) != tmp_bandwidth.end()){
            edge_bandwidth=tmp_bandwidth[{ start_node->index, stop_node->index }];
        }else{
            tmp_bandwidth[{ start_node->index, stop_node->index }]=static_bandwidth[{ start_node->index, stop_node->index }];
            edge_bandwidth=tmp_bandwidth[{ start_node->index, stop_node->index }];
        }
        if(edge_bandwidth<bandwidth){
            bandwidth=edge_bandwidth;
        }
//        cout<<start_node->index<<"->"<<stop_node->index<<"("<<edge_bandwidth<<")"<<endl;
        stop_node=start_node;
        start_node=start_node->parent;

    }
    return bandwidth;
}

void Tree_node::update_tmp_bandwidth(unsigned int bandwidth){
    Tree_node* start_node;
    Tree_node* stop_node;
    start_node=parent;
    stop_node=this;

    while(start_node){

        if(tmp_bandwidth.find({ start_node->index, stop_node->index }) != tmp_bandwidth.end()){
            ;
        }else{
            tmp_bandwidth[{ start_node->index, stop_node->index }]=static_bandwidth[{ start_node->index, stop_node->index }];
        }

        tmp_bandwidth[{ start_node->index, stop_node->index }]-=bandwidth;

        stop_node=start_node;
        start_node=start_node->parent;
    }
}

void Tree_node::update_static_bandwidth(unsigned int bandwidth){
    Tree_node* start_node;
    Tree_node* stop_node;
    start_node=parent;
    stop_node=this;

    while(start_node){
        static_bandwidth[{ start_node->index, stop_node->index }]-=bandwidth;

        stop_node=start_node;
        start_node=start_node->parent;
    }
}

void Tree_node::print_route(void){
    Tree_node* now_node=this;

    string route_string="";
    string node_string="";
    while(now_node){
        node_string=SSTR(now_node->index);
        node_string.append(" ");
        route_string=node_string.append(route_string);
        now_node=now_node->parent;
    }
    cout<<route_string<<endl;
}
