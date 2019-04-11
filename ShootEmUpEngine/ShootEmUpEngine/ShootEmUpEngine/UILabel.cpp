#include "UILabel.h"

UILabel::UILabel(int inputX, int inputY, std::string inputText, SDL_Renderer* gamerenderer, int inputAlignment, TTF_Font* font)//alignment: 0=left, 1=center
{
	x = inputX;
	y = inputY;
	text = inputText;
	renderer = gamerenderer;

	textColor = { 0, 0, 0, 255 };

	defaultFont = font;

	alignment = inputAlignment;

	drawText = TTF_RenderText_Solid(defaultFont, text.c_str(), textColor);
	textTex = SDL_CreateTextureFromSurface(renderer, drawText);//convert the surface to a texture so it can be drawn
	width = drawText->w;
	height = drawText->h;

}

UILabel::~UILabel()
{
	SDL_FreeSurface(drawText);
	drawText = nullptr;
	SDL_DestroyTexture(textTex);
	textTex = nullptr;
}

void UILabel::Update(SDL_Event* e)
{

}


void UILabel::Draw()
{
	SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);//set color to fill color

	//draw the text
	//create a rectangle for the text to be drawn in, this will not be the rectangle used for drawing the text box itself.
	SDL_Rect textRect;
	if (alignment == 0) {	textRect = { x, y, (drawText != nullptr) ? drawText->w : 0, drawText->h }; //input is: xpos, ypos, boxWidth, boxHeight
	}
	else{	textRect = { (drawText != nullptr) ? (x - (drawText->w/2)) : 0, y, (drawText != nullptr) ? drawText->w : 0, drawText->h }; //input is: xpos, ypos, boxWidth, boxHeight
	};

	SDL_RenderCopy(renderer, textTex, NULL, &textRect);
}