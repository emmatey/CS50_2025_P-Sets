#include <cs50.h>
#include <stdio.h>

int do_factorial(int n)
{
    // Base Case
    if (n == 0)
    {
        return 1;
    }
    // Recursive Case
    int answer = n * do_factorial(n-1);
    
    return answer;
}

int main(void)
{
    int factor = get_int("Factorial: ");
    int answer = do_factorial(factor);
    printf("The answer is %i!\n", answer);
}
