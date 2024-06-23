#pragma once

#include <ostream>

/**
 * @file
 * @brief BlockType class
 */
namespace mcpp {
class BlockType {
  public:
    int id;
    int mod;

    constexpr BlockType(int id = 0, int modifier = 0) : id(id), mod(modifier){};

    /**
     * @brief Equality comparison operator.
     *
     * Watch out as this also compares the BlockType.mod element of the block,
     * so some equalities may behave in unexpected ways e.g. rotated stairs
     *
     * @param other The BlockType to compare with the current instance.
     *
     * @return True if the two BlockType instances are not equal, false
     * otherwise.
     */
    bool operator==(const BlockType& other) const;

    /**
     * @brief Inequality comparison operator.
     *
     * Watch out as this also compares the BlockType.mod element of the block,
     * so some equalities may behave in unexpected ways e.g. rotated stairs
     *
     * @param other The BlockType to compare with the current instance.
     * @return True if the two BlockType instances are not equal, false
     * otherwise.
     */
    bool operator!=(const BlockType& other) const;

    /**
     * @brief Stream insertion operator for outputting the BlockType to an
     * output stream.
     *
     * @param out The output stream to write to.
     * @param block The BlockType instance to output.
     * @return A reference to the output stream after the block information has
     * been inserted.
     */
    friend std::ostream& operator<<(std::ostream& out, const BlockType& block);

    /**
     * Returns a new BlockType with the same id and specified modifier, useful
     * for rotations etc.
     *
     * @param modifier New modifier for the BlockType
     * @return New BlockType object with the specified modifier
     */
    [[nodiscard]] BlockType withMod(int modifier) const;
};

// Using script to extract ids from https://minecraft-ids.grahamedgecombe.com/

/**
 * @struct Blocks
 * Struct of static block objects that allows for a "search" of sorts, callable
 * using Blocks::TYPE after importing <block.h>
 */
struct Blocks {
    static constexpr BlockType AIR = BlockType(0);
    static constexpr BlockType STONE = BlockType(1);
    static constexpr BlockType GRANITE = BlockType(1, 1);
    static constexpr BlockType POLISHED_GRANITE = BlockType(1, 2);
    static constexpr BlockType DIORITE = BlockType(1, 3);
    static constexpr BlockType POLISHED_DIORITE = BlockType(1, 4);
    static constexpr BlockType ANDESITE = BlockType(1, 5);
    static constexpr BlockType POLISHED_ANDESITE = BlockType(1, 6);
    static constexpr BlockType GRASS = BlockType(2);
    static constexpr BlockType DIRT = BlockType(3);
    static constexpr BlockType COARSE_DIRT = BlockType(3, 1);
    static constexpr BlockType PODZOL = BlockType(3, 2);
    static constexpr BlockType COBBLESTONE = BlockType(4);
    static constexpr BlockType OAK_WOOD_PLANK = BlockType(5);
    static constexpr BlockType SPRUCE_WOOD_PLANK = BlockType(5, 1);
    static constexpr BlockType BIRCH_WOOD_PLANK = BlockType(5, 2);
    static constexpr BlockType JUNGLE_WOOD_PLANK = BlockType(5, 3);
    static constexpr BlockType ACACIA_WOOD_PLANK = BlockType(5, 4);
    static constexpr BlockType DARK_OAK_WOOD_PLANK = BlockType(5, 5);
    static constexpr BlockType OAK_SAPLING = BlockType(6);
    static constexpr BlockType SPRUCE_SAPLING = BlockType(6, 1);
    static constexpr BlockType BIRCH_SAPLING = BlockType(6, 2);
    static constexpr BlockType JUNGLE_SAPLING = BlockType(6, 3);
    static constexpr BlockType ACACIA_SAPLING = BlockType(6, 4);
    static constexpr BlockType DARK_OAK_SAPLING = BlockType(6, 5);
    static constexpr BlockType BEDROCK = BlockType(7);
    static constexpr BlockType FLOWING_WATER = BlockType(8);
    static constexpr BlockType STILL_WATER = BlockType(9);
    static constexpr BlockType FLOWING_LAVA = BlockType(10);
    static constexpr BlockType STILL_LAVA = BlockType(11);
    static constexpr BlockType SAND = BlockType(12);
    static constexpr BlockType RED_SAND = BlockType(12, 1);
    static constexpr BlockType GRAVEL = BlockType(13);
    static constexpr BlockType GOLD_ORE = BlockType(14);
    static constexpr BlockType IRON_ORE = BlockType(15);
    static constexpr BlockType COAL_ORE = BlockType(16);
    static constexpr BlockType OAK_WOOD = BlockType(17);
    static constexpr BlockType SPRUCE_WOOD = BlockType(17, 1);
    static constexpr BlockType BIRCH_WOOD = BlockType(17, 2);
    static constexpr BlockType JUNGLE_WOOD = BlockType(17, 3);
    static constexpr BlockType OAK_LEAVES = BlockType(18);
    static constexpr BlockType SPRUCE_LEAVES = BlockType(18, 1);
    static constexpr BlockType BIRCH_LEAVES = BlockType(18, 2);
    static constexpr BlockType JUNGLE_LEAVES = BlockType(18, 3);
    static constexpr BlockType SPONGE = BlockType(19);
    static constexpr BlockType WET_SPONGE = BlockType(19, 1);
    static constexpr BlockType GLASS = BlockType(20);
    static constexpr BlockType LAPIS_LAZULI_ORE = BlockType(21);
    static constexpr BlockType LAPIS_LAZULI_BLOCK = BlockType(22);
    static constexpr BlockType DISPENSER = BlockType(23);
    static constexpr BlockType SANDSTONE = BlockType(24);
    static constexpr BlockType CHISELED_SANDSTONE = BlockType(24, 1);
    static constexpr BlockType SMOOTH_SANDSTONE = BlockType(24, 2);
    static constexpr BlockType NOTE_BLOCK = BlockType(25);
    static constexpr BlockType BED = BlockType(26);
    static constexpr BlockType POWERED_RAIL = BlockType(27);
    static constexpr BlockType DETECTOR_RAIL = BlockType(28);
    static constexpr BlockType STICKY_PISTON = BlockType(29);
    static constexpr BlockType COBWEB = BlockType(30);
    static constexpr BlockType DEAD_SHRUB = BlockType(31);
    static constexpr BlockType TALL_GRASS = BlockType(31, 1);
    static constexpr BlockType FERN = BlockType(31, 2);
    static constexpr BlockType DEAD_BUSH = BlockType(32);
    static constexpr BlockType PISTON = BlockType(33);
    static constexpr BlockType PISTON_HEAD = BlockType(34);
    static constexpr BlockType WHITE_WOOL = BlockType(35);
    static constexpr BlockType ORANGE_WOOL = BlockType(35, 1);
    static constexpr BlockType MAGENTA_WOOL = BlockType(35, 2);
    static constexpr BlockType LIGHT_BLUE_WOOL = BlockType(35, 3);
    static constexpr BlockType YELLOW_WOOL = BlockType(35, 4);
    static constexpr BlockType LIME_WOOL = BlockType(35, 5);
    static constexpr BlockType PINK_WOOL = BlockType(35, 6);
    static constexpr BlockType GRAY_WOOL = BlockType(35, 7);
    static constexpr BlockType LIGHT_GRAY_WOOL = BlockType(35, 8);
    static constexpr BlockType CYAN_WOOL = BlockType(35, 9);
    static constexpr BlockType PURPLE_WOOL = BlockType(35, 10);
    static constexpr BlockType BLUE_WOOL = BlockType(35, 11);
    static constexpr BlockType BROWN_WOOL = BlockType(35, 12);
    static constexpr BlockType GREEN_WOOL = BlockType(35, 13);
    static constexpr BlockType RED_WOOL = BlockType(35, 14);
    static constexpr BlockType BLACK_WOOL = BlockType(35, 15);
    static constexpr BlockType DANDELION = BlockType(37);
    static constexpr BlockType POPPY = BlockType(38);
    static constexpr BlockType BLUE_ORCHID = BlockType(38, 1);
    static constexpr BlockType ALLIUM = BlockType(38, 2);
    static constexpr BlockType AZURE_BLUET = BlockType(38, 3);
    static constexpr BlockType RED_TULIP = BlockType(38, 4);
    static constexpr BlockType ORANGE_TULIP = BlockType(38, 5);
    static constexpr BlockType WHITE_TULIP = BlockType(38, 6);
    static constexpr BlockType PINK_TULIP = BlockType(38, 7);
    static constexpr BlockType OXEYE_DAISY = BlockType(38, 8);
    static constexpr BlockType BROWN_MUSHROOM = BlockType(39);
    static constexpr BlockType RED_MUSHROOM = BlockType(40);
    static constexpr BlockType GOLD_BLOCK = BlockType(41);
    static constexpr BlockType IRON_BLOCK = BlockType(42);
    static constexpr BlockType DOUBLE_STONE_SLAB = BlockType(43);
    static constexpr BlockType DOUBLE_SANDSTONE_SLAB = BlockType(43, 1);
    static constexpr BlockType DOUBLE_WOODEN_SLAB = BlockType(43, 2);
    static constexpr BlockType DOUBLE_COBBLESTONE_SLAB = BlockType(43, 3);
    static constexpr BlockType DOUBLE_BRICK_SLAB = BlockType(43, 4);
    static constexpr BlockType DOUBLE_STONE_BRICK_SLAB = BlockType(43, 5);
    static constexpr BlockType DOUBLE_NETHER_BRICK_SLAB = BlockType(43, 6);
    static constexpr BlockType DOUBLE_QUARTZ_SLAB = BlockType(43, 7);
    static constexpr BlockType STONE_SLAB = BlockType(44);
    static constexpr BlockType SANDSTONE_SLAB = BlockType(44, 1);
    static constexpr BlockType WOODEN_SLAB = BlockType(44, 2);
    static constexpr BlockType COBBLESTONE_SLAB = BlockType(44, 3);
    static constexpr BlockType BRICK_SLAB = BlockType(44, 4);
    static constexpr BlockType STONE_BRICK_SLAB = BlockType(44, 5);
    static constexpr BlockType NETHER_BRICK_SLAB = BlockType(44, 6);
    static constexpr BlockType QUARTZ_SLAB = BlockType(44, 7);
    static constexpr BlockType BRICKS = BlockType(45);
    static constexpr BlockType TNT = BlockType(46);
    static constexpr BlockType BOOKSHELF = BlockType(47);
    static constexpr BlockType MOSS_STONE = BlockType(48);
    static constexpr BlockType OBSIDIAN = BlockType(49);
    static constexpr BlockType TORCH = BlockType(50);
    static constexpr BlockType FIRE = BlockType(51);
    static constexpr BlockType MONSTER_SPAWNER = BlockType(52);
    static constexpr BlockType OAK_WOOD_STAIRS = BlockType(53);
    static constexpr BlockType CHEST = BlockType(54);
    static constexpr BlockType REDSTONE_WIRE = BlockType(55);
    static constexpr BlockType DIAMOND_ORE = BlockType(56);
    static constexpr BlockType DIAMOND_BLOCK = BlockType(57);
    static constexpr BlockType CRAFTING_TABLE = BlockType(58);
    static constexpr BlockType WHEAT_CROPS = BlockType(59);
    static constexpr BlockType FARMLAND = BlockType(60);
    static constexpr BlockType FURNACE = BlockType(61);
    static constexpr BlockType BURNING_FURNACE = BlockType(62);
    static constexpr BlockType STANDING_SIGN_BLOCK = BlockType(63);
    static constexpr BlockType OAK_DOOR_BLOCK = BlockType(64);
    static constexpr BlockType LADDER = BlockType(65);
    static constexpr BlockType RAIL = BlockType(66);
    static constexpr BlockType COBBLESTONE_STAIRS = BlockType(67);
    static constexpr BlockType WALLMOUNTED_SIGN_BLOCK = BlockType(68);
    static constexpr BlockType LEVER = BlockType(69);
    static constexpr BlockType STONE_PRESSURE_PLATE = BlockType(70);
    static constexpr BlockType IRON_DOOR_BLOCK = BlockType(71);
    static constexpr BlockType WOODEN_PRESSURE_PLATE = BlockType(72);
    static constexpr BlockType REDSTONE_ORE = BlockType(73);
    static constexpr BlockType GLOWING_REDSTONE_ORE = BlockType(74);
    static constexpr BlockType REDSTONE_TORCH_OFF = BlockType(75);
    static constexpr BlockType REDSTONE_TORCH_ON = BlockType(76);
    static constexpr BlockType STONE_BUTTON = BlockType(77);
    static constexpr BlockType SNOW = BlockType(78);
    static constexpr BlockType ICE = BlockType(79);
    static constexpr BlockType SNOW_BLOCK = BlockType(80);
    static constexpr BlockType CACTUS = BlockType(81);
    static constexpr BlockType CLAY = BlockType(82);
    static constexpr BlockType SUGAR_CANES = BlockType(83);
    static constexpr BlockType JUKEBOX = BlockType(84);
    static constexpr BlockType OAK_FENCE = BlockType(85);
    static constexpr BlockType PUMPKIN = BlockType(86);
    static constexpr BlockType NETHERRACK = BlockType(87);
    static constexpr BlockType SOUL_SAND = BlockType(88);
    static constexpr BlockType GLOWSTONE = BlockType(89);
    static constexpr BlockType NETHER_PORTAL = BlockType(90);
    static constexpr BlockType JACK_OLANTERN = BlockType(91);
    static constexpr BlockType CAKE_BLOCK = BlockType(92);
    static constexpr BlockType REDSTONE_REPEATER_BLOCK_OFF = BlockType(93);
    static constexpr BlockType REDSTONE_REPEATER_BLOCK_ON = BlockType(94);
    static constexpr BlockType WHITE_STAINED_GLASS = BlockType(95);
    static constexpr BlockType ORANGE_STAINED_GLASS = BlockType(95, 1);
    static constexpr BlockType MAGENTA_STAINED_GLASS = BlockType(95, 2);
    static constexpr BlockType LIGHT_BLUE_STAINED_GLASS = BlockType(95, 3);
    static constexpr BlockType YELLOW_STAINED_GLASS = BlockType(95, 4);
    static constexpr BlockType LIME_STAINED_GLASS = BlockType(95, 5);
    static constexpr BlockType PINK_STAINED_GLASS = BlockType(95, 6);
    static constexpr BlockType GRAY_STAINED_GLASS = BlockType(95, 7);
    static constexpr BlockType LIGHT_GRAY_STAINED_GLASS = BlockType(95, 8);
    static constexpr BlockType CYAN_STAINED_GLASS = BlockType(95, 9);
    static constexpr BlockType PURPLE_STAINED_GLASS = BlockType(95, 10);
    static constexpr BlockType BLUE_STAINED_GLASS = BlockType(95, 11);
    static constexpr BlockType BROWN_STAINED_GLASS = BlockType(95, 12);
    static constexpr BlockType GREEN_STAINED_GLASS = BlockType(95, 13);
    static constexpr BlockType RED_STAINED_GLASS = BlockType(95, 14);
    static constexpr BlockType BLACK_STAINED_GLASS = BlockType(95, 15);
    static constexpr BlockType WOODEN_TRAPDOOR = BlockType(96);
    static constexpr BlockType STONE_MONSTER_EGG = BlockType(97);
    static constexpr BlockType COBBLESTONE_MONSTER_EGG = BlockType(97, 1);
    static constexpr BlockType STONE_BRICK_MONSTER_EGG = BlockType(97, 2);
    static constexpr BlockType MOSSY_STONE_BRICK_MONSTER_EGG = BlockType(97, 3);
    static constexpr BlockType CRACKED_STONE_BRICK_MONSTER_EGG =
        BlockType(97, 4);
    static constexpr BlockType CHISELED_STONE_BRICK_MONSTER_EGG =
        BlockType(97, 5);
    static constexpr BlockType STONE_BRICKS = BlockType(98);
    static constexpr BlockType MOSSY_STONE_BRICKS = BlockType(98, 1);
    static constexpr BlockType CRACKED_STONE_BRICKS = BlockType(98, 2);
    static constexpr BlockType CHISELED_STONE_BRICKS = BlockType(98, 3);
    static constexpr BlockType BROWN_MUSHROOM_BLOCK = BlockType(99);
    static constexpr BlockType RED_MUSHROOM_BLOCK = BlockType(100);
    static constexpr BlockType IRON_BARS = BlockType(101);
    static constexpr BlockType GLASS_PANE = BlockType(102);
    static constexpr BlockType MELON_BLOCK = BlockType(103);
    static constexpr BlockType PUMPKIN_STEM = BlockType(104);
    static constexpr BlockType MELON_STEM = BlockType(105);
    static constexpr BlockType VINES = BlockType(106);
    static constexpr BlockType OAK_FENCE_GATE = BlockType(107);
    static constexpr BlockType BRICK_STAIRS = BlockType(108);
    static constexpr BlockType STONE_BRICK_STAIRS = BlockType(109);
    static constexpr BlockType MYCELIUM = BlockType(110);
    static constexpr BlockType LILY_PAD = BlockType(111);
    static constexpr BlockType NETHER_BRICK = BlockType(112);
    static constexpr BlockType NETHER_BRICK_FENCE = BlockType(113);
    static constexpr BlockType NETHER_BRICK_STAIRS = BlockType(114);
    static constexpr BlockType NETHER_WART = BlockType(115);
    static constexpr BlockType ENCHANTMENT_TABLE = BlockType(116);
    static constexpr BlockType BREWING_STAND = BlockType(117);
    static constexpr BlockType CAULDRON = BlockType(118);
    static constexpr BlockType END_PORTAL = BlockType(119);
    static constexpr BlockType END_PORTAL_FRAME = BlockType(120);
    static constexpr BlockType END_STONE = BlockType(121);
    static constexpr BlockType DRAGON_EGG = BlockType(122);
    static constexpr BlockType REDSTONE_LAMP_INACTIVE = BlockType(123);
    static constexpr BlockType REDSTONE_LAMP_ACTIVE = BlockType(124);
    static constexpr BlockType DOUBLE_OAK_WOOD_SLAB = BlockType(125);
    static constexpr BlockType DOUBLE_SPRUCE_WOOD_SLAB = BlockType(125, 1);
    static constexpr BlockType DOUBLE_BIRCH_WOOD_SLAB = BlockType(125, 2);
    static constexpr BlockType DOUBLE_JUNGLE_WOOD_SLAB = BlockType(125, 3);
    static constexpr BlockType DOUBLE_ACACIA_WOOD_SLAB = BlockType(125, 4);
    static constexpr BlockType DOUBLE_DARK_OAK_WOOD_SLAB = BlockType(125, 5);
    static constexpr BlockType OAK_WOOD_SLAB = BlockType(126);
    static constexpr BlockType SPRUCE_WOOD_SLAB = BlockType(126, 1);
    static constexpr BlockType BIRCH_WOOD_SLAB = BlockType(126, 2);
    static constexpr BlockType JUNGLE_WOOD_SLAB = BlockType(126, 3);
    static constexpr BlockType ACACIA_WOOD_SLAB = BlockType(126, 4);
    static constexpr BlockType DARK_OAK_WOOD_SLAB = BlockType(126, 5);
    static constexpr BlockType COCOA = BlockType(127);
    static constexpr BlockType SANDSTONE_STAIRS = BlockType(128);
    static constexpr BlockType EMERALD_ORE = BlockType(129);
    static constexpr BlockType ENDER_CHEST = BlockType(130);
    static constexpr BlockType TRIPWIRE_HOOK = BlockType(131);
    static constexpr BlockType TRIPWIRE = BlockType(132);
    static constexpr BlockType EMERALD_BLOCK = BlockType(133);
    static constexpr BlockType SPRUCE_WOOD_STAIRS = BlockType(134);
    static constexpr BlockType BIRCH_WOOD_STAIRS = BlockType(135);
    static constexpr BlockType JUNGLE_WOOD_STAIRS = BlockType(136);
    static constexpr BlockType COMMAND_BLOCK = BlockType(137);
    static constexpr BlockType BEACON = BlockType(138);
    static constexpr BlockType COBBLESTONE_WALL = BlockType(139);
    static constexpr BlockType MOSSY_COBBLESTONE_WALL = BlockType(139, 1);
    static constexpr BlockType FLOWER_POT = BlockType(140);
    static constexpr BlockType CARROTS = BlockType(141);
    static constexpr BlockType POTATOES = BlockType(142);
    static constexpr BlockType WOODEN_BUTTON = BlockType(143);
    static constexpr BlockType MOB_HEAD = BlockType(144);
    static constexpr BlockType ANVIL = BlockType(145);
    static constexpr BlockType TRAPPED_CHEST = BlockType(146);
    static constexpr BlockType WEIGHTED_PRESSURE_PLATE_LIGHT = BlockType(147);
    static constexpr BlockType WEIGHTED_PRESSURE_PLATE_HEAVY = BlockType(148);
    static constexpr BlockType REDSTONE_COMPARATOR_INACTIVE = BlockType(149);
    static constexpr BlockType REDSTONE_COMPARATOR_ACTIVE = BlockType(150);
    static constexpr BlockType DAYLIGHT_SENSOR = BlockType(151);
    static constexpr BlockType REDSTONE_BLOCK = BlockType(152);
    static constexpr BlockType NETHER_QUARTZ_ORE = BlockType(153);
    static constexpr BlockType HOPPER = BlockType(154);
    static constexpr BlockType QUARTZ_BLOCK = BlockType(155);
    static constexpr BlockType CHISELED_QUARTZ_BLOCK = BlockType(155, 1);
    static constexpr BlockType PILLAR_QUARTZ_BLOCK = BlockType(155, 2);
    static constexpr BlockType QUARTZ_STAIRS = BlockType(156);
    static constexpr BlockType ACTIVATOR_RAIL = BlockType(157);
    static constexpr BlockType DROPPER = BlockType(158);
    static constexpr BlockType WHITE_HARDENED_CLAY = BlockType(159);
    static constexpr BlockType ORANGE_HARDENED_CLAY = BlockType(159, 1);
    static constexpr BlockType MAGENTA_HARDENED_CLAY = BlockType(159, 2);
    static constexpr BlockType LIGHT_BLUE_HARDENED_CLAY = BlockType(159, 3);
    static constexpr BlockType YELLOW_HARDENED_CLAY = BlockType(159, 4);
    static constexpr BlockType LIME_HARDENED_CLAY = BlockType(159, 5);
    static constexpr BlockType PINK_HARDENED_CLAY = BlockType(159, 6);
    static constexpr BlockType GRAY_HARDENED_CLAY = BlockType(159, 7);
    static constexpr BlockType LIGHT_GRAY_HARDENED_CLAY = BlockType(159, 8);
    static constexpr BlockType CYAN_HARDENED_CLAY = BlockType(159, 9);
    static constexpr BlockType PURPLE_HARDENED_CLAY = BlockType(159, 10);
    static constexpr BlockType BLUE_HARDENED_CLAY = BlockType(159, 11);
    static constexpr BlockType BROWN_HARDENED_CLAY = BlockType(159, 12);
    static constexpr BlockType GREEN_HARDENED_CLAY = BlockType(159, 13);
    static constexpr BlockType RED_HARDENED_CLAY = BlockType(159, 14);
    static constexpr BlockType BLACK_HARDENED_CLAY = BlockType(159, 15);
    static constexpr BlockType WHITE_STAINED_GLASS_PANE = BlockType(160);
    static constexpr BlockType ORANGE_STAINED_GLASS_PANE = BlockType(160, 1);
    static constexpr BlockType MAGENTA_STAINED_GLASS_PANE = BlockType(160, 2);
    static constexpr BlockType LIGHT_BLUE_STAINED_GLASS_PANE =
        BlockType(160, 3);
    static constexpr BlockType YELLOW_STAINED_GLASS_PANE = BlockType(160, 4);
    static constexpr BlockType LIME_STAINED_GLASS_PANE = BlockType(160, 5);
    static constexpr BlockType PINK_STAINED_GLASS_PANE = BlockType(160, 6);
    static constexpr BlockType GRAY_STAINED_GLASS_PANE = BlockType(160, 7);
    static constexpr BlockType LIGHT_GRAY_STAINED_GLASS_PANE =
        BlockType(160, 8);
    static constexpr BlockType CYAN_STAINED_GLASS_PANE = BlockType(160, 9);
    static constexpr BlockType PURPLE_STAINED_GLASS_PANE = BlockType(160, 10);
    static constexpr BlockType BLUE_STAINED_GLASS_PANE = BlockType(160, 11);
    static constexpr BlockType BROWN_STAINED_GLASS_PANE = BlockType(160, 12);
    static constexpr BlockType GREEN_STAINED_GLASS_PANE = BlockType(160, 13);
    static constexpr BlockType RED_STAINED_GLASS_PANE = BlockType(160, 14);
    static constexpr BlockType BLACK_STAINED_GLASS_PANE = BlockType(160, 15);
    static constexpr BlockType ACACIA_LEAVES = BlockType(161);
    static constexpr BlockType DARK_OAK_LEAVES = BlockType(161, 1);
    static constexpr BlockType ACACIA_WOOD = BlockType(162);
    static constexpr BlockType DARK_OAK_WOOD = BlockType(162, 1);
    static constexpr BlockType ACACIA_WOOD_STAIRS = BlockType(163);
    static constexpr BlockType DARK_OAK_WOOD_STAIRS = BlockType(164);
    static constexpr BlockType SLIME_BLOCK = BlockType(165);
    static constexpr BlockType BARRIER = BlockType(166);
    static constexpr BlockType IRON_TRAPDOOR = BlockType(167);
    static constexpr BlockType PRISMARINE = BlockType(168);
    static constexpr BlockType PRISMARINE_BRICKS = BlockType(168, 1);
    static constexpr BlockType DARK_PRISMARINE = BlockType(168, 2);
    static constexpr BlockType SEA_LANTERN = BlockType(169);
    static constexpr BlockType HAY_BALE = BlockType(170);
    static constexpr BlockType WHITE_CARPET = BlockType(171);
    static constexpr BlockType ORANGE_CARPET = BlockType(171, 1);
    static constexpr BlockType MAGENTA_CARPET = BlockType(171, 2);
    static constexpr BlockType LIGHT_BLUE_CARPET = BlockType(171, 3);
    static constexpr BlockType YELLOW_CARPET = BlockType(171, 4);
    static constexpr BlockType LIME_CARPET = BlockType(171, 5);
    static constexpr BlockType PINK_CARPET = BlockType(171, 6);
    static constexpr BlockType GRAY_CARPET = BlockType(171, 7);
    static constexpr BlockType LIGHT_GRAY_CARPET = BlockType(171, 8);
    static constexpr BlockType CYAN_CARPET = BlockType(171, 9);
    static constexpr BlockType PURPLE_CARPET = BlockType(171, 10);
    static constexpr BlockType BLUE_CARPET = BlockType(171, 11);
    static constexpr BlockType BROWN_CARPET = BlockType(171, 12);
    static constexpr BlockType GREEN_CARPET = BlockType(171, 13);
    static constexpr BlockType RED_CARPET = BlockType(171, 14);
    static constexpr BlockType BLACK_CARPET = BlockType(171, 15);
    static constexpr BlockType HARDENED_CLAY = BlockType(172);
    static constexpr BlockType BLOCK_OF_COAL = BlockType(173);
    static constexpr BlockType PACKED_ICE = BlockType(174);
    static constexpr BlockType SUNFLOWER = BlockType(175);
    static constexpr BlockType LILAC = BlockType(175, 1);
    static constexpr BlockType DOUBLE_TALLGRASS = BlockType(175, 2);
    static constexpr BlockType LARGE_FERN = BlockType(175, 3);
    static constexpr BlockType ROSE_BUSH = BlockType(175, 4);
    static constexpr BlockType PEONY = BlockType(175, 5);
    static constexpr BlockType FREESTANDING_BANNER = BlockType(176);
    static constexpr BlockType WALLMOUNTED_BANNER = BlockType(177);
    static constexpr BlockType INVERTED_DAYLIGHT_SENSOR = BlockType(178);
    static constexpr BlockType RED_SANDSTONE = BlockType(179);
    static constexpr BlockType CHISELED_RED_SANDSTONE = BlockType(179, 1);
    static constexpr BlockType SMOOTH_RED_SANDSTONE = BlockType(179, 2);
    static constexpr BlockType RED_SANDSTONE_STAIRS = BlockType(180);
    static constexpr BlockType DOUBLE_RED_SANDSTONE_SLAB = BlockType(181);
    static constexpr BlockType RED_SANDSTONE_SLAB = BlockType(182);
    static constexpr BlockType SPRUCE_FENCE_GATE = BlockType(183);
    static constexpr BlockType BIRCH_FENCE_GATE = BlockType(184);
    static constexpr BlockType JUNGLE_FENCE_GATE = BlockType(185);
    static constexpr BlockType DARK_OAK_FENCE_GATE = BlockType(186);
    static constexpr BlockType ACACIA_FENCE_GATE = BlockType(187);
    static constexpr BlockType SPRUCE_FENCE = BlockType(188);
    static constexpr BlockType BIRCH_FENCE = BlockType(189);
    static constexpr BlockType JUNGLE_FENCE = BlockType(190);
    static constexpr BlockType DARK_OAK_FENCE = BlockType(191);
    static constexpr BlockType ACACIA_FENCE = BlockType(192);
    static constexpr BlockType SPRUCE_DOOR_BLOCK = BlockType(193);
    static constexpr BlockType BIRCH_DOOR_BLOCK = BlockType(194);
    static constexpr BlockType JUNGLE_DOOR_BLOCK = BlockType(195);
    static constexpr BlockType ACACIA_DOOR_BLOCK = BlockType(196);
    static constexpr BlockType DARK_OAK_DOOR_BLOCK = BlockType(197);
    static constexpr BlockType END_ROD = BlockType(198);
    static constexpr BlockType CHORUS_PLANT = BlockType(199);
    static constexpr BlockType CHORUS_FLOWER = BlockType(200);
    static constexpr BlockType PURPUR_BLOCK = BlockType(201);
    static constexpr BlockType PURPUR_PILLAR = BlockType(202);
    static constexpr BlockType PURPUR_STAIRS = BlockType(203);
    static constexpr BlockType PURPUR_DOUBLE_SLAB = BlockType(204);
    static constexpr BlockType PURPUR_SLAB = BlockType(205);
    static constexpr BlockType END_STONE_BRICKS = BlockType(206);
    static constexpr BlockType BEETROOT_BLOCK = BlockType(207);
    static constexpr BlockType GRASS_PATH = BlockType(208);
    static constexpr BlockType END_GATEWAY = BlockType(209);
    static constexpr BlockType REPEATING_COMMAND_BLOCK = BlockType(210);
    static constexpr BlockType CHAIN_COMMAND_BLOCK = BlockType(211);
    static constexpr BlockType FROSTED_ICE = BlockType(212);
    static constexpr BlockType MAGMA_BLOCK = BlockType(213);
    static constexpr BlockType NETHER_WART_BLOCK = BlockType(214);
    static constexpr BlockType RED_NETHER_BRICK = BlockType(215);
    static constexpr BlockType BONE_BLOCK = BlockType(216);
    static constexpr BlockType STRUCTURE_VOID = BlockType(217);
    static constexpr BlockType OBSERVER = BlockType(218);
    static constexpr BlockType WHITE_SHULKER_BOX = BlockType(219);
    static constexpr BlockType ORANGE_SHULKER_BOX = BlockType(220);
    static constexpr BlockType MAGENTA_SHULKER_BOX = BlockType(221);
    static constexpr BlockType LIGHT_BLUE_SHULKER_BOX = BlockType(222);
    static constexpr BlockType YELLOW_SHULKER_BOX = BlockType(223);
    static constexpr BlockType LIME_SHULKER_BOX = BlockType(224);
    static constexpr BlockType PINK_SHULKER_BOX = BlockType(225);
    static constexpr BlockType GRAY_SHULKER_BOX = BlockType(226);
    static constexpr BlockType LIGHT_GRAY_SHULKER_BOX = BlockType(227);
    static constexpr BlockType CYAN_SHULKER_BOX = BlockType(228);
    static constexpr BlockType PURPLE_SHULKER_BOX = BlockType(229);
    static constexpr BlockType BLUE_SHULKER_BOX = BlockType(230);
    static constexpr BlockType BROWN_SHULKER_BOX = BlockType(231);
    static constexpr BlockType GREEN_SHULKER_BOX = BlockType(232);
    static constexpr BlockType RED_SHULKER_BOX = BlockType(233);
    static constexpr BlockType BLACK_SHULKER_BOX = BlockType(234);
    static constexpr BlockType WHITE_GLAZED_TERRACOTTA = BlockType(235);
    static constexpr BlockType ORANGE_GLAZED_TERRACOTTA = BlockType(236);
    static constexpr BlockType MAGENTA_GLAZED_TERRACOTTA = BlockType(237);
    static constexpr BlockType LIGHT_BLUE_GLAZED_TERRACOTTA = BlockType(238);
    static constexpr BlockType YELLOW_GLAZED_TERRACOTTA = BlockType(239);
    static constexpr BlockType LIME_GLAZED_TERRACOTTA = BlockType(240);
    static constexpr BlockType PINK_GLAZED_TERRACOTTA = BlockType(241);
    static constexpr BlockType GRAY_GLAZED_TERRACOTTA = BlockType(242);
    static constexpr BlockType LIGHT_GRAY_GLAZED_TERRACOTTA = BlockType(243);
    static constexpr BlockType CYAN_GLAZED_TERRACOTTA = BlockType(244);
    static constexpr BlockType PURPLE_GLAZED_TERRACOTTA = BlockType(245);
    static constexpr BlockType BLUE_GLAZED_TERRACOTTA = BlockType(246);
    static constexpr BlockType BROWN_GLAZED_TERRACOTTA = BlockType(247);
    static constexpr BlockType GREEN_GLAZED_TERRACOTTA = BlockType(248);
    static constexpr BlockType RED_GLAZED_TERRACOTTA = BlockType(249);
    static constexpr BlockType BLACK_GLAZED_TERRACOTTA = BlockType(250);
    static constexpr BlockType WHITE_CONCRETE = BlockType(251);
    static constexpr BlockType ORANGE_CONCRETE = BlockType(251, 1);
    static constexpr BlockType MAGENTA_CONCRETE = BlockType(251, 2);
    static constexpr BlockType LIGHT_BLUE_CONCRETE = BlockType(251, 3);
    static constexpr BlockType YELLOW_CONCRETE = BlockType(251, 4);
    static constexpr BlockType LIME_CONCRETE = BlockType(251, 5);
    static constexpr BlockType PINK_CONCRETE = BlockType(251, 6);
    static constexpr BlockType GRAY_CONCRETE = BlockType(251, 7);
    static constexpr BlockType LIGHT_GRAY_CONCRETE = BlockType(251, 8);
    static constexpr BlockType CYAN_CONCRETE = BlockType(251, 9);
    static constexpr BlockType PURPLE_CONCRETE = BlockType(251, 10);
    static constexpr BlockType BLUE_CONCRETE = BlockType(251, 11);
    static constexpr BlockType BROWN_CONCRETE = BlockType(251, 12);
    static constexpr BlockType GREEN_CONCRETE = BlockType(251, 13);
    static constexpr BlockType RED_CONCRETE = BlockType(251, 14);
    static constexpr BlockType BLACK_CONCRETE = BlockType(251, 15);
    static constexpr BlockType WHITE_CONCRETE_POWDER = BlockType(252);
    static constexpr BlockType ORANGE_CONCRETE_POWDER = BlockType(252, 1);
    static constexpr BlockType MAGENTA_CONCRETE_POWDER = BlockType(252, 2);
    static constexpr BlockType LIGHT_BLUE_CONCRETE_POWDER = BlockType(252, 3);
    static constexpr BlockType YELLOW_CONCRETE_POWDER = BlockType(252, 4);
    static constexpr BlockType LIME_CONCRETE_POWDER = BlockType(252, 5);
    static constexpr BlockType PINK_CONCRETE_POWDER = BlockType(252, 6);
    static constexpr BlockType GRAY_CONCRETE_POWDER = BlockType(252, 7);
    static constexpr BlockType LIGHT_GRAY_CONCRETE_POWDER = BlockType(252, 8);
    static constexpr BlockType CYAN_CONCRETE_POWDER = BlockType(252, 9);
    static constexpr BlockType PURPLE_CONCRETE_POWDER = BlockType(252, 10);
    static constexpr BlockType BLUE_CONCRETE_POWDER = BlockType(252, 11);
    static constexpr BlockType BROWN_CONCRETE_POWDER = BlockType(252, 12);
    static constexpr BlockType GREEN_CONCRETE_POWDER = BlockType(252, 13);
    static constexpr BlockType RED_CONCRETE_POWDER = BlockType(252, 14);
    static constexpr BlockType BLACK_CONCRETE_POWDER = BlockType(252, 15);
    static constexpr BlockType STRUCTURE_BLOCK = BlockType(255);
};
} // namespace mcpp
