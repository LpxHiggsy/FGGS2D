#include "pacman.h"
#include <time.h>
#include <sstream>

void Pacman::CheckFuelCellCollisions()
{
	for (int i = 0; i < _fuelCell.size(); i++)
		//check for fuel collision
	{	
		if (!_fuelCell[i]->collected)
		{
			if (_fuelCell[i]->posRect->Intersects(*_pacman->positionRect))
			{
				_fuelCell[i]->collected = true;
				Audio::Play(_pop);
				_pacman->fuel += 5.0f;
			}
		}

	}
	

}

void Pacman::SpawnFuelCell(Input::MouseState* mouseState)
{
	{
		
		if (_pacman->fuel < 15 && mouseState->RightButton == Input::ButtonState::PRESSED)
		{
			_fuelCell.push_back(new fuel());
			_fuelCell[_fuelCell.size() - 1]->currentFrameTime = 0;
			_fuelCell[_fuelCell.size() - 1]->frameCount = rand() % 1;
			_fuelCell[_fuelCell.size() - 1]->frameTime = 100;
			_fuelCell[_fuelCell.size() - 1]->collected = false;
			_fuelCell[_fuelCell.size() - 1]->texture = new Texture2D();
			_fuelCell[_fuelCell.size() - 1]->texture->Load("Textures/fuelCell.png", true);
			_fuelCell[_fuelCell.size() - 1]->posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 15, 15);
			_fuelCell[_fuelCell.size() - 1]->sourceRect = new Rect(0.0f, 0.0f, 25, 25);
			_newSpawn = true;
		}
		if (mouseState->RightButton == Input::ButtonState::RELEASED)
		{
			_newSpawn = false;
		}

		
	}
}

void Pacman::DeleteFuelCell()
{
	for (int i = 0; i < _fuelCell.size(); i++)
	{
		if (_fuelCell[i]->collected)
		{
			delete _fuelCell[i]->posRect;
			delete _fuelCell[i]->sourceRect;
			delete _fuelCell[i];
			_fuelCell.erase(_fuelCell.begin() + i);
		}
	}
}

void Pacman::UpdateFuelCell(int elapsedTime)
{
	for (int i = 0; i < _fuelCell.size(); i++)	
	// fuel cell update
	{	
		_fuelCell[i]->currentFrameTime += elapsedTime;

			if (_fuelCell[i]->currentFrameTime > _fuelCell[i]->frameTime)
			{
				_fuelCell[i]->frameCount++;

				if (_fuelCell[i]->frameCount >= 2)
				{
					_fuelCell[i]->frameCount = 0;
				}

				_fuelCell[i]->currentFrameTime = 0;
			}

		_fuelCell[i]->sourceRect->X = _fuelCell[i]->sourceRect->Width * _fuelCell[i]->frameCount;

	}


}