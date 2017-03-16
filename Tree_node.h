class Tree_node
{

public:
    Tree_node(unsigned int _index, edge_list* _first_edge):
    index(_index),
    deepth(0),
    parent(NULL),
    brother(NULL),
    bfs_position(_first_edge){};

    Tree_node(
    unsigned int _index,
    unsigned int _deepth,
    Tree_node* _parent
    ):
    index(_index),
    deepth(_deepth),
    parent(_parent),
    brother(NULL){};

    bool is_search(unsigned int node_index);
    unsigned int get_max_bandwidth(unsigned int init_bandwidth);
    void update_tmp_bandwidth(unsigned int bandwidth);
    void update_static_bandwidth(unsigned int bandwidth);
    void print_route(void);

public:
	unsigned int index;
	unsigned int deepth;
	unsigned int search_deep;
	Tree_node* parent;
	Tree_node* brother;
	edge_list* bfs_position;

};

