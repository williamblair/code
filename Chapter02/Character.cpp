#include "Character.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Character::Character(Game* game)
	:Actor(game)
    ,mCurrAnim(0)
    ,mSpr(nullptr)
{
    // Test multiple animations (exercise 2.2)
    mSpr = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> characterAnims = {
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png"),
		game->GetTexture("Assets/Character07.png"),
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png"),
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png"),
	};
    std::vector<AnimRange> characterAnimRanges {
        {0,5,true}, // character walk frames
        {6,14,true}, // character jump frames
        {15,17,false} // character punch frames
    };
	mSpr->SetAnimTextures(characterAnims, characterAnimRanges);
    mSpr->SetAnimFPS(3);
}

void Character::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}

void Character::ProcessKeyboard(const uint8_t* state)
{
    static bool enterHeld = false;
    if (!state[SDL_SCANCODE_RETURN] && enterHeld)
    {
        mCurrAnim = (mCurrAnim + 1) % 3; // 3 animations total
        mSpr->SetAnim(mCurrAnim); // move to the next animation
        enterHeld = false;
    }
    else if (state[SDL_SCANCODE_RETURN])
    {
        enterHeld = true;
    }
}

