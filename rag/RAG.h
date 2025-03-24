#pragma once
//#include "graph.hpp"
#include <unordered_set>
#include <unordered_map>
#include <memory>

struct hash_vertex;
struct equal_to_vertex;

enum class VertexType
{
    VT_Process = 0,
    VT_Resource
};

class Vertex
{
private:
    int FreeCountBackup_;
protected:
    VertexType m_Type;
    int m_Id;
    int m_FreeCount;
public:
    // 进程节点所拥有的资源
    std::unordered_set<int> m_OwnedRv;
    // 进程请求的资源
    std::unordered_set<int> m_RequestRv;

    Vertex(int Id, int Count, VertexType Type) :
        FreeCountBackup_(0),
        m_Type(Type),
        m_Id(Id),
        m_FreeCount(Count)
    {}

    Vertex(const Vertex& v)
    {
        FreeCountBackup_ = v.FreeCountBackup_;
        m_Type = v.m_Type;
        m_FreeCount = v.m_FreeCount;
        m_Id = v.m_Id;
        m_OwnedRv = v.m_OwnedRv;
        m_RequestRv = v.m_RequestRv;
    }

    ~Vertex() {}

    auto stash()->void { FreeCountBackup_ = m_FreeCount; }
    auto revert()->void { m_FreeCount = FreeCountBackup_; }
    auto setId(int Id)->void { m_Id = Id; }
    auto getId()const->int { return m_Id; }
    auto getType()const->VertexType { return m_Type; }
    auto setFreeCount(int Count)->void { m_FreeCount = Count; }
    auto getFreeCount()const->int { return m_FreeCount; }
};
// 自定义vertex的hash规则
struct hash_vertex
{
    auto operator()(const Vertex& v1)const->size_t
    {
        std::hash<int> hs;
        return hs(v1.getId());
    }
};
// 自定义vertex的比较规则
struct equal_to_vertex
{
    auto operator()(const Vertex& v1, const Vertex& v2)const->bool
    {
        return v1.getId() == v2.getId();
    }
}; 

class RAG
{
private:
    // 用于hashCycl函数中，作为临时容器
    std::unordered_set<int> Explorer_;
protected:
    // 顶点个数
    int m_VertextCount;
    unsigned char* m_AdjMatrix;
    // 资源型顶点集合
    std::unordered_map<int, Vertex> m_RVertexs;
    // 消耗资源型的顶点集合
    std::unordered_map<int, Vertex> m_PVertexs;
public:
    RAG(int vertext_num);
    // 拷贝构造
    RAG(const RAG& g);
    // 移动构造
    RAG(RAG&& g) noexcept;
    ~RAG();
     //添加顶点
    auto addEdge(const Vertex& v1, const Vertex& v2)->void;
    // 移除顶点
    auto removeEdge(const Vertex& v1, const Vertex& v2)->void;
    // 检测是否存在环
    auto hasCycl(bool backup = true)->bool;
    // 获取化简后的资源分配图
    auto simplify()->RAG;
};