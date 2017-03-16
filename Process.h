#include"basic.h"

class Process{
public:

    Process(){};

	void init_graph(char * topo[], int line_num);
	void get_search_deep(void);

	void fix_service(unsigned int service_index);
    void find_scheme(void);


	string scheme2string_f(void);

public:


};


