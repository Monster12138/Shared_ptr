#pragma once

#include <atomic>
//�����ͷ�new�����Ŀռ�
template<class T>
struct DFDef
{
	void operator()(T *& pf)
	{
		delete pf;
		pf = nullptr;
	}
};

//ģ����ػ��������ͷ��ļ���ָ��
template<>
struct DFDef<FILE>
{
	void operator()(FILE *& pf)
	{
		fclose(pf);
		pf = nullptr;
	}
};

//�����ͷ�malloc������Ŀռ�
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
	std::atomic_int *_pcount;	//���ü���
public:
	SharedPtr(T *ptr = nullptr) :_ptr(ptr), _pcount(nullptr)
	{
		if (_ptr)
		{
			_pcount = (std::atomic_int*)new int(1);
		}
	}

	SharedPtr(const SharedPtr<T, DF>& sp):SharedPtr(sp._ptr)	//ί�й���
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
			DF()(_ptr);	//Ӧ�ø���ָ��������ѡ��ͬ���ͷŷ�ʽ
			delete _pcount;
			_pcount = nullptr;
		}
	}

	~SharedPtr()
	{
		Relase();
	}
};