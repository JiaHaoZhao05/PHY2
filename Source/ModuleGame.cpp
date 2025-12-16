#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Scenario.h"
#include "Player.h"
#include "Hand.h"
#include "Thumb.h"

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	SetTargetFPS(60);
	Loadsfx();
	App->scenario->LoadMap();
	LoadEntities();
	ReadPlayerFX();
	startTex = LoadTexture("Assets/Textures/screenstart.png");
	endTex = LoadTexture("Assets/Textures/screenend.png");
	countdownTex1 = LoadTexture("Assets/Textures/Countdown/1.png");
	countdownTex2 = LoadTexture("Assets/Textures/Countdown/2.png");
	countdownTex3 = LoadTexture("Assets/Textures/Countdown/3.png");
	countdownTexGO = LoadTexture("Assets/Textures/Countdown/GO.png");

	return ret;
}

bool ModuleGame::CleanUp()
{
	App->scenario->CleanUp();
	App->physics->QueueBodyForDestroy(player->physBody);
	for (Enemy* entity : enemies) {
		for (Items* item : entity->EItems) {
			App->physics->QueueBodyForDestroy(item->physBody);
		}
		entity->EItems.clear();
		App->physics->QueueBodyForDestroy(entity->physBody);
	}
	enemies.clear();
	for (Items* item : player->PItems) {
		App->physics->QueueBodyForDestroy(item->physBody);
	}
	for (Items* item : player->Thumbs) {
		App->physics->QueueBodyForDestroy(item->physBody);
	}
	player->PItems.clear();
	//App->audio->CleanUp();
	
	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	if (!gamePaused) {
		CheckMusic();
	}
	UpdateEntities();
	CheckTimers();
	App->scenario->Update();
	ReadInputs();
	if (player->finished) {
		EndGame();
	}
	return UPDATE_CONTINUE;
}

void ModuleGame::CheckTimers() {
	if (starting) {
		int time = startTimer.ReadSec();
		switch (time) {
		case 0: //3
			App->audio->PlayFx(countdownAudio1-1);
			App->renderer->Draw(countdownTex3, player->pos.x -75, player->pos.y + 100);
			break;
		case 1: //2
			App->audio->PlayFx(countdownAudio2 - 1);
			App->renderer->Draw(countdownTex2, player->pos.x - 75, player->pos.y + 100);
			break;
		case 2: //1
			App->audio->PlayFx(countdownAudio3 - 1);
			App->renderer->Draw(countdownTex1, player->pos.x - 75, player->pos.y + 100);
			break;
		case 3: //GO
			if (gamePaused) {
				App->audio->PlayFx(countdownAudioGO - 1);
				StartGame();
			}
			App->renderer->Draw(countdownTexGO, player->pos.x - 75, player->pos.y + 100);
			
			break;
		case 4:
			starting = false;
			break;
		}
	}
	else
	{
		if (firsttime) {
			App->renderer->Draw(startTex, player->pos.x - 475, player->pos.y - 490);
		}
	}
	if (player->finished) {
		App->renderer->Draw(endTex, player->pos.x - 475, player->pos.y - 490);
	}
}

void ModuleGame::ReadInputs() {
	if (gamePaused){
		if (IsKeyPressed(KEY_ENTER)) {
			if (player->finished) {
				RestartGame();
				firsttime = true;
			}
			else {
				firsttime = false;
				starting = true;
				startTimer.Start();
			}
		}	
		return;
	}
	if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && !(IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))) {
		player->Turn(true, true);
	}
	else if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && !(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))) {
		player->Turn(false, true);
	}
	else if (IsKeyReleased(KEY_D) || IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_A) || IsKeyReleased(KEY_LEFT)) {
		player->Turn(false, false);
	}
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
		player->Throttle(true);
	}
	else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
		player->Throttle(false);
	}
	if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
		player->Brake();
	}
	if (IsKeyPressed(KEY_SPACE)) {
		if (player->PItems.size() < 3) {
			player->AddItem(new Hand(App->physics, player->pos.x, player->pos.y, this, player->physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f)), App));
		}
	}
	if (IsKeyPressed(KEY_T)) {
		if (player->Thumbs.size() < 4) {
			player->Thumbus(new Thumb(App->physics, player->pos.x, player->pos.y, this, player->physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f)), App));
		}
	}
}
		

void ModuleGame::UpdateEntities() {
	player->Update();
	for (Enemy* entity : enemies) {
		if (entity->finished==false){
			entity->Update();
			for (Items* n : entity->EItems) {
				n->Update();
				if (n->pendingToDelete) {
					App->physics->QueueBodyForDestroy(n->physBody);
					delete n;
				}
			}
		}
		else {
			Start();
		}
	}
	for (Items* n : player->PItems) {
		n->Update();
		if (n->pendingToDelete) {
			App->physics->QueueBodyForDestroy(n->physBody);
			delete n;
		}
	}
	for (Items* n : player->Thumbs) {
		n->Update();
		if (n->pendingToDelete) {
			App->physics->QueueBodyForDestroy(n->physBody);
			delete n;
		}
	}
}

void ModuleGame::StartGame() {
	gamePaused = false;
	App->audio->PlayFx(music-1);
	musicTime.Start();
	timer.Start();
	player->isActive = true;
	for (Enemy* entity : enemies) {
		entity->isActive = true;
	}
}

void ModuleGame::RestartGame() {
	player->finished = false;
	App->audio->StopFx(music - 1);
	player->physBody->body->SetFixedRotation(false);
	CleanUp();
	App->scenario->LoadMap();
	LoadEntities();	
	ReadPlayerFX();
}

void ModuleGame::EndGame() {
	gamePaused = true;
	if (!finishedOnce) {
		bestTime = timer.ReadSec();
		finishedOnce = true;
	}
	else if (timer.ReadSec() < bestTime) bestTime = timer.ReadSec();
	for (Enemy* entity : enemies) {
		entity->isActive = false;
	}
	player->physBody->body->SetLinearVelocity({0,0});
	player->physBody->body->SetFixedRotation(true);
}

void ModuleGame::LoadEntities() {
	float rotation = App->scenario->initialRotation;
//	player = new Player(App->physics, App->scenario->mapPos[0].x, App->scenario->mapPos[0].y, this, rotation, App->scenario->checkpoints, App);
	player = new Player(App->physics, App->scenario->mapPos[0].x, App->scenario->mapPos[0].y, this, rotation, App->scenario->checkpoints, App);
	enemies.emplace_back(new Enemy(App->physics, App->scenario->mapPos[1].x, App->scenario->mapPos[1].y, this, rotation, App->scenario->centerLine, App));
	enemies.emplace_back(new EnemyTooth(App->physics, App->scenario->mapPos[2].x, App->scenario->mapPos[2].y, this, rotation, App->scenario->centerLine, App));
	enemies.emplace_back(new EnemyPsy(App->physics, App->scenario->mapPos[3].x, App->scenario->mapPos[3].y, this, rotation, App->scenario->centerLine, App));

	player->Start();
	for (Enemy* entity : enemies) {
		entity->Start();
	}
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB) {
	if (bodyA->type == EntityType::PLAYER) {
		switch (bodyB->type) {
		case(EntityType::ENEMY):
			player->OnCollisionEnemy();
			for (Enemy* n : enemies) {
				if (n->physBody == bodyB) {
					n->Create();
				}
			}
			break;
		case(EntityType::ITEM):
			for (Enemy* n : enemies) {
				for (Items* m : n->EItems) {
					if (m->physBody == bodyB) {
						m->OnCollisionWithPlayer();
					}
				}
			}
			break;
		case(EntityType::BOOSTER_UP):
			player->OnCollisionBooster({0,-1});
			break;
		case(EntityType::BOOSTER_DOWN):
			player->OnCollisionBooster({ 0,1 });
			break;
		case(EntityType::BOOSTER_LEFT):
			player->OnCollisionBooster({-1,0});
			break;
		case(EntityType::BOOSTER_RIGHT):
			player->OnCollisionBooster({1,0});
			break;
		case(EntityType::ROUGH_SURFACE):
			player->OnCollisionRoughSurface();
			break;
		case(EntityType::SLIDING_SURFACE):
			player->OnCollisionSlidingSurface();
			break;
		case EntityType::SPIT:
			player->OnCollisionSpit(true);
			break;
		default:
			break;
		}
	}
	else if (bodyA->type == EntityType::ENEMY) {
		for (Enemy* n : enemies) {
			if (n->physBody == bodyA) {
				switch (bodyB->type) {
				case(EntityType::BOOSTER_UP):
					n->OnCollisionBooster({ 0,-1 });
					break;
				case(EntityType::BOOSTER_DOWN):
					n->OnCollisionBooster({ 0,1 });
					break;
				case(EntityType::BOOSTER_LEFT):
					n->OnCollisionBooster({ -1,0 });
					break;
				case(EntityType::BOOSTER_RIGHT):
					n->OnCollisionBooster({ 1,0 });
					break;
				case(EntityType::ROUGH_SURFACE):
					n->OnCollisionRoughSurface();
					break;
				case(EntityType::SLIDING_SURFACE):
					n->OnCollisionSlidingSurface();
					break;
				case EntityType::SPIT:
					n->OnCollisionSpit(true);
					break;
				default:
					break;
				}
			}
		}
	}
}

void ModuleGame::EndCollision(PhysBody* bodyA, PhysBody* bodyB) {
	if (bodyA->type == EntityType::PLAYER) {
		switch (bodyB->type) {
		case(EntityType::ENEMY):
			break;
		case(EntityType::ITEM):
			break;
		case(EntityType::ROUGH_SURFACE):
			player->EndCollisionSurface();
			break;
		case(EntityType::SLIDING_SURFACE):
			player->EndCollisionSurface();
			break;
		case EntityType::SPIT:
			player->OnCollisionSpit(false);
		default:
			break;
		}
	}
	else if (bodyA->type == EntityType::ENEMY) {
		for (Enemy* n : enemies) {
			if (n->physBody == bodyA) {
				switch (bodyB->type) {
				case(EntityType::ROUGH_SURFACE):
					n->EndCollisionSurface();
					break;
				case(EntityType::SLIDING_SURFACE):
					n->EndCollisionSurface();
					break;
				case EntityType::SPIT:
					n->OnCollisionSpit(false);
				default:
					break;
				}
			}
		}
	}
}

void ModuleGame::CheckMusic() {
	if (musicTime.ReadSec() > 106.945f) {
		App->audio->PlayFx(music-1);
	}
}

void ModuleGame::Loadsfx() {
	throttleFX = App->audio->LoadFx("Assets/Sounds/throttleFX.wav");
	brakeFX = App->audio->LoadFx("Assets/Sounds/brakeFX.wav");
	turnFX = App->audio->LoadFx("Assets/Sounds/turnFX.wav");
	crashFX = App->audio->LoadFx("Assets/Sounds/crashFX.wav");
	engineFX = App->audio->LoadFx("Assets/Sounds/engineFX.wav");
	boostFX = App->audio->LoadFx("Assets/Sounds/boostFX.wav");
	armThrowFX = App->audio->LoadFx("Assets/Sounds/armThrowFX.wav");
	carCollisionWithCarFX = App->audio->LoadFx("Assets/Sounds/carCollisionWithCarFX.wav");

	music = App->audio->LoadFx("Assets/Sounds/music.wav");
	countdownAudio1 = App->audio->LoadFx("Assets/Sounds/Countdown/1D.wav");
	countdownAudio2 = App->audio->LoadFx("Assets/Sounds/Countdown/2.wav");
	countdownAudio3 = App->audio->LoadFx("Assets/Sounds/Countdown/3.wav");
	countdownAudioGO = App->audio->LoadFx("Assets/Sounds/Countdown/GO.wav");
	toothFX = App->audio->LoadFx("Assets/Sounds/toothCollisionFX.wav");
	spitFX = App->audio->LoadFx("Assets/Sounds/toothCollisionFX.wav");
	eyeFX = App->audio->LoadFx("Assets/Sounds/eyeCollisionFX.wav");
}

void ModuleGame::ReadPlayerFX() {
	player->throttleFX = throttleFX;
	player->brakeFX = brakeFX;
	player->turnFX = turnFX;
	player->crashFX = crashFX;
	player->engineFX = engineFX;
	player->boostFX = boostFX;
	player->armThrowFX = armThrowFX;
	player->carCollisionWithCarFX = carCollisionWithCarFX;
}