#include <stdio.h>
#include <stdlib.h>
#include "png.h"

// The PNG file header should start with %PNG and end with IEND.
static byte png_header[4] = { 0x89, 0x50, 0x4E, 0x47 };
static byte png_footer[4] = { 0x49, 0x45, 0x4E, 0x44 };

// Compare bytes to ensure that they match or not.
char bytecmp(const byte *haystack, byte *needle, size_t length) {

    // Store the offset for the current byte.
    size_t size = 0u;

    // Iterate through the bytes until we meet the length.
    for (; size < length; ++size) {

        // Break out of the loop if we find two bytes that don't match.
        if (*(haystack + size) != *(needle + size))
            break;
    }

    // Return false if we had to break out of the loop early, otherwise return true.
    return size == length;
}

// Find the byte offset where "needle" is located in the "haystack".
offset find_bytes(const byte *haystack, byte *needle, offset length) {

    // Store the offset for the current byte.
    offset retval = 0lu;

    // Create a byte buffer of 4 bytes to check at a time.
    byte check[4];

    // Iterate through the bytes until we meet the length.
    for (; retval < length; ++retval) {

        // Fill the 4 byte buffer with the haystack data.
        check[0] = *(haystack + retval);
        check[1] = *(haystack + retval + 1);
        check[2] = *(haystack + retval + 2);
        check[3] = *(haystack + retval + 3);

        // Check for matches in the first 4 bytes of the needle with the buffer.
        char check_b1 = check[0] == *(needle + 0);
        char check_b2 = check[1] == *(needle + 1);
        char check_b3 = check[2] == *(needle + 2);
        char check_b4 = check[3] == *(needle + 3);

        // Break out of the loop early if all 4 bytes of the needle match with the buffer.
        if (check_b1 && check_b2 && check_b3 && check_b4)
            return retval;
    }

    // Return 0 if we did not find the needle in the haystack.
    return 0;
}

// Retrieve the contents of a PNG file.
byte *get_filedata(png_t *png) {

    // Set the current PNG file offset to the beginning (byte 0).
    fseek(png->fp, 0, SEEK_SET);

    // Create a variable to store the contents of the PNG file data.
    byte *filedata = (byte *) malloc((size_t) png->filesize);

    // Read the contents of the PNG file into the variable above.
    fread(filedata, 1, png->filesize, png->fp);

    // Return the contents of the PNG file.
    return filedata;
}

// Find the file offset of the IEND PNG footer.
void find_png_footer(png_t *png) {

    // Allocate four bytes of memory to store the PNG file header data.
    byte *header = (byte *) malloc(4);

    // Read the first four bytes of the PNG file and store in the variable above.
    fread(header, 1, 4, png->fp);

    // If the PNG header is not found in the first 4 bytes of the PNG file then throw an error.
    if (!bytecmp(header, png_header, 4)) {
        fputs("Invalid PNG file!\n", stderr);
        png->footer = 0;
        return;
    }

    // Free the memory for the PNG file header variable.
    free(header);

    // Retrieve the full contents of the PNG file.
    byte *filedata = get_filedata(png);

    // Get the file offset for the IEND PNG footer.
    offset footer_offset = find_bytes(filedata, png_footer, png->filesize);

    // Free the memory for the PNG file data since we no longer need it.
    free(filedata);

    // If the function "find_bytes" returned 0 (false) then this means we did not find the IEND PNG footer.
    if (!footer_offset) {
        fputs("Unable to find PNG footer!\n", stderr);
        png->footer = 0;
        return;
    }

    // Set the PNG footer offset to the actual end of the IEND PNG footer since our "find_bytes" function only returns the first byte location.
    png->footer = footer_offset + 8;
}

// Read the contents of a PNG file by the file name.
png_t read_png(const char *filename) {

    // Open the PNG file for reading.
    FILE *fp = fopen(filename, "rb");

    // Check to see if the PNG file could not be opened for reading, likely caused by missing permissions.
    if (!fp) {
        fputs("Could not open file for reading!\n", stderr);
        exit(1);
    }

    // Go to the EOF (End Of File) for the PNG file.
    fseek(fp, 0, SEEK_END);

    // Retrieve the file size for the PNG file.
    offset filesize = (offset) ftell(fp);

    // Go to the beginning of the PNG file (byte 0).
    fseek(fp, 0, SEEK_SET);

    // Initialize the variable that this function returns.
    png_t retval;

    // Set the file pointer for the PNG file.
    retval.fp = fp;

    // Set the file size for the PNG file.
    retval.filesize = filesize;

    // Set the IEND PNG footer byte offset.
    find_png_footer(&retval);

    // Return the PNG file structure.
    return retval;
}