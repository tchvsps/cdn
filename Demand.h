
typedef struct connect_list{
    Connect* connect;
    connect_list* next;
    connect_list(Connect* _connect){
        connect=_connect;
        next=NULL;
    }
}connect_list;
inline connect_list* insert_connect(connect_list* head,connect_list* new_node);

class Demand
{
public:
    Demand(
    unsigned int _index,
    unsigned int _attached_service,
    unsigned int _demand):
    index(_index),
    attached_service(_attached_service),
    demand(_demand),
    tmp_demand(_demand),
    assigned(false),
    connect_head(NULL)
    {};

    void print_demand(void);

public:
	unsigned int index;
	unsigned int attached_service;
	unsigned int demand;
	unsigned int tmp_demand;
	bool assigned;

	//change to sorted list
	connect_list* connect_head;

};



