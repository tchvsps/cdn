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

    //get data from topo
    my_process->init_graph(topo,line_num);

    //fix the must
    my_process->pre_process();

    //get all need connect
        //search from the demand point
        //just using the normal search algrithom
        //always create a new connect and add to the 2 list 1 vector
    //demand connect_list sorted
    //servic connect_list sorted
    //edge connect_list vector
    predef_deep=5;
    my_process->search_connect();

//    print_connet_list(node2service[38]->connect_head);

    //for unassigned service  get average cost
    //for assigned service if the small than min average cost
    //for assigned demand if average cost too big  redistribute
        //in this process we have to
        //fix connect reovke_connect caculate tmp bandwidth caculate bandwidth
        //if there is a tmp connect fix -> update tmp bandwidth
        //if there is a connect fix -> update bandwidth
    my_process->find_scheme();



    //out

    string out_string=my_process->scheme2string();
    for(unsigned int i=0; i<demand_cnt; i++)
    {
        Demand* demand=demand_vector[i];
        if(demand->test_demand != 0){
            valid_scheme=false;
            cout<<demand->index<<" remain:"<<demand->test_demand<<endl;
        }
    }
//    cout<<pre_fix_cnt<<" pre fix:";
//    cout<<pre_fix_str<<endl;
//    cout<<next_fix_cnt<<" next fix:";
//    cout<<out_string<<endl;
//    cout<<"cost: "<<cost_sum<<endl;
//    cout<<"cost: "<<wast_cost<<endl;
//    cout<<"max length"<<max_length<<endl;

//    if(!valid_scheme)
//    {
//        cout<<endl<<"ERROR !!!"<<endl;
//    }
    string cnt_string=SSTR(pre_fix_cnt+next_fix_cnt);
    cnt_string=cnt_string.append("\n");

    out_string=pre_fix_str.append(out_string);
    out_string=cnt_string.append(out_string);

    write_result(out_string.c_str(), filename);
}
