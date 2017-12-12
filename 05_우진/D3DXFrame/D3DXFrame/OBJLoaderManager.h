#pragma once
#include "singletonBase.h"
#include "OBJLoader.h"

typedef vector<Group*> GROUPLIST;

class OBJLoaderManager : public singletonBase <OBJLoaderManager>
{
public:
	OBJLoaderManager();
	~OBJLoaderManager();

	void release();
	void addOBJFile( char* groupName, char* szFolder, char* szFile );
	void delOBJFile( char* groupName );
	GROUPLIST GetGroup( char* groupName );
	

private:
	map<string, GROUPLIST> groupMapList;
};

