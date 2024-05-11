#ifndef __POC_PNG_H
#define __POC_PNG_H

#include <stdio.h>

typedef unsigned char byte;
typedef unsigned long offset;

typedef struct {
    FILE *fp;        // The PNG file pointer.
    offset filesize; // The PNG file size.
    offset footer;   // The location for the PNG IEND footer.
} png_t;

byte *get_filedata(png_t *);
png_t read_png(const char *);

#endif