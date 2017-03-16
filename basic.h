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

#define MAX_SERVICE_CNT 1000
#define MAX_DEMAND_CNT   500
#define NO_CONNECT_DEMAND 1000

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

typedef struct edge_list
{
	unsigned int length;
	unsigned int start_node;
	unsigned int stop_node;
	struct edge_list* next;

	edge_list(
	unsigned int _length,
    unsigned int _start_node,
    unsigned int _stop_node){
        length=_length;
        start_node=_start_node;
        stop_node=_stop_node;
        next=NULL;
    }
}edge_list;

edge_list* insert_edge(edge_list* head, edge_list* new_node);

#include"Tree_node.h"
#include"Connect.h"
#include"Demand.h"
#include"Service.h"
#include"Process.h"

extern bool valid_scheme;

extern unsigned int service_cnt;
extern unsigned int demand_cnt;
extern unsigned int edge_cnt;
extern unsigned int deploy_cost;

extern set<unsigned int> assigned_service;
extern set<unsigned int> unassigned_service;
extern set<unsigned int> assigned_demand;
extern set<unsigned int> unassigned_demand;

extern vector<Demand*> demand_vector;
extern vector<Service*> service_vector;

extern unsigned int max_search_deep;
extern map<pair<unsigned int, unsigned int>, signed int> static_bandwidth;
extern map<pair<unsigned int, unsigned int>, signed int> tmp_bandwidth;

extern map<pair<unsigned int, unsigned int>, signed int> orignal_bandwidth;
extern vector<signed int> orignal_demand;

void print_test_demand(void);
void print_test_bandwidth(void);
void print_static_bandwidth(void);
void print_tmp_bandwidth(void);


#endif // __BASIC_H
