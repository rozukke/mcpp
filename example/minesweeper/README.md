## Minesweeper
This is a working version of minesweeper in minecraft using `mcpp` and c++.

It should be compilable with something along the lines of `g++ -std=c++17 - Wall -o minesweeper minesweeper.cpp -lmcpp`  
after you have installed the library.

To install the attached resource pack `minesweeper_resource_pack.zip` into your minecraft resourcepack folder.
To find your resource pack folder go into minecraft and follow the steps
Escape - Options - Resource Packs - Open Pack Folder
Once opened move the .zip into the folder, and back into minecraft select it from the available packs list

The game is played by launching the compiled code and originates from the player deleting only blocks where it
requires to place blocks. It is recommended to run the code while flying in the air to avoid clipping issues.
The code continues to loop until either you win, lose or place a barrier block on the board. 

The games size and mine count can be modified by changing the following values from the top of the Minesweeper class
before recompiling
    `const int X_SIZE = 10;`
    `const int Z_SIZE = 10;`
    `const int MINE_COUNT = 20;`