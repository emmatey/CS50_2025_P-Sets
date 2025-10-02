#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validate_input(int argc, char *argv[], char *CIPHER_KEY)
{
    if (argc != 2)
    {
        printf("One and only one argument please!\nargc must be == 2");
        return (1);
    }

    int cipher_len = strlen(argv[1]);
    if (cipher_len != 26)
    {
        printf("Cipher Key must be length = 26\n");
        return (1);
    }
    // Check that all chars in input are alpha.
    for (int i = 0; i < 26; i++)
    {
        int token = isalpha(CIPHER_KEY[i]);
        if (token == 0)
        {
            printf("Cipher Key elements must be alpha chars\n");
            return (1);
        }
    }
    // check for uniqueness. Boolean array, each index represents ascii distnce from a
    int bool_array[26] = {0};
    for (int i = 0; i < 26; i++)
    {
        // Ths causes a side effect that to_lowers the var in main also.
        *(CIPHER_KEY + i) = tolower(CIPHER_KEY[i]);
    }
    // calculate the distance from lowercase a, and set that index in
    // bool array to 1, if it's already 1, return 1, not unique.
    for (int i = 0; i < 26; i++)
    {
        int distance_from_a = CIPHER_KEY[i] - 'a';
        if (bool_array[distance_from_a] == 1)
        {
            printf("Cipher Key elements must be unique\n");
            return 1;
        }
        if (bool_array[distance_from_a] == 0)
        {
            bool_array[distance_from_a] = 1;
        }
    }
    return 0;
}

int case_agnostic_distance_from_a(char character)
{
    if (character >= 'A' && character <= 'Z')
    {
        int distance = (character - 'A');
        return distance;
    }

    if (character >= 'a' && character <= 'z')
    {
        int distance = (character - 'a');
        return distance;
    }
    return 1;
}

char *encode_input(char *input, char *CIPHER_KEY)
{   // The goal of translator_array is to get an int that I can add to each individual char in the 
    //input to mutate it. This process is case insensative. ASCII is just ints.
    int translator_array[26] = {0};
    for (int i = 0; i < 26; i++)
    {
        int alpha_loc = ('a' + i);
        // distance between standard letter order and cipher letter order
        int loc_delta = (CIPHER_KEY[i] - alpha_loc);
        translator_array[i] = loc_delta;
    }

    int input_len = strlen(input);
    int input_mem_size = (strlen(input) + 1); // +1 to account for null
    char *cipher_text = malloc(input_mem_size * sizeof(char));

    for (int i = 0; i < input_len; i++)
    {
        if (isalpha(input[i]) != 0) // ret 0 = not alpha, != 0 is alpha.
        {
            int distance = case_agnostic_distance_from_a(input[i]);
            char mutated_input = input[i] + translator_array[distance];
            cipher_text[i] = mutated_input;
        }
        if (isalpha(input[i]) == 0)
        {
            cipher_text[i] = input[i];
        }
    }
    cipher_text[input_len] = '\0';

    return cipher_text;
}

int main(int argc, char *argv[])
{
    char *CIPHER_KEY = argv[1];
    int ret_code = validate_input(argc, argv, CIPHER_KEY);

    if (ret_code != 0)
    {
        return 1; // i.e end
    }
    if (ret_code == 0)
    {
        char *input = get_string("plaintext: ");
        char *cipher_text = encode_input(input, CIPHER_KEY);
        printf("ciphertext: %s\n", cipher_text);
        free(cipher_text);
    }
}
