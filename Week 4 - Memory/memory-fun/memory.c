#include <stdio.h>
#include <cs50.h>

int main(void)
{
    char *eg = "hi!";
    int input = get_int("what do you want to search for? (int): ");
    int found = 0;
    int took = 0;
    for(int i = 0; found == 0; i++)
    {
        printf("%i\n", eg[i]);
        took ++;
        if (eg[i] == input)
        {
            printf("found! @ %p in %i iterations.", &eg[i], took);
            found = 1;
        }
    }
}

