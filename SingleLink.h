#pragma once
#include <iostream>
#include <memory>

template<class T>
class SingleLinkedList_Iterator;

template<class T>
class ListNode
{
public:
    T m_Value;
    ListNode* m_Next;

public:
    ListNode(T Value = T{}, ListNode* Next = nullptr):
        m_Value(Value),
        m_Next(Next)
    {}

    ~ListNode(){}
};

template<class T>
class SingleLinkedList
{
private:
    ListNode<T>* _Head;
    long _Size;
public:
    SingleLinkedList():
        _Head(nullptr),
        _Size(0)
    {
        _Head = new ListNode<T>();                // ʧ�ܻ����쳣
    }

    ~SingleLinkedList()
    {
        auto p = _Head;                         // ָ��ǰ�ڵ�

        while (p)
        {
            auto t = p->m_Next;                   // �����¸��ڵ�
            delete p;                           // �ͷŵ�ǰ�ڵ� 
            p = t;
        }
    }

    void Insert(int Val)
    {
        // ����ͷ�巨��ÿ�������ڵ㶼�����ײ���ԭ�����ײ������ƶ�
        auto p = new ListNode<T>(Val,_Head);        
        // �²���Ľڵ������ײ�
        _Head = p;

        _InterlockedIncrement(&_Size);
    }

    void Remove(int Val)
    {
        if (_Head->m_Next == nullptr) return;         // û��Ԫ��

        if (_Head->m_Value == Val)
        {
            // ��Ԫ�ؾ���Ҫɾ����Ԫ��
            auto t = _Head->m_Next;
            delete _Head;
            _Head = t;

            _InterlockedDecrement(&_Size);
            return;
        }

        auto p = _Head;
        auto t = p;                             // ������һ���ڵ�
        p = p->m_Next;

        do
        {
            if (p->m_Value == Val)              // �ҵ��˾�ɾ����Ӧ�ڵ�
            {
                t->m_Next = p->m_Next;
                delete p;
                _InterlockedDecrement(&_Size);
                break;
            }

            t = p;
            p = p->m_Next;
        } while (p);
    }

    void Reverse()
    {
        if (_Head->m_Next == NULL || _Head->m_Next->m_Next == NULL) return;        // û��Ԫ�ػ���ֻ��һ��Ԫ�أ�����ν��ת

        auto pre = _Head;
        auto p = _Head->m_Next;
        auto next = p->m_Next;

        while (p->m_Next)
        {
            p->m_Next = pre;
            pre = p;
            p = next;
            next = p->m_Next;
        }

        _Head->m_Next = p;
        _Head = pre;
    }

    SingleLinkedList_Iterator<T> Find(T Value)
    {
        auto p = _Head;

        while (p->m_Next)
        {
            if (p->m_Value == Value) return SingleLinkedList_Iterator<T>(p,_Size);
        }

        return SingleLinkedList_Iterator<T>();
    }

    SingleLinkedList_Iterator<T> Begin()
    {
        return SingleLinkedList_Iterator<T>(_Head, _Size);
    }

    SingleLinkedList_Iterator<T> End()
    {
        auto p = _Head;
        while (p->m_Next) p = p->m_Next;

        return SingleLinkedList_Iterator<T>(p, _Size);
    }

    long Size()
    {
        return _InterlockedCompareExchange(&_Size, 0, 0);
    }

    friend std::ostream& operator<<(std::ostream& os, SingleLinkedList& l)
    {
        for (auto iter = l.Begin(); iter != l.End(); iter++)
        {
            os << iter->m_Value << " ";
        }
        
        return os;
    }

};

template<class T>
class SingleLinkedList_Iterator
{
private:
    ListNode<T>* _Ptr;
    long _Size;
public:
    SingleLinkedList_Iterator(ListNode<T>* Ptr = nullptr, long Size = 0) :
        _Ptr(Ptr),
        _Size(Size)
    {}

    ListNode<T>& operator++()                                   // ǰ��++
    {
        if (_Ptr) _Ptr = _Ptr->m_Next;
        return *this;
    }

    ListNode<T>* Element() const
    {
        return _Ptr;
    }

    const SingleLinkedList_Iterator<T> operator++(int)           // ���غ���++
    {
        SingleLinkedList_Iterator<T> t(_Ptr, _Size);
        if (_Ptr) _Ptr = _Ptr->m_Next;
        return t;
    }

    ListNode<T>& operator*()                                    // ����ָ��*�����
    {
        return *this->_Ptr;
    }

    ListNode<T>* operator->()                                   // ����ָ��->�����    
    {
        return this->_Ptr;
    }

    bool operator==(SingleLinkedList_Iterator<T> Ln)
    {
        return _Ptr == Ln.Element();
    }

    bool operator!=(SingleLinkedList_Iterator<T> Ln)
    {
        return _Ptr != Ln.Element();
    }
};

void Test_SingleList()
{
    try
    {
        SingleLinkedList<int> l;

        l.Insert(10);
        l.Insert(12);
        l.Insert(2);
        l.Insert(4);
        l.Insert(3);
        l.Insert(-1);
        l.Insert(8);

        std::cout << "��ǰ�������ݣ�" << l << "��С��"<< l.Size() << std::endl;

        l.Remove(3);

        std::cout << "ɾ��Ԫ��3����������ݣ�" << l << "��С��" << l.Size() << std::endl;

        l.Reverse();

        std::cout << "������ת������ݣ�" << l << "��С��" << l.Size() << std::endl;


    }
    catch (std::bad_alloc* e)
    {
        std::cout << e->what() << std::endl;
    }
}