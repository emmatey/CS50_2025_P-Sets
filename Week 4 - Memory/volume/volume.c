// Modifies the volume of an audio file
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

void copy_header(FILE *input, FILE *output)
{
    // Copy header from input file to output file
    uint8_t buffer[HEADER_SIZE];
    fread(buffer, 1, HEADER_SIZE, input);
    fwrite(buffer, 1, HEADER_SIZE, output);
}

void mutate_data_stream(FILE *input, FILE *output, float factor)
{
    // Read samples from input file and write updated data to output file.
    int16_t buffer;
    // return value of fread is 0 when failing to read.
    while (fread(&buffer, sizeof(int16_t), 1, input) != 0)
    {
        buffer *= factor;
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }
}

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Transform
    copy_header(input, output);
    mutate_data_stream(input, output, factor);

    // Close files
    fclose(input);
    fclose(output);
}

