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
    // ���̽ڵ���ӵ�е���Դ
    std::unordered_set<int> m_OwnedRv;
    // �����������Դ
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
// �Զ���vertex��hash����
struct hash_vertex
{
    auto operator()(const Vertex& v1)const->size_t
    {
        std::hash<int> hs;
        return hs(v1.getId());
    }
};
// �Զ���vertex�ıȽϹ���
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
    // ����hashCycl�����У���Ϊ��ʱ����
    std::unordered_set<int> Explorer_;
protected:
    // �������
    int m_VertextCount;
    unsigned char* m_AdjMatrix;
    // ��Դ�Ͷ��㼯��
    std::unordered_map<int, Vertex> m_RVertexs;
    // ������Դ�͵Ķ��㼯��
    std::unordered_map<int, Vertex> m_PVertexs;
public:
    RAG(int vertext_num);
    // ��������
    RAG(const RAG& g);
    // �ƶ�����
    RAG(RAG&& g) noexcept;
    ~RAG();
     //��Ӷ���
    auto addEdge(const Vertex& v1, const Vertex& v2)->void;
    // �Ƴ�����
    auto removeEdge(const Vertex& v1, const Vertex& v2)->void;
    // ����Ƿ���ڻ�
    auto hasCycl(bool backup = true)->bool;
    // ��ȡ��������Դ����ͼ
    auto simplify()->RAG;
};