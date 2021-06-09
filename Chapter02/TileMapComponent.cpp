#include "TileMapComponent.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Math.h"
#include <fstream>
#include <sstream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
    :SpriteComponent(owner, drawOrder)
    ,mWidth(0)
    ,mHeight(0)
{
}

TileMapComponent::~TileMapComponent()
{
}

bool TileMapComponent::LoadMap(const int tileSize, const std::string& fileName)
{
    mTileSize = tileSize;
    if (mTexture == nullptr) {
        printf("mTexture is null\n");
        return false;
    }
    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        printf("Failed to open %s\n", fileName.c_str());
        return false;
    }
    std::string curLine;
    mWidth = -1;
    mHeight = 0;
    int curY = 0; // y position to render the tile
    while (std::getline(inFile, curLine))
    {
        int curInt;
        char comma;
        std::stringstream ss(curLine);
        std::vector<Tile> curRow;
        int curX = 0; // x position to render the tile
        while (ss >> curInt)
        {
            if (curInt < 0)
            {
                curRow.push_back({
                    -1, // tile index
                    0, // x and y clip within the texture
                    0,
                    0, // screen position relative to top left of tilesheet
                    0
                });
            }
            else
            {
                // calculate tile x,y position
                int tilesPerRow = mTexWidth / mTileSize;
                int offsetX = curInt % (tilesPerRow);
                int offsetY = curInt / tilesPerRow;
                // set pixel postion based on tile position
                offsetX *= tileSize;
                offsetY *= tileSize;
                // add tile to row vector
                curRow.push_back({
                    curInt, // tile index
                    offsetX, // x and y clip within the texture
                    offsetY,
                    curX, // screen position relative to top left of tilesheet
                    curY
                });
            }
            ss >> comma; // read delimiter
            curX += tileSize;
        }
        mWidth = curRow.size(); // count number of columns
        ++mHeight;  // count number of rows
        curY += tileSize;

        // add row to tile array
        mPositions.push_back(curRow);
    }

    return true;
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
    SDL_Rect posRect;
    SDL_Rect clipRect;

    // TODO - offset tile map position; current assumes 0,0
    for (std::vector<Tile>& row : mPositions)
    {
        for (Tile& tile : row)
        {
            if (tile.tileIndex < 0)
            {
                // don't draw if negative index
                continue;
            }
            
            posRect.x = tile.posX;
            posRect.y = tile.posY;
            posRect.w = mTileSize;
            posRect.h = mTileSize;

            clipRect.x = tile.texX;
            clipRect.y = tile.texY;
            clipRect.w = mTileSize;
            clipRect.h = mTileSize;
            
            if (posRect.x > 1024-posRect.w) {
                printf("Rendering past screen width\n");
            }
            if (posRect.y > 768-posRect.h) {
                printf("Rendering past screen height\n");
            }

    		SDL_RenderCopyEx(renderer,
    			mTexture,
    			&clipRect,
    			&posRect,
    			-Math::ToDegrees(mOwner->GetRotation()),
    			nullptr,
	    		SDL_FLIP_NONE);
        }
    }
}

