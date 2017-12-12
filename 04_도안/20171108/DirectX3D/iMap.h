#pragma once

class iMap
{
public:
	virtual void Load ( char* szFullPath, D3DXMATRIXA16* pmat ) = 0;
	virtual bool GetHeight ( IN const float& x, OUT float& y, IN const float& z ) = 0;
	virtual void Render() = 0;
};

