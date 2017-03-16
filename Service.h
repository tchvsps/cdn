#include"basic.h"
class Service
{
public:
    Service(unsigned int _index):
    index(_index),
    attached_demand(NO_CONNECT_DEMAND),
    edge_head(NULL),
    search_head(NULL)
    {};

    void search_connect(void);
    void jude_new_node(Tree_node* new_tree_node);
    void add_connect(void);
    float get_average_cost(void);
    void fix_service(void);
    string scheme2string(void);
    string scheme2string_f(void);


    void print_edge_list(void);
    void print_search_list(void);
    void print_demand_set(void);
    void print_connec_queue(void);


public:
	unsigned int index;
	unsigned int attached_demand;
	edge_list* edge_head;
	Tree_node* search_head;
	queue<Tree_node*> demand_queue;
	queue<Connect*> connect_queue;
	unsigned int best_deepth;
};
