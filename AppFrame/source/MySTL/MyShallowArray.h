#pragma once
#include"MyIterator.h"

template <typename T>
class MyShallowArray
{
public:
	MyShallowArray(size_t size)
		:m_size(size)
	{
		m_ptr = new T[size];
	}
	MyShallowArray(T* ptr, size_t size)
		:m_ptr(ptr)
		,m_size(size)
	{

	}

	~MyShallowArray()
	{
	}

	T& operator[](size_t num) { return m_ptr[num]; }

	T& front()const { m_ptr[0]; }
	T& back()const { m_ptr[m_size - 1]; }

	MyIterator<T> begin() { return MyIterator<T>(m_ptr, 0); }
	MyIterator<T> end() { return MyIterator<T>(m_ptr, m_size); }

	bool empty()const { return m_size == 0; }
	size_t size()const { return m_size; }
private:
	T* m_ptr;				//配列の先頭
	size_t m_size;			//配列サイズ
};

