#pragma once

#include <XEngine.h>
#include <AI.h>

class Resource : public AI::Entity
{
public:
	Resource(AI::AIWorld& world);

	void Initialize();
	void Render();

	int GetHealth() const { return mHealth; }
	void SetHealth(int health) { mHealth = health; }

private:
	X::TextureId mTextureId;
	int mHealth = 100;
};