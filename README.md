# DS-isometric-tiles
This project is a mock-up of typical isometric perspective games, using the tile mode on the nintendo DS, as part of course EE-310 at EPFL, Lausanne.

# The project's main challenge
Isometric view(technically Dimetric view in this case, but I will be calling it Isometric to fit the term used in gaming) looks something like this:

The difficulty is I intend to draw it using fixed predefined tiles of 8x8 pixels.
The first observation I made when starting this project is that we can draw the blocks in such a way where each tile is split up to three slices:
![Triangle slices on each tile, displaying a cube drawn by filling these triangles](readme_files/tile_slices.png)
If we could paint each individual triangle, we can create the isometric look fairly easily




