#include"basic.h"

class Process{
public:

    Process(){};

	void init_graph(char * topo[], int line_num);
	void pre_process(void);
    void search_connect(void);
	void fix_service(unsigned int index);
    void find_scheme(void);


	string scheme2string(void);

public:


};


