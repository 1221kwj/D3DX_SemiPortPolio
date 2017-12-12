#pragma once

enum
{
	BUTTON1,
	BUTTON2,
	TEXTVIEW
};

class UIManager : public iUIButtonDelegate
{
public:
	UIManager();
	~UIManager();

	void Init();
	void Update();
	void Render();
	void Destroy();


private:
	UIObject*				uiRoot = nullptr;
	LPD3DXSPRITE			sprite = nullptr;

	// iUIButtonDelegate override
	virtual void OnClick( UIButton* pSender ) override;


};

