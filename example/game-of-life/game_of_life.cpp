// Conway's game of life
#include <chrono>
#include <mcpp/mcpp.h>
#include <thread>

class Life {
private:
  int width, depth;
  int** game; // 1 is active
  bool isRunning;
  int delay;

  const mcpp::BlockType PLAY_BLOCK = mcpp::Blocks::GREEN_CONCRETE;
  const mcpp::BlockType PAUSE_BLOCK = mcpp::Blocks::RED_CONCRETE;
  const mcpp::BlockType DEAD_BLOCK = mcpp::Blocks::BLACK_CONCRETE;
  const mcpp::BlockType ALIVE_BLOCK = mcpp::Blocks::WHITE_CONCRETE;
  const mcpp::BlockType SPEED_BLOCK = mcpp::Blocks::YELLOW_CONCRETE;
  const mcpp::BlockType SLOW_BLOCK = mcpp::Blocks::BLUE_CONCRETE;
  const mcpp::BlockType RANDOM_BLOCK = mcpp::Blocks::PINK_CONCRETE;
  const mcpp::BlockType CLEAR_BLOCK = mcpp::Blocks::BARRIER;

  mcpp::Coordinate buildPosition, bounds, upperBuildPosition, upperBounds;

  mcpp::MinecraftConnection mc;

  int GetNeighbors(int x, int y);

public:
  Life(int width, int depth);
  ~Life();

  void Update();

  void Pause() { isRunning = false; };
  void Play() { isRunning = true; };
  void Delay() { std::this_thread::sleep_for(std::chrono::milliseconds(delay)); };
};

int main(int argc, char* argv[]) {
  int width = 40;
  int depth = 40;
  if (argc >= 2) {
    width = atoi(argv[1]);
    depth = atoi(argv[1]);
  }
  if (argc >= 3) {
    depth = atoi(argv[2]);
  }

  Life life(width, depth);

  while (true)
    life.Update();

  return 0;
}

Life::Life(int width, int depth) : width(width), depth(depth), isRunning(false), delay(250) {
  buildPosition = mc.get_player_position();
  buildPosition.y -= 5;
  buildPosition.x -= width / 2;
  buildPosition.z -= depth / 2;

  bounds =
      mcpp::Coordinate(buildPosition.x + width - 1, buildPosition.y, buildPosition.z + depth - 1);
  upperBounds = mcpp::Coordinate(buildPosition.x + width - 1, buildPosition.y + 1,
                                 buildPosition.z + depth - 1);
  upperBuildPosition = mcpp::Coordinate(buildPosition.x, buildPosition.y + 1, buildPosition.z);

  game = new int*[width];
  for (int x = 0; x < width; x++) {
    game[x] = new int[depth];
    for (int y = 0; y < depth; y++) {
      game[x][y] = 0;
    }
  }

  mc.do_command("clear @p");
  mc.do_command("give @p minecraft:white_concrete");
  mc.do_command("give @p minecraft:black_concrete");
  mc.do_command("give @p minecraft:green_concrete");
  mc.do_command("give @p minecraft:red_concrete");
  mc.do_command("give @p minecraft:yellow_concrete");
  mc.do_command("give @p minecraft:blue_concrete");
  mc.do_command("give @p minecraft:pink_concrete");
  mc.do_command("give @p minecraft:barrier");
}

Life::~Life() {
  for (int x = 0; x < width; x++) {
    delete[] game[x];
  }
  delete[] game;
}

void Life::Update() {
  // check for actions
  mcpp::Chunk chunk = mc.get_blocks(upperBuildPosition, upperBounds);
  for (int x = 0; x < width; x++) {
    for (int z = 0; z < depth; z++) {
      mcpp::BlockType block = chunk.get(x, 0, z);

      mcpp::Coordinate position(upperBuildPosition.x + x, upperBuildPosition.y,
                                upperBuildPosition.z + z);
      if (block != mcpp::Blocks::AIR) {
        mc.set_block(position, mcpp::Blocks::AIR);
      }

      if (block == PLAY_BLOCK) {
        Play();
        mc.post_to_chat("Playing");
      } else if (block == PAUSE_BLOCK) {
        Pause();
        mc.post_to_chat("Pausing");
      } else if (block == ALIVE_BLOCK) {
        game[x][z] = 1;
      } else if (block == DEAD_BLOCK) {
        game[x][z] = 0;
      } else if (block == SPEED_BLOCK) {
        delay = std::max(delay / 2, 1);
        mc.post_to_chat("Speeding up");
      } else if (block == SLOW_BLOCK) {
        delay *= 2;
        mc.post_to_chat("Slowing down");
      } else if (block == RANDOM_BLOCK) {
        for (int x = 0; x < width; x++) {
          for (int z = 0; z < depth; z++) {
            game[x][z] = rand() % 2;
          }
        }
        mc.post_to_chat("Randomizing");
      } else if (block == CLEAR_BLOCK) {
        for (int x = 0; x < width; x++) {
          for (int z = 0; z < depth; z++) {
            game[x][z] = 0;
          }
        }
        mc.post_to_chat("Clearing");
      }
    }
  }

  if (isRunning) {
    // neighbor counts
    int** neighbors = new int*[width];
    for (int x = 0; x < width; x++) {
      neighbors[x] = new int[depth];
      for (int y = 0; y < depth; y++) {
        neighbors[x][y] = GetNeighbors(x, y);
      }
    }

    // next generation state
    for (int x = 0; x < width; x++) {
      for (int z = 0; z < depth; z++) {
        if (game[x][z] == 1) {
          if (neighbors[x][z] < 2) {
            game[x][z] = 0;
          } else if (neighbors[x][z] > 3) {
            game[x][z] = 0;
          }
        } else if (neighbors[x][z] == 3) {
          game[x][z] = 1;
        }
      }
    }

    for (int x = 0; x < width; x++) {
      delete[] neighbors[x];
    }
    delete[] neighbors;
  }

  // draw
  mc.set_blocks(buildPosition, bounds, DEAD_BLOCK);
  for (int x = 0; x < width; x++) {
    for (int z = 0; z < depth; z++) {
      mcpp::Coordinate position(buildPosition.x + x, buildPosition.y, buildPosition.z + z);
      if (game[x][z] == 1) {
        mc.set_block(position, ALIVE_BLOCK);
      }
    }
  }

  Delay();
}

int Life::GetNeighbors(int x, int y) {
  int alive = 0;

  int neighbors[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

  for (int i = 0; i < 8; i++) {
    int nx = x + neighbors[i][0];
    int ny = y + neighbors[i][1];

    if (nx >= 0 && nx < width && ny >= 0 && ny < depth && game[nx][ny] == 1) {
      alive++;
    }
  }

  return alive;
}