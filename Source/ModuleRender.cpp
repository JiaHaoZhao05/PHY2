#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "Scenario.h"
#include <math.h>

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    background = RAYWHITE;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRender::PreUpdate()
{
    camera.target = { App->game->player->physBody->body->GetPosition().x * PIXELS_PER_METER, App->game->player->physBody->body->GetPosition().y * PIXELS_PER_METER };
    //camera.rotation = -App->game->player->physBody->body->GetAngle() * RAD2DEG; //Rotating camera option
    //Draw the background

    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    
    App->scenario->Draw();
    //EndMode2D();
    return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleRender::Update()
{
    // NOTE: This function setups render batching system for
    // maximum performance, all consecutive Draw() calls are
    // not processed until EndDrawing() is called
    if(App->game->gamePaused==false)DrawText(TextFormat("Time: %.2f", App->game->timer.ReadSec()), camera.target.x - SCREEN_WIDTH/2, camera.target.y-SCREEN_HEIGHT/2, GetFontDefault(), 20, WHITE, 30);
    if (App->game->finishedOnce)DrawText(TextFormat("Best time: %.2f", App->game->bestTime), camera.target.x - SCREEN_WIDTH / 2, camera.target.y - SCREEN_HEIGHT / 2 + 40, GetFontDefault(), 20, WHITE, 30);
    if (App->game->gamePaused == false)DrawText(TextFormat("Ammo: %i", 3- App->game->player->PItems.size()), camera.target.x + SCREEN_WIDTH / 3, camera.target.y - SCREEN_HEIGHT / 2, GetFontDefault(), 20, WHITE, 30);
    //BeginMode2D(camera);
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRender::PostUpdate()
{
    // Draw everything in our batch!
    DrawFPS(10, 10);
    
    EndMode2D();
  
    EndDrawing();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	return true;
}

void ModuleRender::SetBackgroundColor(Color color)
{
	background = color;
}

// Draw to screen
bool ModuleRender::Draw(Texture2D texture, int x, int y, const Rectangle* section, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;

	float scale = 1.0f;
    Vector2 position = { (float)x, (float)y };
    Rectangle rect = { 0.f, 0.f, (float)texture.width, (float)texture.height };

    if (section != NULL) rect = *section;

    position.x = (float)(x - pivot_x) * scale;// +camera.x;
    position.y = (float)(y - pivot_y) * scale;// +camera.y;
    
	rect.width *= scale;
	rect.height *= scale;

    DrawTextureRec(texture, rect, position, WHITE);

	return ret;
}

bool ModuleRender::DrawText(const char * text, int x, int y, Font font, int spacing, Color tint, float size) const
{
    bool ret = true;

    Vector2 position = { (float)x, (float)y };

    DrawTextEx(font, text, position, size, (float)spacing, tint);

    return ret;
}