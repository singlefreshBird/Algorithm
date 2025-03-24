#include "graph.h"
#include <queue>
#include <stack>
#include <set>
#include <exception>

#define min(x,y)((x) < (y) ? (x) : (y))

graph::~graph() 
{
	if (_AdjMatrix) delete[] _AdjMatrix;
	if (_Weight) delete[] _Weight;
}

graph::graph(bool directed, uint32_t num_of_vertex):
	_IsDirected(directed),
	_AdjMatrix(nullptr),
	_Weight(nullptr),
	_NumOfVertex(0)
{
	if (init(num_of_vertex) == false) exit(1);
}

bool graph::init(uint32_t num_of_vertex)
{
	bool bRet = false;

	try
	{
		_AdjMatrix = new bool[num_of_vertex * num_of_vertex];

		memset(_AdjMatrix, false, sizeof(bool) * num_of_vertex * num_of_vertex);

		_Weight = new uint32_t[num_of_vertex * num_of_vertex];

		// ��ʼȨ�ظ���Ϊ�����
		memset(_Weight, -1, sizeof(uint32_t) * num_of_vertex * num_of_vertex);

		this->_NumOfVertex = num_of_vertex;

		for (uint32_t i = 0; i < num_of_vertex; i++)
		{
			_InDegree.emplace_back(0);
			_OutDegree.emplace_back(0);
			_Degree.emplace_back(0);
		}

		bRet = true;
	}
	catch (std::bad_alloc& e)
	{
		std::cout << "init(): " << e.what() << std::endl;
	}

	return bRet;
}

void graph::addEdge(uint32_t v1, uint32_t v2, uint32_t w)	// ���ڽӾ������һ����
{
	if ((v1 > 0 && v1 <= _NumOfVertex) && (v2 > 0 && v2 <= _NumOfVertex))
	{
		_InDegree[v2 - 1] += 1;
		_OutDegree[v1 - 1] += 1;
		_Degree[v1 - 1] = _InDegree[v1 - 1] + _OutDegree[v1 - 1];
		_Degree[v2 - 1] = _InDegree[v2 - 1] + _OutDegree[v2 - 1];
		_AdjMatrix[(v1 - 1) * _NumOfVertex + (v2 - 1)] = true;
		_Weight[(v1 - 1) * _NumOfVertex + (v2 - 1)] = w;
		_Weight[(v1 - 1) * _NumOfVertex + (v1 - 1)] = 0;
	}
}

void graph::removeEdge(uint32_t v1, uint32_t v2)
{
	if ((v1 > 0 && v1 <= _NumOfVertex) && 
		(v2 > 0 && v2 <= _NumOfVertex) && 
		_AdjMatrix[(v1 - 1) * _NumOfVertex + (v2 - 1)])
	{
		_InDegree[v1 - 1] -= 1;
		_OutDegree[v2 - 1] -= 1;
		_Degree[v1 - 1] = _InDegree[v1 - 1] + _OutDegree[v1 - 1];
		_AdjMatrix[(v1 - 1) * _NumOfVertex + (v2 - 1)] = false;
		_Weight[(v1 - 1) * _NumOfVertex + (v2 - 1)] = -1;
	}
}

bool graph::hasCycle()
/*
 * 1. ����һ������Q����������������Ϊ0�Ķ��㣬��������ͼ��˵���Ŷ�Ϊ1�Ķ��㡣
 * 2. ���������ͱ���num����¼���Ϊ0���߶�Ϊ1�Ķ�������
 * 3. ���������ͼ���������Ϊ0�Ķ������Q�����������ͼ�����ж�Ϊ1�Ķ������Q������num��
 * 4. ȡQ�ײ�Ԫ��p�����p����v���������������������p�ڽӵĵ�vi����ȼ�һ�����vi�����Ϊ0
 *  ������ͼ�����߶�Ϊ1������ͼ������vi����Q������num��
 * 5. ѭ������4��ֱ��QΪ�ա���ʱnum���������ͼ�����ж������������˵��ͼ�д��ڻ�����������޻���
 */
{
	std::queue< uint32_t> Q;
	int32_t num = 0;


	std::vector<uint32_t> V(_IsDirected ? _InDegree : _Degree);

	for (int32_t v = 0; v < _NumOfVertex; v++)
	{
		if ((_IsDirected && V[v] == 0) || (_IsDirected && V[v] == 1))
		{
			Q.push(v);
			num += 1;
		}
	}

	while (!Q.empty())
	{
		int32_t v = Q.front();
		Q.pop();
		
		for (int32_t i = 0; i < _NumOfVertex; i++)
		{
			if (_AdjMatrix[v * _NumOfVertex + i])
			{
				V[i] -= 1;
				if ((_IsDirected && V[i] == 0) || (_IsDirected && V[i] == 1))
				{
					Q.push(i);
					num += 1;
				}
			}
		}
	}

	return num != _NumOfVertex;;
}

uint32_t graph::getInDegree(uint32_t v)
{
	if ((v > 0 && v <= _NumOfVertex) ) return _InDegree[v - 1];

	return -1;
}

uint32_t graph::getOutDegree(uint32_t v)
{
	if ((v > 0 && v <= _NumOfVertex)) return _OutDegree[v - 1];

	return -1;
}

uint32_t graph::getDegree(uint32_t v)
{	
	if ((v > 0 && v <= _NumOfVertex)) return _Degree[v - 1];

	return -1;
}

std::shared_ptr<std::vector<uint32_t>> graph::topologySort(uint32_t v)
/*
 * ���ж�ͼ���ڻ�һ�����㷨
 */
{
	std::vector<uint32_t> resVector;
	std::queue<uint32_t> Q;
	std::vector<uint32_t> V(_IsDirected ? _InDegree : _Degree);

	if (v == 0 && v > _NumOfVertex) goto end_ret;

	for (int32_t i = 0; i < _NumOfVertex; i++)
	{
		if ((_IsDirected && V[i] == 0) || (!_IsDirected && V[i] == 1)) Q.push(i);
	}

	while (!Q.empty())
	{
		int32_t p = Q.front();
		Q.pop();

		resVector.push_back(p + 1);

		if (p + 1 == v) break;

		for (int32_t i = 0; i < _NumOfVertex; i++)
		{
			if (_AdjMatrix[p * _NumOfVertex + i])
			{
				V[i] -= 1;
				if ((_IsDirected && V[i] == 0) || (!_IsDirected && V[i] == 1)) Q.push(i);
			}
		}
	}

end_ret:
	return std::make_shared<std::vector<uint32_t>>(resVector);
}

std::shared_ptr<std::vector<uint32_t>> graph::dfs(uint32_t v)
/*
 * ��������������������������������
 */
{
	std::vector<uint32_t> resVector;
	std::stack<uint32_t> S;
	bool recurse_over;
	bool* visited = nullptr;

	if (v == 0 || v > _NumOfVertex + 1) goto end_ret;

	visited = new(std::nothrow) bool[_NumOfVertex];
	if (visited == nullptr || _AdjMatrix == nullptr) return nullptr;

	// ���һ�±��
	memset(visited, false, sizeof(bool) * _NumOfVertex);

	visited[v - 1] = true;
	S.push(v-1);

	while (!S.empty())
	{
		uint32_t p = S.top();
		recurse_over = true;

		for (uint32_t i = 0; i < _NumOfVertex; i++)
		{	
			if (!visited[i] && _AdjMatrix[p * _NumOfVertex + i])
			{
				visited[i] = true;
				S.push(i);
				recurse_over = false;
				// ֻҪ�ҵ�һ���ڽӵ����ջ��Ȼ����������ѭ�������Ըõ�����ظ�����Ĳ���
				// ʵ��ģ��ݹ�
				break;
			}
		}

		if (recurse_over)
		{
			resVector.push_back(p+1);
			S.pop();
		}
	}

end_ret:
	if (visited) delete[] visited;
	return std::make_shared<std::vector<uint32_t>>(resVector);
}

std::shared_ptr<std::vector<uint32_t>> graph::bfs(uint32_t v)
/*
 * ���ù�������������������Ĳ�����������岽�裺
 *	1.ѡ��һ��������Ϊ��ʼ��������У�����Ǹõ�Ϊ�ѷ���
 *	2.�Ӷ����ײ�ȡһ�����㣬���ҵ���֮���ڵĶ��㣬���ҵ��ĵ�������β��������Ǹõ�Ϊ�ѷ���
 *	3.�ظ�����2��ֱ������Ϊ��
 */
{
	std::vector<uint32_t> resVector;
	std::queue<uint32_t> Q;
	bool* visited = nullptr;

	if (v == 0 || v > _NumOfVertex + 1) goto end_ret;

	visited = new(std::nothrow) bool[_NumOfVertex];
	if (visited == nullptr) goto end_ret;

	// ���һ�±��
	memset(visited, false, sizeof(bool) * _NumOfVertex);

	// ��ǳ�ʼ����Ϊ�ѷ���
	visited[v-1] = true;
	// ��ʼ�������
	Q.push(v-1);
	// ��������
	while (!Q.empty())
	{
		uint32_t p = Q.front();
		resVector.push_back(p+1);
		Q.pop();

		for (uint32_t i = 0; i < _NumOfVertex; i++)
		{
			// ��������p�����ڵĵ�
			if (!visited[i] && _AdjMatrix[p * _NumOfVertex + i])
			{
				visited[i] = true;
				Q.push(i);
			}
		}
	}

end_ret:
	if (visited) delete[] visited;
	return std::make_shared<std::vector<uint32_t>>(resVector);
}

std::shared_ptr<std::vector<uint32_t>> graph::getShortestPath(
	uint32_t v1,							
	uint32_t v2,
	uint32_t& value,
	bool with_wight)
{
	std::vector<uint32_t>shortestPath;
	uint32_t prevLen = -1;

	if ((v1 == 0 || v1 > _NumOfVertex) || (v2 == 0 || v2 > _NumOfVertex)) goto end_ret;

	if (with_wight == false)
	{
		auto r = getAllPath(v1, v2);
		if (r != nullptr)
		{
			for (auto i : *r)
			{
				if (i.size() < prevLen)
				{
					prevLen = i.size();
					shortestPath = i;
				}
			}
		}
	}
	else
	{
		std::vector<uint32_t> d;
		auto r = DijkstraAlogrithm(v1, d);
		if (r->find(v2 - 1) != r->end())
			shortestPath = (*r)[v2 - 1];
	}
	
end_ret:
	return std::make_shared<std::vector<uint32_t>>(shortestPath);
}

std::shared_ptr<std::vector<uint32_t>> graph::getLongestPath(							
	uint32_t v1,							
	uint32_t v2,							
	uint32_t& value,					
	bool with_wight)
{
	std::vector<uint32_t>longestPath;
	uint32_t prevLen = 0;

	if ((v1 == 0 || v1 > _NumOfVertex) || (v2 == 0 || v2 > _NumOfVertex)) goto end_ret;

	if (with_wight == false)
	{
		auto r = getAllPath(v1, v2);
		if (r != nullptr)
		{
			for (auto i : *r)
			{
				if (i.size() > prevLen)
				{
					prevLen = i.size();
					longestPath = i;
				}
			}
		}
	}
	else
	{
		std::vector<uint32_t> d;
		auto r = DijkstraAlogrithm(v1, d, true);
		if (r->find(v2 - 1) != r->end())
			longestPath = (*r)[v2 - 1];
	}

end_ret:
	return std::make_shared<std::vector<uint32_t>>(longestPath);
}

std::shared_ptr<std::vector<std::vector<uint32_t>>> graph::getAllPath(uint32_t v1, uint32_t v2)
/*
 * ��ȡָ����������������·���㷨��
 *
 */	
{
	std::stack<uint32_t> S;
	std::vector<uint32_t> path;
	std::vector<std::vector<uint32_t>> resVec;
	std::vector<int32_t> pos;
	bool recursed = false;
	bool* visited = nullptr;

	if (v1 == 0 || v1 > _NumOfVertex || (v2 == 0 || v2 > _NumOfVertex)) goto end_exit;

	visited = new(std::nothrow) bool[_NumOfVertex];
	if (visited == nullptr) goto end_exit;
	// ��ʼ��
	for (uint32_t i = 0; i < _NumOfVertex; i++)
	{
		visited[i] = false;
		pos.push_back(0);
	}

	S.push(v1 - 1);
	path.push_back(v1);
	visited[v1 - 1] = true;

	while (!S.empty())
	{
		auto v = S.top();
		recursed = false;

		for (uint32_t i = pos[v]; i < _NumOfVertex; i++)
		{
			if (!visited[i] && _AdjMatrix[v * _NumOfVertex + i])
			{
				if (i == v2 - 1)
				{
					std::vector<uint32_t>tmpVec(path);
					tmpVec.push_back(i + 1);
					resVec.push_back(tmpVec);
				}
				else
				{
					visited[i] = true;
					S.push(i);
					path.push_back(i + 1);
					pos[i] = 0;

					pos[v] = i + 1;
					recursed = true;
					break;
				}
			}
		}

		if (!recursed)
		{// ���ݴ�
			path.pop_back();
			v = S.top();
			if(pos[v]) pos[v] -= 1;
			visited[v] = false;
			S.pop();
		}
	}
	
end_exit:
	if (visited) delete[] visited;
	return std::make_shared<std::vector<std::vector<uint32_t>>>(resVec);
}

std::shared_ptr<std::unordered_map<uint32_t, std::vector<uint32_t>>> graph::DijkstraAlogrithm(
	uint32_t v1,
	std::vector<uint32_t>& dis,
	bool reverse)
{
	std::unordered_map<uint32_t, std::vector<uint32_t>> path;
	std::queue<uint32_t> Q;
	uint32_t v;
	bool* visited = nullptr;

	visited = new(std::nothrow) bool[_NumOfVertex];
	if (visited == nullptr) goto end_ret;

	// ���һ�±��
	memset(visited, false, sizeof(bool) * _NumOfVertex);
	
	for (uint32_t i = 0; i < _NumOfVertex; i++)
	{
		// ��ʼ��ָ��Դ�㵽������������·��
		path.insert(std::pair<uint32_t, std::vector<uint32_t>>(i, std::vector<uint32_t>()));
		// ��ʼ������
		if (i == v1 - 1) dis.push_back(0);
		else dis.push_back(reverse ? 0 : -1);
	}

	Q.push(v1 - 1);
	visited[v1 - 1] = true;

	while (!Q.empty())
	{
		auto v = Q.front();
		Q.pop();
		uint32_t s = v;
		uint32_t prev_dis = reverse ? 0 : -1;

		for (uint32_t i = 0; i < _NumOfVertex; i++)
		{
			if (!visited[i] && _AdjMatrix[v * _NumOfVertex + i])
			{
				auto w = _Weight[v * _NumOfVertex + i] + dis[v];

				if (reverse)
				{
					if (prev_dis < w)
					{
						// ���±���Ѱ�ҵ����·������
						prev_dis = w;
						s = i; // s ʼ��ָ�򱾴�Ѱ�ҵ����·�����յ�
					}

					if (w > dis[i])
					{
						// ��̬�����·��
						dis[i] = w;
						if (path[v].size()) path[i] = std::vector<uint32_t>(path[v]);
						path[i].push_back(v + 1);
					}
				}
				else
				{
					if (prev_dis > w)
					{
						// ���±���Ѱ�ҵ������·������
						prev_dis = w;
						s = i; // s ʼ��ָ�򱾴�Ѱ�ҵ������·�����յ�
					}

					if (w < dis[i])
					{
						// ��̬�������·��
						dis[i] = w;
						if (path[v].size()) path[i] = std::vector<uint32_t>(path[v]);
						path[i].push_back(v + 1);
					}
				}
			}
		}

		if (s != v)
		{
			// ���������·���յ���Ϊ�ѷ��ʣ���ζ�Ŵ�ָ��Դ�㵽s �����·���Ѿ��ҵ���
			visited[s] = true;
			Q.push(s);
		}
	}
	
end_ret:
	if (visited) delete[] visited;
	return std::make_shared<std::unordered_map<uint32_t, std::vector<uint32_t>>>(path);
}

void TestBfs()
{
	graph g1(false, 5);

	g1.addEdge(1, 2);
	g1.addEdge(1, 3);
	g1.addEdge(2, 3);
	g1.addEdge(2, 4);
	g1.addEdge(3, 4);
	g1.addEdge(3, 5);
	g1.addEdge(4, 1);
	g1.addEdge(4, 2);
	g1.addEdge(5, 4);

	auto res = g1.bfs(4);

	printf("g1:");
	for (auto i : *res)
	{
		printf("%d,", i);
	}

	printf("\n");

	graph g2(true, 4);

	g2.addEdge(3, 2);
	g2.addEdge(2, 4);
	g2.addEdge(2, 1);
	g2.addEdge(4, 1);


	auto res2 = g2.bfs(4);

	printf("g2:");
	for (auto i : *res2)
	{
		printf("%d,", i);
	}

	printf("\n");
}

void TestDfs()
{
	graph g1(false,5);

	g1.addEdge(1, 2);
	g1.addEdge(1, 3);
	g1.addEdge(2, 3);
	g1.addEdge(2, 4);
	g1.addEdge(3, 4);
	g1.addEdge(3, 5);
	g1.addEdge(4, 1);
	g1.addEdge(4, 2);
	g1.addEdge(5, 4);

	auto res = g1.dfs(4);

	printf("g1:");
	for (auto i : *res)
	{
		printf("%d,", i);
	}

	printf("\n");

	graph g2(true, 4);

	g2.addEdge(3, 2);
	g2.addEdge(2, 4);
	g2.addEdge(2, 1);
	g2.addEdge(4, 1);


	auto res2 = g2.dfs(4);

	printf("g2:");
	for (auto i : *res2)
	{
		printf("%d,", i);
	}

	printf("\n");

}

void TestGetDegree()
{
	graph g(true, 5);

	g.addEdge(1, 2);
	g.addEdge(1, 3);
	g.addEdge(2, 3);
	g.addEdge(2, 4);
	g.addEdge(3, 2);
	g.addEdge(3, 4);
	g.addEdge(5, 4);
	g.addEdge(5, 3);

	uint32_t id = 0, od = 0, d = 0;
	uint32_t v = 2;
	id = g.getInDegree(v);
	od = g.getOutDegree(v);
	d = g.getDegree(v);

	printf("v%d out degree: %d,in degree: %d, all degree: %d\n", v, od, id, d);
}

void TestJudgeCycle()
{
	graph g1(false, 4);

	g1.addEdge(1, 2);
	g1.addEdge(2, 3);
	g1.addEdge(3, 4);
	g1.addEdge(4, 2);

	graph g2(true, 4);

	g2.addEdge(3, 2);
	g2.addEdge(2, 1);
	g2.addEdge(2, 4);
	g2.addEdge(4, 1);

	if (g1.hasCycle()) printf("g1 cycle!\n");
	else printf("g1 No!\n");

	if (g2.hasCycle()) printf("g2 cycle!\n");
	else printf("g2 No!\n");
}

void TestToplogySort()
{
	graph g2(true, 5);

	g2.addEdge(1, 5);
	g2.addEdge(2, 1);
	g2.addEdge(2, 3);
	g2.addEdge(3, 5);
	g2.addEdge(3, 4);
	g2.addEdge(4, 5);

	auto r = g2.topologySort(5);

	for (auto i : *r)
	{
		printf("%d,", i);
	}

	printf("\n");
}

void TestGetAllPath()
{
	graph g1(true, 8);

	g1.addEdge(1, 3);
	g1.addEdge(1, 2);
	g1.addEdge(1, 4);
	g1.addEdge(2, 6);
	g1.addEdge(3, 5);
	g1.addEdge(4, 6);
	g1.addEdge(5, 7);
	g1.addEdge(6, 4);
	g1.addEdge(6, 7);
	g1.addEdge(6, 8);

	auto r = g1.getAllPath(1,7);

	for (auto i : *r)
	{
		for (auto j : i)
		{
			printf("%d ->",j);
		}

		printf("\n");
	}
}

void TestGetShortestPath()
{
	graph g1(false, 6);

	g1.addEdge(1, 3);
	g1.addEdge(1, 2);
	g1.addEdge(1, 6);
	g1.addEdge(2, 6);
	g1.addEdge(2, 1);
	g1.addEdge(2, 5);
	g1.addEdge(2, 4);
	g1.addEdge(2, 3);
	g1.addEdge(3, 1);
	g1.addEdge(3, 2);
	g1.addEdge(3, 4);
	g1.addEdge(4, 3);
	g1.addEdge(4, 2);
	g1.addEdge(4, 5);
	g1.addEdge(5, 2);
	g1.addEdge(5, 4);
	g1.addEdge(6, 2);
	g1.addEdge(6, 1);

	uint32_t d;
	auto r = g1.getShortestPath(6, 5, d);
	for (auto i : *r)
	{
		printf("%d ->", i);
	}

	printf("\n");
}

void TestGetLongestPath()
{
	graph g1(false, 6);

	g1.addEdge(1, 3);
	g1.addEdge(1, 2);
	g1.addEdge(1, 6);
	g1.addEdge(2, 6);
	g1.addEdge(2, 1);
	g1.addEdge(2, 5);
	g1.addEdge(2, 4);
	g1.addEdge(2, 3);
	g1.addEdge(3, 1);
	g1.addEdge(3, 2);
	g1.addEdge(3, 4);
	g1.addEdge(4, 3);
	g1.addEdge(4, 2);
	g1.addEdge(4, 5);
	g1.addEdge(5, 2);
	g1.addEdge(5, 4);
	g1.addEdge(6, 2);
	g1.addEdge(6, 1);

	uint32_t d;
	auto r = g1.getLongestPath(6, 5,d);
	for (auto i : *r)
	{
		printf("%d ->", i);
	}

	printf("\n");
}

void TestDijkstra()
{
	graph g1(false, 7);

	g1.addEdge(1, 2, 14);
	g1.addEdge(1, 7, 9);
	g1.addEdge(1, 4, 8);
	g1.addEdge(2, 1, 14);
	g1.addEdge(2, 7, 16);
	g1.addEdge(2, 6, 12);
	g1.addEdge(3, 6, 10);
	g1.addEdge(3, 7, 6);
	g1.addEdge(3, 4, 1);
	g1.addEdge(3, 5, 3);
	g1.addEdge(4, 1, 8);
	g1.addEdge(4, 7, 2);
	g1.addEdge(4, 3, 1);
	g1.addEdge(4, 5, 4);
	g1.addEdge(5, 3, 3);
	g1.addEdge(5, 4, 4);
	g1.addEdge(6, 3, 10);
	g1.addEdge(6, 7, 7);
	g1.addEdge(6, 2, 12);
	g1.addEdge(7, 6, 7);
	g1.addEdge(7, 2, 16);
	g1.addEdge(7, 1, 9);
	g1.addEdge(7, 4, 2);
	g1.addEdge(7, 3, 6);

	std::vector<uint32_t> d;
	auto m = g1.DijkstraAlogrithm(6,d);
	for (auto i : *m)
	{
		for (auto v : i.second)
		{
			printf("%d ->", v);
		}
		printf("%d\n", i.first + 1);
	}
	printf("\n");
}

void TestReDijkstra()
{
	graph g1(false, 7);

	g1.addEdge(1, 2, 14);
	g1.addEdge(1, 7, 9);
	g1.addEdge(1, 4, 8);
	g1.addEdge(2, 1, 14);
	g1.addEdge(2, 7, 16);
	g1.addEdge(2, 6, 12);
	g1.addEdge(3, 6, 10);
	g1.addEdge(3, 7, 6);
	g1.addEdge(3, 4, 1);
	g1.addEdge(3, 5, 3);
	g1.addEdge(4, 1, 8);
	g1.addEdge(4, 7, 2);
	g1.addEdge(4, 3, 1);
	g1.addEdge(4, 5, 4);
	g1.addEdge(5, 3, 3);
	g1.addEdge(5, 4, 4);
	g1.addEdge(6, 3, 10);
	g1.addEdge(6, 7, 7);
	g1.addEdge(6, 2, 12);
	g1.addEdge(7, 6, 7);
	g1.addEdge(7, 2, 16);
	g1.addEdge(7, 1, 9);
	g1.addEdge(7, 4, 2);
	g1.addEdge(7, 3, 6);

	std::vector<uint32_t> d;
	auto m = g1.DijkstraAlogrithm(6, d,true);
	for (auto i : *m)
	{
		for (auto v : i.second)
		{
			printf("%d ->", v);
		}
		printf("%d\n", i.first + 1);
	}
	printf("\n");
}