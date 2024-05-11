#include <stdio.h>
#include "png.h"

typedef struct {
    char     magic[8];     // CHARDAT with the NULL terminator character.
    char     charname[23]; // The character name, set to ensure that the structure is divisible by 8.
    char     level;        // The character level which can be between -128 and 127.
    unsigned gold;         // The character gold which can be between 0 and 4,294,967,295.
} chardata;

// Function for creating the new PNG file with the user data appended to it.
void create_card(const char *cardname, byte *data, offset filesize, chardata *addon) {

    // Open the new PNG file for writing.
    FILE *fp = fopen(cardname, "wb+");
    
    // fopen should not return 0 (false) unless there was an error.
    if (!fp) {
        fputs("Unable to open card file for writing!\n", stderr);
        return;
    }

    // Write the contents of the original PNG file.
    fwrite(data, 1, filesize, fp);

    // Write the contents of the user data which should append to the new PNG file.
    fwrite(addon, 1, sizeof(chardata), fp);

    // Close the new PNG file stream.
    fclose(fp);
}

// Entrypoint function
int main(int argc, char **argv) {

    // Determine if we are passing the correct number of arguments.
    if (argc != 3) {
        fprintf(stdout, "Usage: %s {input PNG filename} {output PNG filename}\n", *argv);
        return 0;
    }

    // Skip over the program name.
    --argc; ++argv;

    // Open our original PNG file.
    png_t png = read_png(*argv);

    // Skip over the input PNG filename.
    --argc; ++argv;
    
    // Determine if the footer for the PNG file is outside the boundaries of the PNG file.
    if (png.footer > png.filesize) {
        fputs("PNG footer offset greater than PNG file size!\n", stderr);
        return 1;
    }

    // Read the contents of the PNG file.
    byte *filedata = get_filedata(&png);

    // Close the original PNG file pointer.
    fclose(png.fp);

    // Create our test character data to throw into the new PNG file.
    chardata test = (chardata) {
        .magic = "CHARDAT\0",
        .charname = "Test Character",
        .level = 22,
        .gold = 54000  
    };

    // Create our new PNG file with the contents of our original PNG file with the test character data appended.
    create_card(*argv, filedata, png.footer, &test);

    // Obligatory return statement for the entrypoint function.
    return 0;
}