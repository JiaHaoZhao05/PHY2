#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"
#include "Items.h"
Items::Items(PhysBody* _body, Module* _listener, EntityType _type, ModuleAudio* _audio) : PhysicEntity(_body, _listener, _type)
{
	audio = _audio;
}
bool Items::Update() {
	Behave();
	Draw();
	return true;
}
void Items::Draw() { //I'm sick of copying and pasting this
	Vector2 position{ physBody->body->GetPosition().x * PIXELS_PER_METER, physBody->body->GetPosition().y * PIXELS_PER_METER };
	float scale = 1.0f;
	Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
	Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
	float rotation = physBody->body->GetAngle() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}
void Items::Destroy() {
	pendingToDelete = true;
}