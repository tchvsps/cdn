#ifndef __BASIC_H
#define __BASIC_H

using namespace std;

#include <stdlib.h>
#include <stdio.h>

#include <set>
#include <queue>
#include <map>
#include <string>
#include <sstream>
#include <iostream>

#include<time.h>
#include<utime.h>

#define MAX_SERVICE_CNT 1000
#define MAX_DEMAND_CNT   500
#define NO_CONNECT_DEMAND 1000

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class Connect;

struct edge
{
	unsigned int length;
	unsigned int edge_bandwidth;
	int test_edge_bandwidth;
	unsigned int tmp_edge_bandwidth;
	unsigned int start_node;
	unsigned int stop_node;
	struct edge* next;
	struct edge* brother;
	unsigned int wast_band;
	vector<Connect*> connect_vector;

	edge(
	unsigned int _length,
    unsigned int _start_node,
    unsigned int _stop_node,
    unsigned int _edge_bandwidth
    ){
        length=_length;
        start_node=_start_node;
        stop_node=_stop_node;
        edge_bandwidth=_edge_bandwidth;
        tmp_edge_bandwidth=_edge_bandwidth;
        test_edge_bandwidth=_edge_bandwidth;
        next=NULL;
        brother=NULL;
        wast_band=0;
    }
};



edge* insert_edge(edge* head, edge* new_node);

#include"Connect.h"
#include"Demand.h"
#include"Service.h"

#include"Process.h"

extern bool valid_scheme;

extern unsigned int node_cnt;
extern unsigned int demand_cnt;
extern unsigned int edge_cnt;
extern unsigned int deploy_cost;
extern unsigned int cost_sum;
extern unsigned int wast_cost;

extern set<Service*> assigned_service;
extern set<Demand*> assigned_demand;
extern set<Demand*> unassigned_demand;

extern map<unsigned int, Service*> node2service;

extern vector<Demand*> demand_vector;
extern vector<Service*> service_vector;
extern vector<struct node*> node_vector;

extern connect_list* waiting_connect_head;

extern unsigned int max_search_deep;
extern unsigned int max_add_deep;

extern unsigned int predef_deep;

extern map<pair<unsigned int, unsigned int>, signed int> static_edge_bandwidth;
extern map<pair<unsigned int, unsigned int>, signed int> tmp_edge_bandwidth;
extern map<unsigned int, Service*> node2service;

extern map<pair<unsigned int, unsigned int>, signed int> orignal_bandwidth;
extern vector<signed int> orignal_demand;

void print_test_demand(void);
void print_test_bandwidth(void);
void print_static_bandwidth(void);
void print_tmp_bandwidth(void);
void demand_vaild_detect(void);

connect_list* insert_connect(connect_list* head,connect_list* new_node);


extern unsigned int pre_fix_cnt;
extern string pre_fix_str;
extern unsigned int next_fix_cnt;
extern unsigned int max_length;




#endif // __BASIC_H
