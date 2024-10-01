#include <mcpp/mcpp.h>

#define X_SIZE 100
#define Z_SIZE 100
#define MINE_COUNT 15

#define clear mcpp::Blocks::TNT
#define flag mcpp::Blocks::SEA_LANTERN
#define uncleared mcpp::Blocks::LIGHT_GRAY_CONCRETE
#define quit mcpp::Blocks::BARRIER
#define air mcpp::Blocks::AIR
#define goodmine mcpp::Blocks::EMERALD_BLOCK
#define badflag mcpp::Blocks::GLOWSTONE
#define badmine mcpp::Blocks::MAGMA_BLOCK

int main() {
    mcpp::MinecraftConnection mc;
    mcpp::Coordinate origin = mc.getPlayerTilePosition();
    srand(time(nullptr));

    // The third layer to the board has mine values in [0] and flag/clears in
    // [1]
    int board[X_SIZE][Z_SIZE][2] = {{{0}}};

    // Used later for the colours of the blocks, values 0-9
    std::vector<mcpp::BlockType> blocks = {
        mcpp::Blocks::WHITE_CONCRETE,  mcpp::Blocks::LIGHT_BLUE_CONCRETE,
        mcpp::Blocks::CYAN_CONCRETE,   mcpp::Blocks::GREEN_CONCRETE,
        mcpp::Blocks::LIME_CONCRETE,   mcpp::Blocks::YELLOW_CONCRETE,
        mcpp::Blocks::ORANGE_CONCRETE, mcpp::Blocks::RED_CONCRETE,
        mcpp::Blocks::BROWN_CONCRETE,  mcpp::Blocks::TNT};

    // Gives the player required items and lets them know how to play
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

    // Makes the board and legend
    mcpp::Coordinate cornerOrigin = origin;
    mcpp::Coordinate cornerOpposite = origin;

    cornerOpposite.x += X_SIZE - 1;
    cornerOpposite.z += Z_SIZE - 1;
    mc.setBlocks(cornerOrigin, cornerOpposite, uncleared);

    // Moves corners up for reuse later to get the board
    cornerOpposite.y++;
    cornerOrigin.y++;

    mcpp::Coordinate legendPrinter = origin;
    legendPrinter.z--;
    legendPrinter.y++;
    for (int i = 0; i <= 9; i++) {
        mc.setBlock(legendPrinter, blocks[i]);
        legendPrinter.x++;
    }
    mc.setBlock(legendPrinter, flag);
    legendPrinter.y++;
    mc.setBlock(legendPrinter, badflag);
    legendPrinter.x--;
    mc.setBlock(legendPrinter, goodmine);
    legendPrinter.y++;
    mc.setBlock(legendPrinter, badmine);

    // Makes the mines and increases the adjacent counts by 1
    for (int mines = 0; mines < MINE_COUNT; mines++) {
        int x = rand() % X_SIZE;
        int z = rand() % Z_SIZE;

        if (board[x][z][0] !=
            9) {                // Select a random place and if its not a mine
            board[x][z][0] = 9; // Make it a mine

            for (int xOfset = -1; xOfset <= 1;
                 xOfset++) { // For area 3x3 around mine
                if (x + xOfset >= 0 &&
                    x + xOfset < X_SIZE) { // if not out of bounds or
                    for (int zOfset = -1; zOfset <= 1;
                         zOfset++) { // a mine increase the count
                        if (z + zOfset >= 0 &&
                            z + zOfset < Z_SIZE) { // by 1. Area checked :
                            if (board[x + xOfset][z + zOfset][0] !=
                                9) {                                //  + + +
                                board[x + xOfset][z + zOfset][0]++; //  + 9 +
                            }                                       //  + + +
                        }
                    }
                }
            }
        } else { // If not a valid spot for a mine make another mine
            mines--;
        }
    }

    // Win condition, used to track if all squares that aren't mines are cleared
    int clears = X_SIZE * Z_SIZE - MINE_COUNT;

    mcpp::Coordinate printer = origin;
    mcpp::Chunk choices = mc.getBlocks(cornerOrigin, cornerOpposite);
    bool playing = true;

    while (playing) {
        choices = mc.getBlocks(cornerOrigin, cornerOpposite); // Get whole board
        for (int x = 0; x < X_SIZE; x++) {
            for (int z = 0; z < Z_SIZE; z++) { // For all locations in the board
                printer.x = origin.x + x;
                printer.z = origin.z + z;
                printer.y = origin.y; // Set printer pos

                if (choices.get(x, 0, z) == clear) {
                    if (board[x][z][1] ==
                        0) { // If selected to clear and not already cleared

                        mc.setBlock(printer, blocks[board[x][z][0]]); // clear

                        if (board[x][z][0] == 9) { // If a mine, you loose
                            mc.postToChat("GAMEOVER!");
                            playing = false;
                        } else if (board[x][z][0] ==
                                   0) { // If zero add location to a stack
                            std::vector<int*> stack = {new int[2]{x, z}};
                            unsigned int pos = 0;
                            while (
                                pos <
                                stack.size()) { // until all values in the stack
                                                // are checked keep stepping
                                int* back = stack.at(pos);
                                int zeroX = back[0]; // Reveals & Checks all
                                                     // values around a 0
                                int zeroZ = back[1]; // # # #
                                                     // # x #
                                for (int xo = -1; xo <= 1; xo++) { // # # #
                                    if (zeroX + xo >= 0 &&
                                        zeroX + xo < X_SIZE) {
                                        for (int zo = -1; zo <= 1; zo++) {
                                            if (zeroZ + zo >= 0 &&
                                                zeroZ + zo < Z_SIZE) {
                                                if (board[zeroX + xo]
                                                         [zeroZ + zo][0] == 0 &&
                                                    board[zeroX + xo]
                                                         [zeroZ + zo][1] != 2) {
                                                    // if 0 and not already
                                                    // cleared add to the stack
                                                    stack.push_back(
                                                        new int[2]{zeroX + xo,
                                                                   zeroZ + zo});
                                                }
                                                // No spots around a 0 are a
                                                // mine so clear them all
                                                printer.x =
                                                    origin.x + zeroX + xo;
                                                printer.z =
                                                    origin.z + zeroZ + zo;
                                                mc.setBlock(
                                                    printer,
                                                    blocks[board[zeroX + xo]
                                                                [zeroZ + zo]
                                                                [0]]);

                                                // If not already cleared step
                                                // towards everything being
                                                // cleared
                                                if (board[zeroX + xo]
                                                         [zeroZ + zo][1] != 2)
                                                    clears--;
                                                board[zeroX + xo][zeroZ +
                                                                  zo][1] =
                                                    2; // Sets spot to cleared
                                            }
                                        }
                                    }
                                }

                                pos++;
                            }
                        }
                        if (board[x][z][1] != 2)
                            clears--;
                        board[x][z][1] = 2;
                    }

                    printer.y++; // removes block above the board
                    mc.setBlock(printer, air);

                } else if (choices.get(x, 0, z) == flag) { // if set to flag
                    if (board[x][z][1] == 0) {             // If not a flag
                        board[x][z][1] = 1;                // make it a flag
                        mc.setBlock(printer, flag);
                    } else if (board[x][z][1] == 1) { // if a flag
                        board[x][z][1] = 0;           // make it not a flag
                        mc.setBlock(printer, uncleared);
                    }

                    printer.y++;
                    mc.setBlock(printer, air);

                } else if (choices.get(x, 0, z) == quit) {
                    printer.y++;
                    mc.setBlock(printer, air);
                    mc.postToChat("Quitting");
                    playing = false;
                }
            }
        }

        if (clears == 0) {
            mc.postToChat("YOU WIN!!");
            playing = false;
        }
    }

    printer = origin;
    for (int x = 0; x < X_SIZE; x++) {     // Steps through the whole board
        for (int z = 0; z < Z_SIZE; z++) { // shows what you did correctly and
            if (board[x][z][0] == 9) {     // incorrectly
                if (board[x][z][1] == 1) {
                    mc.setBlock(printer, goodmine);
                } else if (board[x][z][1] == 0) {
                    mc.setBlock(printer, clear);
                } else {
                    mc.setBlock(printer, badmine);
                }
            } else if (board[x][z][1] == 1) {
                mc.setBlock(printer, badflag);
            }
            printer.z++;
        }
        printer.z = origin.z;
        printer.x++;
    }
}