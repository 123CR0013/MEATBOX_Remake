#pragma once
#include<vector>
#include"MyIterator.h"

template<typename T>
class MyShallowVector final
{
public:
	MyShallowVector(std::vector<T>* owner, size_t offSet = 0)
		:m_pOwner(owner)
		, m_ptr(owner->data() + offSet)
		, m_size(owner->size() - offSet)
	{

	}
	~MyShallowVector(){}

	T& operator[](size_t num) { return m_ptr[num]; }

	T& front() { return m_ptr[0]; }
	T& back() { return m_ptr[m_size - 1]; }

	MyIterator<T> begin() { return MyIterator<T>(m_ptr, 0); }
	MyIterator<T> end() { return MyIterator<T>(m_ptr, m_size); }

	template <typename S>
	void emplace_back(S value) { m_pOwner->emplace_back(value); ++m_size; }
	template <typename S>
	void push_back(S value) { m_pOwner->push_back(S(value)); ++m_size;}

	bool empty()const { return m_size == 0; }
	size_t size()const { return m_size; }

	std::vector<T>& owner()const { return *m_pOwner; }

private:
	std::vector<T>* m_pOwner;
	T* m_ptr;
	size_t m_size;
};