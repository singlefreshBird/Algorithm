#include "RAG.h"
#include <assert.h>

RAG::RAG(int vertext_num):
    m_VertextCount(vertext_num),
    m_AdjMatrix(nullptr)
{
    m_AdjMatrix = new unsigned char[m_VertextCount * m_VertextCount];
    memset(m_AdjMatrix, 0, sizeof(uint8_t) * m_VertextCount * m_VertextCount);
}

RAG::RAG(const RAG& g)
{
    m_VertextCount = g.m_VertextCount;
    m_AdjMatrix = new unsigned char[m_VertextCount * m_VertextCount];
    memcpy(m_AdjMatrix, g.m_AdjMatrix, sizeof(unsigned char) * m_VertextCount * m_VertextCount);
    m_RVertexs = g.m_RVertexs;
    m_PVertexs = g.m_PVertexs;
}

RAG::RAG(RAG&& g) noexcept
{
    m_VertextCount = g.m_VertextCount;
    m_AdjMatrix = g.m_AdjMatrix;
    g.m_AdjMatrix = nullptr;
    m_RVertexs = std::move(g.m_RVertexs);
    m_PVertexs = std::move(g.m_PVertexs);
}

RAG::~RAG()
{
    delete[] m_AdjMatrix;
}


auto RAG::addEdge(const Vertex& v1, const Vertex& v2) ->void
{
    auto id1 = v1.getId();
    auto id2 = v2.getId();

    if ((id1 >= 0 && id1 < m_VertextCount) && (id2 >= 0 && id2 < m_VertextCount) && (v1.getType() != v2.getType()))
    {
        // 更新邻接矩阵的状态
        m_AdjMatrix[id1 * m_VertextCount + id2] += 1;

        if (v1.getType() == VertexType::VT_Process)
        {   
            auto fpv = m_PVertexs.find(id1);
            if (fpv == m_PVertexs.end())
            {
                Vertex pv(v1);

                pv.m_RequestRv.insert(id2);
                m_PVertexs.emplace(std::pair<int, Vertex>(id1, pv));
                Explorer_.insert(id1);
            }
            else
            {
                fpv->second.m_RequestRv.insert(id2);
            }
            
            // v1如果是进程节点，那么v2就一定是资源节点，需要将v2加入到资源节点集合里
            m_RVertexs.insert(std::pair<int, Vertex>(id2, v2));
        }
        else
        {
            /*
             * 资源节点分配给进程，需要将v1插入到资源节点，并递减资源的空闲数目
             */
            auto frv = m_RVertexs.find(id1);
            if (frv == m_RVertexs.end())
            {
                Vertex rv(v1);

                rv.setFreeCount(rv.getFreeCount() - 1);
                m_RVertexs.emplace(std::pair<int, Vertex>(id1, rv));
            }
            else
            {
                frv->second.setFreeCount(frv->second.getFreeCount() - 1);
            }

            // v1如果是资源节点，那么v2就一定是进程节点，需要将v2加入到进程节点集合里
            m_PVertexs.insert(std::pair<int, Vertex>(id2, v2));
            Explorer_.insert(id2);

            // 将资源节点加入到进程节点的所属集合里
            auto fpv = m_PVertexs.find(id2);
            assert(fpv != m_PVertexs.end());
            fpv->second.m_OwnedRv.insert(id1);
        }
    }
}

auto RAG::removeEdge(const Vertex& v1, const Vertex& v2) ->void
{
    auto id1 = v1.getId();
    auto id2 = v2.getId();

    if ((id1 >= 0 && id1 < m_VertextCount) && (id2 >= 0 && id2 < m_VertextCount) && (v1.getType() != v2.getType()))
    {
        // 更新邻接矩阵的状态
        if(m_AdjMatrix[id1 * m_VertextCount + id2] > 0) m_AdjMatrix[id1 * m_VertextCount + id2] -= 1;

        if (v1.getType() == VertexType::VT_Process)
        {
            // 如果v1是进程节点，则删掉对应请求
            auto fpv = m_PVertexs.find(id1);
            assert(fpv != m_PVertexs.end());
            fpv->second.m_RequestRv.erase(id2);
        }
        else
        {
            // 如果v1是资源节点，需要递增空闲资源数
            auto frv = m_RVertexs.find(id1);
            assert(frv != m_RVertexs.end());
            frv->second.setFreeCount(frv->second.getFreeCount() + 1);
        }
    }
}

// backup如果为true表示备份数据，否则该函数会修改RAG内部的状态，并不可恢复，导致rag其它方法失效。
auto RAG::hasCycl(bool backup/* = true*/) ->bool
{
    /*
     * 采用算法思想是尝试化简RAG，直到不能再化简的时候，如果此时RAG中还存在边则判定存在死锁
     */
    
    // 满足数
    int iSatisfy;
    auto bRetry = true;
    // 临时变量，防止更改原来数据
    std::unordered_set<int> sTrace(Explorer_);

    if (backup)
    {
        for (auto rv : m_RVertexs) rv.second.stash();
    }

    while (!sTrace.empty() && bRetry)
    {
        auto prevSize = sTrace.size();
        // S1.查看都有哪些进程在申请资源，找出哪些进程可以被满足
        for (auto id_iter = sTrace.begin();  id_iter != sTrace.end(); id_iter++)
        {
            iSatisfy = 0;
            auto id1 = *id_iter;
            const auto& pv = m_PVertexs.find(id1);
            assert(pv != m_PVertexs.end());

            for (auto id2 : pv->second.m_RequestRv)
            {
                const auto& rv = m_RVertexs.find(id2);
                assert(rv != m_RVertexs.end());

                if (rv->second.getFreeCount() >= m_AdjMatrix[id1 * m_VertextCount + id2])
                {
                    // 如果空闲资源能够满足申请的数量，则递增iSatisfy
                    iSatisfy += 1;
                }
            }

            // S2.如果该进程节点所有的请求都能满足，则可以删除该进程节点与相关资源节点的所有边
            if (iSatisfy == pv->second.m_RequestRv.size())
            {
                sTrace.erase(id_iter);
                // 删除set容器元素后会导致原来的迭代器失效，所以这里要重新获取迭代器
                id_iter = sTrace.begin();
                // 这里是递增资源空闲树来实现逻辑上的删除边

                for (auto id2 : pv->second.m_OwnedRv)
                {
                    const auto& rv = m_RVertexs.find(id2);
                    assert(rv != m_RVertexs.end());

                    rv->second.setFreeCount(rv->second.getFreeCount() + m_AdjMatrix[id2 * m_VertextCount + id1]);
                }
            }

            // 如果set容器已经为空就直接跳出循环，否则迭代器无效，避免出错和多余的循环
            if (sTrace.empty()) break;
        }

        bRetry = prevSize > sTrace.size(); // 如果每一轮探索都能消除一些进程节点，则代表能够继续化简。
    }

    if (backup)
    {
        for (auto rv : m_RVertexs) rv.second.revert();
    }

    // 化简结束后，如果待探索的进程节点为空，则说明无死锁
    return !sTrace.empty();
}

auto RAG::simplify()->RAG
{
    // 备份关键数据
    for (auto rv : m_RVertexs) rv.second.stash();

    // 满足数
    int iSatisfy;
    auto bRetry = true;
    // 临时变量，防止更改原来数据
    std::unordered_set<int> sTrace(Explorer_);

    while (!sTrace.empty() && bRetry)
    {
        auto prevSize = sTrace.size();
        // S1.查看都有哪些进程在申请资源，找出哪些进程可以被满足
        for (auto id_iter = sTrace.begin(); id_iter != sTrace.end(); id_iter++)
        {
            iSatisfy = 0;
            auto id1 = *id_iter;
            const auto& pv = m_PVertexs.find(id1);
            assert(pv != m_PVertexs.end());

            for (auto id2 : pv->second.m_OwnedRv)
            {
                const auto& rv = m_RVertexs.find(id2);
                assert(rv != m_RVertexs.end());

                if (rv->second.getFreeCount() >= m_AdjMatrix[id1 * m_VertextCount + id2])
                {
                    // 如果空闲资源能够满足申请的数量，则递增iSatisfy
                    iSatisfy += 1;
                }
            }

            // S2.如果该进程节点所有的请求都能满足，则可以删除该进程节点与相关资源节点的所有边
            if (iSatisfy == pv->second.m_OwnedRv.size())
            {
                sTrace.erase(id_iter);
                // 删除set容器元素后会导致原来的迭代器失效，所以这里要重新获取迭代器
                id_iter = sTrace.begin();
                // 这里是递增资源空闲树来实现逻辑上的删除边

                for (auto id2 : pv->second.m_OwnedRv)
                {
                    const auto& rv = m_RVertexs.find(id2);
                    assert(rv != m_RVertexs.end());

                    rv->second.setFreeCount(rv->second.getFreeCount() + m_AdjMatrix[id2 * m_VertextCount + id1]);
                }
            }

            // 如果set容器已经为空就直接跳出循环，否则迭代器无效，避免出错和多余的循环
            if (sTrace.empty()) break;
        }

        bRetry = prevSize > sTrace.size(); // 如果每一轮探索都能消除一些进程节点，则代表能够继续化简。
    }

    // 构造新的RAG对象
    RAG rag(m_VertextCount);

    // 构建新rag对象的邻接矩阵
    if (sTrace.empty())
    {
        for (const auto& r : m_RVertexs) rag.m_RVertexs.insert(std::pair<int, Vertex>(r.first, r.second));
        for (const auto& p : m_PVertexs) rag.m_PVertexs.insert(std::pair<int, Vertex>(p.first, p.second));
    }
    else
    {
        for (const auto& id1 : sTrace)
        {
            auto fpv = m_PVertexs.find(id1);
            assert(fpv != m_PVertexs.end());

            for (const auto& id2 : fpv->second.m_OwnedRv)
            {
                // 构建资源节点到进程节点方向的邻接表
                rag.m_AdjMatrix[id2 * rag.m_VertextCount + id1] = m_AdjMatrix[id2 * rag.m_VertextCount + id1];

                const auto& frv = m_RVertexs.find(id2);
                assert(frv != m_RVertexs.end());
                // 插入进程顶点和资源顶点
                rag.m_PVertexs.insert(std::pair<int, Vertex>(id1, fpv->second));
                rag.m_RVertexs.insert(std::pair<int, Vertex>(id2, frv->second));
            }

            for (const auto& id2 : fpv->second.m_RequestRv)
            {
                // 构建进程节点到资源节点方向的邻接表
                rag.m_AdjMatrix[id1 * rag.m_VertextCount + id2] = m_AdjMatrix[id1 * rag.m_VertextCount + id2];

                const auto& frv = m_RVertexs.find(id2);
                assert(frv != m_RVertexs.end());
                // 插入进程顶点和资源顶点
                rag.m_PVertexs.insert(std::pair<int, Vertex>(id1, fpv->second));
                rag.m_RVertexs.insert(std::pair<int, Vertex>(id2, frv->second));
            }
        }
    }
    

    // 恢复关键数据
    for (auto rv : m_RVertexs) rv.second.revert();

    return rag;
}
