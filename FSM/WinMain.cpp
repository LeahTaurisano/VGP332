#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>

#include "Miner.h"
#include "Dog.h"

//Miner miner;
Dog dog;

void GameInit()
{
	dog.Initialize();
}

bool GameLoop(float deltaTime)
{
	if (X::IsKeyPressed(X::Keys::SPACE))
	{
		dog.Update(deltaTime);
	}

	ImGui::Begin("FSM", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	dog.DebugUI();
	ImGui::End();

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{
	dog.Terminate();
}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("FSM");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}