#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Read the file with fread to get the pointer to the start of the file.
    if (argc != 2)
    {
        printf("Please provide only one argument, a file path to the input.\n");
        return 1;
    }
    FILE *mem_card = fopen(argv[1], "r");
    if (mem_card == NULL)
    {
        printf("Input file %s could not be read.\n", argv[1]);
        return 1;
    }

    unsigned char *read_buffer = malloc(sizeof(unsigned char) * 512);
    if (read_buffer == NULL)
    {
        fclose(mem_card);
        printf("unable to allocate memory for read_buffer.\n");
        return 2;
    }
    // initialize to -1 to catch picture 000
    int counter = 0;
    FILE *nth_file = NULL;
    bool found = false;

    while (fread(read_buffer, 512, 1, mem_card) == 1)
    {
        bool jpg_sig = (read_buffer[0] == 0xff) && (read_buffer[1] == 0xd8) &&
                       (read_buffer[2] == 0xff) && (read_buffer[3] >= 0xe0) &&
                       (read_buffer[3] <= 0xef);

        if (jpg_sig)
        {
            found = true;

            if (nth_file != NULL)
            {
                fclose(nth_file);
            }

            char new_file_name[8];
            sprintf(new_file_name, "%.3i.jpg", counter);

            nth_file = fopen(new_file_name, "w");
            if (nth_file != NULL)
            {
                fwrite(read_buffer, 512, 1, nth_file);
            }

            counter++;
        }
        else if (found == true)
        {
            fwrite(read_buffer, 512, 1, nth_file);
        }
    }

    if(found == true)
    {
        fclose(nth_file);
    }
    fclose(mem_card);
    free(read_buffer);
    return 0;
}