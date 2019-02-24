# Tree Fractal Generator
A full tree fractal GUI program utilizing ROOT libraries,
and includes a random tree generator. 

Requires CERN's ROOT library to be installed:
https://root.cern.ch/download/root_v6.14.04.source.tar.gz

The program may be compiled in the terminal using the following bash script:

./compile.sh

which should open a GUI for the user.

## Program Description
* A complete description of the program may be found <a href="https://freddyox.github.io/blog/tree-fractal-v2/">within my website</a>, and a small video may be found <a href="https://www.youtube.com/watch?v=yIIv5fRgoSY&feature=youtu.be">here</a>. 
* Program originally was designed to generate symmetric Pythagorean fractal trees, where the number of branches, angles, lengths, etc. could be adjusted by the user; however, a natural extension to a apply random fluctuations which results in trees that look more natural.
* In the context of symmetric trees, if one chooses the angles/lengths properly the outputs become very surprising, *e.g.* I was able to replicate a variation of the Sierpinski Triangle among other structures. 
#### Symmetric Outputs:
* Going clockwise and starting at the top left, the number of branches to generate at a new node is 2, 3, 4, 5, and 9:

![Screenshot](website_pics/assortment_nsplits_small_white.png)
