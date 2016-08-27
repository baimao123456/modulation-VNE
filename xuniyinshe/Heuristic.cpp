#include "Heuristic.h"
#include "UsTime.h"
#include "Graph.h"



static int nodeID = -1;
static vector<Nodes> nodes_temp;
bool sort_nodes(const Nodes &n1, const Nodes &n2){				//��������������sort��������
	return n1.degree>n2.degree;
}


void Heuristic::sortnetwork(){
	
	bool sort_edges(const Edge &e1, const Edge &e2);

	for(int vertexID = 0; vertexID < demand->n; vertexID++){
		Nodes tempnode;
		tempnode.vertexID = vertexID, tempnode.degree = demand->degree[vertexID];
		nodes.push_back(tempnode);
	}
	
	sort(nodes.begin(),nodes.end(),sort_nodes);
	for(int k = 0; k < demand->n; k++){
		int edge_reserve = demand->edges.size();	//����demand��edges��ѡ�룬vector��С���С�������ж�����Ҫ���ű�С
		vector<Edge>::iterator it = demand->edges.begin();
		for(int ij = 0 ; ij < demand->edges.size(); ij++){
			if(demand->edges[ij].head == nodes[k].vertexID || demand->edges[ij].tail == nodes[k].vertexID){
				edges_middle.push_back(demand->edges[ij]);
				demand->edges.erase(it);
				ij--;			//ÿ��ɾ�����ݺ� ij��Ӧ��ֵҲҪ����һ����Ϊ����vector����ѹ����һ��
			}
			else
				it++;
		}
		nodeID = k;
		nodes_temp = nodes;
		sort(edges_middle.begin(),edges_middle.end(),sort_edges);			//�����м�������Աߵ����ڽڵ�Ƚ��������ٰ����ӽ�һ�ܵ�vector����
		for (int i = 0; i < edges_middle.size(); i++){
			edges_after.push_back(edges_middle[i]);
		}
		edges_middle.clear();
	}	
}
	//sort(nodes.begin(),nodes.end(),demand->degree(n));


 bool sort_edges(const Edge &e1, const Edge &e2){

	int degree_e1, degree_e2;
	if (e1.head == nodeID)
		degree_e1 = nodes_temp[e1.tail].degree;
	if (e1.tail == nodeID)
		degree_e1 = nodes_temp[e1.head].degree;
	if (e2.head == nodeID)
		degree_e2 = nodes_temp[e2.tail].degree;
	if (e2.tail == nodeID)
		degree_e2 = nodes_temp[e2.head].degree;
	return degree_e1>degree_e2;
}

Network Heuristic::auxGraph_generate(int demand_head, int demand_tail,int a,int g){
	//int mod = demand->mod;
	vector<int> r_in, r_mj, d_in, d_mj;
	int lanmda_v_i = 0; 
	for(int k = 0; k < resource->m; k++){
		r_in.push_back(100000);
		r_mj.push_back(100000);	
		d_in.push_back(0);
		d_mj.push_back(0);//��ʽ17��r_in,r_jn δ���
	}


//	int F_g_1 = resource->maxBandwidth - mod +1;
//	for (int a = 0; a < F_g_1; a++){
		{auxGraph.edges.clear();
		auxGraph.ck.clear();
		auxGraph.degree.clear();
		auxGraph.vertexWeight.clear();
		
		}
		 auxGraph = *resource ;



		int nshift = auxGraph.n, mshift = auxGraph.m;
		int n_number= auxGraph.n, m_number = auxGraph.m;
			for(int i = 0; i < n_number; i++){
				
				auxGraph.vertexWeight.push_back(auxGraph.vertexWeight[i]);		
				auxGraph.degree.push_back(auxGraph.degree[i]);
				auxGraph.ck.push_back(auxGraph.ck[i]);
				auxGraph.n += 1;
			}
			for(int ij = 0; ij < m_number; ij++){	
				vector<Edge>::iterator it = auxGraph.edges.begin();
				int if_cut_the_edge = 0;
				
				int dest_a = a + g;
				for( int ak = a; ak < dest_a; ak++){
					if(auxGraph.edges[ij].wave[ak] != true)
						if_cut_the_edge = 1;
				}
				if(if_cut_the_edge){
					auxGraph.edges.erase(it);
					ij--;
					m_number--;
					auxGraph.m--;
				}	
				else
					it++;
			}													//ɾ�������������ıߣ������������б߷����γ���ͼ
			for(int ij = 0; ij < m_number; ij++){
				Edge te = auxGraph.edges[ij];
				te.head += nshift;
				te.tail += nshift;
				te.cmn *= auxGraph.edges[ij].distance;	//����cmn ���Ͼ���
				auxGraph.edges[ij].cmn = te.cmn;
				auxGraph.edges.push_back(te);
				auxGraph.maxBandwidth = max(auxGraph.maxBandwidth, te.bandwidth);
				auxGraph.m += 1;

				auxGraph.edges[ij].head += nshift;				//�ó�ʼ�ĵ������µ�ͼ���µ���ȥ

			}
		
			//������Ҫ���� i,j�Ƿ�ӳ��ѡ��������ߵ������������ӳ����ֻ����һ����û�еĻ���ȫ���ϡ�
			
			auxGraph.n +=2;	//��������������ڵ�
			int FIk_i = -1, FIk_j = -1, if_node_embeded = 0;	//��ʼ��ӳ��flag �ٶ�û�б�ӳ�䵽�κ�һ����
			for (int k = 0; k < resource->n; k++){
					if(FIk[demand_head][k] == 1)
						FIk_j = k;
					if(FIk[demand_tail][k] == 1)
						FIk_i = k;
			}
			if(FIk_i >= 0){
				Edge *te = new Edge(auxGraph.n-2,FIk_i,auxGraph.maxBandwidth ,r_in[FIk_i],0);	//Ϊ����i,j��Ų���ʵ�ʽڵ�k��Ÿ�죬��jΪ���һ���ڵ㣬iΪ�����ڶ���
				auxGraph.edges.push_back(*te);
				auxGraph.m +=1;
			}			//���i�㱻ӳ����ˣ��Ǿ�ֻ��һ����ӳ��ıߡ������û�б�ӳ������ǾͰ��������б߶�����ȥ��
			else{
				for(int k = 0; k < resource->n; k++){
					for (int demand_k = 0; demand_k < demand->n; demand_k ++){
						if(FIk[demand_k][k] == 1 )
							if_node_embeded = 1;
					}
					int dmn,rin = 0;
					if(if_node_embeded == 0){
					Edge *te = new Edge(auxGraph.n-2,k,auxGraph.maxBandwidth ,r_in[k],0);
/************************************************************************
					int count=0;
					for (int edgeID = 0; edgeID < auxGraph.m; edgeID++){
						
			            if(auxGraph.edges[edgeID].head == k||auxGraph.edges[edgeID].tail == k){
						    count++;
							for(int waveID = 0; waveID+te->bandwidth <= resource->maxBandwidth; waveID++)
							rin ++;//��ô���?
						}
					}
					if (count != 0)
						rin=rin/count;		   
					rin = pow(float(rin),resource->degree[k]);
					
					
					int Dis = 0,aa=0,mid=0; //aa��¼deta(v,n)�ĸ���
					for(int ij = 0 ; ij < edges_after.size(); ij++){
						
			           if(edges_after[ij].head == nodes[k].vertexID ){
						     for(int kk = 0; kk < resource->n; kk++){
								 if(FIk[edges_after[ij].tail][kk] == 1 ){
							         if_node_embeded = 1;
								     mid=kk;//��mid��¼�ڵ�vӳ�������ڵ�
								     break;
						         }
						      }	
							 if(if_node_embeded = 1){
								int dis = findpath(resource,k,mid);//����һ��find�ҵ�Dis(M,n),
								Dis+=dis;
								aa++;
							 }
					    }

			           if(edges_after[ij].tail == nodes[k].vertexID ){
						     for(int kk = 0; kk < resource->n; kk++){
						           if(FIk[edges_after[ij].head][kk] == 1 ){
							           if_node_embeded = 1;
								       mid=kk;//��mid��¼�ڵ�vӳ�������ڵ�
								       break;
						           	}
						     }
							if( if_node_embeded = 1){
								int dis = findpath(resource,k,mid);//����һ��find�ҵ�Dis(M,n),
								Dis+=dis;
								aa++;
							}
						}  							  	
					if( Dis != 0)
					 dmn=Dis/aa;	
                    int cmn= rin * dmn;
					te->cmn = cmn;

					}
					*///************************************************************************
					auxGraph.edges.push_back(*te);
					auxGraph.m +=1;
					}
					if_node_embeded = 0;
				}
			}	


			if(FIk_j >= 0){
				Edge *te = new Edge(FIk_j+resource->n,auxGraph.n-1,auxGraph.maxBandwidth ,r_mj[FIk_j],0);	//���Ƶ�ͼ���յ㣬ÿ���ڵ��Ŷ�Ҫshift��
				auxGraph.edges.push_back(*te);
				auxGraph.m +=1;
				
			}			//���i�㱻ӳ����ˣ��Ǿ�ֻ��һ����ӳ��ıߡ������û�б�ӳ������ǾͰ��������б߶�����ȥ��
			else{
				for(int k = 0; k < resource->n; k++){
					for (int demand_k = 0; demand_k < demand->n; demand_k ++){
						if(FIk[demand_k][k] == 1 )
							if_node_embeded = 1;
					}
				
					int dmn,rmj = 0;
					if(if_node_embeded == 0){
					Edge *te = new Edge(k+resource->n,auxGraph.n-1,auxGraph.maxBandwidth ,r_mj[k],0);
/************************************************************************
					int count=0;
					for (int edgeID = 0; edgeID < auxGraph.m; edgeID++){
						
			            if(auxGraph.edges[edgeID].head == k||auxGraph.edges[edgeID].tail == k){
						    count++;
							for(int waveID = 0; waveID+te->bandwidth <= resource->maxBandwidth; waveID++)
							rmj ++;//��ô���?
						}
					}
					if (count != 0)
						rmj=rmj/count;		   
					rmj = pow(float(rmj),resource->degree[k]);
					
					
					int Dis = 0,aa=0,mid=0; //aa��¼deta(v,n)�ĸ���
					for(int ij = 0 ; ij < edges_after.size(); ij++){
						
			           if(edges_after[ij].head == nodes[k].vertexID ){
						     for(int kk = 0; kk < resource->n; kk++){
								 if(FIk[edges_after[ij].tail][kk] == 1 ){
							         if_node_embeded = 1;
								     mid=kk;//��mid��¼�ڵ�vӳ�������ڵ�
								     break;
						         }
						      }	
							 if(if_node_embeded = 1){
								int dis = findpath(resource,k,mid);//����һ��find�ҵ�Dis(M,n),
								Dis+=dis;
								aa++;
							 }
					    }

			           if(edges_after[ij].tail == nodes[k].vertexID ){
						     for(int kk = 0; kk < resource->n; kk++){
						           if(FIk[edges_after[ij].head][kk] == 1 ){
							           if_node_embeded = 1;
								       mid=kk;//��mid��¼�ڵ�vӳ�������ڵ�
								       break;
						           	}
						     }
							if( if_node_embeded = 1){
								int dis = findpath(resource,k,mid);//����һ��find�ҵ�Dis(M,n),
								Dis+=dis;
								aa++;
							}
						}  							  	
					if( Dis != 0)
					 dmn=Dis/aa;	
                    int cmn= rmj * dmn;
					te->cmn = cmn;

					}
*///**********************************************************
					auxGraph.edges.push_back(*te);
					auxGraph.m +=1;
					}
					if_node_embeded = 0;
				}
			}
		
		return auxGraph;
			
}

int Heuristic::findpath(){
	int temp_dist=100000000,temp_node_id ,findcounts = 0;
	vector<double>dist(auxGraph.n,1000000000);
	//vector<int> last_vertex_ID;							//��¼��һ���ڵ��ID

	last_vertex_ID.clear();
	for(int i = 0; i < auxGraph.n; i++)
		last_vertex_ID.push_back(-1);
	
	vector<int> used_vertex_ID;							//��¼�Ѿ�����ĵ�ID
	used_vertex_ID.push_back(auxGraph.n-2);				//auxGraph.n-1����ľ���Դ�ڵ� auxGraph.n����Ŀ�Ľڵ�
	dist[auxGraph.n-2] = 0;						//��ʼ����������
	do{
	for (int i = 0; i < used_vertex_ID.size(); i++){
		for (int edgeID = 0; edgeID < auxGraph.m; edgeID++){
			if(auxGraph.edges[edgeID].tail == used_vertex_ID[i]  )
				if(dist[auxGraph.edges[edgeID].tail]+auxGraph.edges[edgeID].cmn < dist[auxGraph.edges[edgeID].head]){
					dist[auxGraph.edges[edgeID].head] = dist[auxGraph.edges[edgeID].tail]+auxGraph.edges[edgeID].cmn;
					last_vertex_ID[auxGraph.edges[edgeID].head] = auxGraph.edges[edgeID].tail;		//��¼����һ���ڵ�λ�á�
				}
		}
	}
	for (int i = 0; i < auxGraph.n; i++){
		int if_i_is_used = 0;
		for (int k = 0; k < used_vertex_ID.size(); k++){
			if( i == used_vertex_ID[k])
				if_i_is_used = 1;
		}
		if (dist[i] < temp_dist && if_i_is_used == 0){
			temp_dist = dist[i];
			temp_node_id = i;
		}
	}								//���������һ���ڵ�ʹ������С���ѽڵ�������¼���ֱ������Ŀ�Ľڵ�
	used_vertex_ID.push_back(temp_node_id);
	temp_dist=100000000;
	findcounts ++;
	if( findcounts >= auxGraph.m){
		return 0;						//	���Ѱ�Ҵ������������˻�û�����յ㣬��Ѱ��ʧ�ܷ���0 ����������֪���ж��Ƿ���ȷ
	}
	}while(temp_node_id != auxGraph.n-1);

	return dist[auxGraph.n-1];			//��������൱��cmn֮�ͣ��������Ҫ����

}

int Heuristic::findpath(Network *r,int source_vertexID, int dest_vertexID){
	int temp_dist=100000000,temp_node_id ,findcounts = 0;
	Network *tempnet = r;
	vector<double>dist(tempnet->n,1000000000);
	//vector<int> last_vertex_ID;							//��¼��һ���ڵ��ID

	last_vertex_ID.clear();
	for(int i = 0; i < tempnet->n; i++)
		last_vertex_ID.push_back(-1);
	
	vector<int> used_vertex_ID;							//��¼�Ѿ�����ĵ�ID
	used_vertex_ID.push_back(source_vertexID);				
	dist[source_vertexID] = 0;						//��ʼ����������
	do{
	for (int i = 0; i < used_vertex_ID.size(); i++){
		for (int edgeID = 0; edgeID < tempnet->m; edgeID++){
			if(tempnet->edges[edgeID].tail == used_vertex_ID[i])
				if(dist[tempnet->edges[edgeID].tail]+tempnet->edges[edgeID].distance < dist[tempnet->edges[edgeID].head]){//����ĳ���distance���������cmn
					dist[tempnet->edges[edgeID].head] = dist[tempnet->edges[edgeID].tail]+tempnet->edges[edgeID].distance;
					last_vertex_ID[tempnet->edges[edgeID].head] = tempnet->edges[edgeID].tail;		//��¼����һ���ڵ�λ�á�
				}
		}
	}
	for (int i = 0; i < tempnet->n; i++){
		int if_i_is_used = 0;
		for (int k = 0; k < used_vertex_ID.size(); k++){
			if( i == used_vertex_ID[k])
				if_i_is_used = 1;
		}
		if (dist[i] < temp_dist && if_i_is_used == 0){
			temp_dist = dist[i];
			temp_node_id = i;
		}
	}								//���������һ���ڵ�ʹ������С���ѽڵ�������¼���ֱ������Ŀ�Ľڵ�
	used_vertex_ID.push_back(temp_node_id);
	temp_dist=100000000;
	findcounts ++;
	if( findcounts >= tempnet->m){
		return 0;						//	���Ѱ�Ҵ������������˻�û�����յ㣬��Ѱ��ʧ�ܷ���0 ����������֪���ж��Ƿ���ȷ
	}
	}while(temp_node_id != dest_vertexID);

	return dist[dest_vertexID];			

}




Heu_return Heuristic::work(){
	UsClock clk;
	clk.setStartTime();

	Event ret = Event(0, 0, 0, resource);
	Heu_return heu_ret;
	for(int ij = 0; ij < edges_after.size(); ij++){
		
		int e_has_been_embeded = 0;
		for(int k = 0; k < resource->n; k++){
			if(FIk[edges_after[ij].head][k] == 1 || FIk[edges_after[ij].tail][k] == 1)
				e_has_been_embeded++;
		}
				{vector<int> t;
		heu_ret.demand_tail.push_back(edges_after[ij].tail);
		heu_ret.demand_head.push_back(edges_after[ij].head);
		heu_ret.edge_head.push_back(t);
		heu_ret.edge_tail.push_back(t);
		heu_ret.total_distance.push_back(100000000);
		heu_ret.grid_a.push_back(-1);	
		heu_ret.physical_distance.push_back(0);				//��ʼ�������
		heu_ret.bandwidth.push_back(-1);
		heu_ret.modtype.push_back(-1);
		}

		//if(e_has_been_embeded == 2)							//��һ���ж��Ƿ��Ѿ���ӳ�䣬��������㶼��ӳ����ˣ����������ѱ�ӳ�䣬continue		
			//continue;
		

		int flag_FIk_same_point = 0 , same_point_ID;		//�ж��Ƿ�ӳ�䵽ͬһ��
		int DijisSuccess;
		for (int mod = 16; mod > 1; mod = mod/2){ 
			int g = ceil(double(edges_after[ij].bandwidth)/(mod));
			int F_g_1 = resource->maxBandwidth - g + 1;	//��������ȡ��F=maxBandwidth ѭ������Ӧ��Ҳ��Ӱ�죬���ҵ�����֮ǰ���ҵ����Ҳ���ѭ������Ҳ�Ҳ���
			for (int a = 1; a < F_g_1; a++){
				heu_ret.grid_a[ij] = a;

				auxGraph_generate(edges_after[ij].head,edges_after[ij].tail,a,g);	//���ɸ���ͼ

				if(flag_FIk_same_point == 1){
					flag_FIk_same_point = 0;
					int m_number = auxGraph.m;
					for(int mn = 0; mn < m_number; mn++){	
						vector<Edge>::iterator it = auxGraph.edges.begin();
						if(mn == same_point_ID){
							auxGraph.edges.erase(it);
							mn--;
						}	
						else
							it++;
					}				
				}							//ɾ������һ����ߣ���֤����ӳ�䵽ͬһ����

				 DijisSuccess = findpath();		//�������ɸ���ͼѰ�����·
				if (!DijisSuccess)
					continue;
				else{
					if(DijisSuccess < heu_ret.total_distance[ij]){
						heu_ret.total_distance[ij] = DijisSuccess;
						heu_ret.edge_head[ij].clear();
						heu_ret.edge_tail[ij].clear();			//�����С��ɾ����һ�α����Ľ��

							int j_m ,i_n ,temp = auxGraph.n-1;
							j_m = last_vertex_ID[auxGraph.n-1];
							heu_ret.edge_head[ij].push_back(auxGraph.n-1);
							heu_ret.edge_tail[ij].push_back(j_m);
					
							do{
								i_n = temp;
								temp = last_vertex_ID[temp];
								heu_ret.edge_head[ij].push_back(temp);
								heu_ret.edge_tail[ij].push_back(last_vertex_ID[temp]);


							}while(last_vertex_ID[temp] != auxGraph.n-2);
							i_n = temp;

							if(i_n + resource->n == j_m){			//������I,Jӳ�䵽ͬһ������ڵ�����
								flag_FIk_same_point = 1;
								same_point_ID = j_m;
								a--;
								continue;
							}

							for (int demand_k = 0; demand_k<resource->n;demand_k++){
								if(demand_k == j_m || demand_k == j_m-resource->n)
								FIk[edges_after[ij].head][demand_k]=1;
							}
							for (int demand_k = 0; demand_k<resource->n;demand_k++){
								if(demand_k == i_n || demand_k == i_n-resource->n)
								FIk[edges_after[ij].tail][demand_k]=1;
							}														//���ڵ�ӳ���¼����
						}
					break;
					}
			}
			int physical_distance = 0;
			if(DijisSuccess){				//����ҵ���һ��·����break ������Դ�������
				for(int mn = 0; mn < resource->m; mn++){
					for(int k = 0; k < heu_ret.edge_head[ij].size(); k++){
						if(resource->edges[mn].head == heu_ret.edge_head[ij][k] ||resource->edges[mn].head == heu_ret.edge_head[ij][k]-resource->n)
							if(resource->edges[mn].tail == heu_ret.edge_tail[ij][k] ||resource->edges[mn].tail == heu_ret.edge_tail[ij][k]-resource->n){
								physical_distance += resource->edges[mn].distance;
								for(int a = heu_ret.grid_a[ij]; a < heu_ret.grid_a[ij]+g; a++){
									resource->edges[mn].wave[a] = false;
								}
							}
					}					
				}
				int mod_num = 0-2,tempmod = mod;
				do{tempmod = tempmod/ 2;
					mod_num++ ;
				}while(tempmod != 0);
				if( physical_distance >= resource->moddistance[mod_num])
					continue;
				else{
					heu_ret.physical_distance[ij] = physical_distance;
					heu_ret.modtype[ij] = mod;
					heu_ret.bandwidth[ij] = g;
					printf("******total distance = %d\n",heu_ret.total_distance[ij]);
					break;
				}
			}
			if(mod == 2){
				heu_ret.isSuccess = 0;
				return heu_ret;
			}
		}
	}
	clk.setEndTime();
	heu_ret.proccessTime = clk.getTime();
	heu_ret.isSuccess = 1;
	return heu_ret;
}




Heuristic::Heuristic(Network *d, Network *r, FILE *lf){
	de = *d, re = *r ,ori = r;
	resource = new Network (*r);
	demand = new Network (*d);
	resource->mod++;
	logFile = lf;
	result.isSuccess = result.proccessCircle = result.proccessTime = result.proccessTreeNode = result.resultWeight = 0;
	
	for(int nodei=0;nodei<demand->n;nodei++){
		vector<int> tempFIk;
		FIk.push_back(tempFIk);
	}
	for(int nodei=0;nodei<demand->n;nodei++){
		for(int k=0;k<resource->n;k++){
			FIk[nodei].push_back(0);
		}
	}
	
	sortnetwork();
	for (int i = 0; i < resource->n; i++){
		vector<int> tempneighbor;
		resource->neighbor.push_back(tempneighbor);
	}

/*	for (int i = 0; i < resource->n; i++)				//����ÿ���ڵ���ھӽڵ�
		for( int k = 0; k < resource->n; k++)
			resource->neighbor[i].push_back(0);*/
	for(int k = 0; k < resource->m; k++){
		int head = resource->edges[k].head;
		int tail = resource->edges[k].tail;
		resource->neighbor[head].push_back(tail);
		resource->neighbor[tail].push_back(head);
	}
	for(int i = 0; i < resource->n; i++){
		sort( resource->neighbor[i].begin(),resource->neighbor[i].end());
		resource->neighbor[i].erase( unique(resource->neighbor[i].begin(),resource->neighbor[i].end()),resource->neighbor[i].end());
		
	}
}

Heuristic::~Heuristic(){

	
	delete resource;
	
//	fclose(optimalData);
}

Event Heuristic::solveByHeuristic(Network *d, Network *r, FILE *logFile){
	Event ret = Event(0, 0, 0, r);
	int numILPSuccess, numILPBlock, numHeuSuccess;
		double totalILPTime, totalILPWeight, totalHeuTime, totalHeuWeight, HeuWeight;
		Network original_resource = *r;
		Network *resource_Hr = &original_resource;
			 Heu_return heu_ret;
			 heu_ret = work();
			 HeuWeight = 0;
			 if(heu_ret.isSuccess){
				 int n = d->edges.size();
				for(int ij = 0; ij < n; ij++){
					for (int mn = 0; mn < resource_Hr->edges.size(); mn++){
						for (int k = 0; k < heu_ret.edge_head[ij].size(); k++){
							if(resource_Hr->edges[mn].head == heu_ret.edge_head[ij][k] || resource_Hr->edges[mn].head == heu_ret.edge_head[ij][k]-resource_Hr->n)
								if(resource_Hr->edges[mn].tail == heu_ret.edge_tail[ij][k] || resource_Hr->edges[mn].tail == heu_ret.edge_tail[ij][k]-resource_Hr->n)
									for(int ak = 0; ak < heu_ret.bandwidth[ij]; ak++){
										HeuWeight += resource_Hr->edges[mn].cmn*heu_ret.bandwidth[ij];
										int wave_ak = heu_ret.grid_a[ij] + ak;
										resource_Hr->edges[mn].wave[wave_ak] = false;
									}
						}
					}
				}
			 }
			 

			 if(heu_ret.isSuccess){
				numHeuSuccess += 1;
				totalILPTime += heu_ret.proccessTime;
				totalILPWeight += HeuWeight;
				fprintf(logFile, "Heu success, get object of %f, using Time %f\n ", HeuWeight, heu_ret.proccessTime);
				ret.proccessTime = heu_ret.proccessTime;
				for(int ij = 0; ij < heu_ret.demand_tail.size(); ij++){
					for(int k = 0; k < heu_ret.edge_head[ij].size(); k++){
						if(heu_ret.edge_head[ij][k] >= resource_Hr->n )
							heu_ret.edge_head[ij][k] -= resource_Hr->n;
						if(heu_ret.edge_tail[ij][k] >= resource_Hr->n )
							heu_ret.edge_tail[ij][k] -= resource_Hr->n;
					}
				}


				for(int ij = 0; ij < heu_ret.demand_tail.size(); ij++){
					printf("Virtual node: %d, %d ==> Physical node: %d, %d\n",heu_ret.demand_tail[ij],heu_ret.demand_head[ij],heu_ret.edge_head[ij][heu_ret.edge_head[ij].size()-1],heu_ret.edge_tail[ij][0]);


				}
			}
			 else
					fprintf(logFile, "Heu failed\n");
			 ret.resultWeight = HeuWeight;
			for(int ij = 0; ij < ret.demand->edges.size(); ij++)
				for(int b = 0; b < ret.demand->edges[ij].wave.size(); b++)
					ret.demand->edges[ij].wave[b] = false;

			for(int k = 0; k < heu_ret.modtype.size(); k++){
				if(heu_ret.modtype[k] == 2)
					heu_ret.modtype[k] = 0;
				if(heu_ret.modtype[k] == 4)
					heu_ret.modtype[k] = 1;
				if(heu_ret.modtype[k] == 8)
					heu_ret.modtype[k] = 2;
				if(heu_ret.modtype[k] == 16)
					heu_ret.modtype[k] = 3;
			}
			for(int i = 0; i < 10; i++)
		      for(int j = 0; j < 20; j++)
				  for (int a =0; a < 30; a++)
					  for(int mod =0; mod < 4; mod++){
						  ret.b[i][j][a][mod] = 0;
					  }
			 for(int i = 0; i < 10; i++)
		      for(int j = 0; j < 10; j++)
			 {
			      ret.a[i][j] = 0;
			}

		for(int mn = 0; mn < r->m; mn++){
			for(int ij = 0; ij < d->m; ij++){
				for( int k = 0; k < heu_ret.edge_head[ij].size(); k++){
					if(r->edges[mn].head == heu_ret.edge_head[ij][k] )
						if(r->edges[mn].tail == heu_ret.edge_tail[ij][k]){
							int a_start = heu_ret.grid_a[ij];
							for(int a = heu_ret.grid_a[ij]; a < heu_ret.grid_a[ij] + heu_ret.bandwidth[ij]; a++){
								ret.b[ij][mn][a_start][heu_ret.modtype[ij]] = 1;
							    ret.demand->edges[mn].wave[a] = true;
							}
						}
				}
			}
		}
		for (int i = 0; i < d->n ; i ++){
			for(int k = 0; k < r->n; k++){
				if(FIk[i][k] == 1)
					ret.a[i][k] = 1;
			}
		}
		
		for(int k = 0; k < r->n; k++)
			ret.demand->vertexWeight[k] = 0;
		for(int k = 0; k < r->n; k++)
			for(int i = 0; i < d->n; i++)
				if(ret.a[i][k] == 1)
					ret.demand->vertexWeight[k] += d->vertexWeight[i];
		
		ret.isSuccess = heu_ret.isSuccess;
		return ret;

					  
					  
					  


}