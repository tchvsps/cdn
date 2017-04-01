#ifndef __CONNECT_H
#define __CONNECT_H
#include"basic.h"

class Connect
{
public:
    Connect(
    unsigned int _demand,
    unsigned int _service,
    unsigned int _length,
    unsigned int _connect_bandwidth,
    struct search_node* _leaf_node
    ):
    demand_index(_demand),
    node_index(_service),
    length(_length),
    connect_bandwidth(_connect_bandwidth),
    tmp_connect_bandwidth(_connect_bandwidth),
    leaf_node(_leaf_node)
    {};

    void tmp_fix_connect(void);
    void fix_connect(void);
    void edge_tmp_init(void);
    string to_string(void);
    string to_string_f(void);

public:
	unsigned int demand_index;
	unsigned int node_index;
	unsigned int length;
	unsigned int connect_bandwidth;
	unsigned int tmp_connect_bandwidth;
	unsigned int best_connect_bandwidth;
    struct search_node* leaf_node;
};

struct connect_list{
    Connect* connect;
    connect_list* next;
    connect_list* end_node;
    connect_list* bigger;
    connect_list(Connect* _connect){
        connect=_connect;
        next=NULL;
        bigger=NULL;
        end_node=NULL;
    }
};
inline connect_list* insert_connect(connect_list* head,connect_list* new_node);

#endif


