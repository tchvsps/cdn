#ifndef __CONNECT_H
#define __CONNECT_H
#include"basic.h"

class Connect
{
public:
//    Connect(unsigned int )
    Connect(
    unsigned int _demand,
    unsigned int _service,
    unsigned int _length,
    Tree_node* _leaf_node
    ):
    demand_index(_demand),
    service_index(_service),
    length(_length),
    leaf_node(_leaf_node)
    {};

    void tmp_fix_connect(void);
    void fix_connect(void);
    string scheme2string(void);
    string scheme2string_f(void);

public:
	unsigned int demand_index;
	unsigned int service_index;
	unsigned int length;
	unsigned int bandwidth;
	Tree_node* leaf_node;
};

#endif


