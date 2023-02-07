#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//define block size
#define size 512
//create an 8-bit unsigned integer to store a byte of data
typedef uint8_t BYTE;

//initiate check for jpg parameters
int jpgcheck(BYTE buffer[]);

int main(int argc, char *argv[])
{
    //exit program if argument false
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        exit(1);
    }

    //open crd.raw with correctly provided argument
    FILE *file = fopen(argv[1], "r");

    //exit program if card.raw invalid/absent
    if (file == NULL)
    {
        printf("Unable to open %s\n", argv[1]);
        exit(1);
    }

    //initialize needed variables and allocate necessary memory
    BYTE buffer[size];
    char name[8];
    FILE *image;
    int counter = 0;

    //find consequent jpg images after the first one
    while (fread(buffer, size, 1, file) == 1)
    {
        //if jpg check passed then continue to next step
        if (jpgcheck(buffer) == 1)
        {
            //close previous image with exception of the first image
            if (counter != 0)
            {
                fclose(image);
            }

            //increase counter for adjacent images
            sprintf(name, "%03i.jpg", counter++);
            image = fopen(name, "w");
            fwrite(buffer, size, 1, image);
        }

        else if (counter > 0)
        {
            fwrite(buffer, size, 1, image);
        }

    }

    //close opened memories to avoid valgrind/memory errors
    fclose(file);
    fclose(image);
    return 0;
}

int jpgcheck(BYTE buffer[])
{
    //check if file is jpg usign its specified signature
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return 1;
    }
    return 0;
}