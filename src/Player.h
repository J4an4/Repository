#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		32

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	28

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			2

//Vertical speed while on a ladder
//#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
//#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
//#define PLAYER_JUMP_FORCE		10

//Frame delay for updating the jump velocity
//#define PLAYER_JUMP_DELAY		1
//
////Player is levitating when abs(speed) <= this value
//#define PLAYER_LEVITATING_SPEED	4
//
////Gravity affects jumping velocity when jump_delay is 0
//#define GRAVITY_FORCE			1

//Logic states
enum class State { IDLE, WALKING, PUSHING};
enum class Look { RIGHT, LEFT, UP, DOWN };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	IDLE_UP, IDLE_DOWN,
	WALKING_LEFT, WALKING_RIGHT,
	WALKING_UP, WALKING_DOWN,
	PUSHING_LEFT, PUSHING_RIGHT,
	PUSHING_UP, PUSHING_DOWN,
	NUM_ANIMATIONS
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);
	void SetAuxTileMap(TileMap* tilemap);


	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	int steps = 0;
	int level = 0;
	bool lost = false;
	bool won = false;

	bool PushingUp = false;
	bool PushingDown = false;
	bool PushingRight = false;
	bool PushingLeft = false;

	int x, x1, x2, y, y1, y2;

	Tile nextTile, Destination;

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	bool Moving = false;

	bool IsMoving() const;

	//Player mechanics
	void MoveX();
	void MoveY();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();

	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();

	int maxMove;

	void StartPushingLeft();
	void StartPushingRight();
	void StartPushingUp();
	void StartPushingDown();

	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();

	void WaitForInput();
	bool CanMove;

	void CheckSteps();

	//float timer;
	//float lifetime;

	//void StarTimer(float *t, float *l);
	//void UpdateTimer(float *t);
	//bool TimerDone(float *t);

	State state;
	Look look;

	TileMap* map;
	TileMap* map1;

	int score;
};

