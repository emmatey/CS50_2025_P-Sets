#include <cs50.h>
#include <stdio.h>

int divide(int n, int divisor)
{
    // Base Case
    if(n < divisor)
    {
        return 0;
    }

    // Recursive Case
    int subtraction_counter = divide(n - divisor, divisor);
    subtraction_counter ++;

    return subtraction_counter;
}

int main(void)
{
    int n = get_int("Enter Dividend: ");
    int divisor = get_int("Enter Divisor : ");
    int answer = divide(n, divisor);

    int remainder_check = answer * divisor;
    int remainder = 0;

    if(remainder_check < n)
    {
        remainder = n - (divisor * answer);
    }

    printf("The answer is : %i!\nWith a remainder of : %i!\ni.e %i + (%i/%i)\n", answer, remainder, answer, remainder, divisor);
}
