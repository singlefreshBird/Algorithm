#include <iostream>
#include "RAG.h"

void TestRAG1()
{
    Vertex p1(0, 1, VertexType::VT_Process);           
    Vertex p2(1, 1, VertexType::VT_Process);           
    Vertex r1(2, 3, VertexType::VT_Resource);          
    Vertex r2(3, 2, VertexType::VT_Resource);          
    
    RAG rag(4);

    rag.addEdge(p1, r2);
    rag.addEdge(r2, p2);
    rag.addEdge(p2, r1);
    rag.addEdge(r1, p1);
    rag.addEdge(r1, p1);
    rag.addEdge(r1, p2);

    if (rag.hasCycl()) std::cout << "TestRAG1(): Death lock has been detected!" << std::endl;
    else std::cout << "TestRAG1(): Pass!" << std::endl;
}

void TestRAG2()
{
    Vertex p0(0, 1, VertexType::VT_Process);
    Vertex p1(1, 1, VertexType::VT_Process);
    Vertex p2(2, 1, VertexType::VT_Process);
    Vertex p3(3, 1, VertexType::VT_Process);
    Vertex p4(4, 1, VertexType::VT_Process);
    Vertex r0(5, 2, VertexType::VT_Resource);
    Vertex r1(6, 1, VertexType::VT_Resource);
    Vertex r2(7, 2, VertexType::VT_Resource);
    Vertex r3(8, 1, VertexType::VT_Resource);
    Vertex r4(9, 1, VertexType::VT_Resource);

    RAG rag(10);

    rag.addEdge(p0, r1);
    rag.addEdge(p0, r2);
    rag.addEdge(p1, r4);
    rag.addEdge(p3, r3);
    rag.addEdge(p4, r3);

    rag.addEdge(r0, p0);
    rag.addEdge(r0, p1);
    rag.addEdge(r1, p2);
    rag.addEdge(r2, p2);
    rag.addEdge(r2, p3);
    rag.addEdge(r3, p1);
    rag.addEdge(r4, p4);

    if (rag.hasCycl()) std::cout << "TestRAG2(): Death lock has been detected!" << std::endl;
    else std::cout << "TestRAG2(): Pass!" << std::endl;
}

void TestRAG3()
{
    Vertex p1(0, 1, VertexType::VT_Process);
    Vertex p2(1, 1, VertexType::VT_Process);
    Vertex p3(2, 1, VertexType::VT_Process);
    Vertex r1(3, 1, VertexType::VT_Resource);
    Vertex r2(4, 2, VertexType::VT_Resource);
    Vertex r3(5, 2, VertexType::VT_Resource);
    Vertex r4(6, 1, VertexType::VT_Resource);

    RAG rag(7);

    rag.addEdge(p1, r1);
    rag.addEdge(p2, r4);
    rag.addEdge(p3, r3);
    rag.addEdge(p3, r2);

    rag.addEdge(r1, p2);
    rag.addEdge(r2, p1);
    rag.addEdge(r2, p3);
    rag.addEdge(r3, p2);
    rag.addEdge(r4, p3);

    if (rag.hasCycl()) std::cout << "TestRAG3(): Death lock has been detected!" << std::endl;
    else std::cout << "TestRAG3(): Pass!" << std::endl;
}

void TestRAG4()
{
    Vertex p1(0, 1, VertexType::VT_Process);
    Vertex p2(1, 1, VertexType::VT_Process);
    Vertex p3(2, 1, VertexType::VT_Process);
    Vertex p4(3, 1, VertexType::VT_Process);
    Vertex r1(4, 1, VertexType::VT_Resource);
    Vertex r2(5, 1, VertexType::VT_Resource);
    Vertex r3(6, 1, VertexType::VT_Resource);

    RAG rag(7);

    rag.addEdge(p1, r2);
    rag.addEdge(p1, r1);
    rag.addEdge(p2, r2);
    rag.addEdge(p2, r3);
    rag.addEdge(p3, r1);
    rag.addEdge(p3, r3);
    rag.addEdge(p4, r2);

    if (rag.hasCycl()) std::cout << "TestRAG4(): Death lock has been detected!" << std::endl;
    else std::cout << "TestRAG4(): Pass!" << std::endl;
}

void TestSimplify()
{
    Vertex p0(0, 1, VertexType::VT_Process);
    Vertex p1(1, 1, VertexType::VT_Process);
    Vertex p2(2, 1, VertexType::VT_Process);
    Vertex p3(3, 1, VertexType::VT_Process);
    Vertex p4(4, 1, VertexType::VT_Process);
    Vertex r0(5, 2, VertexType::VT_Resource);
    Vertex r1(6, 1, VertexType::VT_Resource);
    Vertex r2(7, 2, VertexType::VT_Resource);
    Vertex r3(8, 1, VertexType::VT_Resource);
    Vertex r4(9, 1, VertexType::VT_Resource);

    RAG rag(10);

    rag.addEdge(p0, r1);
    rag.addEdge(p0, r2);
    rag.addEdge(p1, r4);
    rag.addEdge(p3, r3);
    rag.addEdge(p4, r3);

    rag.addEdge(r0, p0);
    rag.addEdge(r0, p1);
    rag.addEdge(r1, p2);
    rag.addEdge(r2, p2);
    rag.addEdge(r2, p3);
    rag.addEdge(r3, p1);
    rag.addEdge(r4, p4);

    auto simplyGraph = rag.simplify();

    std::cout << "ok!" << std::endl;
}

int main(_In_ int argc, _In_reads_(argc) _Pre_z_ char** argv, _In_z_ char** envp)
{
    TestRAG1();
    TestRAG2();
    TestRAG3();
    TestRAG4();
    TestSimplify();
    system("pause");
    return 0;
}