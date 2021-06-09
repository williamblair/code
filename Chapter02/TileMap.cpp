#include "TileMap.h"
#include "Game.h"
#include "TileMapComponent.h"

TileMap::TileMap(Game* game)
    :Actor(game)
    ,mTile(nullptr)
{
}

bool TileMap::Load(const std::string& fileName, int renderDepth)
{
    mTile = new TileMapComponent(this, renderDepth);
    mTile->SetTexture(GetGame()->GetTexture("Assets/Tiles.png"));
    int tileSizePx = 32;
    if (!mTile->LoadMap(tileSizePx, fileName)) {
        return false;
    }
    return true;
}

