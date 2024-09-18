#pragma once
template<typename T>
class MyIterator
{
public:
	MyIterator(T* ptr, size_t num)
		:m_pOwner(ptr)
		, m_num(num)
	{

	}

	~MyIterator() {}

	bool operator != (MyIterator right) { return m_pOwner == right.m_pOwner && m_num != right.m_num; }
	MyIterator& operator ++ () { ++m_num; return *this; }
	T& operator *() { return m_pOwner[m_num]; }
private:
	T* m_pOwner;
	size_t m_num;
};