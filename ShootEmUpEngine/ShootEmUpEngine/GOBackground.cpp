#include "GameObject.h"



void GOBackground::Update(SDL_Rect gameScreenRect)
{
	int mouseX, mouseY, screenX, screenY, screenWidth, screenHeight;
	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_GetWindowPosition(window, &screenX, &screenY);
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);

	if (justDropped)
	{
		activeScreenRect = gameScreenRect;
		x = activeScreenRect.x;
		y = activeScreenRect.y;
		justDropped = false;
		scrollSpeed = 0.0f;
	}

	destRect.x = x + modX;
	destRect.y = y;
	destRect.w = gameScreenRect.w;
	destRect.h = gameScreenRect.h;

	lastFrameX = x;
	lastFrameY = y;

	for (size_t i = 0; i < UIComponents.size(); i++)
	{
		UIComponents[i]->Update(gameScreenRect);
	}


	if (isClicked)
	{
		((UITextBox*)UIComponents[0])->SetText(name);//update name
	}

	FileDialog* newFD = new FileDialog();
	if (isSelected)
	{
		if (((UITextBox*)UIComponents[0])->IsUpdated()) {
			name = ((UITextBox*)UIComponents[0])->GetText().c_str(); isUIUpdated = true; }
		else if (((UIButton*)UIComponents[1])->CheckClick()) {	SetNewImgFromFile(newFD->GetFile()); isUIUpdated = true; }
		else if (((UITextBox*)UIComponents[2])->IsUpdated()) { scrollSpeed = atof(((UITextBox*)UIComponents[2])->GetText().c_str()); isUIUpdated = true; }
	}
	delete newFD;


}
void GOBackground::Draw(int IscrollBarOffset)
{
	scrollBarOffset = IscrollBarOffset;

	modX = scrollBarOffset;
	modX %= destRect.w;
	if (modX > 0) destRect.x = destRect.w / modX, modX += 10;

	SDL_Point centre = { width / 2, height / 2 };
	SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotation, &centre, SDL_RendererFlip::SDL_FLIP_NONE);

	SDL_Rect secondBG = destRect;
	secondBG.x += secondBG.w;
	SDL_RenderCopyEx(renderer, texture, NULL, &secondBG, rotation, &centre, SDL_RendererFlip::SDL_FLIP_NONE);


	for (size_t i = 0; i < UIComponents.size(); i++)
	{
		UIComponents[i]->Draw();
	}
}