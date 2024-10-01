#include <mcpp/block.h>
#include <mcpp/mcpp.h>
#include <string>
#include <vector>

int main() {

    const int size_x = 10;
    const int size_z = 10;
    const int minecount = 15;

    long int seed = static_cast<long int>(time(NULL));

    int flagToWin = minecount;
    int clearsToWin = (size_x * size_z) - minecount;

    srand(seed);

    int board[size_x]
             [size_z]; // Defaults board states to 0 (meaning no mines nearby)
    int state[size_x][size_z]; // And 0 meaning no clear/flag
    for (int x = 0; x < size_x; x++) {
        for (int z = 0; z < size_z; z++) {
            board[x][z] = 0;
            state[x][z] = 0;
        }
    }

    mcpp::MinecraftConnection mc;
    mcpp::Coordinate orig;   // hols the location of the board origin
    mcpp::Coordinate corner; // holds the location of the opposite corner
    mcpp::Coordinate tpos;   // used for temporary positions
    mcpp::Coordinate cnr1;
    mcpp::Coordinate cnr2;

    mc.doCommand("gamerule sendCommandFeedback false");
    mc.doCommand("clear @p");
    mc.doCommand("give @p tnt");
    mc.doCommand("give @p sea_lantern");
    mc.doCommand("give @p barrier");
    mc.doCommand("gamerule sendCommandFeedback true");

    mc.postToChat("The TNT/Mine clears the spot below");
    mc.postToChat("The Sea_Lantern/Flag flags the spot below");
    mc.postToChat("The barrier exits the game");
    mc.postToChat("To play, place blocks above the board");

    std::vector<mcpp::BlockType> minecols = {
        mcpp::Blocks::WHITE_CONCRETE,  mcpp::Blocks::LIGHT_BLUE_CONCRETE,
        mcpp::Blocks::CYAN_CONCRETE,   mcpp::Blocks::GREEN_CONCRETE,
        mcpp::Blocks::LIME_CONCRETE,   mcpp::Blocks::YELLOW_CONCRETE,
        mcpp::Blocks::ORANGE_CONCRETE, mcpp::Blocks::RED_CONCRETE,
        mcpp::Blocks::BROWN_CONCRETE,  mcpp::Blocks::TNT};

    orig = mc.getPlayerTilePosition();

    corner = orig;
    corner.x += size_x - 1;
    corner.z += size_z - 1;

    tpos = orig;
    tpos.z--;
    tpos.y++;
    for (int i = 1; i < 10; i++) {
        mc.setBlock(tpos, minecols[i]);
        tpos.x++;
    }
    mc.setBlock(tpos, mcpp::Blocks::SEA_LANTERN);
    tpos.y++;
    mc.setBlock(tpos, mcpp::Blocks::GLOWSTONE);
    tpos.x--;
    mc.setBlock(tpos, mcpp::Blocks::EMERALD_BLOCK);

    mc.setBlocks(orig, corner, mcpp::Blocks::LIGHT_GRAY_CONCRETE);

    for (int i = 0; i < minecount; i++) {
        int x = rand() % size_x;
        int z = rand() % size_z;

        if (board[x][z] == 9)
            i--;
        else {
            board[x][z] = 9;
            for (int xmod = -1; xmod < 2; xmod++) {
                for (int zmod = -1; zmod < 2; zmod++) {
                    if ((x + xmod) >= 0 && (x + xmod) < size_x &&
                        (z + zmod) >= 0 && (z + zmod) < size_z) {
                        if (board[x + xmod][z + zmod] != 9) {
                            board[x + xmod][z + zmod] += 1;
                        }
                    }
                }
            }
        }
    }

    cnr1 = orig;
    cnr2 = corner;
    cnr1.y++;
    cnr2.y++;

    bool gameRunning = true;

    while (gameRunning) {
        mcpp::Chunk game = mc.getBlocks(cnr1, cnr2);
        for (int x = 0; x < size_x; x++) {
            for (int z = 0; z < size_z; z++) {

                if (game.get(x, 0, z) == mcpp::Blocks::TNT) {
                    tpos.x = orig.x + x;
                    tpos.y = orig.y;
                    tpos.z = orig.z + z;
                    tpos.y++;
                    mc.setBlock(tpos, mcpp::Blocks::AIR);
                    tpos.y--;

                    if (state[x][z] == 0) {
                        clearsToWin--;
                        state[x][z] = 1;
                        mc.setBlock(tpos, minecols.at(board[x][z]));

                        if (board[x][z] == 9) {
                            mc.postToChat("GameOver");
                            gameRunning = false;
                        } else if (board[x][z] == 0) {
                            std::vector<mcpp::Coordinate> zc;
                            zc.push_back(tpos);
                            mc.postToChat("Zero Clear");

                            for (size_t pos = 0; pos < zc.size(); pos++) {
                                mcpp::Coordinate cord = zc.at(pos);
                                for (int xmod = -1; xmod <= 1; xmod++) {
                                    for (int zmod = -1; zmod <= 1; zmod++) {
                                        if (cord.x + xmod >= orig.x &&
                                            cord.x + xmod <= corner.x &&
                                            cord.z + zmod >= orig.z &&
                                            cord.z + zmod <= corner.z) {
                                            tpos = cord;
                                            tpos.x += xmod;
                                            tpos.z += zmod;

                                            if (state[tpos.x - orig.x]
                                                     [tpos.z - orig.z] != 1) {
                                                clearsToWin--;
                                                if (board[tpos.x - orig.x]
                                                         [tpos.z - orig.z] ==
                                                    0) {
                                                    zc.push_back(tpos);
                                                }
                                            }
                                            mc.setBlock(
                                                tpos,
                                                minecols.at(
                                                    board[tpos.x - orig.x]
                                                         [tpos.z - orig.z]));
                                            state[tpos.x - orig.x]
                                                 [tpos.z - orig.z] = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else if (game.get(x, 0, z) == mcpp::Blocks::SEA_LANTERN) {
                    tpos.x = orig.x + x;
                    tpos.y = orig.y;
                    tpos.z = orig.z + z;

                    if (state[x][z] == 0) {
                        state[x][z] = 2;
                        mc.setBlock(tpos, mcpp::Blocks::SEA_LANTERN);
                        if (board[x][z] == 9)
                            flagToWin--;
                        else
                            flagToWin++;
                    } else if (state[x][z] == 2) {
                        state[x][z] = 0;
                        mc.setBlock(tpos, mcpp::Blocks::LIGHT_GRAY_CONCRETE);
                        if (board[x][z] == 9)
                            flagToWin++;
                        else
                            flagToWin--;
                    }

                    tpos.y++;
                    mc.setBlock(tpos, mcpp::Blocks::AIR);
                } else if (game.get(x, 0, z) == mcpp::Blocks::BARRIER) {
                    tpos.x = orig.x + x;
                    tpos.y = orig.y;
                    tpos.z = orig.z + z;
                    tpos.y++;
                    mc.setBlock(tpos, mcpp::Blocks::AIR);
                    gameRunning = false;
                    mc.postToChat("Quitting");
                }
            }
        }

        if (clearsToWin == 0 || flagToWin == 0) {
            mc.postToChat(std::to_string(flagToWin));
            mc.postToChat(std::to_string(clearsToWin));
            mc.postToChat("You Win!!");
            gameRunning = false;
        }
    }

    for (int x = 0; x < size_x; x++) {
        for (int z = 0; z < size_z; z++) {
            tpos = orig;
            tpos.x += x;
            tpos.z += z;

            if (state[x][z] == 2) {
                if (board[x][z] == 9) {
                    mc.setBlock(tpos, mcpp::Blocks::EMERALD_BLOCK);
                } else {
                    mc.setBlock(tpos, mcpp::Blocks::GLOWSTONE);
                }
            } else if (board[x][z] == 9) {
                mc.setBlock(tpos, mcpp::Blocks::TNT);
            }
        }
    }
}