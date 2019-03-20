#pragma once

#include <atomic>
//用来释放new出来的空间
template<class T>
struct DFDef
{
	void operator()(T *& pf)
	{
		delete pf;
		pf = nullptr;
	}
};

//模板的特化，用来释放文件流指针
template<>
struct DFDef<FILE>
{
	void operator()(FILE *& pf)
	{
		fclose(pf);
		pf = nullptr;
	}
};

//用来释放malloc族申请的空间
struct Free
{
	void operator()(void * pf)
	{
		free(pf);
		pf = nullptr;
	}
};


template<class T, class DF = DFDef<T>>
class SharedPtr
{
private:
	T *_ptr;
	std::atomic_int *_pcount;	//引用计数
public:
	SharedPtr(T *ptr = nullptr) :_ptr(ptr), _pcount(nullptr)
	{
		if (_ptr)
		{
			_pcount = (std::atomic_int*)new int(1);
		}
	}

	SharedPtr(const SharedPtr<T, DF>& sp):SharedPtr(sp._ptr)	//委托构造
	{
		_pcount = sp._pcount;
		++(*_pcount);
	}

	SharedPtr<T, DF>& operator=(const SharedPtr<T, DF>& sp)
	{
		if (this != &sp)
		{
			Relase();
			_ptr = sp._ptr;
			_pcount = sp._pcount;
			++(*_pcount);
		}
		return *this;
	}

	T* operator->()
	{
		return _ptr;
	}

	T* operator*()
	{
		return _ptr;
	}

	int Getuse()
	{
		assert(_pcount != nullptr);
		return *_pcount;
	}

	void Relase()
	{
		if (_ptr && 0 == --(*_pcount))
		{
			DF()(_ptr);	//应该根据指针类型来选择不同的释放方式
			delete _pcount;
			_pcount = nullptr;
		}
	}

	~SharedPtr()
	{
		Relase();
	}
};