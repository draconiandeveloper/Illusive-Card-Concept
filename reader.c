#include <stdio.h>
#include "png.h"

typedef struct {
    char     magic[8];     // CHARDAT with the NULL terminator character.
    char     charname[23]; // The character name, set to ensure that the structure is divisible by 8.
    char     level;        // The character level which can be between -128 and 127.
    unsigned gold;         // The character gold which can be between 0 and 4,294,967,295.
} chardata;

// Function for reading the character data from a modified PNG file.
void read_card(chardata *character) {
    // Print out the character data.
    fprintf(
        stdout, 
        "Character Name: %s\nLevel: %d\nGold: %u\n", 
        character->charname, character->level, character->gold
    );
}

// Entrypoint function
int main(int argc, char **argv) {

    // Determine if we are passing the correct number of arguments.
    if (argc != 2) {
        fprintf(stdout, "Usage: %s {input PNG filename}\n", *argv);
        return 0;
    }

    // Skip over the program name.
    --argc; ++argv;

    // Open our PNG file.
    png_t png = read_png(*argv);
    
    // Determine if the footer for the PNG file is outside the boundaries of the PNG file.
    if (png.footer > png.filesize) {
        fputs("PNG footer offset greater than PNG file size!\n", stderr);
        return 1;
    }

    // Set the offset of our file to the IEND PNG footer.
    fseek(png.fp, png.footer, SEEK_SET);

    // Create the variable to store the character data at the end of the PNG file.
    chardata character;

    // Read the contents of the PNG file that stores the custom data into the variable above.
    fread(&character, 1, sizeof(chardata), png.fp);

    // Close the PNG file pointer.
    fclose(png.fp);

    // Create our new PNG file with the contents of our original PNG file with the test character data appended.
    read_card(&character);

    // Obligatory return statement for the entrypoint function.
    return 0;
}