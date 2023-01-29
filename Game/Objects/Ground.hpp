#ifndef GROUND_H
#define GROUND_H

#include "Engine/Object.hpp"
#include <vector>
#include <deque>

class Ground : public Object
{
    public:
        Ground(float tileSize, int yCount, int xCount, float xSpeed);

        void Start() override;
        void Tick(const Window &window, float deltaTime) override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override;

    private:
        Texture* sheet;

        int xCount, yCount;
        float tileSize;
        float xSpeed;

        int texXCount, texYCount;
        
        // which image to display on each tile
        // everytime the view scroll enoughs, the column(first element for this vector) will be removed and width of the tile will be added to the x-coordinate
        std::deque<std::vector<int>> state;

        // index of soil tiles, random choosen
        std::vector<int> soilTiles;

        // index of floor tiles, random choosen
        std::vector<int> floorTiles;
};

#endif
