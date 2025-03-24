#pragma once
#include <iostream>
#include <vector>
#include <xmemory>
#include <unordered_map>

class graph
{
public:
	/// <summary>
	/// 功能：图构造函数。
	/// </summary>
	/// <param name="directed">: 是否为有向图</param>
	/// <param name="num_of_vertex">: 顶点数</param>
	graph(bool directed, uint32_t num_of_vertex);
	~graph();
	/// <summary>
	/// 功能：向图中添加一条边。
	/// </summary>
	/// <param name="v1">: 起始点</param>
	/// <param name="v2">: 终点</param>
	/// <param name="w">: 权值。主要用于带权情况下的寻最短/长路径</param>
	void addEdge(uint32_t v1, uint32_t v2, uint32_t w = -1);						
	/// <summary>
	/// 功能：移除图中的一条边。
	/// </summary>
	/// <param name="v1">: 起始点</param>
	/// <param name="v2">: 终点</param>
	void removeEdge(uint32_t v1, uint32_t v2);	
	/// <summary>
	/// 功能：判断图中是否存在环。
	/// </summary>
	/// <returns>true = 存在环，false = 不存在环。</returns>
	bool hasCycle();	
	/// <summary>
	/// 功能：取指定顶点的入度。用于有向图。
	/// </summary>
	/// <param name="v">: 指定顶点</param>
	/// <returns>uint32_t: 顶点的入度</returns>
	uint32_t getInDegree(uint32_t v);											
	/// <summary>
	/// 功能：取指定顶点的出度。用于有向图。
	/// </summary>
	/// <param name="v">: 指定顶点</param>
	/// <returns>uint32_t: 顶点的出度</returns>
	uint32_t getOutDegree(uint32_t v);												
	/// <summary>
	/// 功能：取指定顶点的度。用于无向图。
	/// </summary>
	/// <param name="v">: 指定顶点</param>
	/// <returns>uint32_t: 顶点的度</returns>
	uint32_t getDegree(uint32_t v);
	std::shared_ptr<std::vector<std::vector<uint32_t>>> getCycle(uint32_t v);
	/// <summary>
	/// 功能：拓扑排序
	/// </summary>
	/// <param name="v">: 终点</param>
	/// <returns>std::vector&lt;uint32_t&gt;: 拓扑排序序列</returns>
	std::shared_ptr<std::vector<uint32_t>> topologySort(uint32_t v);	
	/// <summary>
	/// 功能：深度优先遍历。用于获取从指定顶点出发能够到达哪些顶点，可判断图连通性。
	/// </summary>
	/// <param name="v">: 指定顶点</param>
	/// <returns>std::vector&lt;uint32_t&gt;: 可达顶点序列</returns>
	std::shared_ptr<std::vector<uint32_t>> dfs(uint32_t v);							
	/// <summary>
	/// 功能：广度优先遍历。用于获取从指定顶点出发能够到达哪些顶点，可判断图连通性。
	/// </summary>
	/// <param name="v">: 指定顶点</param>
	/// <returns>std::vector&lt;uint32_t&gt;: 可达顶点序列</returns>																				
	std::shared_ptr<std::vector<uint32_t>> bfs(uint32_t v);		
	/// <summary>
	/// 功能：获取指定两个顶点间的最短路径
	/// </summary>
	/// <param name="v1">: 起始点</param>
	/// <param name="v2">: 终点</param>
	/// <param name="value">: 两点间的最长距离</param>
	/// <param name="with_wight">: 是否使用带权方式作为距离</param>
	/// <returns>std::vector&lt;uint32_t&gt;: 路径序列</returns>
	std::shared_ptr<std::vector<uint32_t>> getShortestPath(
		uint32_t v1,
		uint32_t v2,
		uint32_t& value,
		bool with_wight = false);
	/// <summary>
	/// 功能：获取指定两个顶点间的最长路径
	/// </summary>
	/// <param name="v1">: 起始点</param>
	/// <param name="v2">: 终点</param>
	/// <param name="value">: 两点间的最长距离</param>
	/// <param name="with_wight">: 是否使用带权方式作为距离</param>
	/// <returns>std::vector&lt;uint32_t&gt;: 路径序列</returns>
	std::shared_ptr<std::vector<uint32_t>> getLongestPath(
		uint32_t v1,							
		uint32_t v2,							
		uint32_t& value,						
		bool with_wight = false);												
	/// <summary>
	/// 功能: 获取指定两点间的所有路径
	/// </summary>
	/// <param name="v1">: 起始点</param>
	/// <param name="v2">: 终点</param>
	/// <returns>std::vector&lt;std::vector&lt;uint32_t&gt;&gt;: 所有路径序列</returns>
	std::shared_ptr<std::vector<std::vector<uint32_t>>> getAllPath(	
		uint32_t v1,
		uint32_t v2);
	// Dijkstra 算法求解的问题是指定单源点下的最短路径，性能优于Floyd。
	std::shared_ptr<std::unordered_map<uint32_t, std::vector<uint32_t>>> DijkstraAlogrithm(
		uint32_t v1,
		std::vector<uint32_t>& dis,
		bool reverse = false);
private:
	// 初始化内部数据
	bool init(uint32_t num_of_vertex);
	

private:
	// 是否为有向图
	bool _IsDirected;	
	// 邻接矩阵
	bool* _AdjMatrix;			
	// 权重矩阵
	uint32_t* _Weight;				
	// 缓存各顶点的出度
	std::vector<uint32_t> _OutDegree;	
	// 缓存各顶点的入度
	std::vector<uint32_t> _InDegree;
	// 缓存各顶点的度数
	std::vector<uint32_t> _Degree;
	// 顶点数
	uint32_t _NumOfVertex;															
};

// 功能测试
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