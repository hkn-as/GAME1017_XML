#pragma once
#include "SDL.h"

class Box
{
public:
	Box(const SDL_Rect r);
	void Update();
	void Render();
	void Reset(); 
	void SetData(int x, int y, Uint8 r, Uint8 g, Uint8 b);
	const SDL_Rect& GetRect();
	const SDL_Color& GetColor();

private:
	SDL_Rect m_dst;
	SDL_Rect m_startPos;
	SDL_Color m_color;
};