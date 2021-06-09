#pragma once
#include <string>
#include "Actor.h"

class TileMap : public Actor
{
public:
    TileMap(class Game* game);
    // Returns true on success, false on failure
    bool Load(const std::string& fileName, int renderDepth);
private:
    class TileMapComponent* mTile;
};
