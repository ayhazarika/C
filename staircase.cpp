#include <iostream>

int main();

int main(){
    //Local variable initialization
    double x{};

    //User inputs value for stair function
    std::cout << "Enter value of x: ";
    std::cin >> x;

    int stair{0};

    //TASK: write code that calculates n integer value corresponding to the value of the function stair(x) here
    if(x >= 0){
        for(int k{0}; k <= x; k++){ 
            if(x >= k && x < (k+1)){
                stair = k;
                break;
            }
        }
    }
    else{
        for(int k{0}; k >= x; k--){ 
            if(x == k){
                stair = k;
            }
            else if(x <= k && x > (k-1)){
                stair = (k-1);
                break;
            }
        }
    }
        
    //Outputs value of stair(x) to screen
    std::cout << "The value of stair(x) is: ";
    std::cout << stair;
    std::cout << std::endl;
    
    return 0;
}