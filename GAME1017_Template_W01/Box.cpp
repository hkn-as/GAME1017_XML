#include "Box.h"
#include "Engine.h"
#include "EventManager.h"
#include <cstdlib>

Box::Box(const SDL_Rect r):m_dst(r), m_startPos(r), m_color({ 255, 255, 255, 128 })
{
	/*m_dst = m_startPos = r;
	m_color = { 255, 255, 255, 255 };*/
}

void Box::Update()
{
	if (SDL_PointInRect(&EVMA::GetMousePos(), &m_dst))
	{
		if (EVMA::MouseHeld(1))
		{
			m_dst.x = EVMA::GetMousePos().x - m_dst.w/2;
			m_dst.y = EVMA::GetMousePos().y - m_dst.h/2;
		}
		if (EVMA::MousePressed(3)) // Try MouseHeld(3) instead!
		{
			m_color.r = rand() % 256;
			m_color.g = rand() % 256;
			m_color.b = rand() % 256;
		}
	}
}

void Box::Render()
{
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &m_dst);
}

void Box::Reset()
{
	m_dst = m_startPos;
	m_color = { 255,255,255,255 };
}

void Box::SetData(int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	m_dst.x = x;
	m_dst.y = y;
	m_color = { r, g, b, 255 };
}

const SDL_Rect& Box::GetRect() { return m_dst; }
const SDL_Color& Box::GetColor() { return m_color; }


/* Raw check for point in rect:

if (mx < (m_rDst.x + m_rDst.w) && mx > m_rDst.x && // If cursor is within X bounds
	my < (m_rDst.y + m_rDst.h) && my > m_rDst.y)   // And cursor is within Y bounds
	
*/