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
    assigned(false)
    {};

    bool redistribute(unsigned int next_cost);
    void print_demand(void);

public:
	unsigned int index;
	unsigned int attached_service;
	unsigned int demand;
	unsigned int tmp_demand;
	bool assigned;

	//change to sorted list
	queue<Connect*> connect_queue;

};
