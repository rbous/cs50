#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *f = fopen(argv[1], "r");

    // While there's still data left to read from the memory card
    typedef uint8_t BYTE;

    BYTE b[512];
    int num = 0;
    char filename[8] = "000.jpg";
    FILE *img = NULL;

    while (fread(&b, sizeof(BYTE), 512, f) == 512)
    {
        if (b[0] == 0xff && b[1] == 0xd8 && b[2] == 0xff && (b[3] & 0xf0) == 0xe0)
        {

            // Create JPEGs from the data

            if (img != NULL)
            {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", num);
            num++;
            img = fopen(filename, "w");
        }

        if (img != NULL)
        {
            fwrite(&b, sizeof(BYTE), 512, img);
        }
    }

    if (img != NULL)
    {
        fclose(img);
    }

    fclose(f);
}
