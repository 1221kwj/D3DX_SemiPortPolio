#pragma once
#include "singletonBase.h"
#include <map>

class gameNode;

class sceneManager : public singletonBase <sceneManager>
{
public:
	typedef map<string, gameNode*> mapSceneList;
	typedef map<string, gameNode*>::iterator mapSceneIter;

private:
	static gameNode* _currentScene;
	static gameNode* _loadingScene;
	static gameNode* _readyScene;

	mapSceneList _mSceneList;
	mapSceneList _mLoadingSceneList;

	DWORD _loadingThreadID;

public:
	sceneManager();
	~sceneManager();

	HRESULT init( void );
	void release( void );
	void update( void );
	void render( void );

	//�� �߰�
	gameNode* addScene( string sceneName, gameNode* scene );

	//�ε� �� �߰�
	gameNode* addLoadingScene( string loadingSceneName, gameNode* scene );

	HRESULT changeScene( string sceneName );
	HRESULT changeScene( string sceneName, string loadingSceneName );

	friend DWORD CALLBACK loadingThread( LPVOID prc );
};

