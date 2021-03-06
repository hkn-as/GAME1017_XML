#include "Engine.h"
#include "CollisionManager.h"
#include "DebugManager.h"
#include "EventManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include <ctime>
#include <iostream>
#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
using namespace std;

Engine::Engine():m_running(false){ cout << "Engine class constructed!" << endl; }

bool Engine::Init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	cout << "Initializing game..." << endl;
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				EVMA::Init();
				SOMA::Init();
				TEMA::Init();
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	// Example specific initialization.
	srand((unsigned)time(NULL));
	xmlDoc.LoadFile("boxes.xml");
	m_boxes.reserve(4);
	m_boxes.push_back(new Box({ 100, 100, 100, 100 }));
	m_boxes.push_back(new Box({ 824, 100, 100, 100 }));
	m_boxes.push_back(new Box({ 100, 568, 100, 100 }));
	m_boxes.push_back(new Box({ 824, 568, 100, 100 }));
	XMLNode* pRoot = xmlDoc.FirstChildElement("Root");
	XMLElement* pElement = pRoot->FirstChildElement("Box");
	int i = 0;
	while (pElement != nullptr)
	{
		int x, y, r, g, b;
		pElement->QueryIntAttribute("x", &x);
		pElement->QueryIntAttribute("y", &y);
		pElement->QueryIntAttribute("r", &r);
		pElement->QueryIntAttribute("g", &g);
		pElement->QueryIntAttribute("b", &b);
		m_boxes[i++]->SetData(x, y, (Uint8)r, (Uint8)g, (Uint8)b);
		pElement = pElement->NextSiblingElement("Box");
	}
	// Final engine initialization calls.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	m_running = true; // Everything is okay, start the engine.
	cout << "Engine Init success!" << endl;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Engine::HandleEvents()
{
	EVMA::HandleEvents();
}

void Engine::Update()
{
	// Parse example-specific input.
	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE))
		for (unsigned i = 0; i < m_boxes.size(); i++)
			m_boxes[i]->Reset();
	// Update the boxes.
	for (unsigned i = 0; i < m_boxes.size(); i++)
		m_boxes[i]->Update();
}

void Engine::Render() 
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Draw anew.
	for (unsigned i = 0; i < m_boxes.size(); i++)
		m_boxes[i]->Render();
	// Flip buffers.
	SDL_RenderPresent(m_pRenderer);
}

void Engine::Clean()
{
	cout << "Cleaning game." << endl;
	// Write XML.
	XMLElement* pElement = xmlDoc.FirstChildElement("Root")->FirstChildElement("Box");
	int i = 0;
	while (pElement != nullptr)
	{
		pElement->SetAttribute("x", m_boxes[i]->GetRect().x);
		pElement->SetAttribute("y", m_boxes[i]->GetRect().y);
		pElement->SetAttribute("r", m_boxes[i]->GetColor().r);
		pElement->SetAttribute("g", m_boxes[i]->GetColor().g);
		pElement->SetAttribute("b", m_boxes[i++]->GetColor().b);
		pElement = pElement->NextSiblingElement("Box");
	}
	xmlDoc.SaveFile("boxes.xml");
	// Example-specific cleaning.
	for (unsigned i = 0; i < m_boxes.size(); i++)
	{
		delete m_boxes[i];
		m_boxes[i] = nullptr; // Wrangle your dangle.
	}
	m_boxes.clear(); // capacity() is still 4, but size() is 0.
	m_boxes.shrink_to_fit(); // shrinks capacity() to size()
	// Finish cleaning.
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	DEMA::Quit();
	EVMA::Quit();
	FOMA::Quit();
	SOMA::Quit();
	STMA::Quit();
	TEMA::Quit();
	IMG_Quit();
	SDL_Quit();
}

int Engine::Run()
{
	if (m_running) // What does this do and what can it prevent?
		return -1; 
	if (Init("GAME1017 Engine Template", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;
	while (m_running) // Main engine loop.
	{
		Wake();
		HandleEvents();
		Update();
		Render();
		if (m_running)
			Sleep();
	}
	Clean();
	return 0;
}

Engine& Engine::Instance()
{
	static Engine instance; // C++11 will prevent this line from running more than once. Magic statics.
	return instance;
}

SDL_Renderer* Engine::GetRenderer() { return m_pRenderer; }
bool& Engine::Running() { return m_running; }
