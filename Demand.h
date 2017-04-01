



class Demand
{
public:
    Demand(
    unsigned int _index,
    unsigned int _attached_service,
    unsigned int _demand):
    index(_index),
    attached_node(_attached_service),
    demand(_demand),
    test_demand(_demand),
    tmp_demand(_demand),
    assigned(false),
    connect_head(NULL),
    fix_connect_head(NULL)
    {};

    void print_demand(void);
    void search_connect(unsigned int deepth,bool is_assigned);
    void print_connect(void);

public:
	unsigned int index;
	unsigned int attached_node;
	unsigned int demand;
	unsigned int tmp_demand;
	int test_demand;
	set<unsigned int> service_set;
	bool assigned;
	struct connect_list* connect_head;
	struct connect_list* fix_connect_head;
};

struct search_node{
    unsigned int index;
    unsigned int deepth;
    unsigned int bandwidth;
    struct edge* edge;
    search_node* parent;


    search_node(unsigned int _index,unsigned int _bandwidth)
    {
        index=_index;
        deepth=0;
        parent=NULL;
        bandwidth=_bandwidth;
    };

    search_node(unsigned int _index, unsigned int _deepth,struct edge* _edge, struct search_node* _parent, unsigned int _bandwidth)
    {
        index=_index;
        deepth=_deepth;
        edge=_edge;
        parent=_parent;
        bandwidth=_bandwidth;
    };
};

void print_connet_list(connect_list* head);
inline connect_list* insert_connect(connect_list* head,connect_list* new_node);
inline connect_list* insert_connect_head(connect_list* head, connect_list* new_head);



