#pragma once

#include <mutex>
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
struct DFDef<FILE*>
{
	void operator()(FILE *& pf)
	{
		delete pf;
		pf = nullptr;
	}
};

//ģ����ػ��������ͷ�malloc������Ŀռ�
template<>
struct DFDef<void*>
{
	void operator()(void *& pf)
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
	int *_pcount;	//���ü���
	std::mutex _mutex;	//��֤��_pcount��1�������̰߳�ȫ������֤��_ptr�������̰߳�ȫ��
public:
	SharedPtr(T *ptr = nullptr) :_ptr(ptr), _pcount(nullptr)
	{
		if (_ptr)
		{
			_pcount = new int(1);
		}
	}

	SharedPtr(const SharedPtr<T, DF>& sp):SharedPtr(sp._ptr)	//ί�й���
	{
		_pcount = sp._pcount;
		SafeAddCount();
	}

	SharedPtr<T, DF>& operator=(const SharedPtr<T, DF>& sp)
	{
		if (this != &sp)
		{
			Relase();
			_ptr = sp._ptr;
			_pcount = sp._pcount;
			SafeAddCount();
		}
		return *this;
	}

	void SafeAddCount()
	{
		_mutex.lock();
		++(*_pcount);
		_mutex.unlock();
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