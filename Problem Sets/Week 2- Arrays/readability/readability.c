#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Coleman-Liau index
    //  index = 0.0588 * L - 0.296 * S - 15.8
    string input = get_string("Text: ");
    int letters = 0;
    int sentences = 0;
    int spaces = 0;

    for (int i = 0, len = strlen(input); i < len; i++)
    {
        int alpha = isalpha(input[i]);

        if (input[i] == '.' || input[i] == '?' || input[i] == '!')
        {
            sentences++;
        }
        else if (i != 0 && input[i - 1] != ' ' && input[i] == ' ')
        {
            spaces++;
        }
        else if (alpha != 0)
        {
            letters++;
        }
    }
    int words = spaces + 1;

    float l = ((float) letters / (float) words) * 100.0;
    float s = ((float) sentences / (float) words) * 100.0;

    float CLI = (0.0588 * l) - (0.296 * s) - 15.8;
    int grade = round(CLI);

    if (grade >= 1 && grade < 16)
    {
        printf("Grade %i\n", grade);
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
}
