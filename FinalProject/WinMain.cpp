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

float eatTimerMaxGatherer = 40.0f;
float eatTimerCurrentGatherer = eatTimerMaxGatherer;

float eatTimerMaxHunter = 120.0f;
float eatTimerCurrentHunter = eatTimerMaxHunter;

float resourceGrowTimerMax = 8.0f;
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
	SpawnGatherer();
	SpawnGatherer();

	SpawnHunter();
	SpawnHunter();
}

bool GameLoop(float deltaTime)
{
	//==============ImGui================
	ImGui::Begin("PathFinding", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		const int columns = tileMap.GetColumns();
		const int rows = tileMap.GetRows();

		ImGui::Text("GathererEatTimer: %.2f", eatTimerCurrentGatherer);
		ImGui::Text("GathererFood: %i", gathererFood);
		ImGui::Text("HunterEatTimer: %.2f", eatTimerCurrentHunter);
		ImGui::Text("HunterFood: %i", hunterFood);

		if (ImGui::Checkbox("ShowDebug", &showDebug))
		{
			for (auto& agent : gathererAgents)
			{
				agent->ShowDebug(showDebug);
				agent->SetShowDebug(showDebug);
			}
			for (auto& agent : hunterAgents)
			{
				agent->ShowDebug(showDebug);
				agent->SetShowDebug(showDebug);
			}
			tileMap.SetShowDebug(showDebug);
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

	//============Gatherers================
	for(auto iter = gathererAgents.begin(); iter != gathererAgents.end();)
	{
		if (iter->get()->GetHealth() <= 0)
		{
			iter->get()->Unload();
			iter = gathererAgents.erase(iter);
		}
		else
		{
			iter->get()->Update(deltaTime);
			if (iter->get()->GetDepositedResource())
			{
				gathererFood += 2;
				iter->get()->SetDepositedResource(false);
			}
			iter->get()->Render();
			++iter;
		}
	}
	if (eatTimerCurrentGatherer < 0)
	{
		eatTimerCurrentGatherer = eatTimerMaxGatherer;
		gathererFood -= gathererAgents.size();
		while (gathererFood < 0)
		{
			++gathererFood;
			KillGatherer();
		}
		if (gathererFood >= 0)
		{
			SpawnGatherer();
		}
	}
	eatTimerCurrentGatherer -= deltaTime;
	//=====================================

	//=============Hunters================
	for (auto& agent : hunterAgents)
	{
		agent->Update(deltaTime);
		if (agent->GetDepositedGatherer())
		{
			++hunterFood;
			agent->SetDepositedGatherer(false);
		}
		agent->Render();
	}
	if (eatTimerCurrentHunter < 0)
	{
		eatTimerCurrentHunter = eatTimerMaxHunter;
		hunterFood -= hunterAgents.size();
		while (hunterFood < 0)
		{
			++hunterFood;
			KillHunter();
		}
		if (hunterFood >= 0)
		{
			SpawnHunter();
		}
	}
	eatTimerCurrentHunter -= deltaTime;
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