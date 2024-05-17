#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	// 0 < id < 50: static tiles
	BLOCK_SQUARE1_TL = 1, BLOCK_SQUARE1_TR, BLOCK_SQUARE1_BL, BLOCK_SQUARE1_BR,
	BLOCK_SQUARE2_TL, BLOCK_SQUARE2_TR, BLOCK_SQUARE2_BL, BLOCK_SQUARE2_BR,
	BLOCK_VERT2_T, BLOCK_VERT2_B, BLOCK_HORIZ2_L, BLOCK_HORIZ2_R, BLOCK_BLUE,
	BLOCK_HORIZ3_L, BLOCK_HORIZ3_M, BLOCK_HORIZ3_R,
	BLOCK_BEAM_L, BLOCK_BEAM_R,
	
	LADDER_L = 20, LADDER_R, LADDER_TOP_L, LADDER_TOP_R,
	LOCK_RED = 30, LOCK_YELLOW, 
	LASER_L = 40, LASER_R,

	// 50 <= id < 100: special tiles
	KEY_RED = 60, YELLOW_KEY, ITEM_APPLE, ITEM_CHILI,

	// id >= 100: entities' initial locations
	PLAYER = 100,

	//Intervals
	STATIC_LAST = BLOCK_SQUARE2_TL,
	

	SOLID_FIRST = BLOCK_SQUARE2_TL,
	SOLID_LAST = BLOCK_SQUARE2_TL,


	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = PLAYER
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	const Texture2D* img_box;


	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();
	bool CheckWin(int winCount);

	Tile GetTileIndex(int x, int y) const;
	Tile GetOldTileIndex(int x, int y) const;

	void ChangeTile(int x, int y, Tile t) const;

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	bool TestCollisionWallUp(const AABB& box) const;
	bool TestCollisionWallDown(const AABB& box) const;
	
	bool TestBoxLeft(const AABB& box) const;
	bool TestBoxRight(const AABB& box) const;
	bool TestBoxUp(const AABB& box) const;
	bool TestBoxDown(const AABB& box) const;

	bool MoveBoxRight(const AABB& box) const;
	bool MoveBoxLeft(const AABB& box) const;
	bool MoveBoxUp(const AABB& box) const;
	bool MoveBoxDown(const AABB& box) const;

	void DrawBox(int x, int y) const;
	void DrawWin() const;
	void DrawLose() const;




private:
	void InitTileDictionary();

	bool IsTileSolid(Tile tile) const;

	bool IsTileBox(Tile tile) const;
	bool IsTileGoal(Tile tile) const;


	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	
	bool CollisionBoxX(const Point& p, int distance) const;
	bool CollisionBoxY(const Point& p, int distance) const;

	bool BoxRight(const Point& p, int distance) const;
	bool BoxLeft(const Point& p, int distance) const;
	bool BoxUp(const Point& p, int distance) const;
	bool BoxDown(const Point& p, int distance) const;

	//Tile map
	Tile *map;
	Tile* map1;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	
	
	//Tile sheet
	const Texture2D *img_tiles;
	const Texture2D* img_hud;
	const Texture2D* img_win;
	const Texture2D* img_lose;
};

