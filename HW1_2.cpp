/*
    Author: Gian Carlo Rosario
    netID: gor10
    RUID: 217007258
    The below code is for part 3 of HW1 Group Assignment
*/
#include <iostream>
#include <cstdint>
using namespace std;

/*
    computes n! using uint64_t.
    this overflows quickly because factorial grows super fast:
    21! already exceeds 64-bit range
*/
uint64_t factorial_u64(uint64_t n) {
    uint64_t f = 1;
    for (uint64_t i = 2; i <= n; ++i)
        f *= i;  // OVERFLOW happens here
    return f;
}

/*
    direct formula: n_r = n! / (r! (n-r)!)
    even when final answer fits in 64 bits, the intermediate
    factorial values overflow BEFORE division happens
 */
uint64_t nr_overflow(uint64_t n, uint64_t r) {
    return factorial_u64(n) / (factorial_u64(r) * factorial_u64(n - r));
}

/*
    safe integer version using cancellation:

    n_r = n(n-1)(n-2)...(n-r+1) / r!

    instead of computing large factorials, we multiply one
    numerator term and divide by one denominator term each step:

    result = result * (n - r + i) / i

    this keeps intermediate values small and prevents overflow
    for moderate inputs like 52_6
*/
uint64_t nr_safe(uint64_t n, uint64_t r) {
    if (r > n) return 0;

    // Use symmetry: n_r == n_(n-r)
    if (r > n - r) r = n - r;

    uint64_t result = 1;

    for (uint64_t i = 1; i <= r; ++i) {
        // Multiply then divide to prevent growth
        result = result * (n - r + i) / i;
    }

    return result;
}

/*
    double-precision version for large values.

    uses same product formula as safe integer version but in
    floating point:
    result *= (n - r + i) / i

    this avoids integer overflow but introduces small rounding
    errors. Suitable for approximations like 200_20
*/
double nr_double(uint64_t n, uint64_t r) {
    if (r > n) return 0.0;
    if (r > n - r) r = n - r;

    double result = 1.0;

    for (uint64_t i = 1; i <= r; ++i) {
        result *= (double)(n - r + i) / (double)i;
    }

    return result;
}

int main() {
    // demonstrates overflow despite small final answer
    cout << "overflow 52_6: " << nr_overflow(52, 6) << endl;

    // dorrect exact integer computation
    cout << "no overflow  52_6: " << nr_safe(52, 6) << endl;

    // large value approximation
    cout << "Double 200_20 ≈ " << nr_double(200, 20) << endl;
}
