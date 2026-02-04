/*
    Author: Gian Carlo Rosario
    netID: gor10
    RUID: 217007258
    The below code is for part 3 of HW1 Group Assignment
*/
#include <iostream>
using namespace std;

/* 
    - The two variables below are global variable and 
    are accessible throughout the entire program
    - Static storage duration
*/
int globalVar;            // automatically initializes to 0
int globalInit = 42;      // initialized

void showStaticLocal() {
    // exists in program, but only visible in this function
    // meaning that outside calls will not be able to use the static function
    static int staticLocal = 0;

    int autoVar = 0;

    staticLocal++;
    autoVar++;

    cout << "  staticLocal = " << staticLocal
         << "  (lives across function calls)\n";

    cout << "  autoVar     = " << autoVar
         << "  (recreated every call)\n";
}

void showUninitializedAuto() {
    int garbage;  // NOT initialized
    cout << "Uninitialized automatic variable contains: "
         << garbage << endl;
}

void scopeExample() {
    int x = 10;  // local to this function cannot be accessed outside of function
    cout << "Inside scopeExample, x = " << x << endl;
}

int main() {
    cout << "(GLOBAL VARIABLES)\n";
    cout << "globalVar (default zero) = " << globalVar << endl;
    cout << "globalInit = " << globalInit << endl;

    cout << "\n(STATIC LOCAL vs AUTOMATIC)\n";
    showStaticLocal();
    showStaticLocal();
    showStaticLocal();

    cout << "\n(UNINITIALIZED AUTO VARIABLE)\n";
    showUninitializedAuto();

    cout << "\n(SCOPE DEMONSTRATION)\n";
    scopeExample();

    // undoing the comment below will cause a compile error:
    // cout << x;  // x does not exist here

    cout << "\nAfter this line of text static and global variables will die.\n";
    return 0;
}
