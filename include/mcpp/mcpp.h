#pragma once

#include "block.h"
#include "chunk.h"
#include "coordinate.h"
#include "heightmap.h"

#include <memory>

/** @file
 * @brief MinecraftConnection class.
 *
 */

/**
 * @brief Namespace containing all the the mcpp library classes.
 *
 * The mcpp namespace includes classes and functions designed to facilitate
 * interaction with the Minecraft world through various server commands
 * and data manipulations.
 */
namespace mcpp {
// Forward declare to avoid polluting namespace
class SocketConnection;

const uint16_t MCPP_PORT = 4711;

class MinecraftConnection {
private:
  /// Handle to the socket connection.
  std::unique_ptr<SocketConnection> _conn;

public:
  /**
   * @brief Represents the main endpoint for interaction with the minecraft
   * world.
   *
   * @param address String address in IPV4 format, defaults to "localhost"
   * @param port Integer port to run on, defaults to 4711 as that is the port
   * for ELCI
   */
  explicit MinecraftConnection(const std::string& address = "localhost", uint16_t port = MCPP_PORT);

  // Declared here, defaulted in mcpp.cpp to allow for forward declare of
  // SocketConnection
  ~MinecraftConnection();

  // NOLINTBEGIN(readability-identifier-naming)
  /**
   * @brief Sends a message to the in-game chat, does not require a joined
   * player
   *
   * @param message
   */
  void postToChat(const std::string& message);

  /**
   * @brief Performs an in-game minecraft command. Players have to exist on
   * the server and should be server operators (default with ELCI)
   *
   * @param command Command string in the in-game format (e.g. "time set day")
   */
  void doCommand(const std::string& command);

  /**
   * @brief Sets player pos (block pos of lower half of playermodel) to
   * specified Coordinate
   *
   * @param pos Coordinate to set
   */
  void setPlayerPosition(const Coordinate& pos);

  /**
   * @brief Returns a coordinate representing player position (block pos of
   * lower half of playermodel)
   *
   * @return Coordinate of location
   */
  [[nodiscard]] Coordinate getPlayerPosition() const;

  /**
   * @brief Sets player position to be one above specified tile (i.e. tile =
   * block player is standing on)
   *
   * @param tile Coordinate to set
   */
  void setPlayerTilePosition(const Coordinate& tile);

  /**
   * @brief Returns the coordinate location of the block the player is
   * standing on
   *
   * @return Coordinate of location
   */
  [[nodiscard]] Coordinate getPlayerTilePosition() const;

  /**
   * @brief Sets block at Coordinate loc to the BlockType specified by
   * blockType
   *
   * @param loc
   * @param blockType
   */
  void setBlock(const Coordinate& loc, const BlockType& block_type);

  /**
   * @brief Sets a cuboid of blocks to the specified BlockType blockType, with
   * the corners of the cuboid provided by the Coordinate loc1 and loc2
   *
   * @param loc1
   * @param loc2
   * @param blockType
   */
  void setBlocks(const Coordinate& loc1, const Coordinate& loc2, const BlockType& block_type);

  /**
   * @brief Returns BlockType object from the specified Coordinate loc with
   * modifier
   *
   * @param loc
   * @return BlockType of the requested block
   */
  [[nodiscard]] BlockType
  getBlock(const Coordinate& loc) const; // NOLINT(readability-identifier-naming)

  /**
   * @brief Returns a 3D vector of the BlockTypes of the requested cuboid with
   * modifiers
   *
   * @param loc1 1st corner of the cuboid
   * @param loc2 2nd corner of the cuboid
   * @return Chunk containing the blocks in the specified area.
   */
  [[nodiscard]] Chunk getBlocks(const Coordinate& loc1, const Coordinate& loc2) const;

  /**
   * @brief Returns the height of the specific provided x and z coordinate
   *
   * ***IMPORTANT:***
   * DO NOT USE FOR LARGE AREAS, IT WILL BE VERY SLOW
   * USE getHeights() INSTEAD
   *
   * Gets the y-value of the highest non-air block at the specified (x, z)
   * coordinate.
   * @param x
   * @param z
   * @return Returns the integer y-height at the requested coordinate.
   */
  [[nodiscard]] int32_t getHeight(int x, int z) const;

  /**
   * @brief Provides a scaled option of the getHeight call to allow for
   * considerable performance gains.
   *
   * \par USE THIS instead of getHeight in a for loop.
   *
   * @param loc1
   * @param loc2
   * @return Returns a vector of integers representing the 2D area of heights.
   */
  [[nodiscard]] HeightMap getHeights(const Coordinate& loc1, const Coordinate& loc2) const;

  // NOLINTEND(readability-identifier-naming)
};
} // namespace mcpp
