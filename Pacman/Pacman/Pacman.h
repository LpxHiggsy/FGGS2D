#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
#include <time.h>
//munchie array
#define MUNCHIECOUNT 1
#define LARGEMETEORCOUNT 4
#define MISSILECOUNT 5
#define METEORCOUNT 4

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

// Data to represent Pacman
struct Player
{
	Rect* positionRect;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	int frame;
	int currentFrameTime;
	float speedMultiplier;
	bool dead;
	bool shield;
	float shieldSize;
	float fuel;
	float score;

};
struct Collectable
{
	Rect* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	int frameCount;
	int currentFrameTime;
	int frameTime;
	bool eaten;
};

struct Missile
{
	Rect* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	int frameCount;
	int currentFrameTime;
	int frameTime;
	int direction;
	float speed;
};
struct meteor
{
	Rect* sourceRect;
	Rect* posRect;
	Texture2D* texture;
	int frameCount;
	int currentFrameTime;
	int frameTime;
	int direction;
	float speed;
	bool ok;
	
};
struct fuel
{
	Rect* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	int frameCount;
	int currentFrameTime;
	int frameTime;
	bool collected;
};
struct Menu
{	
	
	Texture2D* background;
	Rect* rectangle;
	Vector2* stringPosition;
	bool paused;
    bool pKeyDown;
	bool start;
	bool spaceKeyDown;

};

class Pacman : public Game
{
private:
	
	//background
	Texture2D* _Background;
	Rect* _BackgroundRect;
	
	Player* _pacman;
	SoundEffect* _pop;
	SoundEffect* _explosion;
	SoundEffect* _woosh;
	
	// Data to represent Munchie/Cherry/Enemys
	Collectable* _munchies[MUNCHIECOUNT];
	meteor* _meteor[METEORCOUNT];
	meteor* _largeMeteor[LARGEMETEORCOUNT];
	Missile* _missile[MISSILECOUNT];
	
	vector <fuel*> _fuelCell;
	bool _newSpawn;	
	bool _rKeyDown;
	bool _fKeyDown;
	
	// Position for String
	Vector2* _stringPosition;

	//Constant data for Game Variables 
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;

	// Data for Menu 
	Menu* _menu;

	//Prototype,Methods
	void Reset();
	//Input Definition 
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);
	
	//Check Methods 
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey); 
	void StartMenu(Input::KeyboardState*state, Input::Keys startKey);
	void PacmanDead(Input::KeyboardState*state, Input::Keys RestartKey);
	void shield(Input::MouseState* mouseState);

	//Update Methods 
	void UpdatePacman(int elapsedTime); 
	void CheckViewportCollision();
	void UpdateMunchie(int elapsedTime);
	void UpdateMeteor(int elapsedTime);
	void UpdateLargeMeteor(int elapsedTime);
	void UpdateMissile(int elapsedTime);
	void CheckMunchieCollisions();
	void CheckMeteorCollisions();
	void CheckLargeMeteorCollisions();
	void CheckMissileCollisions();
	
	//array list fuel cell methods
	void UpdateFuelCell(int elaspsedTime);
	void CheckFuelCellCollisions();
	void SpawnFuelCell(Input::MouseState* mouseState);
	void DeleteFuelCell();

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};