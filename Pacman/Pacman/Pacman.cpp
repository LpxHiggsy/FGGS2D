#include "Pacman.h"
#include <time.h>
#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250)
{
	srand(time(NULL));
	
	_pacman = new Player();
	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;
	_pacman->speedMultiplier = 1.0f;
	_pacman->dead = false;
	_pacman->shield = false;
	_pacman->shieldSize = 10.0f;
	_pacman->fuel = 15.0f;
	_pacman->score = 0;


	
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Collectable();
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frameCount = rand() % 1;
		_munchies[i]->frameTime = rand() % 500 + 50;
		_munchies[i]->eaten = false;
	}

	for (int i = 0; i < MISSILECOUNT; i++)
	{
		_missile[i] = new Missile();
		_missile[i]->direction = 0;
		_missile[i]->frameCount = 0;
		_missile[i]->frameTime = 100;
		_missile[i]->direction = 0;
		_missile[i]->speed = 0.2f;
	}
	
	for (int i = 0; i < METEORCOUNT; i++)
	{
		_meteor[i] = new meteor;
		_meteor[i]->currentFrameTime = 0;
		_meteor[i]->frameCount = 0;
		_meteor[i]->frameTime = 1000;
		_meteor[i]->direction = 0;
		_meteor[i]->speed = 0.1f;
	}
	for (int i = 0; i < LARGEMETEORCOUNT; i++)
	{
		_largeMeteor[i] = new meteor;
		_largeMeteor[i]->currentFrameTime = 0;
		_largeMeteor[i]->frameCount = 0;
		_largeMeteor[i]->frameTime = 1000;
		_largeMeteor[i]->ok = false;
	}
	
	_menu = new Menu();
	_menu->paused = false;
	_menu->pKeyDown = false;
	_menu->start = true;
	_menu->spaceKeyDown = false;
	_rKeyDown = false;
	
	
	Audio::Initialise();
	
	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	
	delete _munchies[0]->texture;
	for (int i = 0; i < MUNCHIECOUNT; i++) 
	{ 
		delete _munchies[i]->sourceRect; 
		delete _munchies[i]->posRect; 
	} 
	delete[] _munchies;

	for (int i = 0; i < _fuelCell.size(); i++)
	{	
		delete _fuelCell[i]->texture;
		delete _fuelCell[i]->sourceRect;
		delete _fuelCell[i];
		_fuelCell.erase(_fuelCell.begin() + i);
	}
	_fuelCell.clear();
	
	delete _meteor[0]->texture;
	for (int i = 0; i < METEORCOUNT; i++)
	{
		delete _meteor[i]->sourceRect;
		delete _meteor[i]->posRect;
	}
	delete[] _meteor;
	
	delete _largeMeteor[0]->texture;
	for (int i = 0; i < LARGEMETEORCOUNT; i++)
	{
		delete _largeMeteor[i]->sourceRect;
		delete _largeMeteor[i]->posRect;
	}
	delete[] _largeMeteor;

	
	delete _missile[0]->texture;
	for (int i = 0; i < MISSILECOUNT; i++)
	{
		delete _missile[i]->sourceRect;
		delete _missile[i]->posRect;
	}
	delete[] _missile;
}
	

void Pacman::LoadContent()
{
	_pop = new SoundEffect;
	_pop->Load("Sounds/pop.wav");	
	_explosion = new SoundEffect;
	_explosion->Load("Sounds/explosion.wav");
	_woosh = new SoundEffect;
	_woosh->Load("Sounds/woosh.wav");
	
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Ship2.tga", false);

	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/munchie.png", true);

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;
	}
	for (int i = 0; i < _fuelCell.size(); i++)
	{
		_fuelCell[i]->texture = new Texture2D();
		_fuelCell[i]->texture->Load("Textures/fuelCell.png", true);
	}
	

	for (int i = 0; i < METEORCOUNT; i++)
	{
		_meteor[i]->texture = new Texture2D();
		_meteor[i]->texture->Load("Textures/meteorani.png", true);
	}


	for (int i = 0; i < LARGEMETEORCOUNT; i++)
	{
		_largeMeteor[i]->texture = new Texture2D();
		_largeMeteor[i]->texture->Load("Textures/largemeteor.png", true);	
	}
		

	for (int i = 0; i < MISSILECOUNT; i++)
	{
		_missile[i]->texture = new Texture2D();
		_missile[i]->texture->Load("Textures/EnemyM.png", true);
	}	
	
	_Background = new Texture2D();
	_Background->Load("Textures/background.png", true);
	
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	
	Reset();
}

void Pacman::Reset()
{

	_pacman->positionRect = new Rect(350.0f, 350.0f, 32, 32);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 12, 12);
		_munchies[i]->sourceRect = new Rect(0.0f, 0.0f, 12, 12);
	}
	
	for (int i = 0; i < _fuelCell.size(); i++)
	{	
		_fuelCell[i]->posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 15, 15);
		_fuelCell[i]->sourceRect = new Rect(0.0f, 0.0f, 25, 25);
	}


	for (int i = 0; i < METEORCOUNT; i++)
	{
		_meteor[i]->posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 50, 50);
		_meteor[i]->sourceRect = new Rect(0.0f, 0.0f, 200, 200);
	}

	for (int i = 0; i < LARGEMETEORCOUNT; i++)
	{
		/*for (int j = 0; i < i; j++)
		{
			while (!_largeMeteor[i]->ok)
			{
				_largeMeteor[i]->ok = true;*/
				_largeMeteor[i]->posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 75, 75);
				_largeMeteor[i]->sourceRect = new Rect(0.0f, 0.0f, 400, 400);
			/*	if (_largeMeteor[i]->posRect->Intersects(*_largeMeteor[j]->posRect))
				{
					_largeMeteor[i]->ok = false;
				}
			}
		}*/

	}
	
	for (int i = 0; i < MISSILECOUNT; i++)
	{
		_missile[i]->posRect = new Rect(rand() % Graphics::GetViewportWidth(), Graphics::GetViewportHeight() + 32, 20, 40);
		_missile[i]->sourceRect = new Rect(0.0f, 0.0f, 19, 40);
	}
	
	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//set Background
	_BackgroundRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	// Set Menu Paramters 
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f-110.0f, Graphics::GetViewportHeight() / 2.0f+8.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard/mouse
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();
	//Call the start menu method 
	StartMenu(keyboardState, Input::Keys::SPACE);
	//Call the Checkpaused method 
	CheckPaused(keyboardState, Input::Keys::P);
	
	PacmanDead(keyboardState, Input::Keys::R);

	if (!_pacman->dead)
	{
		if (!_menu->start)
		{
			if (!_menu->paused)
			{

				Input(elapsedTime, keyboardState, mouseState);

				shield(mouseState);

				CheckViewportCollision();

				UpdatePacman(elapsedTime);

				UpdateMunchie(elapsedTime);

				UpdateMeteor(elapsedTime);

				UpdateLargeMeteor(elapsedTime);

				UpdateMissile(elapsedTime);

				CheckMunchieCollisions();

				CheckMeteorCollisions();
				
				CheckLargeMeteorCollisions();
				
				CheckMissileCollisions();
				
				UpdateFuelCell(elapsedTime);
				
				CheckFuelCellCollisions();
				
				SpawnFuelCell(mouseState);

			}
		}
	}
	
	
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{ 
	//Pacman speed calculation 
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;
	//Pacman movement and direction
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->positionRect->X += pacmanSpeed;
		_pacman->direction = 0;
	}

	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacman->positionRect->Y -= pacmanSpeed;
		_pacman->direction = 3;
	}

	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacman->positionRect->Y += pacmanSpeed;
		_pacman->direction = 1;

	}

	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacman->positionRect->X -= pacmanSpeed;
		_pacman->direction = 2;

	}
	
	//Pacman bost
	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		_pacman->speedMultiplier = 2.0f;
	}
	else
	{
		_pacman->speedMultiplier = 1.0f;
	}


}
void Pacman::shield(Input::MouseState* mouseState)
{
	// pacmans shields	
	if (_pacman->shieldSize <= 0)
	{
		_pacman->shield = false;
		_pacman->shieldSize = 0.0f;
	}
	else 
	{
		if (mouseState->LeftButton == Input::ButtonState::PRESSED && _pacman->shieldSize > 0)
		{
			_pacman->shield = true;
			_pacman->shieldSize -= 0.1f;
		}
		else if (mouseState->LeftButton == Input::ButtonState::RELEASED)
		{
			_pacman->shield = false;
			_pacman->shieldSize -= 0.0f;
		}

	}
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	//The pause screen code
	if (state->IsKeyDown(Input::Keys::P) && !_menu->pKeyDown)
	{
		_menu->pKeyDown = true;
		_menu->paused = !_menu->paused;
	}

	if (state->IsKeyUp(Input::Keys::P))
	{
		_menu->pKeyDown = false;
	}
}		

void Pacman::StartMenu(Input::KeyboardState*state, Input::Keys startKey)
{
	//The start menu code for entering the game
	if (state->IsKeyDown(Input::Keys::SPACE) && !_menu->spaceKeyDown)
	{
		_menu->spaceKeyDown = true;
		_menu->start = false;
	}
	if (state->IsKeyUp(Input::Keys::SPACE))
	{
		_menu->spaceKeyDown = false;
	}
		
}

void Pacman::CheckViewportCollision()
{
	// Checks if Pacman is trying to disappear of screen
	if (_pacman->positionRect->X + _pacman->sourceRect->Width >  Graphics::GetViewportWidth() +32)
	{
		_pacman->positionRect->X = 0 - _pacman->sourceRect->Width;
		Audio::Play(_woosh);
	}

	if (_pacman->positionRect->Y + _pacman->sourceRect->Height >Graphics::GetViewportHeight() +32)
	{
		_pacman->positionRect->Y = 0 - _pacman->sourceRect->Height;
		Audio::Play(_woosh);
	}

	if (_pacman->positionRect->X + _pacman->sourceRect->Width < 0)
	{
		_pacman->positionRect->X = Graphics::GetViewportWidth() + 32  - _pacman->sourceRect->Width;
		Audio::Play(_woosh);
	}

	if (_pacman->positionRect->Y + _pacman->sourceRect->Height  < 0)
	{
		_pacman->positionRect->Y = Graphics::GetViewportHeight() +32 - _pacman->sourceRect->Height;
		Audio::Play(_woosh);
	}
}

void Pacman::UpdatePacman(int elapsedTime)
{
	//Pacman animation update
	_pacman->currentFrameTime += elapsedTime;

	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;

		if (_pacman->frame >= 2)
		{
			_pacman->frame = 0;
		}

		_pacman->currentFrameTime = 0;		
		
	}


	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;

	//pacman fuel
	_pacman->fuel -= 0.01f;

	if (_pacman->fuel <= 0)
	{
		_pacman->dead = true;
		_pacman->fuel = 0.0f;
	}

	//players score
	_pacman->score += 1.0f;

	
}

void Pacman::UpdateMunchie(int elapsedTime)
{
	//Munchie animation update
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		
		_munchies[i]->currentFrameTime += elapsedTime;

		if (_munchies[i]->currentFrameTime > _munchies[i]->frameTime)
		{
			_munchies[i]->frameCount++;

			if (_munchies[i]->frameCount >= 2)
			{
				_munchies[i]->frameCount = 0;
			}

				_munchies[i]->currentFrameTime = 0;
		}
	
		_munchies[i]->sourceRect->X = _munchies[i]->sourceRect->Width * _munchies[i]->frameCount;
	
	}
	
}


void Pacman::UpdateMeteor(int elapsedTime)
{
	//meteor update
	for (int i = 0; i < METEORCOUNT; i++)
	{
		_meteor[i]->currentFrameTime += elapsedTime;

		if (_meteor[i]->currentFrameTime > _meteor[i]->frameTime)
		{
			_meteor[i]->frameCount++;

			if (_meteor[i]->frameCount >= 4)
			{
				_meteor[i]->frameCount = 0;
			}

			_meteor[i]->currentFrameTime = 0;
		}
		_meteor[i]->sourceRect->X = _meteor[i]->sourceRect->Width * _meteor[i]->frameCount;
		
		
		//moving meteor
		if (_meteor[i]->direction == 0) 
		{
			_meteor[i]->posRect->X += _meteor[i]->speed * elapsedTime;
		}
		else if (_meteor[i]->direction == 1) 
		{
			_meteor[i]->posRect->X -= _meteor[i]->speed * elapsedTime;
		}
		
		// hits edges
		if (_meteor[i]->posRect->X + _meteor[i]->posRect->Width >= Graphics::GetViewportWidth()) //Hits Right edge
		{
			_meteor[i]->direction = 1; //Change direction
		}
		else if (_meteor[i]->posRect->X <= i) //Hits left edge
		{
			_meteor[i]->direction = 0;
		}

	}
	

}

void Pacman::UpdateLargeMeteor(int elapsedTime)
{
	// large meteor update
	for (int i = 0; i < LARGEMETEORCOUNT; i++)
	{
		_largeMeteor[i]->currentFrameTime += elapsedTime;

		if (_largeMeteor[i]->currentFrameTime > _largeMeteor[i]->frameTime)
		{
			_largeMeteor[i]->frameCount++;

			if (_largeMeteor[i]->frameCount >= 4)
			{
				_largeMeteor[i]->frameCount = 0;
			}

			_largeMeteor[i]->currentFrameTime = 0;
		}
		_largeMeteor[i]->sourceRect->X = _largeMeteor[i]->sourceRect->Width * _largeMeteor[i]->frameCount;
	}
}

void Pacman::UpdateMissile(int elapsedTime)
{
	for (int i = 0; i < MISSILECOUNT; i++)
	{
		// Moving the missile
		if (_missile[i]->direction == 0) //Moves down
		{
			_missile[i]->posRect->Y += _missile[i]->speed * elapsedTime;
		}
		
		if (_missile[i]->posRect->Y + _missile[i]->posRect->Height >= Graphics::GetViewportHeight()) //Hits bottom
		{
			_missile[i]->posRect->Y = 0 - _missile[i]->sourceRect->Height; 
		}

		//Update missile
		_missile[i]->currentFrameTime += elapsedTime;

		if (_missile[i]->currentFrameTime > _missile[i]->frameTime)
		{
			_missile[i]->frameCount++;

			if (_missile[i]->frameCount >= 2)
			{
				_missile[i]->frameCount = 0;
			}

			_missile[i]->currentFrameTime = 0;
		}
		_missile[i]->sourceRect->X = _missile[i]->sourceRect->Width * _missile[i]->frameCount;
	}
}

void Pacman::CheckMunchieCollisions()
{
	//check for munchie collision
	for (int i = 0; i < MUNCHIECOUNT; i++) 
	{
		if (!_munchies[i]->eaten)
		{
			if (_munchies[i]->posRect->Intersects(*_pacman->positionRect))
			{
				_munchies[i]->eaten = true;
				Audio::Play(_pop);
				_pacman->shieldSize += 10.0f;
			}
		}
		 
		
	}
}


void Pacman::CheckMeteorCollisions()
{
	//check for meteor collision
	for (int i = 0; i < METEORCOUNT; i++)
	{
		if (!_pacman->dead)
		{
			if (!_pacman->shield)
			{
				if (_meteor[i]->posRect->Intersects(*_pacman->positionRect))
				{
					_pacman->dead = true;
					Audio::Play(_explosion);
				}
			}
		}

	}
	
}

void Pacman::CheckLargeMeteorCollisions()
{
	//check the large meteor collision
	for (int i = 0; i < LARGEMETEORCOUNT; i++)
	{
		if (!_pacman->dead)
		{
			if (!_pacman->shield)
			{
				if (_largeMeteor[i]->posRect->Intersects(*_pacman->positionRect))
				{
					_pacman->dead = true;
					Audio::Play(_explosion);
				}
			}
		}

	}
}

void Pacman::CheckMissileCollisions()
{
	//check missile collision
	for (int i = 0; i < MISSILECOUNT; i++)
	{
		if (!_pacman->dead)
		{
			if (!_pacman->shield)
			{
				if (_missile[i]->posRect->Intersects(*_pacman->positionRect))
				{
					_pacman->dead = true;
					Audio::Play(_explosion);
				}

			}
				
		}
	}
	
}

void Pacman::PacmanDead(Input::KeyboardState*state, Input::Keys restartKey)
{
	// restart the game
	if (_pacman->dead)
	{
		if (state->IsKeyDown(Input::Keys::R) && !_rKeyDown)
		{
			_rKeyDown = true;
			_pacman->shieldSize = 10.0f;
			_pacman->fuel = 15.0f;
			_pacman->score = 0;
			_pacman->dead = false;
			Reset();
		}
		if (state->IsKeyUp(Input::Keys::R))
		{
			_rKeyDown = false;
		}
		
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << " Shield Size: " << _pacman->shieldSize << " fuel remaining: " << _pacman->fuel << " YOUR SCORE: " << _pacman->score;

	SpriteBatch::BeginDraw(); // Starts Drawing

	SpriteBatch::Draw(_Background, _BackgroundRect, nullptr);
	//draws pacman
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->positionRect, _pacman->sourceRect);// Draws Pacman
	}
	//draws fuel cell
		for (int i = 0; i < _fuelCell.size(); i++)
		{	
			if (!_fuelCell[i]->collected)
			{
				SpriteBatch::Draw(_fuelCell[i]->texture, _fuelCell[i]->posRect, _fuelCell[i]->sourceRect);
			}
		}

	//draws meteor
	for (int i = 0; i < METEORCOUNT; i++)
	{
		SpriteBatch::Draw(_meteor[i]->texture, _meteor[i]->posRect, _meteor[i]->sourceRect);
	}
	// draws large meteor
	for (int i = 0; i < LARGEMETEORCOUNT; i++)
	{
		SpriteBatch::Draw(_largeMeteor[i]->texture, _largeMeteor[i]->posRect, _largeMeteor[i]->sourceRect);
	}
	// draws munchie
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		if (!_munchies[i]->eaten)
		{
			SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->posRect, _munchies[i]->sourceRect);
		}
	}
	
	// Draws missile
	for (int i = 0; i < MISSILECOUNT; i++)
	{
		SpriteBatch::Draw(_missile[i]->texture, _missile[i]->posRect, _missile[i]->sourceRect);
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	
	//Draws Start screen
	if (_menu->start)
	{
		std::stringstream menuStream;
		menuStream << "PRESS SPACE TO START";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition,
			Color::Red);
	}
	//Draws pause screen
	if (_menu->paused)
	{
		std::stringstream menuStream;
		menuStream << "          PAUSED!";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition,
			Color::Red);
	}
	//draws dead screen
	if (_pacman->dead && !_rKeyDown)
	{
		std::stringstream menuStream;
		menuStream << "YOUR DEAD, PRESS R TO RESTART\n";
		menuStream << "YOUR SCORE WAS " << _pacman->score;

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition,
			Color::Red);
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}