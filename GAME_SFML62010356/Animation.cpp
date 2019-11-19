#include "Animation.h"
#include<SFML/Graphics.hpp>
using namespace sf;
Animation::Animation(Texture* texture, Vector2u imageCount, float switchTime)
{
	this->imageCount = imageCount;
	this->switchTime = switchTime;
	totalTime = 0.0f;
	currentImage.x = 0;//Start from first image of texture

	uvRect.width = texture->getSize().x / (float)imageCount.x;//get width of a single image
	uvRect.height = texture->getSize().y / (float)imageCount.y;//get height of a single image
}

Animation::~Animation()
{
}

//deltaTime use as rate of increament
void Animation::Update(int row, float deltaTime, bool faceRight)//choose row to use and tell time for each frame
{
	currentImage.y = row;//set row to use
	totalTime += deltaTime;//working like millis() in arduino

	if (totalTime >= switchTime)//Change the image
	{
		totalTime -= switchTime;//don't use totalTime=0. This will make game smoother
		currentImage.x++;//change image
		if (currentImage.x >= imageCount.x)//Prevent out of bound
		{
			currentImage.x = 0;//return to first image of the row
		}

		uvRect.top = currentImage.y * uvRect.height;//shift image area that is being use
		if (faceRight)
		{
			uvRect.left = currentImage.x * uvRect.width;
			uvRect.width = abs(uvRect.width);
		}
		else
		{
			uvRect.left = (currentImage.x + 1) * abs(uvRect.width);//Start from right side of the image to make it reverse
			uvRect.width = -abs(uvRect.width);//Reverse the render image

		}
	}


}

bool Animation::playRow(int row, float deltaTime,bool faceRight)
{
	currentImage.y = row;//set row to use
	totalTime += deltaTime;//working like millis() in arduino
	const float switchTime = 0.5f;
	if (totalTime >= switchTime)//Change the image
	{
		totalTime -= switchTime;//don't use totalTime=0. This will make game smoother
		currentImage.x++;//change image
		if (currentImage.x >= imageCount.x)//If end of row, then stop playing
		{
			return false;
		}

		uvRect.top = currentImage.y * uvRect.height;//shift image area that is being use
		if (faceRight)
		{
			uvRect.left = currentImage.x * uvRect.width;
			uvRect.width = abs(uvRect.width);
		}
		else
		{
			uvRect.left = (currentImage.x + 1) * abs(uvRect.width);//Start from right side of the image to make it reverse
			uvRect.width = -abs(uvRect.width);//Reverse the render image

		}
	}
	return true;
}
