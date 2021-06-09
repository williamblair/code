#pragma once
#include "SpriteComponent.h"
#include <string>
#include <vector>

// Exercise 2.3 - tilemap
class TileMapComponent : public SpriteComponent
{
public:
    TileMapComponent(class Actor* owner, int drawOrder = 100);
    virtual ~TileMapComponent();

    // tileSize: the width and height of a single tile in pixels from the
    //           loaded texture
    // fileName: CSV file containing the sprite tile index for that square
    // assumes SetTexture() has already been called
    bool LoadMap(const int tileSize,
                 const std::string& fileName);

    virtual void Draw(SDL_Renderer* renderer) override;

private:
    int mWidth; // number of tiles wide and tall
    int mHeight;
    int mTileSize; // width and height in px of a single tile
    struct Tile
    {
        int tileIndex;
        int texX; // offset within the tile sheet texture
        int texY;
        int posX; // position to draw on the screen
        int posY;
    };
    std::vector<std::vector<Tile>> mPositions;
};

