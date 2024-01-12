#include <iostream>
#include <climits>

#ifndef MARMOSET_TESTING
int main();
#endif
//ALL CODE INCLUDED IN HERE-----------------------------------------------------

//adds checksum - single piece of data
unsigned int add_checksum(unsigned int n);
unsigned int add_checksum(unsigned int n){ 
	//function variables and arrays
	int number[9]{};
	int checkSumArray[8]{};
	int checkSumValue{0};
	int updatedNumber{0};

	if(n > 99999999 || n < 0){
		return UINT_MAX;
	}
	else{
		//places each digit of the number into number[] (original, untouched) and checkSumArray[] (upon which calculations will be done)
		for(int k = 0; k < 8; k++){
			int currentDigit = n%10;
			number[9-(k+2)] = currentDigit;
			checkSumArray[9-(k+2)] = currentDigit;
			n/=10;
		}

		//calculate checksum value upon checkSumArray (copy of number array)
		for(int k = 0; k < 8; k+=2){
			//input every other # from last index into array, and multiply it by 2
			checkSumArray[7-k] = 2*(checkSumArray[7-k]);

			//if the new number is > 9, replace it with the sum of the two digits
			if(checkSumArray[7-k] > 9){
				checkSumArray[7-k] = ((checkSumArray[7-k]/10)%10) + (checkSumArray[7-k]%10);
			}
		}

		//sums all values
		for(int k = 0; k < 8; k++){
			checkSumValue += checkSumArray[k];
		}

		checkSumValue = ((9*checkSumValue)%10);

		//TESTING PRINT
		//std::cout << "\nCHECKSUM DIGIT: " << checkSumValue << std::endl;

		//add checksum to last index of array
		number[8] = checkSumValue;

		//convert array back into integer (including the checksum value)
		for(int k = 0; k < 9; k++){
			updatedNumber *= 10;
			updatedNumber += number[k];
		}
		
		//return updated number
		return updatedNumber;
	}
	

}

//verifies checksum
bool verify_checksum(unsigned int n);
bool verify_checksum(unsigned int n){
	//function-specific variables/arrays
	int newValue{0};
	int newCheckSumDigit{0};

	//compute checksum on newValue (which is n minus the checksumvalue)
	newValue = n/10;
	newValue = add_checksum(newValue);

	//compare if checksum values are equal
	if((newValue%10) == (n%10) && n <= 999999999){
		return true;
	}
	else{
		return false;
	}

	
}

//removes checksum - single piece of data
unsigned int remove_checksum(unsigned int n);
unsigned int remove_checksum(unsigned int n){
	if(n > 999999999 || !verify_checksum(n)){
		return UINT_MAX;
	}
	else{
		//divide by 10 to remove last digit
		n /= 10;
		return n;
	}
}

//adds checksum - array consisting of multiple data pieces
void add_checksum(unsigned int array[], std::size_t capacity);
void add_checksum(unsigned int array[], std::size_t capacity){
	
	//for loop which iterates through each element
	for(int k = 0; k < capacity; k++){
		array[k] = add_checksum(array[k]);
	}
}

//removes checksum - array consisting of multiple data pieces
void remove_checksum(unsigned int array[], std::size_t capacity);
void remove_checksum(unsigned int array[], std::size_t capacity){
	//for loop that iterates through each element in array
	for(int k = 0; k < capacity; k++){
		array[k] = remove_checksum(array[k]);
	}
}


//-------------------------------------------------------------------------------

#ifndef MARMOSET_TESTING
int main() {
	unsigned int value_to_protect{17343722};
	unsigned int protected_value = add_checksum(value_to_protect);
	std::cout << "The value " << value_to_protect << " with the checksum added is " << protected_value << "." << std::endl;
	
	if (verify_checksum(protected_value))
	{
		std::cout << "The checksum is valid." << std::endl;
	}
	else   {
		std::cout << "The checksum is invalid." << std::endl;
	} 
	const std::size_t QTY_VALUES {3};
	unsigned int value_series[QTY_VALUES] {20201122, 20209913, 20224012};
	
	add_checksum(value_series, QTY_VALUES);
	
	std::cout << "Series with checksums added: ";
	for (std::size_t series_index {0};
		series_index < QTY_VALUES; series_index++)
	{
		std::cout << value_series[series_index] << " ";
	}

    std::cout << std::endl;
    return 0;

}
#endif