#include <mcpp/mcpp.h>

using namespace mcpp;

int main() {
  MinecraftConnection mc;

  // Post chat to Minecraft
  mc.post_to_chat("Hello, Minecraft!");
}
