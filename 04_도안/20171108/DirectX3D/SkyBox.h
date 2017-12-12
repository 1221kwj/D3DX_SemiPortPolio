#pragma once
class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	void Init();
	void Render();

private:
	std::vector<VertexPT> skyBox;
};

