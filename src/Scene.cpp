#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	level2 = nullptr;

	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	if (level2 != nullptr)
	{
		level2->Release();
		delete level2;
		level2 = nullptr;
	}
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	level2 = new TileMap();
	if (level2 == nullptr)
	{
		LOG("Failed to allocate memory for Level");
		return AppStatus::ERROR;
	}
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	if (level2->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	player->SetTileMap(level2);

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	Object *obj;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
			 1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  
			 1,   1,   5,   5,   5,   5,   5,   5,   5,   1,   1,  
		     1,   1,   5,   11,  11,  12,  12,   12,   5,   1,   1,  
		     1,   1,   5,   11,  11,  11,  5,  5,   5,   5,   1,  
		     1,   5,   5,   5,   10,  11,  11,  11,   11,  5,   1,  
			 1,   5,   11,  11,  11,  5,   10,  5,   11,  5,   1,  
		     1,   5,   11,  10,  11,  5,   11,  11,   11,  5,   1,  
		     1,   5,   11,  11,  11,  5,   5,  5,   5,   5,   1,
			 1,   5,   5,   5,   5,   5,   1,   1,    1,   1,   1,  
			 1,   1,   1,   1,   1,   1,   1,   1,    1,   1,   1,  
		     1,   1,   1,   1,   1,   1,   1,   1,    1,   1,   1,  
		    
		};

		map1 = new int[size]
		{
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 5, 5, 5, 5, 5, 5, 5, 1, 1,
				1, 1, 5, 11, 11, 12, 12, 12, 5, 1, 1,
				1, 1, 5, 11, 11, 11, 5, 5, 5, 5, 1,
				1, 5, 5, 5, 10, 11, 11, 11, 11, 5, 1,
				1, 5, 11, 11, 11, 5, 10, 5, 11, 5, 1,
				1, 5, 11, 10, 11, 5, 11, 11, 11, 5, 1,
				1, 5, 11, 11, 11, 5, 5, 5, 5, 5, 1,
				1, 5, 5, 5, 5, 5, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		};

		player->InitScore();
	}
	
	
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			
			else if (tile == Tile::COMPLETION)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				objects.push_back(obj);
				map[i] = 0;
			}
			++i;
		}
	}
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)level2[i];
			if (tile == Tile::EMPTY)
			{
				level2[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				level2[i] = 0;
			}
			else if (tile == Tile::COMPLETION)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;;
				objects.push_back(obj);
				level2[i] = 0;
			}
			++i;
		}
	}
	pos.x = 4 * TILE_SIZE;
	pos.y = 3 * TILE_SIZE - 1;
	player->SetPos(pos);
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	level2->Load(map1, LEVEL_WIDTH, LEVEL_HEIGHT);


	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))		LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))	LoadLevel(2);

	level->Update();
	player->Update();
	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	level1->Release();

	player->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if(player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());
			
			//Delete the object
			delete* it; 
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it); 
		}
		else
		{
			//Move to the next object
			++it; 
		}
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Temporal approach
	//DrawText(TextFormat("SCORE : %d", player->GetScore()), 10, 10, 8, LIGHTGRAY);
	if (player->level == 1)
	{
		DrawText(TextFormat("STAGE", player->GetScore()), 10, 10, 8, YELLOW);
		DrawText(TextFormat("01", player->GetScore()), 58, 10, 8, LIGHTGRAY);

		DrawText(TextFormat("STEP", player->steps), 10, 24, 8, YELLOW);

		if (player->steps < 10)
		{
			DrawText(TextFormat("000%i", player->steps), 45, 24, 8, LIGHTGRAY);
		}
		else if (player->steps < 100)
		{
			DrawText(TextFormat("00%i", player->steps), 45, 24, 8, LIGHTGRAY);
		}
		else if (player->steps < 1000)
		{
			DrawText(TextFormat("0%i", player->steps), 45, 24, 8, LIGHTGRAY);
		}
		DrawText(TextFormat("LIMIT", player->steps), 10, 34, 8, YELLOW);
		DrawText(TextFormat("0090", player->steps), 45, 34, 8, LIGHTGRAY);


		if (player->PushingLeft)
		{
			level->DrawBox(player->x - TILE_SIZE, player->y - TILE_SIZE + 1);
		}
		if (player->PushingRight)
		{
			level->DrawBox(player->x + TILE_SIZE, player->y - TILE_SIZE + 1);
		}
		if (player->PushingUp)
		{
			level->DrawBox(player->x, player->y - TILE_SIZE * 2);
		}
		if (player->PushingDown)
		{
			level->DrawBox(player->x, player->y);
		}
		if (player->won)
		{
			level->DrawWin();
		}
		if (player->lost)
		{
			level->DrawLose();
		}
	}
}