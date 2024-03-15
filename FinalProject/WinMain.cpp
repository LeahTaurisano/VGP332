#include "TileMap.h"
#include <XEngine.h>
#include <ImGui/Inc/imgui.h>

#include <AI.h>
#include "Gatherer.h"
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

uint32_t gathererFood;
uint32_t hunterFood;

AIWorld aiWorld;
std::vector<std::unique_ptr<Gatherer>> gathererAgents;


void SpawnGatherer()
{
	auto& agent = gathererAgents.emplace_back(std::make_unique<Gatherer>(aiWorld));
	agent->Load();
	agent->SetTileMap(&tileMap);
	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	agent->position = agent->GetGathererHome();
	agent->radius = radius;
	agent->ShowDebug(showDebug);
	agent->InitializeStates();
	agent->ChangeState(GathererState::GoToGatherSpot);
}
void KillGatherer()
{
	auto& agent = gathererAgents.front();
	agent->Unload();
	gathererAgents.erase(gathererAgents.begin());
}

//--------------------------------------------------
void GameInit()
{
	tileMap.LoadTiles("tiles.txt");
	tileMap.LoadMap("map.txt");

	for (uint32_t i = 0; i < 10; ++i)
	{
		auto& mineral = resources.emplace_back(std::make_unique<Resource>(aiWorld));
		mineral->Initialize();
	}
}

bool GameLoop(float deltaTime)
{
	//==============ImGui================
	ImGui::Begin("PathFinding", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		const int columns = tileMap.GetColumns();
		const int rows = tileMap.GetRows();

		if (ImGui::Button("SpawnGatherer"))
		{
			SpawnGatherer();
		}
		ImGui::SameLine();
		if (ImGui::Button("KillGatherer") && !gathererAgents.empty())
		{
			KillGatherer();
		}
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
	}
	for (auto& agent : gathererAgents)
	{
		agent->Render();
	}
	//=====================================

	//============Minerals=================
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
	for (auto& resource : resources)
	{
		resource.reset();
	}
	gathererAgents.clear();
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