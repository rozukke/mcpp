#include <mcpp/block.h>
#include <mcpp/mcpp.h>
#include <mcpp/util.h>
#include <algorithm>

// Change location here
mcpp::Coordinate ORIGIN {0, 0, 0};
int PYRAMID_HEIGHT = 50;

// Can declare globally
mcpp::MinecraftConnection mc;

void make_ring(mcpp::Coordinate base_pt, int side_len) {
    // Flat plane
    mc.setBlocks(
        base_pt, 
        base_pt + mcpp::Coordinate(side_len, 0, side_len), 
        mcpp::Blocks::SANDSTONE
    );

    // Air inside to make border
    base_pt = base_pt + mcpp::Coordinate(1, 0, 1);
    mc.setBlocks(
        base_pt, 
        base_pt + mcpp::Coordinate(side_len - 2, 0, side_len - 2), 
        mcpp::Blocks::AIR
    );
}

int main() {
    int pyramid_base_len = PYRAMID_HEIGHT * 2;

    // Get heights of build area
    mcpp::HeightMap heights = mc.getHeights(
        ORIGIN, 
        ORIGIN + mcpp::Coordinate(pyramid_base_len, 0, pyramid_base_len)
    );

    // Use minimum height of the area as the lowest point on the pyramid
    int min_height = *std::min_element(
        heights.begin(), 
        heights.end()
    );

    // Build rings, diminishing up to pyramid height
    mcpp::Coordinate base_pt = heights.base_pt();
    base_pt.y = min_height;
    int side_len = pyramid_base_len;
    for(int i = 0; i < PYRAMID_HEIGHT; i++) {
        make_ring(base_pt + mcpp::Coordinate(i, i, i), side_len - (i * 2));
    } 
}
