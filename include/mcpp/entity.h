#pragma once

#include <string>

namespace mcpp {
    class Entity {
    public:
        constexpr Entity(int id, const char *name = nullptr) : id(id), name(name) {};

        bool operator==(Entity& other) const {
            return this->id == other.id;
        }

    private:
        int id;
        const char *name;
    };


    struct Entities {
        static constexpr Entity EXPERIENCE_ORB = Entity(2, "EXPERIENCE_ORB");
        static constexpr Entity AREA_EFFECT_CLOUD = Entity(3, "AREA_EFFECT_CLOUD");
        static constexpr Entity ELDER_GUARDIAN = Entity(4, "ELDER_GUARDIAN");
        static constexpr Entity WITHER_SKELETON = Entity(5, "WITHER_SKELETON");
        static constexpr Entity STRAY = Entity(6, "STRAY");
        static constexpr Entity EGG = Entity(7, "EGG");
        static constexpr Entity LEASH_HITCH = Entity(8, "LEASH_HITCH");
        static constexpr Entity PAINTING = Entity(9, "PAINTING");
        static constexpr Entity ARROW = Entity(10, "ARROW");
        static constexpr Entity SNOWBALL = Entity(11, "SNOWBALL");
        static constexpr Entity FIREBALL = Entity(12, "FIREBALL");
        static constexpr Entity SMALL_FIREBALL = Entity(13, "SMALL_FIREBALL");
        static constexpr Entity ENDER_PEARL = Entity(14, "ENDER_PEARL");
        static constexpr Entity ENDER_SIGNAL = Entity(15, "ENDER_SIGNAL");
        static constexpr Entity THROWN_EXP_BOTTLE = Entity(17, "THROWN_EXP_BOTTLE");
        static constexpr Entity ITEM_FRAME = Entity(18, "ITEM_FRAME");
        static constexpr Entity WITHER_SKULL = Entity(19, "WITHER_SKULL");
        static constexpr Entity PRIMED_TNT = Entity(20, "PRIMED_TNT");
        static constexpr Entity HUSK = Entity(23, "HUSK");
        static constexpr Entity SPECTRAL_ARROW = Entity(24, "SPECTRAL_ARROW");
        static constexpr Entity SHULKER_BULLET = Entity(25, "SHULKER_BULLET");
        static constexpr Entity DRAGON_FIREBALL = Entity(26, "DRAGON_FIREBALL");
        static constexpr Entity ZOMBIE_VILLAGER = Entity(27, "ZOMBIE_VILLAGER");
        static constexpr Entity SKELETON_HORSE = Entity(28, "SKELETON_HORSE");
        static constexpr Entity ZOMBIE_HORSE = Entity(29, "ZOMBIE_HORSE");
        static constexpr Entity ARMOR_STAND = Entity(30, "ARMOR_STAND");
        static constexpr Entity DONKEY = Entity(31, "DONKEY");
        static constexpr Entity MULE = Entity(32, "MULE");
        static constexpr Entity EVOKER_FANGS = Entity(33, "EVOKER_FANGS");
        static constexpr Entity EVOKER = Entity(34, "EVOKER");
        static constexpr Entity VEX = Entity(35, "VEX");
        static constexpr Entity VINDICATOR = Entity(36, "VINDICATOR");
        static constexpr Entity ILLUSIONER = Entity(37, "ILLUSIONER");
        static constexpr Entity MINECART_COMMAND = Entity(40, "MINECART_COMMAND");
        static constexpr Entity BOAT = Entity(41, "BOAT");
        static constexpr Entity MINECART = Entity(42, "MINECART");
        static constexpr Entity MINECART_CHEST = Entity(43, "MINECART_CHEST");
        static constexpr Entity MINECART_FURNACE = Entity(44, "MINECART_FURNACE");
        static constexpr Entity MINECART_TNT = Entity(45, "MINECART_TNT");
        static constexpr Entity MINECART_HOPPER = Entity(46, "MINECART_HOPPER");
        static constexpr Entity MINECART_MOB_SPAWNER = Entity(47, "MINECART_MOB_SPAWNER");
        static constexpr Entity CREEPER = Entity(50, "CREEPER");
        static constexpr Entity SKELETON = Entity(51, "SKELETON");
        static constexpr Entity SPIDER = Entity(52, "SPIDER");
        static constexpr Entity GIANT = Entity(53, "GIANT");
        static constexpr Entity ZOMBIE = Entity(54, "ZOMBIE");
        static constexpr Entity SLIME = Entity(55, "SLIME");
        static constexpr Entity GHAST = Entity(56, "GHAST");
        static constexpr Entity PIG_ZOMBIE = Entity(57, "PIG_ZOMBIE");
        static constexpr Entity ENDERMAN = Entity(58, "ENDERMAN");
        static constexpr Entity CAVE_SPIDER = Entity(59, "CAVE_SPIDER");
        static constexpr Entity SILVERFISH = Entity(60, "SILVERFISH");
        static constexpr Entity BLAZE = Entity(61, "BLAZE");
        static constexpr Entity MAGMA_CUBE = Entity(62, "MAGMA_CUBE");
        static constexpr Entity ENDER_DRAGON = Entity(63, "ENDER_DRAGON");
        static constexpr Entity WITHER = Entity(64, "WITHER");
        static constexpr Entity BAT = Entity(65, "BAT");
        static constexpr Entity WITCH = Entity(66, "WITCH");
        static constexpr Entity ENDERMITE = Entity(67, "ENDERMITE");
        static constexpr Entity GUARDIAN = Entity(68, "GUARDIAN");
        static constexpr Entity SHULKER = Entity(69, "SHULKER");
        static constexpr Entity PIG = Entity(90, "PIG");
        static constexpr Entity SHEEP = Entity(91, "SHEEP");
        static constexpr Entity COW = Entity(92, "COW");
        static constexpr Entity CHICKEN = Entity(93, "CHICKEN");
        static constexpr Entity SQUID = Entity(94, "SQUID");
        static constexpr Entity WOLF = Entity(95, "WOLF");
        static constexpr Entity MUSHROOM_COW = Entity(96, "MUSHROOM_COW");
        static constexpr Entity SNOWMAN = Entity(97, "SNOWMAN");
        static constexpr Entity OCELOT = Entity(98, "OCELOT");
        static constexpr Entity IRON_GOLEM = Entity(99, "IRON_GOLEM");
        static constexpr Entity HORSE = Entity(100, "HORSE");
        static constexpr Entity RABBIT = Entity(101, "RABBIT");
        static constexpr Entity POLAR_BEAR = Entity(102, "POLAR_BEAR");
        static constexpr Entity LLAMA = Entity(103, "LLAMA");
        static constexpr Entity LLAMA_SPIT = Entity(104, "LLAMA_SPIT");
        static constexpr Entity PARROT = Entity(105, "PARROT");
        static constexpr Entity VILLAGER = Entity(120, "VILLAGER");
        static constexpr Entity ENDER_CRYSTAL = Entity(200, "ENDER_CRYSTAL");
    };
}