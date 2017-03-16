#include"basic.h"

//class Service
//{
//public:
//    Service(unsigned int _index):
//    index(_index),
//    attached_demand(NO_CONNECT_DEMAND),
//    edge_head(NULL),
//    search_head(NULL)
//    {};
//
//    void search_route(void);
//    unsigned int get_average_cost(void);
//    void fix_service(void);
//
//
//public:
//	unsigned int index;
//	unsigned int attached_demand;
//	edge_list* edge_head;
//	tree_node_list* search_head;
//	queue<Tree_node*> demand_queue;
//	queue<Connect*> connect_queue;
//};

void Service::jude_new_node(Tree_node* new_tree_node)
{

    unsigned int demand_index;
    demand_index=service_vector[new_tree_node->index]->attached_demand;
    if(demand_index!=NO_CONNECT_DEMAND && !demand_vector[demand_index]->assigned)
    {

        demand_queue.push(new_tree_node);

        unsigned int demand_index=service_vector[new_tree_node->index]->attached_demand;
        unsigned int service_index=index;
        //add to connect_queue

        Connect* new_connect=new Connect(
            demand_index,
            service_index,
            new_tree_node->deepth,
            new_tree_node
        );

//                    cout<<new_connect->scheme2string()<<endl;
        new_connect->bandwidth=new_tree_node->get_max_bandwidth
        (demand_vector[demand_index]->tmp_demand);
        if(new_connect->bandwidth==0)
        {
            free(new_connect);
        }
        else
        {
//            cout<<endl<<"new queue upda:"<<endl;
//            demand_vector[0]->print_demand();
//            print_tmp_bandwidth();

            new_connect->tmp_fix_connect();
            connect_queue.push(new_connect);

//            demand_vector[0]->print_demand();
//            print_tmp_bandwidth();
        }
    }

}

inline Tree_node* insert_search_list(Tree_node* head,Tree_node* new_node)
{
    new_node->brother=NULL;


    if(!head)
    {
        head=new_node;
    }
    else if(head->search_deep>new_node->search_deep)
    {
        new_node->brother=head;
        head=new_node;
    }
    else
    {
        Tree_node* older_node=head;

        while(older_node->brother)
        {
            if(older_node->brother->search_deep> new_node->search_deep)
            {
                new_node->brother=older_node->brother;
                break;
            }
            else
            {
                older_node=older_node->brother;
            }
        }
        older_node->brother=new_node;
    }
    return head;

}


inline connect_list* insert_connect(connect_list* head,connect_list* new_node)
{
    new_node->next=NULL;

    if(!head)
    {
        head=new_node;
    }
    else if(head->connect->length > new_node->connect->length)
    {
        new_node->next=head;
        head=new_node;
    }
    else
    {
        connect_list* older_node=head;

        while(older_node->next)
        {
            if(older_node->next->connect->length > new_node->connect->length)
            {
                new_node->next=older_node->next;
                break;
            }
            else
            {
                older_node=older_node->next;
            }
        }
        older_node->next=new_node;
    }

    return head;
}


void Service::search_connect(void)
{
//    cout<<endl<<"service:"<<index<<endl;
    tmp_bandwidth.clear();

    set<unsigned int>::iterator demand_iter;
    for(demand_iter=unassigned_demand.begin(); demand_iter!=unassigned_demand.end(); ++demand_iter)
    {
        demand_vector[*demand_iter]->tmp_demand=demand_vector[*demand_iter]->demand;
    }
//    demand_vector[0]->print_demand();


    unsigned int connect_size=connect_queue.size();
    for(unsigned int i=0; i<connect_size; i++)
    {
        Connect* connect=connect_queue.front();
        connect_queue.pop();

        if(!demand_vector[connect->demand_index]->assigned)
        {
            connect->bandwidth=connect->leaf_node->get_max_bandwidth
            (demand_vector[connect->demand_index]->tmp_demand);

            if(connect->bandwidth)
            {
//                cout<<endl<<"remain queue upda:"<<endl;
//                demand_vector[0]->print_demand();
//                print_tmp_bandwidth();

                connect->tmp_fix_connect();
                connect_queue.push(connect);

//                demand_vector[0]->print_demand();
//                print_tmp_bandwidth();

            }
        }
    }

    while(search_head)
    {
        if(search_head->search_deep > max_search_deep)
        {
            break;
        }
        else
        {
//            cout<<"search node: "<<search_head->index<<endl;
            Tree_node* now_tree_node;
            edge_list* now_edge_node;

            now_tree_node=search_head;
            search_head=search_head->brother;
            now_edge_node=now_tree_node->bfs_position;



            unsigned int new_node_index=now_edge_node->stop_node;
            bool searched_flg=false;

            searched_flg=now_tree_node->is_search(new_node_index);
            if(!searched_flg)
            {

                Tree_node* new_tree_node=new Tree_node(
                    new_node_index,
                    now_tree_node->deepth+now_edge_node->length,
                    now_tree_node);
                new_tree_node->bfs_position=service_vector[new_node_index]->edge_head;
                new_tree_node->search_deep=new_tree_node->deepth+new_tree_node->bfs_position->length;

                search_head=insert_search_list(search_head,new_tree_node);
                jude_new_node(new_tree_node);
//                cout<<"    add new node:"<<new_tree_node->index<<" route:";
//                new_tree_node->print_route();
//                print_search_list();



            }

            //move the next edge
            now_tree_node->bfs_position=now_tree_node->bfs_position->next;
            if(now_tree_node->bfs_position)
            {
                now_tree_node->search_deep=now_tree_node->deepth+now_tree_node->bfs_position->length;
                search_head=insert_search_list(search_head,now_tree_node);
//                print_search_list();
            }

        }
    }
}



void Service::add_connect(void)
{
    while(search_head)
    {
        if(search_head->search_deep > max_search_deep+5)
        {
            break;
        }
        else
        {
            Tree_node* now_tree_node;
            edge_list* now_edge_node;

            now_tree_node=search_head;
            search_head=search_head->brother;
            now_edge_node=now_tree_node->bfs_position;

            unsigned int new_node_index=now_edge_node->stop_node;
            bool searched_flg=false;

            searched_flg=now_tree_node->is_search(new_node_index);
            if(!searched_flg)
            {
                Tree_node* new_tree_node=new Tree_node(
                    new_node_index,
                    now_tree_node->deepth+now_edge_node->length,
                    now_tree_node);
                new_tree_node->bfs_position=service_vector[new_node_index]->edge_head;
                new_tree_node->search_deep=new_tree_node->deepth+new_tree_node->bfs_position->length;

                search_head=insert_search_list(search_head,new_tree_node);

                unsigned int demand_index=service_vector[new_tree_node->index]->attached_demand;
                if(demand_index!=NO_CONNECT_DEMAND && !demand_vector[demand_index]->assigned)
                {
                    unsigned int demand_index=service_vector[new_tree_node->index]->attached_demand;
                    unsigned int service_index=index;
                    //add to connect_queue

                    Connect* new_connect=new Connect(
                        demand_index,
                        service_index,
                        new_tree_node->deepth,
                        new_tree_node
                    );
                    new_connect->bandwidth=0;
                    connect_list* new_connect_node=new connect_list(new_connect);
                    waiting_connect_head=insert_connect(waiting_connect_head,new_connect_node);
                }
            }

            //move the next edge
            now_tree_node->bfs_position=now_tree_node->bfs_position->next;
            if(now_tree_node->bfs_position)
            {
                now_tree_node->search_deep=now_tree_node->deepth+now_tree_node->bfs_position->length;
                search_head=insert_search_list(search_head,now_tree_node);
            }
        }
    }
}

float Service::get_average_cost(void)
{

    unsigned int cost_sum=deploy_cost;
    unsigned int bandwidth_sum=0;

    float average_cost=1000;
    float tmp_average_cost=1000;

    unsigned int seprate_length=0;
    unsigned int last_seprate_length=0;

    bool stop_flg=false;

    unsigned int connect_size=connect_queue.size();
    for(unsigned int i=0; i<connect_size; i++)
    {
        Connect* connect=connect_queue.front();
        connect_queue.pop();

        if(!stop_flg)
        {
//            cout<<connect->length<<" "<<bandwidth_sum<<endl;
            if(connect->length == seprate_length)
            {
                cost_sum+=connect->length*connect->bandwidth;
                bandwidth_sum+=connect->bandwidth;
            }
            else
            {

                if(bandwidth_sum==0)
                {
                    tmp_average_cost=1000;
                }
                else
                {
                    tmp_average_cost=(float)cost_sum/bandwidth_sum;
                }

                if(tmp_average_cost<=average_cost)
                {
                    average_cost=tmp_average_cost;
                    stop_flg=false;
                    cost_sum+=connect->length*connect->bandwidth;
                    bandwidth_sum+=connect->bandwidth;
                    last_seprate_length=seprate_length;
                    seprate_length=connect->length;
                }
                else
                {
                    best_deepth=last_seprate_length;
                    stop_flg=true;
                    continue;
                }
            }
        }
        connect_queue.push(connect);
    }
    if(!stop_flg)
    {
        if(bandwidth_sum==0)
        {
            tmp_average_cost=1000;
        }
        else
        {
            tmp_average_cost=(float)cost_sum/bandwidth_sum;
        }

        if(tmp_average_cost<=average_cost)
        {
            average_cost=tmp_average_cost;
            best_deepth=seprate_length;
        }
        else
        {
            best_deepth=last_seprate_length;
        }
    }
    return average_cost;
}

void Service::fix_service(void)
{
    assigned_service.insert(index);
    unassigned_service.erase(unassigned_service.find(index));

    unsigned int connect_size=connect_queue.size();
    for(unsigned int i=0; i<connect_size; i++)
    {
        Connect* connect=connect_queue.front();
        connect_queue.pop();
        if(connect->bandwidth == 0)
        {
            free(connect);
            continue;
        }
        if(connect->length <= best_deepth)
        {
            connect->fix_connect();
            connect_queue.push(connect);
        }
        else
        {
            connect_list* new_connect_node=new connect_list(connect);
            waiting_connect_head=insert_connect(waiting_connect_head,new_connect_node);
        }
    }
}

string Service::scheme2string(void)
{
    string service_string="";
    string connect_string="";
    unsigned int connect_size=connect_queue.size();

    for(unsigned int i=0; i<connect_size; i++)
    {
        Connect* connect=connect_queue.front();
        connect_queue.pop();
        connect_queue.push(connect);

        connect_string=connect->scheme2string();
        service_string.append(connect_string);
    }

    return service_string;
}

string Service::scheme2string_f(void)
{
    string service_string="";
    string connect_string="";
    unsigned int connect_size=connect_queue.size();

    cost_sum+=deploy_cost;

    for(unsigned int i=0; i<connect_size; i++)
    {
        Connect* connect=connect_queue.front();
        connect_queue.pop();
        connect_queue.push(connect);

        connect_string=connect->scheme2string_f();
        cost_sum+=connect->bandwidth*connect->length;
        service_string.append(connect_string);
    }

    return service_string;
}

void Service::print_edge_list(void)
{
    edge_list* edge=edge_head;
    while(edge)
    {
        cout<<" "<<edge->start_node<<"->"<<edge->stop_node<<"("<<edge->length<<")";
        edge=edge->next;
    }
    cout<<endl;
}

void Service::print_search_list(void)
{
    Tree_node* search_node=search_head;
    cout<<"    search_list: ";
    while(search_node)
    {
        cout<<"->"<<search_node->index<<"("<<search_node->search_deep<<")";
        search_node=search_node->brother;
    }
    cout<<endl;
}

void Service::print_demand_set(void)
{
    for(unsigned int i=0; i<demand_queue.size(); i++)
    {
        Tree_node* leaf;
        leaf=demand_queue.front();
        demand_queue.pop();
        demand_queue.push(leaf);
        cout<<index<<"->"<<leaf->index<<" :deepth: "<<leaf->deepth<<endl;
    }

}

void Service::print_connec_queue(void)
{
    unsigned int queue_size;
    queue_size=connect_queue.size();
    Connect* connect;
    for(unsigned int i=0; i<queue_size; i++)
    {
        connect=connect_queue.front();
        connect_queue.pop();
        connect_queue.push(connect);
        cout<<" "<<connect->length;
    }
    cout<<endl;
}

