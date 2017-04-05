
#include "zkw.h"

    void MCMF_ZKW::dw(int &a,int b){ if (b< a) a=b; }

    void MCMF_ZKW:: add(int a,int b,int up,int co)
    {
      ++tot;V[tot]=b;G[tot]=up;C[tot]=co;N[tot]=F[a];F[a]=tot;
      ++tot;V[tot]=a;G[tot]=0;C[tot]=-co;N[tot]=F[b];F[b]=tot;
      B[tot]=tot-1;B[tot-1]=tot;
    }

    int MCMF_ZKW:: aug(int u,int f)
    {
       int p,t,left=f;
       if (u==T) { ans+=f*d[S];return f; }
       v[u]=1;
       for (p=F[u];p;p=N[p])
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
       return f-left;
    }

    bool MCMF_ZKW:: modlabel()
    {
        int delt=INF,i;
        for (i=1;i<=T;i++)
          if (!v[i]) { dw(delt,slk[i]);slk[i]=INF;}
        if (delt==INF) return true;
        for (i=1;i<=T;i++)
          if (v[i]) d[i]+=delt;
        return false;
    }

    void MCMF_ZKW:: Zkw_Flow()
    {
         int i;ans=0;
         for (i=1;i<=T;i++) d[i]=0,slk[i]=INF;
         do{
             do {memset(v,0,sizeof(v));}while (aug(S,INF));
         }while (!modlabel());
    }

    void MCMF_ZKW:: Init (int _s,int _t)    //算法初始化
   {
      S=_s;T=_t;
   }

    void MCMF_ZKW:: init_graph(char * topo[], int line_num)
   {
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
//        memcpy(this->origal_edges,this->edges,sizeof(edges));
   }





