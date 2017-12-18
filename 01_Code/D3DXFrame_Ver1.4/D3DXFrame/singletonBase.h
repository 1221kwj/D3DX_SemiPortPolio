#pragma once

template <typename T> class singletonBase
{
protected:
	static T* singleton;					//�̱��� �ν��Ͻ� ����

	singletonBase( void ) {};
	~singletonBase( void ) {};

public:
	static T* getSingleton( void );			//�̱��� ������
	void releaseSingleton( void );			//�̱��� ���� �Լ�
};

//�̱��� �ʱ�ȭ
template <typename T> T* singletonBase<T>::singleton = 0;

//�̱��� �ҷ����� �Լ�
template <typename T> T* singletonBase<T>::getSingleton( void )
{
	if ( !singleton ) singleton = new T;	//���࿡ �̱����� ���ٸ� -> �̱��� ����
	return singleton;						//�׸��� ��ȯ
}

//�̱��� �޸� ����
template <typename T> void singletonBase<T>::releaseSingleton( void )
{
	if ( singleton )
	{
		delete singleton;
		singleton = 0;
	}
}