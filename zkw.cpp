
#include "zkw.h"

void MCMF_ZKW::dw(int &a,int b){ if (b< a) a=b; }

void MCMF_ZKW:: add(int a,int b,int up,int co)
{
//tot: edge num
//G cap
//C cost
//V to node
//B friend
  ++tot;V[tot]=b;G[tot]=up;C[tot]=co;

  N[tot]=F[a];
//  if(N[tot]==tot)
//    cout<<"error"<<endl;
//  cout<<"node "<<a<<" heda: from "<<F[a];
  F[a]=tot;
//  cout<<" to "<<F[a]<<endl;

  ++tot;V[tot]=a;G[tot]=0;C[tot]=-co;
  N[tot]=F[b];
//  if(N[tot]==tot)
//    cout<<"error"<<endl;
//  cout<<"node "<<b<<" heda: from "<<F[b];
  F[b]=tot;
//  cout<<" to "<<F[b]<<endl;

  B[tot]=tot-1;B[tot-1]=tot;
}

void MCMF_ZKW::delete_service(void)
{
  for(unsigned int i=0; i<this->zkw_last_service_size; i++)
  {
    //edge index :tot
    int _start = V[tot];
    int _end=V[tot-1];
    F[_start]=N[F[_start]];
    F[_end]=N[F[_end]];

    cout<<"node "<<_start<<"'s first edge:"<<F[_start]<<endl;
    cout<<"node "<<_end<<"'s first edge:"<<F[_end]<<endl;
    tot--;
    tot--;
  }
  memcpy(G,origal_G,sizeof(G));
}


int MCMF_ZKW:: aug(int u,int f)
{
   int p,t,left=f;
   if (u==T) { all_demand+=f; ans+=f*d[S];return f; }
   v[u]=1;
   for (p=F[u];p;p=N[p])
   {
     if (G[p]>0&&!v[V[p]])
     {
       t=d[V[p]]+C[p]-d[u];
       if (t==0)
       {
          int delt=aug(V[p],G[p]< left? G[p] : left);
          if (delt>0) G[p]-=delt,G[B[p]]+=delt,left-=delt;
          if (left==0) return f;
       }else dw(slk[V[p]],t);
     }
   }
   return f-left;
}

bool MCMF_ZKW:: modlabel()
{
    int delt=INF,i;
    for (i=0;i<=T;i++)
      if (!v[i]) { dw(delt,slk[i]);slk[i]=INF;}
    if (delt==INF) return true;
    for (i=0;i<=T;i++)
      if (v[i]) d[i]+=delt;
    return false;
}

extern unsigned int deploy_cost;
extern set<unsigned int> service_set;
extern unsigned int demand_sum;

void MCMF_ZKW:: Zkw_Flow()
{
     int i;ans=0;
     all_demand=0;

     for (i=0;i<=T;i++) d[i]=0,slk[i]=INF;
     do{
         do {
            memset(v,0,sizeof(v));
         }while (aug(S,INF));
     }while (!modlabel());

//    cout<<ans<<endl;
    if(all_demand<demand_sum)
    {
        ans=-1;
    }else
    {
         ans+=zkw_last_service_size*deploy_cost;
    }
//    cout<<ans<<endl;

}

void MCMF_ZKW:: Init (int _s,int _t)    //算法初始化
{
  S=_s;T=_t;
}

void MCMF_ZKW:: init_graph(char * topo[], int line_num)
{

    memset(G,sizeof(G),0);
    memset(V,sizeof(V),0);
    memset(C,sizeof(C),0);
    memset(N,sizeof(N),0);
    memset(F,sizeof(F),0);
    memset(B,sizeof(B),0);

    int _node_cnt,_edge_cnt,_demand_cnt;
    sscanf(topo[0],"%d %d %d",&_node_cnt,&_edge_cnt,&_demand_cnt);


    unsigned int start_node,stop_node,bandwidth,length;

    for(unsigned int i=4; i<4+_edge_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d %d",&start_node,&stop_node,&bandwidth,&length);
        add(start_node,stop_node,bandwidth,length);
        add(stop_node,start_node,bandwidth,length);
    }

    unsigned int demand_index,service_index,demand;
    for(unsigned int i=5+_edge_cnt; i<5+_edge_cnt+_demand_cnt; i++)
    {
        sscanf(topo[i],"%d %d %d",&demand_index,&service_index,&demand);
        add(_node_cnt,service_index,demand,0);
    }

    memcpy(origal_G,G,sizeof(G));
    memcpy(origal_V,V,sizeof(V));
    memcpy(origal_C,C,sizeof(C));
    memcpy(origal_N,N,sizeof(N));
    memcpy(origal_F,F,sizeof(F));
    memcpy(origal_B,B,sizeof(B));
    origal_tot=tot;
}

extern unsigned int node_cnt;
extern unsigned int demand_cnt;

void MCMF_ZKW:: add_service(set<unsigned int> ser_set)
{

//    cout<<endl<<endl<<endl<<endl;
//    delete_service();

    memcpy(G,origal_G,sizeof(G));
    memcpy(V,origal_V,sizeof(V));
    memcpy(C,origal_C,sizeof(C));
    memcpy(N,origal_N,sizeof(N));
    memcpy(F,origal_F,sizeof(F));
    memcpy(B,origal_B,sizeof(B));
    tot=origal_tot;

    set<unsigned int>::iterator set_iter;
    for(set_iter=ser_set.begin(); set_iter!=ser_set.end(); set_iter++)
    {
        add(*set_iter,node_cnt+1,INF,0);
    }
    this->zkw_last_service_size=ser_set.size();

}







