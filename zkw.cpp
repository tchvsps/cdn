#include <cmath>
#include <queue>
#include <cstdio>
#include <cstring>
using namespace std;
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

const int INF = 0x3fffffff ;   //权值上限
const int MAXPT = 5000*2+100 ;     //顶点数上限
const int MAXEG = 120005 ;    //边数上限

//template<typename Type>
class MCMF_ZKW
{
private:
   int head[MAXPT],d[MAXPT],e;
   bool vis[MAXPT];
   int ans,cost,src,des,n;

   struct Node
   {
      int v,flow,cost,next,re;
      Node(){}
      Node (int _v,int _flow,int _cost,int _next,int _re)
      {
         v=_v,flow=_flow,cost=_cost;
         next=_next,re=_re;
      }
   }edges[MAXEG];

   int aug (int u,int f)
   {
      if (u == des)
      {
         ans+=cost*f;
         return f;
      }
      vis[u]=true;
      int tmp=f;
      for (int i=head[u]; i!=-1; i=edges[i].next)
         if (edges[i].flow && !edges[i].cost && vis[edges[i].v]==false)
         {
            int delta = aug(edges[i].v, tmp < edges[i].flow ? tmp : edges[i].flow);
            edges[i].flow -= delta;
            edges[edges[i].re].flow += delta;
            tmp -= delta;
            if (tmp==0)
               return f;
         }
      return f-tmp;
   }

   bool modlabel ()
   {
      for (int i=0;i<=n;i++)
         d[i]=INF;
      d[des] = 0;
      deque<int>Q;
      Q.push_back(des);
      while (!Q.empty())
      {
         int u=Q.front(),tmp;
         Q.pop_front();
         for (int i=head[u]; i!=-1; i=edges[i].next)
            if (edges[edges[i].re].flow && (tmp = d[u]-edges[i].cost) < d[edges[i].v])
               (d[edges[i].v] = tmp) <= d[Q.empty() ? src : Q.front()] ? Q.push_front(edges[i].v) : Q.push_back(edges[i].v);
      }
      for (int u=1;u<=n;u++)
         for (int i=head[u]; i!=-1; i=edges[i].next)
            edges[i].cost += d[edges[i].v] - d[u];
         cost+=d[src];
         return d[src] < INF;
   }
public:

   void Init (int _s,int _t)    //算法初始化
   {
      src=_s;des=_t;
      e=0,n=des;
      memset (head,-1,sizeof(head));
      ans=cost=0;
   }

   void Add (int u,int v,int flow,int cost)
   {
      edges[e]=Node(v,flow,cost,head[u],e+1);
      head[u] = e++;
      edges[e]=Node(u,0,-cost,head[v],e-1);
      head[v] = e++;
   }

   int ZKW ()
   {
      while(modlabel())
         do
         {
            memset(vis,false,sizeof(vis));
         }while(aug(src,INF));
      return ans;
   }
};
