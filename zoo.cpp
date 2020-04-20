/**
 * Implements a Zoo namespace with methods for constructing Grid objects containing various creatures in the Game of Life.
 *      - Creatures like gliders, light weight spaceships, and r-pentominos can be spawned.
 *          - These creatures are drawn on a Grid the size of their bounding box.
 *
 *      - Grids can be loaded from and saved to an ascii file format.
 *          - Ascii files are composed of:
 *              - A header line containing an integer width and height separated by a space.
 *              - followed by (height) number of lines, each containing (width) number of characters,
 *                terminated by a newline character.
 *              - (space) ' ' is Cell::DEAD, (hash) '#' is Cell::ALIVE.
 *
 *      - Grids can be loaded from and saved to an binary file format.
 *          - Binary files are composed of:
 *              - a 4 byte int representing the grid width
 *              - a 4 byte int representing the grid height
 *              - followed by (width * height) number of individual bits in C-style row/column format,
 *                padded with zero or more 0 bits.
 *              - a 0 bit should be considered Cell::DEAD, a 1 bit should be considered Cell::ALIVE.
 *
 * @author 957552
 * @date March, 2020
 */
#include <fstream>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <cmath>
#include "zoo.h"

// Include the minimal number of headers needed to support your implementation.
// #include ...

/**
 * Zoo::glider()
 *
 * Construct a 3x3 grid containing a glider.
 * https://www.conwaylife.com/wiki/Glider
 *
 * @example
 *
 *      // Print a glider in a Grid the size of its bounding box.
*      std::cout << Zoo::glider() << std::endl;
*
 *      +---+
 *      | # |
 *      |  #|
 *      |###|
 *      +---+
 *
 * @return
 *      Returns a Grid containing a glider.
 */
Grid Zoo::glider(){
    Grid glider(3);
    glider.set(1,0,ALIVE);
    glider.set(2,1,ALIVE);
    glider.set(0,2,ALIVE);
    glider.set(1,2,ALIVE);
    glider.set(2,2,ALIVE);
    return glider;
}

/**
 * Zoo::r_pentomino()
 *
 * Construct a 3x3 grid containing an r-pentomino.
 * https://www.conwaylife.com/wiki/R-pentomino
 *
 * @example
 *
 *      // Print an r-pentomino in a Grid the size of its bounding box.
 *      std::cout << Zoo::r_pentomino() << std::endl;
 *
 *      +---+
 *      | ##|
 *      |## |
 *      | # |
 *      +---+
 *
 * @return
 *      Returns a Grid containing a r-pentomino.
 */
Grid Zoo::r_pentomino(){
    Grid r_pentomino(3);
    r_pentomino.set(1,0,ALIVE);
    r_pentomino.set(2,0,ALIVE);
    r_pentomino.set(0,1,ALIVE);
    r_pentomino.set(1,1,ALIVE);
    r_pentomino.set(1,2,ALIVE);
    return r_pentomino;
}

/**
 * Zoo::light_weight_spaceship()
 *
 * Construct a 5x4 grid containing a light weight spaceship.
 * https://www.conwaylife.com/wiki/Lightweight_spaceship
 *
 * @example
 *
 *      // Print a light weight spaceship in a Grid the size of its bounding box.
 *      std::cout << Zoo::light_weight_spaceship() << std::endl;
 *
 *      +-----+
 *      | #  #|
 *      |#    |
 *      |#   #|
 *      |#### |
 *      +-----+
 *
 * @return
 *      Returns a grid containing a light weight spaceship.
 */
Grid Zoo::light_weight_spaceship(){
    Grid light_weight_spaceship(5,4);
    light_weight_spaceship.set(1,0,ALIVE);
    light_weight_spaceship.set(4,0,ALIVE);
    light_weight_spaceship.set(0,1,ALIVE);
    light_weight_spaceship.set(0,2,ALIVE);
    light_weight_spaceship.set(4,2,ALIVE);
    light_weight_spaceship.set(0,3,ALIVE);
    light_weight_spaceship.set(1,3,ALIVE);
    light_weight_spaceship.set(2,3,ALIVE);
    light_weight_spaceship.set(3,3,ALIVE);
    return light_weight_spaceship;
}

/**
 * Zoo::load_ascii(path)
 *
 * Load an ascii file and parse it as a grid of cells.
 * Should be implemented using std::ifstream.
 *
 * @example
 *
 *      // Load an ascii file from a directory
 *      Grid grid = Zoo::load_ascii("path/to/file.gol");
 *
 * @param path
 *      The std::string path to the file to read in.
 *
 * @return
 *      Returns the parsed grid.
 *
 * @throws
 *      Throws std::runtime_error or sub-class if:
 *          - The file cannot be opened.
 *          - The parsed width or height is not a positive integer.
 *          - Newline characters are not found when expected during parsing.
 *          - The character for a cell is not the ALIVE or DEAD character.
 */
Grid Zoo::load_ascii(const std::string& path){
    std::ifstream file(path);
    if(file){
        int width;
        int height;
        char buffer;
        Grid new_grid;
        file >> width;
        file >> height;
        if(width > 0 || height > 0){
            new_grid = Grid(width,height);
            for(int j=0; j<height; j++){
                file.get(buffer);
                if(buffer == '\n'){
                    for(int i=0; i<width; i++){
                        file.get(buffer);
                        if(buffer == ALIVE){
                            new_grid.set(i, j, ALIVE);
                        } else if(buffer == DEAD){
                            new_grid.set(i, j, DEAD);
                        } else{
                            throw(std::runtime_error("The character for a cell is not the ALIVE or DEAD character."));
                        }
                    }
                } else{
                    throw(std::runtime_error("Newline characters are not found when expected during parsing."));
                }
            }
        } else{
            throw(std::runtime_error("The parsed width or height is not a positive integer."));
        }

        file.close();
        return new_grid;
    } else{
        file.close();
        throw(std::runtime_error("The file cannot be opened."));
    }
}

/**
 * Zoo::save_ascii(path, grid)
 *
 * Save a grid as an ascii .gol file according to the specified file format.
 * Should be implemented using std::ofstream.
 *
 * @example
 *
 *      // Make an 8x8 grid
 *      Grid grid(8);
 *
 *      // Save a grid to an ascii file in a directory
 *      try {
 *          Zoo::save_ascii("path/to/file.gol", grid);
 *      }
 *      catch (const std::exception &ex) {
 *          std::cerr << ex.what() << std::endl;
 *      }
 *
 * @param path
 *      The std::string path to the file to write to.
 *
 * @param grid
 *      The grid to be written out to file.
 *
 * @throws
 *      Throws std::runtime_error or sub-class if the file cannot be opened.
 */
void Zoo::save_ascii(const std::string& path, const Grid& grid){
    std::ofstream file(path);
    if(file){
        file << grid.get_width() << " " << grid.get_height() << "\n";
        for(unsigned int j = 0; j<grid.get_height(); j++){
            for(unsigned int i = 0; i<grid.get_width(); i++){
                if(grid.get((int)i,(int)j) == DEAD){
                    file << (char) DEAD;
                } else if (grid.get((int)i,(int)j) == ALIVE){
                    file << (char) ALIVE;
                }
            }
            file << "\n";
        }
    } else{
        throw(std::runtime_error("The path given to function: Zoo::save_ascii is incorrect."));
    }
    file.close();
}

/**
 * Zoo::load_binary(path)
 *
 * Load a binary file and parse it as a grid of cells.
 * Should be implemented using std::ifstream.
 *
 * @example
 *
 *      // Load an binary file from a directory
 *      Grid grid = Zoo::load_binary("path/to/file.bgol");
 *
 * @param path
 *      The std::string path to the file to read in.
 *
 * @return
 *      Returns the parsed grid.
 *
 * @throws
 *      Throws std::runtime_error or sub-class if:
 *          - The file cannot be opened.
 *          - The file ends unexpectedly.
 */
Grid Zoo::load_binary(const std::string& path){
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if(file){
        int width;
        int height;
        Grid new_grid;

        file.read((char*)& width, sizeof width);
        file.read((char*)& height, sizeof height);
        new_grid = Grid(width,height);

        // he equation below calculates height * width + bits needed to get to the nearest byte.
        int* bits = new int[(int)(ceil(((double)height*(double)width/8)))*8];
        char c;
        int count = 0;
        for (int j = 0; file.get(c); j++){
            for (int i = 0; i < 8; i++){
                //bitshift operator to read bit by bit since c++ smallest operator type is 1 Byte.
                if((c >> i & 1) == 0 && count < (ceil(((double)height*(double)width/8)))*8){
                    bits[j*8+i] = 0;
                }
                if((c >> i & 1) == 1 && count < (ceil(((double)height*(double)width/8)))*8){
                    bits[j*8+i] = 1;
                }
                count++;
            }
        }

        if(count < (ceil(((double)height*(double)width/8)))*8){
            delete[] bits;
            throw(std::runtime_error("The file: '" + path + "' ends unexpectedly."));
        }

        // Make the grid from the array.
        for(int j=0; j<height; j++){
            for(int i=0; i<width; i++){
                if(bits[(j*width)+i] == 0){
                    new_grid.set(i,j,DEAD);
                } else if(bits[(j*width)+i] == 1){
                    new_grid.set(i,j,ALIVE);
                }
            }
        }

        delete[] bits;
        file.close();
        return new_grid;
    } else{
        file.close();
        throw(std::runtime_error("The path given to function: Zoo::load_binary is incorrect."));
    }
}

/**
 * Zoo::save_binary(path, grid)
 *
 * Save a grid as an binary .bgol file according to the specified file format.
 * Should be implemented using std::ofstream.
 *
 * @example
 *
 *      // Make an 8x8 grid
 *      Grid grid(8);
 *
 *      // Save a grid to an binary file in a directory
 *      try {
 *          Zoo::save_binary("path/to/file.bgol", grid);
 *      }
 *      catch (const std::exception &ex) {
 *          std::cerr << ex.what() << std::endl;
 *      }
 *
 * @param path
 *      The std::string path to the file to write to.
 *
 * @param grid
 *      The grid to be written out to file.
 *
 * @throws
 *      Throws std::runtime_error or sub-class if the file cannot be opened.
 */
void Zoo::save_binary(const std::string &path, const Grid &grid){
    std::ofstream file(path);
    if(file){
        int width = (int) grid.get_width();
        int height = (int) grid.get_height();
        file.write(reinterpret_cast<const char *>(&width), sizeof(width));
        file.write(reinterpret_cast<const char *>(&height), sizeof(height));

        // bitset buffer
        std::bitset<8> bits;
        unsigned int count=0;
        for(unsigned int j=0; j<grid.get_height(); j++){
            for(unsigned int i=0; i<grid.get_width(); i++){
                // set bit in bitset to true if ALIVE, else false.
                bits.set(count % 8, grid.get((int)i, (int)j) == Cell::ALIVE);
                count++;

                // once the bitset reaches 8 bits. write that byte into the file and reset the bitset array for next byte.
                if(count % 8 == 0 || count == grid.get_total_cells()){
                    file.write((char*)(&bits), 1);
                    bits.reset();
                }
            }
        }
    } else{
        throw(std::runtime_error("The path given to function: Zoo::save_binary is incorrect."));
    }
    file.close();
}
