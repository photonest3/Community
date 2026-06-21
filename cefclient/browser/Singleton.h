#pragma once

template<class T>
class Singleton {

public:
	static T& Instance()
	{
		if (0 == _instance)
		{
			_instance = new T();
			atexit(Destroy);
		}
		return *_instance;
	}

protected:
	Singleton() {}
	~Singleton() {}

private:
	static void Destroy()
	{
		if (_instance != 0)
		{
			delete _instance;
			_instance = 0;
		}
	}

	static T * volatile _instance; // The one and only instance
};

template<class T>
T * volatile Singleton<T>::_instance = 0;

