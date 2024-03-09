#include "Resource.h"
#include "TypeIds.h"

Resource::Resource(AI::AIWorld& world)
	: Entity(world, static_cast<uint32_t>(AgentType::Resource))
{
}

void Resource::Initialize()
{
	mTextureId = X::LoadTexture("mushroom.png");
	position = X::RandomVector2({ 500.0f, 100.0f }, { 780.0f, 710.0f });
	position.x += X::Random(0, 1) == 0 ? -300.0f : 300.0f;
}

void Resource::Render()
{
	if (mHealth > 0)
	{
		X::DrawSprite(mTextureId, position);
	}
}
