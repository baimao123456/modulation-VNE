#pragma once
#include"common.h"


#define d_same 300		//������ͬ����ƽ������
#define d_diff 1000		//���岻ͬ����ƽ������
#define max_slot 20
//#define m_degree 3		//ƽ��ÿ���ڵ�Ķ�

int uniform(int a, int b){
	return a + rand()%(b-a+1);
}

void mapGenerate(char route[], int n, int m, int area, int maxWeight,int maxdistance){
/*
	�����������
	routeΪ�����ļ��洢·��
	nΪ������mΪ����
	areaΪ��Ҫ���ֵ�������
*/
	FILE *out = fopen(route, "w");
	
	/*��һ��vector�д�area���������б���ͬ�������ڵ����
	 *Ĭ�����нڵ㰴˳�����У�����1-10��һ������11-15��һ�������Դ�����
	 *vector�����[10,15,..]
	 */
	vector<int> area_n;
	for(int i = 0; i < area; i++){
		area_n.push_back(rand()%n-1);
	}
	sort(area_n.begin(),area_n.end());
	vector<int>::iterator k = area_n.begin();
    area_n.erase(k);
	area_n.push_back(n);//������һ������Ϊ�յ�
	//�������ƽ���ڵ��
	int n_degree = uniform(1,2*m/n);
	/* n_ �͵�ǰ�����ӵĵ�, n_index �������ĸ�����, flag �Ƿ�ӳ���һ����
	 * distance ������ɵľ���, m_ �����Ѿ�ӳ��ı���
	 */
	int n_,n_index,flag,distance,m_;
	n_index = 0;
	flag = 0;
	m_ = 0;
	for(int i = 0; i < n; i++)
		fprintf(out, "%d ", 1000); //�ڵ����Դ
	fprintf(out, "\n");
	for(int i = 0; i < m; i++)
		fprintf(out, "%d ", 2); //???
	fprintf(out, "\n");

	for(int i = 0; i < n; i++){
		//�ж����ĸ�����
		if(i >= area_n[n_index])
			n_index++;
		for(int j = 0; j < uniform(0,n_degree);j++){
			//���������һ������ô����������ڵĵ�����
			if(flag==0){
				if(n_index == 0){
					do{
						n_ = rand()%(area_n[n_index]);//��ֹ���
					}while(n_ == i);
				}
				else{
					do{
						n_ = rand()%(area_n[n_index]-area_n[n_index-1])+area_n[n_index-1];
					}while(n_ == i);
				}
				distance = uniform(0,2*d_same);
				fprintf(out, "%d %d %d %d %d\n", i, n_,max_slot, 1, distance);
				fprintf(out, "%d %d %d %d %d\n", n_, i,max_slot, 1, distance);
				flag = 1;
				if(++m_ == m)
					break;
			}
			do{
				n_ = rand()%n;
			}while(n_ == i);
			distance = uniform(0,2*d_diff);
			fprintf(out, "%d %d %d %d %d\n", i, n_,max_slot, 1, distance);
			fprintf(out, "%d %d %d %d %d\n", n_, i,max_slot, 1, distance);
			if(++m_ == m)
				break;
		}
		flag = 0;
		if(m_ == m)
			break;
	}
	//���������������
	if(m_< m){
		int n__;
		for(int i = m_; i < m; i++){
			do{
				n_ = rand()%n;
				n__ = rand()%n;
			}while(n_ == n__);
			distance = uniform(0,2*d_diff);
			fprintf(out, "%d %d %d %d %d\n", n__, n_,max_slot, 1, distance);
			fprintf(out, "%d %d %d %d %d\n", n_, n__,max_slot, 1, distance);
		}
	}

	fclose(out);
	system("type \inputData\\demand_r.txt>>\inputData\\alldemand.txt");
}




int findpath(Network *r,int source_vertexID, int dest_vertexID){
	int temp_dist=100000000,temp_node_id ,findcounts = 0;
	Network *tempnet = r;
	vector<double>dist(tempnet->n,1000000000);
	vector<int> last_vertex_ID;							//��¼��һ���ڵ��ID

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


void chooseLi(Network *r,int boundary){
	Network *tempr = r;
	vector<vector<int>>node;
	//�������·�㷨���ÿ���㵽�������е����
	for(int i = 0; i < tempr->n; i++){
		vector<int>ndist;
		for(int j = 0; j < tempr->n; j++){
			int dist = findpath(tempr,i,j);
			//����boundary���Ƶõ�ÿ�������������
			if(dist < boundary)
				ndist.push_back(j);	  
		}
		node.push_back(ndist);
	}
	r->neighbor = node;


}
//�����demand�ڵ㰲����Ҫӳ�������
void randomGenerate(Network *d,Network *r){

	vector<int> startArray;
	for(int i = 0; i <r->n; i++){
		startArray.push_back(i);
	}	    
	int N = d->n;//���������    
	vector<int> resultArray;  
	for(int i = 0; i < N; i++)    
	{    
		int seed = rand()%(startArray.size()-i);    
		vector<int> pp;
		d->neighbor.push_back(pp);
		d->neighbor[i].push_back(startArray[seed]);
		//resultArray.push_back(startArray[seed]);//��ֵ���������    
		startArray[seed] = startArray[startArray.size() - i - 1];//���������������λ���滻Ϊδ��ѡ�е�ֵ��    
	}    

}
