#include <mcpp/mcpp.h>
#include <unistd.h>

#define clear mcpp::Blocks::TNT
#define flag mcpp::Blocks::SEA_LANTERN
#define uncleared mcpp::Blocks::LIGHT_GRAY_CONCRETE
#define quit mcpp::Blocks::BARRIER
#define air mcpp::Blocks::AIR
#define goodmine mcpp::Blocks::EMERALD_BLOCK
#define badflag mcpp::Blocks::GLOWSTONE
#define badmine mcpp::Blocks::MAGMA_BLOCK

class Minesweeper {
  private:
    const int X_SIZE = 10;
    const int Z_SIZE = 10;
    const int MINE_COUNT = 5;

    int*** field;
    int clearstowin;
    bool playing;

    std::vector<mcpp::BlockType> blocks;

    mcpp::MinecraftConnection mc;
    mcpp::Coordinate origin;
    mcpp::Coordinate cornerOrigin;
    mcpp::Coordinate cornerOpposite;
    mcpp::Coordinate printer;

  public:
    Minesweeper();
    bool Playing();
    void Flag(int x, int z);
    void Clear(int x, int z);
    void ZeroClear(int x, int z);
    void Reveal();
};

int main() {
    Minesweeper ms;
    while (ms.Playing()) {
        // std::cout<<"Tick"<<std::endl;
        usleep(5000);
    }
    ms.Reveal();
}

Minesweeper::Minesweeper() {
    // field[X_SIZE][Z_SIZE][STATE]
    // State [0] values 0-9 representing how many mines around it, 9=mine
    // State [1] (0) = none (1) = flag (2) = cleared
    field = new int**[X_SIZE];
    for (int i = 0; i < X_SIZE; i++) {
        field[i] = new int*[Z_SIZE];
        for (int j = 0; j < Z_SIZE; j++) {
            field[i][j] = new int[2]{0, 0};
        }
    }
    // Block Representations numbers 0-9
    blocks = {mcpp::Blocks::WHITE_CONCRETE,  mcpp::Blocks::LIGHT_BLUE_CONCRETE,
              mcpp::Blocks::CYAN_CONCRETE,   mcpp::Blocks::GREEN_CONCRETE,
              mcpp::Blocks::LIME_CONCRETE,   mcpp::Blocks::YELLOW_CONCRETE,
              mcpp::Blocks::ORANGE_CONCRETE, mcpp::Blocks::RED_CONCRETE,
              mcpp::Blocks::BROWN_CONCRETE,  mcpp::Blocks::TNT};

    // Gives player items to play
    mc.doCommand("gamerule sendCommandFeedback false");
    mc.doCommand("clear @p");
    mc.doCommand("give @p tnt");
    mc.doCommand("give @p sea_lantern");
    mc.doCommand("give @p barrier");
    mc.doCommand("gamerule sendCommandFeedback true");
    // Lets player know how to play
    mc.postToChat("The TNT/Mine clears the spot below");
    mc.postToChat("The Sea_Lantern/Flag flags the spot below");
    mc.postToChat("The barrier exits the game");
    mc.postToChat("To play, place blocks above the board");

    // Builds board
    origin = mc.getPlayerTilePosition();
    cornerOrigin = origin;
    cornerOpposite = origin;
    cornerOpposite.x += X_SIZE - 1;
    cornerOpposite.z += Z_SIZE - 1;
    mc.setBlocks(cornerOrigin, cornerOpposite, uncleared);

    // Moves corners up for later use in grabbing game area;
    cornerOpposite.y++;
    cornerOrigin.y++;

    // Prints the legend
    printer = origin;
    printer.z--;
    printer.y++;
    for (int i = 0; i <= 9; i++) {
        mc.setBlock(printer, blocks[i]);
        printer.x++;
    }
    mc.setBlock(printer, flag);
    printer.y++;
    mc.setBlock(printer, badflag);
    printer.x--;
    mc.setBlock(printer, goodmine);
    printer.y++;
    mc.setBlock(printer, badmine);

    // Generates mines and increases values around mines by 1
    srand(time(nullptr));
    for (int mines = 0; mines < MINE_COUNT; mines++) {
        int x = rand() % X_SIZE;
        int z = rand() % Z_SIZE;

        if (field[x][z][0] != 9) {
            field[x][z][0] = 9;

            for (int xOfset = -1; xOfset <= 1; xOfset++) {
                if (x + xOfset >= 0 && x + xOfset < X_SIZE) {
                    for (int zOfset = -1; zOfset <= 1; zOfset++) {
                        if (z + zOfset >= 0 && z + zOfset < Z_SIZE) {
                            if (field[x + xOfset][z + zOfset][0] != 9) {
                                field[x + xOfset][z + zOfset][0]++;
                            }
                        }
                    }
                }
            }
        } else { // If a mine is already in the place make another mine
            mines--;
        }
    }
    // This value counts down until you are finished clearing the field
    clearstowin = X_SIZE * Z_SIZE - MINE_COUNT;

    playing = true;
}

bool Minesweeper::Playing() {
    mcpp::Chunk choices = mc.getBlocks(cornerOrigin, cornerOpposite);
    for (int x = 0; x < X_SIZE; x++) {
        for (int z = 0; z < Z_SIZE; z++) {
            printer.x = origin.x + x;
            printer.z = origin.z + z;
            printer.y = origin.y; // Set printer pos

            if (choices.get(x, 0, z) == clear) {
                Clear(x, z);
            } else if (choices.get(x, 0, z) == flag) {
                Flag(x, z);
            } else if (choices.get(x, 0, z) == quit) {
                playing = false;
                printer.y++;
                mc.setBlock(printer, air);
                mc.postToChat("Quitting");
            }
        }
    }

    if (clearstowin == 0) {
        mc.postToChat("YOU WIN!!");
        playing = false;
    }

    return playing;
}

void Minesweeper::Flag(int x, int z) {
    if (field[x][z][1] == 0) {
        field[x][z][1] = 1;
        mc.setBlock(printer, flag);
    } else if (field[x][z][1] == 1) {
        field[x][z][1] = 0;
        mc.setBlock(printer, uncleared);
    }

    printer.y++;
    mc.setBlock(printer, air);
}

void Minesweeper::Clear(int x, int z) {
    if (field[x][z][1] == 0) {

        mc.setBlock(printer, blocks[field[x][z][0]]);

        if (field[x][z][0] == 9) { // If a mine, you loose
            mc.postToChat("GAMEOVER!");
            playing = false;
        } else if (field[x][z][0] == 0) { // If zero add location to a stack
            ZeroClear(x, z);
        }

        if (field[x][z][1] != 2) {
            clearstowin--;
        }

        field[x][z][1] = 2;
    }

    printer.y++; // removes block above the board
    mc.setBlock(printer, air);
}

void Minesweeper::ZeroClear(int x, int z) {
    std::vector<int*> stack = {new int[2]{x, z}};
    size_t pos = 0;

    while (pos < stack.size()) {
        int* zeroCoordinate = stack.at(pos);
        int xC = zeroCoordinate[0];
        int zC = zeroCoordinate[1];
        for (int xo = -1; xo <= 1; xo++) {
            for (int zo = -1; zo <= 1; zo++) {
                if (xC + xo >= 0 && xC + xo < X_SIZE && zC + zo >= 0 &&
                    zC + zo < Z_SIZE) {

                    if (field[xC + xo][zC + zo][0] == 0 &&
                        field[xC + xo][zC + zo][1] != 2) {
                        stack.push_back(new int[2]{xC + xo, zC + zo});
                    }

                    printer.x = origin.x + xC + xo;
                    printer.z = origin.z + zC + zo;
                    mc.setBlock(printer, blocks[field[xC + xo][zC + zo][0]]);

                    if (field[xC + xo][zC + zo][1] != 2) {
                        clearstowin--;
                    }

                    field[xC + xo][zC + zo][1] = 2;
                }
            }
        }
        pos++;
    }
}

void Minesweeper::Reveal() {
    printer = origin;
    for (int x = 0; x < X_SIZE; x++) {     // Steps through the whole board
        for (int z = 0; z < Z_SIZE; z++) { // shows what you did correctly and
            if (field[x][z][0] == 9) {     // incorrectly
                if (field[x][z][1] == 1) {
                    mc.setBlock(printer, goodmine);
                } else if (field[x][z][1] == 0) {
                    mc.setBlock(printer, clear);
                } else {
                    mc.setBlock(printer, badmine);
                }
            } else if (field[x][z][1] == 1) {
                mc.setBlock(printer, badflag);
            }
            printer.z++;
        }
        printer.z = origin.z;
        printer.x++;
    }
}