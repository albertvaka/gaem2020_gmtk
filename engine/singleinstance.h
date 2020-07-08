#pragma once

#include <cassert>

template <typename T>
class SingleInstance
{
public:
	static T*& instance()
	{
		static T* theOne = nullptr;
		return theOne;
	}
protected:
		SingleInstance()
		{
			instance() = (T*)this;
		}
private:
		SingleInstance(const SingleInstance& other) { }
		void operator=(const SingleInstance& other) { }
};

