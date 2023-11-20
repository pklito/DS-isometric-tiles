# DS-isometric-tiles
This project is a mock-up of typical isometric perspective games, using the tile mode on the nintendo DS, as part of course EE-310 at EPFL, Lausanne.

## The project's main challenge
Isometric view(technically Dimetric view in this case, but I will be calling it Isometric to fit the term used in gaming) looks something like this:

The difficulty is I intend to draw it using fixed predefined tiles of 8x8 pixels.
The first observation I made when starting this project is that we can draw the blocks in such a way where each tile is split up to three slices:  
<img src="readme_files/tile_slices.png" width="100" />  

If we could paint each individual triangle, we can create the isometric look fairly easily.

## How the code works
### Turning the world into triangles
We start with a 3 dimentional cube world, in the code, it is called `WORLD_MAP`. a 0 in this array means air, a 1 means block 1, a 2, block 2.
We call the function `ISO_GenerateTiles` this function takes this 3D world and converts it into an array of 32x32 squares, from the screens perspective.
These aren't graphical tiles, they store the 3 colors of the triangles that will be in the tile in that location. represented as such:  
|15|14-10|98765|43210|
|---|---|---|---|
|on bit|data top|data middle| data bottom|  
where data would look like this: `(01)(001)`  
the bottom 3 bits store the type of the block which had that face, and the top 2 bits store if it was the Top face of a block (FLOOR), the left wall (WALL_LEFT) or the right wall (WALL_RIGHT). (11) is undefined.  

### Converting triangles into tiles
we now have a `u16` array of the triangles in our screen which we need to convert to tiles.  
**we can now observe an issue:** there are many combinations of triangle colors that can fit into one tile. we need to have a different tile for each combination  
The solution is to cleverly use Palettes to allow you to switch around colors.

**observation:** there are some impossible combinations for triangles, such as  
Floor 1 /  
  /-- Wall 2  
Wall 1 \\--  
If a floor cuts above another block, it will have its own wall under it. so there is no way Wall 2 can be directly under floor 1.  
We can use this to map out the possible combinations and split them to an ordering that is logical.  

#### Our division
Tiles are split into **which colors repeat themselves on the tile**: AAA, AAB, ABA, ABB, ABC  

#### Our palette
|colors\\Palettes| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| 0 |Water|Water|Water|Water|Water|Water|Water|Water|
| 1 | F1 | F1 | F2 | F2 | Sand | |Sand |_W1r_|_W2r_|
| 2 | F2 | Sand | F1 | Sand | | F1 | F2 |_F1_|_Sand_|
...

### technical limitations
Since every combination has to be accounted for with the addition of more palettes or more tiles, the amount of different blocks you can implement is very limited.  
For this reason, I would suggest **not** to implement isometric or complex views in Tile mode, but rather use **Rotoscope** after the Triangle Generation step.  
It is worth noting however, that this implementation takes up a mere **794B** for the tiles and **64B** for the palette, as opposed to the **24KB** of a rotoscope background.  also, consider that you may need to use two backgrounds in order to hide sprites behind some, effectively doubling the amount of memory*
*- in rotoscope, it is possible to "mask" a second layer only around sprites, however if there is more than one on screen, there is no choice but to use the whole 24KB


### afterword




