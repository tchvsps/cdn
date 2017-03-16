#include "deploy.h"
#include <stdio.h>
#include"basic.h"

//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{

	// 需要输出的内容
	// char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)

    Process* my_process=new Process();

    my_process->init_graph(topo,line_num);
//    demand_vector[0]->print_demand();
//    print_test_demand();
//    print_test_bandwidth();


//    service_vector[0]->print_edge_list(); cout<<endl;
//    service_vector[3]->print_edge_list(); cout<<endl;
//    service_vector[26]->print_edge_list(); cout<<endl;
//    service_vector[27]->print_edge_list(); cout<<endl;

//    cout<<static_bandwidth[{0,2}]<<endl;
//    cout<<static_bandwidth[{2,0}]<<endl;

    my_process->find_scheme();
    cost_sum=0;
    string out_string=my_process->scheme2string_f();

    demand_vaild_detect();



    if(!valid_scheme){
        while(1){
            cout<<"INVALID SCHEME"<<endl;
        }
    }
    cout<<"Cost Sum:"<<cost_sum<<endl;
    cout<<"Scheme:"<<endl;
    cout<<out_string<<endl;



	// 需要输出的内容
//	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(out_string.c_str(), filename);
//    print_test_demand();
//    print_test_bandwidth();

}
