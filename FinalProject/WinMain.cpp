#include "TileMap.h"
#include <XEngine.h>
#include <ImGui/Inc/imgui.h>

#include <AI.h>
#include "Gatherer.h"
#include "Hunter.h"
#include "Resource.h"

using namespace AI;

TileMap tileMap;
X::Math::Vector3 cameraPos = X::Math::Vector3::Zero();
std::vector<std::unique_ptr<Resource>> resources;

float camSpeed = 90.0f;

bool showDebug = false;
float wanderJitter = 5.0f;
float wanderRadius = 20.0f;
float wanderDistance = 50.0f;
float radius = 50.0f;

int gathererFood = 0;
int hunterFood = 0;

float eatTimerMax = 60.0f;
float eatTimerCurrent = 60.0f;

float resourceGrowTimerMax = 15.0f;
float resourceGrowTimerCurrent = 0.0f;

AIWorld aiWorld;
std::vector<std::unique_ptr<Gatherer>> gathererAgents;
std::vector<std::unique_ptr<Hunter>> hunterAgents;


void SpawnGatherer()
{
	auto& agent = gathererAgents.emplace_back(std::make_unique<Gatherer>(aiWorld));
	agent->SetTileMap(&tileMap);
	agent->Load();
	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	agent->position = agent->GetGathererHome();
	agent->radius = radius;
	agent->ShowDebug(showDebug);
	agent->InitializeStates();
	agent->GoToGather();
}
void KillGatherer()
{
	auto& agent = gathererAgents.front();
	agent->Unload();
	gathererAgents.erase(gathererAgents.begin());
}

void SpawnHunter()
{
	auto& agent = hunterAgents.emplace_back(std::make_unique<Hunter>(aiWorld));
	agent->SetTileMap(&tileMap);
	agent->Load();
	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	agent->position = agent->GetHunterHome();
	agent->radius = radius;
	agent->ShowDebug(showDebug);
	agent->InitializeStates();
	agent->GoToHunt();
}
void KillHunter()
{
	auto& agent = hunterAgents.front();
	agent->Unload();
	hunterAgents.erase(hunterAgents.begin());
}

void SpawnResource()
{
	auto& resource = resources.emplace_back(std::make_unique<Resource>(aiWorld));
	resource->Initialize();
}

//--------------------------------------------------
void GameInit()
{
	tileMap.LoadTiles("tiles.txt");
	tileMap.LoadMap("map.txt");

	for (uint32_t i = 0; i < 10; ++i)
	{
		SpawnResource();
	}

	SpawnGatherer();
	SpawnGatherer();
}

bool GameLoop(float deltaTime)
{
	//==============ImGui================
	ImGui::Begin("PathFinding", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		const int columns = tileMap.GetColumns();
		const int rows = tileMap.GetRows();

		if (ImGui::Button("SpawnHunter"))
		{
			SpawnHunter();
		}
		ImGui::SameLine();
		if (ImGui::Button("KillHunter"))
		{
			KillHunter();
		}

		ImGui::Text("EatTimer: %.2f", eatTimerCurrent);
		ImGui::Text("GathererFood: %i", gathererFood);

		if (ImGui::Checkbox("ShowDebug", &showDebug))
		{
			for (auto& agent : gathererAgents)
			{
				agent->ShowDebug(showDebug);
				agent->SetShowDebug(showDebug);
				tileMap.SetShowDebug(showDebug);
			}
		}
	}
	ImGui::End();
	//=====================================

	tileMap.Render();

	//==============Camera================
	if (X::IsKeyDown(X::Keys::UP))
	{
		cameraPos.y += camSpeed * deltaTime;
		X::SetPanPosition(cameraPos);
	}
	else if (X::IsKeyDown(X::Keys::DOWN))
	{
		cameraPos.y -= camSpeed * deltaTime;
		X::SetPanPosition(cameraPos);
	}
	if (X::IsKeyDown(X::Keys::LEFT))
	{
		cameraPos.x += camSpeed * deltaTime;
		X::SetPanPosition(cameraPos);
	}
	else if (X::IsKeyDown(X::Keys::RIGHT))
	{
		cameraPos.x -= camSpeed * deltaTime;
		X::SetPanPosition(cameraPos);
	}
	//=====================================

	//============Gatherers================
	for (auto& agent : gathererAgents)
	{
		agent->Update(deltaTime);
		if (agent->GetDepositedResource())
		{
			++gathererFood;
			agent->SetDepositedResource(false);
		} 
	}
	for (auto& agent : gathererAgents)
	{
		agent->Render();
	}
	if (eatTimerCurrent < 0)
	{
		eatTimerCurrent = eatTimerMax;
		gathererFood -= gathererAgents.size();
		while (gathererFood < 0)
		{
			++gathererFood;
			KillGatherer();
		}
		if (gathererFood > 0)
		{
			SpawnGatherer();
		}
	}
	//=====================================

	//=============Hunters================
	for (auto& agent : hunterAgents)
	{
		agent->Update(deltaTime);
		if (agent->GetDepositedGatherer())
		{
			++gathererFood;
			agent->SetDepositedGatherer(false);
		}
	}
	for (auto& agent : hunterAgents)
	{
		agent->Render();
	}
	if (eatTimerCurrent < 0)
	{
		eatTimerCurrent = eatTimerMax;
		hunterFood -= hunterAgents.size();
		while (hunterFood < 0)
		{
			++hunterFood;
			KillHunter();
		}
		if (hunterFood > 0)
		{
			SpawnHunter();
		}
	}
	//=====================================
	eatTimerCurrent -= deltaTime;

	//============Resources================
	auto iter = resources.begin();
	while (iter != resources.end())
	{
		if (iter->get()->GetHealth() <= 0)
		{
			iter->reset();
			iter = resources.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	for (auto& resource : resources)
	{
		resource->Render();
	}
	if (resourceGrowTimerCurrent > resourceGrowTimerMax)
	{
		SpawnResource();
		resourceGrowTimerCurrent = 0.0f;
	}
	resourceGrowTimerCurrent += deltaTime;
	
	//=====================================

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{
	for (auto& agent : gathererAgents)
	{
		agent->Unload();
		agent.reset();
	}
	for (auto& agent : hunterAgents)
	{
		agent->Unload();
		agent.reset();
	}
	for (auto& resource : resources)
	{
		resource.reset();
	}
	gathererAgents.clear();
	hunterAgents.clear();
	resources.clear();
}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Pathfinding");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}