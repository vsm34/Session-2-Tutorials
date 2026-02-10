/*
Author: Shiv Sharma
netID: ss4166
RUID: 217003095

Below is the code for group 1 assignment part 5

*/

#include <iostream>


  

// Function prototype for adding two 32-bit integers
int add32(int a, int b);

// Function prototype for multiplying two 32-bit integers
int mul32(int a, int b);

// Function prototype for dividing two 32-bit integers
int div32(int a, int b);

// Function prototype for adding two 64-bit integers
long long add64(long long a, long long b);

// Function prototype for multiplying two 64-bit integers
long long mul64(long long a, long long b);

// Function prototype for dividing two 64-bit integers
long long div64(long long a, long long b);

// Function prototype for adding two double-precision values
double addDouble(double a, double b);

// Function prototype for multiplying two double-precision values
double mulDouble(double a, double b);

// Function prototype for dividing two double-precision values
double divDouble(double a, double b);

// Function prototype for displaying the global variable
void showGlobal();


// Main function where program execution begins
int main()
{

// Print a header message to the console
std::cout << "===== Systems Programming Demo =====\n\n";
      
// Declare and initialize two 32-bit integer variables
int a = 10, b = 3;

// Declare and initialize two 64-bit integer variables
long long x = 100000, y = 3;

// Declare and initialize two double-precision variables
double d1 = 10.5, d2 = 3.2;

// Print label for 32-bit integer operations
    std::cout << "32-bit int:\n";

    // Call add32() and print the result
    std::cout << "Add: " << add32(a, b) << "\n";

    // Call mul32() and print the result
    std::cout << "Mul: " << mul32(a, b) << "\n";

    // Call div32() and print the result
    std::cout << "Div: " << div32(a, b) << "\n\n";

    // Print label for 64-bit integer operations
    std::cout << "64-bit int:\n";

    // Call add64() and print the result
    std::cout << "Add: " << add64(x, y) << "\n";

    // Call mul64() and print the result
    std::cout << "Mul: " << mul64(x, y) << "\n";

    // Call div64() and print the result
    std::cout << "Div: " << div64(x, y) << "\n\n";

    // Print label for double-precision operations
    std::cout << "Double:\n";

    // Call addDouble() and print the result
    std::cout << "Add: " << addDouble(d1, d2) << "\n";

    // Call mulDouble() and print the result
    std::cout << "Mul: " << mulDouble(d1, d2) << "\n";

    // Call divDouble() and print the result
    std::cout << "Div: " << divDouble(d1, d2) << "\n\n";

    
    // Print label for global variable demonstration
    std::cout << "Global Variable Demo:\n";

    // Call function that prints the global variable
    showGlobal();

    // Print message indicating program completion
    std::cout << "\nProgram Finished\n";

    // Return 0 to indicate successful execution
    return 0;
}




// Takes two integers and returns their sum
int add32(int a, int b)
{
    return a + b;
}


// Takes two integers and returns their product
int mul32(int a, int b)
{
    return a * b;
}


// Takes two integers and returns their quotient
int div32(int a, int b)
{
    return a / b;
}




// Takes two long long values and returns their sum
long long add64(long long a, long long b)
{
    return a + b;
}


// Takes two long long values and returns their product
long long mul64(long long a, long long b)
{
    return a * b;
}


// Takes two long long values and returns their quotient
long long div64(long long a, long long b)
{
    return a / b;
}




// Takes two doubles and returns their sum
double addDouble(double a, double b)
{
    return a + b;
}

/
// Takes two doubles and returns their product
double mulDouble(double a, double b)
{
    return a * b;
}


// Takes two doubles and returns their quotient
double divDouble(double a, double b)
{
    return a / b;
}


// Prints the value of the global variable
void showGlobal()
{
    std::cout << "Global variable value: " << globalVar << "\n";
}