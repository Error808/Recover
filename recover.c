#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //check if exactly one command line has been used
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    //opens the image for reading
    FILE *card = fopen(argv[1], "r");
    if (!card)
    {
        fprintf(stderr, "Image cannot be opened for reading\n");
        return 2;
    }

    //unsigned char buffer that can hold 512 of 1 byte
    unsigned char buffer[512];
    //count number of pics
    int pics = 0;

    //buffer for sprintf
    char *bufferS = malloc(sizeof(char) * 7);

    //pointer to a new JPEG file
    FILE *jPEG = NULL;

    int x = 0;
    //scan the file
    while (1)
    {
        int s = fread(buffer, 1, 512, card);
        //JPEG signatures?
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //found a new jpeg?
            if (jPEG != NULL)
            {
                fclose(jPEG);
                sprintf(bufferS, "%03d.jpg", x);
                jPEG = fopen(bufferS, "w");
                fwrite(buffer, 1, s, jPEG);
                x++;
                pics++;
            }
            //very first jpeg
            else
            {
                sprintf(bufferS, "%03d.jpg", x);
                jPEG = fopen(bufferS, "w");
                fwrite(buffer, 1, s, jPEG);
                x++;
                pics++;
            }
        }
        //resume of the previous jpeg
        else if (jPEG != NULL)
        {
            fwrite(buffer, 1, s, jPEG);
        }
        // if it doesnt read any more byte, its end of the file
        if (s == 0)
        {
            break;
        }

    }

    printf("%d\n", pics);
    fclose(card);
    fclose(jPEG);
    free(bufferS);
}

