#include <iostream>
#include "geesespotter_lib.h"

//function declarations
char *create_board(std::size_t x_dim, std::size_t y_dim);
void clean_board(char *board);
void print_board(char *board, std::size_t x_dim, std::size_t y_dim);
void hide_board(char *board, std::size_t x_dim, std::size_t y_dim);
int mark(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc);
void compute_neighbours(char *board, std::size_t x_dim, std::size_t y_dim);
bool is_game_won(char *board, std::size_t x_dim, std::size_t y_dim);
int reveal(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc);

int coordinates_to_index(std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc);

//helper function definitions
int coordinates_to_index(std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    return((y_loc * x_dim) + x_loc);
}

//main function definitions
char *create_board(std::size_t x_dim, std::size_t y_dim){
    char *board{new char[x_dim * y_dim]{}};

    //initializing all values to 0
    for(int i = 0; i < (x_dim*y_dim); i++){
        board[i] = 0;
    }

    return board;
}

void clean_board(char *board){
    delete[] board;
    board = nullptr;
}

void print_board(char *board, std::size_t x_dim, std::size_t y_dim){  
    //iterates through each row element
    for(int i = 0; i < y_dim; i++){
        //iterates through each column element
        for(int k = 0; k < x_dim; k++){
            //determines what to print
            if((board[(x_dim*i)+k] & marked_mask()) == marked_mask()){
                std::cout << "M"; //prints M if the value is masked
            }
            else if((board[(x_dim*i)+k] & hidden_mask()) == hidden_mask()){
                std::cout << '*'; //prints * if the value is hidden
            }
            else{ 
                std::cout << ((board[(x_dim*i)+k] & value_mask())); //prints 0-9 value
            }
        }
        //prints an endl at the end of each row
        std::cout << std::endl;
    }
}

void hide_board(char *board, std::size_t x_dim, std::size_t y_dim){
    //iterates through each element and sets the "hidden bit" to 0
    for(int i = 0; i < (x_dim*y_dim); i++){
        board[i] |= hidden_mask();
    }
}

int mark(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    //runs helper function to convert the x and y coordinates to index value
    int index = coordinates_to_index(x_dim, y_dim, x_loc, y_loc);
    
    //checks if already revealed
    if((board[index] & hidden_mask()) != hidden_mask()){
        return 2;
    }
    else{
        //flip the mark bit it
        board[index] ^= marked_mask();
        return 0;
    }
}

void compute_neighbours(char *board, std::size_t x_dim, std::size_t y_dim){
    //iterates through each row
    for(int i = 0; i < y_dim; i++){
        //iterates through each column element
        for(int k = 0; k < x_dim; k++){
            //determines current index
            int index = coordinates_to_index(x_dim, y_dim, k, i);

            //only computes neighbors if the value at current index is 0
            if((board[index] & value_mask()) == 0x00){
                int surrounding_geese{0};
                  
                //iterates through each of the three surrounding rows (above, current and below)
                for(int y = -1; y <= 1; y++){
                    //iterates through each of the three surrounding columns (left, current, right)
                    for(int x = -1; x <= 1; x++){
                        //calculate indeces of the current neighboring space being checked
                        int x_neighboring = k + x;
                        int y_neighboring = i + y;

                        //check if x-y coordinate values of neighboring space exists (i.e. checks for edge cases)
                        if(x_neighboring >= 0 && x_neighboring < x_dim && y_neighboring >= 0 && y_neighboring < y_dim){
                            //converts neighboring coordinates to neighboring index
                            int neighbor_index = coordinates_to_index(x_dim, y_dim, x_neighboring, y_neighboring);

                            //checks if board at neighbor_index is goose, and increments character if so
                            if((board[neighbor_index] & value_mask()) == 0x09){
                                surrounding_geese++;
                            }
                        }
                    }
                }
                //clears element, and then sets number of surrounding geese to first four bits of character
                board[index] &= value_mask();
                board[index] |= surrounding_geese;
            }    
        }
    }
}

bool is_game_won(char *board, std::size_t x_dim, std::size_t y_dim){
    //iterates through each grid
    for(int i = 0; i < x_dim*y_dim; i++){
        //if value is hidden and does not equal a 9, game is not won
        if((board[i] & hidden_mask()) == hidden_mask() && (board[i] & value_mask()) != 0x09){
            return false;
        }
    }

    //return true if no such value is found
    return true;
}

int reveal(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    int index = coordinates_to_index(x_dim, y_dim, x_loc, y_loc);
    
    //if it's marked, return 1
    if((board[index] & marked_mask()) == marked_mask()){
        return 1;
    }
    //if it's not hidden, return 2
    else if((board[index] & hidden_mask()) != hidden_mask()){
        return 2;
    }
    else{
        //if it's a goose
        if((board[index] & value_mask()) == 0x09){
            board[index] ^= hidden_mask();
            return 9;
        }
        //if not a 9, and number of geese is 0, reveal it AND all the other surrounding UNMARKED elements
        else if((board[index] & value_mask()) == 0x00){
            if((board[index] & hidden_mask()) == hidden_mask()){
                board[index] ^= hidden_mask();
            }

            //iterate through all neighboring indeces, and unhide the neighboring ones
            for(int y = -1; y <= 1; y++){
                for(int x = -1; x <= 1; x++){
                    int x_neighboring = x_loc + x;
                    int y_neighboring = y_loc + y;
                    int neighbor_index = coordinates_to_index(x_dim, y_dim, x_neighboring, y_neighboring);

                    //check if index of neighboring space exists, and if so, whether 
                    //it is already marked (i.e. checks for edge cases)
                    if(neighbor_index >= 0 && neighbor_index < (x_dim*y_dim) && (board[neighbor_index] & marked_mask()) != marked_mask() && (board[neighbor_index] & hidden_mask()) == hidden_mask()){
                        board[neighbor_index] ^= hidden_mask();
                    }
                }
            }
            return 0;
        }
        //if the number of geese is between 1-8
        else{
            if((board[index] & hidden_mask()) == hidden_mask()){
                board[index] ^= hidden_mask();
            }
            return 0;
        }        
    }
}