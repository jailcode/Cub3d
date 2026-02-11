*This project has been created as part of the 42 curriculum by pdangwal and rhaas*

# Description
This project is inspired by the world-famous Wolfenstein 3D game, which is
considered the first FPS ever created. It offers an opportunity to explore ray-casting.

cub3D shows a dynamic view inside a maze, where the first person character can move around in all four directions (forward, backward, left and right - relative to the player's current direction of view) and turn in any direction (left, righit, up, down).

# Instructions
+ First, the program needs to be built
  + The source code contains a Makefile, so the program can be built by running `make re`.\
  + As a prerequisite, the MinilibX library needs to be available. Make sure it is compiled and referenced correctly in the current projects Makefile in variable `MLX_FLAGS`.
+ Once compiled, the program can be run from the command line by using the following command: `./cub3D <someFileName>.cub`\
  `<someFileName>.cub` is the filename (with extension ".cub") of a valid scene description file (See follwing paragraph for details).
+ Then have fun, walk around, look in all directions, walk through space and along the walls...

### Valid map requirements
+ The map must be composed of only 6 possible characters:\
 0 for an empty space\
 1 for a wall\
 N,S,E or W for the player’s start position and spawning
orientation.\
This is a simple valid map:\
111111\
100101\
101001\
1100N1\
111111
+ The map must be closed/surrounded by walls, if not the program must returnan error.
+ Except for the map content, each type of element can be separated by one or more empty lines.
+ Except for the map content which always has to be the last, each type of
element can be set in any order in the file.
+ Except for the map, each type of information from an element can be separated by one or more spaces.
+ Except for the map, each element must begin with its type identifier (composed by one or two characters), followed by its specific information in a strict order:\
  - North texture: NO ./path_to_the_north_texture
    - identifier: NO
    - path to the north texture
  - South texture:
SO ./path_to_the_south_texture
    - identifier: SO
    - path to the south texture
  - West texture: WE ./path_to_the_west_texture
    - identifier: WE
    - path to the west texture
  - East texture: EA ./path_to_the_east_texture
    - identifier: EA
    - path to the east texture
  - Floor color: F 220,100,0
    - identifier: F
    - R,G,B colors in range [0,255]: 0, 255, 255
  - Ceiling color: C 225,30,0
    - identifier: C
    - R,G,B colors in range [0,255]: 0, 255, 255
+ Example of the mandatory part with a minimalist .cub scene:\
NO ./path_to_the_north_texture\
SO ./path_to_the_south_texture\
WE ./path_to_the_west_texture\
EA ./path_to_the_east_texture\
\
F 220,100,0\
C 225,30,0\
\
1111111111111111111111111\
1000000000110000000000001\
1011000001110000000000001\
1001000000000000000000001\
111111111011000001110000000000001\
100000000011000001110111111111111\
11110111111111011100000010001\
11110111111111011101010010001\
11000000110101011100000010001\
10000000000000001100000010001\
10000000000000001101010010001\
11000001110101011111011110N0111\
11110111 1110101 101111010001\
11111111 1111111 111111111111

# Resources

### Information and study material+
+ https://lodev.org/cgtutor/raycasting.html
+ https://www.youtube.com/watch?v=qjWkNZ0SXfo


### AI usage
AI wasn't used for this project.
