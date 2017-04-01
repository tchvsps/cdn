#include"basic.h"
class Service
{
public:
    void add_connect(void);
    float get_average_cost(void);

    void print_connect(void);

    Service(unsigned int _node_index):
    index(_node_index),
    connect_head(NULL),
    fix_connect_head(NULL),
    best_deepth(0),
    bandwidth_sum(0)
    {}
public:

    unsigned int index;
	struct connect_list* connect_head;
	set<unsigned int> demand_set;
	struct connect_list* fix_connect_head;
	unsigned int best_deepth;
	unsigned int bandwidth_sum;
	string to_string(void);

	void update_service_cnt(void);
	void init_connect(void);
};

struct node{
    unsigned int index;
    unsigned int attached_demand;

    struct edge* edge_head;

    node(unsigned int _index){
        index=_index;
        attached_demand=NO_CONNECT_DEMAND;
        edge_head=NULL;
    }
};
