#pragma once
#include <iostream>
#include <vector>
#include <xmemory>
#include <unordered_map>

class graph
{
public:
	/// <summary>
	/// ���ܣ�ͼ���캯����
	/// </summary>
	/// <param name="directed">: �Ƿ�Ϊ����ͼ</param>
	/// <param name="num_of_vertex">: ������</param>
	graph(bool directed, uint32_t num_of_vertex);
	~graph();
	/// <summary>
	/// ���ܣ���ͼ�����һ���ߡ�
	/// </summary>
	/// <param name="v1">: ��ʼ��</param>
	/// <param name="v2">: �յ�</param>
	/// <param name="w">: Ȩֵ����Ҫ���ڴ�Ȩ����µ�Ѱ���/��·��</param>
	void addEdge(uint32_t v1, uint32_t v2, uint32_t w = -1);						
	/// <summary>
	/// ���ܣ��Ƴ�ͼ�е�һ���ߡ�
	/// </summary>
	/// <param name="v1">: ��ʼ��</param>
	/// <param name="v2">: �յ�</param>
	void removeEdge(uint32_t v1, uint32_t v2);	
	/// <summary>
	/// ���ܣ��ж�ͼ���Ƿ���ڻ���
	/// </summary>
	/// <returns>true = ���ڻ���false = �����ڻ���</returns>
	bool hasCycle();	
	/// <summary>
	/// ���ܣ�ȡָ���������ȡ���������ͼ��
	/// </summary>
	/// <param name="v">: ָ������</param>
	/// <returns>uint32_t: ��������</returns>
	uint32_t getInDegree(uint32_t v);											
	/// <summary>
	/// ���ܣ�ȡָ������ĳ��ȡ���������ͼ��
	/// </summary>
	/// <param name="v">: ָ������</param>
	/// <returns>uint32_t: ����ĳ���</returns>
	uint32_t getOutDegree(uint32_t v);												
	/// <summary>
	/// ���ܣ�ȡָ������Ķȡ���������ͼ��
	/// </summary>
	/// <param name="v">: ָ������</param>
	/// <returns>uint32_t: ����Ķ�</returns>
	uint32_t getDegree(uint32_t v);
	std::shared_ptr<std::vector<std::vector<uint32_t>>> getCycle(uint32_t v);
	/// <summary>
	/// ���ܣ���������
	/// </summary>
	/// <param name="v">: �յ�</param>
	/// <returns>std::vector&lt;uint32_t&gt;: ������������</returns>
	std::shared_ptr<std::vector<uint32_t>> topologySort(uint32_t v);	
	/// <summary>
	/// ���ܣ�������ȱ��������ڻ�ȡ��ָ����������ܹ�������Щ���㣬���ж�ͼ��ͨ�ԡ�
	/// </summary>
	/// <param name="v">: ָ������</param>
	/// <returns>std::vector&lt;uint32_t&gt;: �ɴﶥ������</returns>
	std::shared_ptr<std::vector<uint32_t>> dfs(uint32_t v);							
	/// <summary>
	/// ���ܣ�������ȱ��������ڻ�ȡ��ָ����������ܹ�������Щ���㣬���ж�ͼ��ͨ�ԡ�
	/// </summary>
	/// <param name="v">: ָ������</param>
	/// <returns>std::vector&lt;uint32_t&gt;: �ɴﶥ������</returns>																				
	std::shared_ptr<std::vector<uint32_t>> bfs(uint32_t v);		
	/// <summary>
	/// ���ܣ���ȡָ���������������·��
	/// </summary>
	/// <param name="v1">: ��ʼ��</param>
	/// <param name="v2">: �յ�</param>
	/// <param name="value">: �����������</param>
	/// <param name="with_wight">: �Ƿ�ʹ�ô�Ȩ��ʽ��Ϊ����</param>
	/// <returns>std::vector&lt;uint32_t&gt;: ·������</returns>
	std::shared_ptr<std::vector<uint32_t>> getShortestPath(
		uint32_t v1,
		uint32_t v2,
		uint32_t& value,
		bool with_wight = false);
	/// <summary>
	/// ���ܣ���ȡָ�������������·��
	/// </summary>
	/// <param name="v1">: ��ʼ��</param>
	/// <param name="v2">: �յ�</param>
	/// <param name="value">: �����������</param>
	/// <param name="with_wight">: �Ƿ�ʹ�ô�Ȩ��ʽ��Ϊ����</param>
	/// <returns>std::vector&lt;uint32_t&gt;: ·������</returns>
	std::shared_ptr<std::vector<uint32_t>> getLongestPath(
		uint32_t v1,							
		uint32_t v2,							
		uint32_t& value,						
		bool with_wight = false);												
	/// <summary>
	/// ����: ��ȡָ������������·��
	/// </summary>
	/// <param name="v1">: ��ʼ��</param>
	/// <param name="v2">: �յ�</param>
	/// <returns>std::vector&lt;std::vector&lt;uint32_t&gt;&gt;: ����·������</returns>
	std::shared_ptr<std::vector<std::vector<uint32_t>>> getAllPath(	
		uint32_t v1,
		uint32_t v2);
	// Dijkstra �㷨����������ָ����Դ���µ����·������������Floyd��
	std::shared_ptr<std::unordered_map<uint32_t, std::vector<uint32_t>>> DijkstraAlogrithm(
		uint32_t v1,
		std::vector<uint32_t>& dis,
		bool reverse = false);
private:
	// ��ʼ���ڲ�����
	bool init(uint32_t num_of_vertex);
	

private:
	// �Ƿ�Ϊ����ͼ
	bool _IsDirected;	
	// �ڽӾ���
	bool* _AdjMatrix;			
	// Ȩ�ؾ���
	uint32_t* _Weight;				
	// ���������ĳ���
	std::vector<uint32_t> _OutDegree;	
	// �������������
	std::vector<uint32_t> _InDegree;
	// ���������Ķ���
	std::vector<uint32_t> _Degree;
	// ������
	uint32_t _NumOfVertex;															
};

// ���ܲ���
void TestBfs();
void TestDfs();
void TestGetDegree();
void TestJudgeCycle();
void TestToplogySort();
void TestGetAllPath();
void TestGetShortestPath();
void TestGetLongestPath();
void TestDijkstra();
void TestReDijkstra();