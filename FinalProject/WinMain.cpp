#include "TileMap.h"
#include <XEngine.h>
#include <ImGui/Inc/imgui.h>

#include <AI.h>
#include "Gatherer.h"

using namespace AI;

TileMap tileMap;
X::Math::Vector2 destination = X::Math::Vector2::Zero();
X::Math::Vector3 cameraPos = X::Math::Vector3::Zero();
Path path;
int startX = 5;
int startY = 9;
int endX = 20;
int endY = 12;

bool showDebug = false;
float wanderJitter = 5.0f;
float wanderRadius = 20.0f;
float wanderDistance = 50.0f;
float radius = 50.0f;
float viewRange = 300.0f;
float viewAngle = 45.0f;

AIWorld aiWorld;
std::vector<std::unique_ptr<Gatherer>> gathererAgents;


void SpawnGatherer()
{
	auto& agent = gathererAgents.emplace_back(std::make_unique<Gatherer>(aiWorld));
	agent->Load();

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	agent->position = X::RandomVector2({ 100.0f, 100.0f },
		{ screenWidth - 100.0f, screenHeight - 100.0f });
	agent->destination = destination;
	agent->radius = radius;
	agent->ShowDebug(showDebug);
}
void KillGatherer()
{
	auto& agent = gathererAgents.back();
	agent->Unload();
	gathererAgents.pop_back();
}

//--------------------------------------------------
void GameInit()
{
	tileMap.LoadTiles("tiles.txt");
	tileMap.LoadMap("map.txt");
}

bool GameLoop(float deltaTime)
{
	ImGui::Begin("PahthFinding", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
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
		ImGui::DragInt("StartX", &startX, 1, 0, columns - 1);
		ImGui::DragInt("StartY", &startY, 1, 0, rows - 1);
		ImGui::DragInt("EndX", &endX, 1, 0, columns - 1);
		ImGui::DragInt("EndY", &endY, 1, 0, rows - 1);

		if (ImGui::Button("RunBFS"))
		{
			path = tileMap.FindPathBFS(startX, startY, endX, endY);
		}
		if (ImGui::Button("RunDFS"))
		{
			path = tileMap.FindPathDFS(startX, startY, endX, endY);
		}
		if (ImGui::Button("RunDijkstra"))
		{
			path = tileMap.FindPathDijkstra(startX, startY, endX, endY);
		}
		if (ImGui::Button("RunAstar"))
		{
			path = tileMap.FindPathAStar(startX, startY, endX, endY);
		}
	}
	ImGui::End();

	tileMap.Render();

	for (int i = 1; i < path.size(); ++i)
	{
		X::DrawScreenLine(path[i - 1], path[i], X::Colors::Red);
	}
	X::DrawScreenCircle(tileMap.GetPixelPosition(startX, startY), 10.0f, X::Colors::Pink);
	X::DrawScreenCircle(tileMap.GetPixelPosition(endX, endY), 10.0f, X::Colors::Yellow);

	if (X::IsKeyDown(X::Keys::UP))
	{
		cameraPos.y += 15.0f * deltaTime;
		X::SetPanPosition(cameraPos);
	}
	else if (X::IsKeyDown(X::Keys::DOWN))
	{
		cameraPos.y -= 15.0f * deltaTime;
		X::SetPanPosition(cameraPos);
	}
	if (X::IsKeyDown(X::Keys::LEFT))
	{
		cameraPos.x += 15.0f * deltaTime;
		X::SetPanPosition(cameraPos);
	}
	else if (X::IsKeyDown(X::Keys::RIGHT))
	{
		cameraPos.x -= 15.0f * deltaTime;
		X::SetPanPosition(cameraPos);
	}

	aiWorld.Update();

	for (auto& agent : gathererAgents)
	{
		agent->Update(deltaTime);
	}
	for (auto& agent : gathererAgents)
	{
		agent->Render();
	}

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
	gathererAgents.clear();
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