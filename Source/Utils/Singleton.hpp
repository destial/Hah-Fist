#pragma once
#ifndef __SINGLETON_H
#define __SINGLETON_H

template <typename N>
class Singleton {
protected:
	static N* instance;
public:
	static N* GetInstance();
	static void Free();
};

template<typename N>
N* Singleton<N>::instance = nullptr;

template<typename N>
N* Singleton<N>::GetInstance() {
	if (instance == nullptr) {
		instance = new N;
	}
	return instance;
}

template<typename N>
void Singleton<N>::Free() {
	if (instance) {
		delete instance;
	}
	instance = nullptr;
}

#endif