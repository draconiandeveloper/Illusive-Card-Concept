## Requirements
- GNU Make

## Usage

```
writer {original PNG filename} {new PNG filename}

reader {new PNG filename}
```

## What is this?

Illusion games have a character saving system where they take a screenshot of the in-game character and append the in-game assets and settings that pertain to the character. This also extends to including the mods needed to generate the character in-game.

So I figured that I might as well write a simple proof-of-concept for this system in the C language as this might be a simplified method for those wanting to create their own variant of this system for their game(s).

I chose the C language since it's somewhat simple to comprehend what all is going on and many languages are influenced by the C language to some extent.

If all else fails, you could compile this into a library and set up the C ABI calls in your game to access these functions. I'm sure FFI *(Foreign Function Interface)* can be utilized in your language of choice if available to make use of this if that's your kind of thing.

## Process

PNG files are designed to be streamable so the file size is nowhere to be found within the PNG specifications. To make matters all the more difficult, there's no fixed size or anything that tells us the size of each `IDAT` chunk within a PNG file.

Although we can assume that the input PNG file is not modified, then we could presume that the writer is receiving a PNG file whose `IEND` footer is located at the EOF *(End of File)*.

This is nice for the creation of the "card" but this doesn't help us out all that much for reading the data at the end of the "card".

So the writing process simply starts with reading the contents of the unmodified PNG file into the "card" file.

Then we can append the character data which in this case could be achieved through serializing the character data structure/class and then writing that to the end of the PNG file.

Since all image viewers will stop when they reach the `IEND` PNG footer, the file can still be looked at without issue. This program can also be re-worked to determine if there is any data at the end of a PNG file or to simply strip out any data past the `IEND` PNG footer.

## Issues

Since the reader and writer sources all read from the same header file, one could make a simple check to determine if the program that's accessing the header functions is the writer program so that we can receive modified "card" files and overwrite the character data without appending them over-and-over.

Some systems might not return the actual file size using `fopen`, `fseek`, and `ftell`. On *nix systems this can be solved by using the kernel functions available through `fcntl.h` or using the equivalent Windows SDK function for reading files in `winbase.h`.

Any character name that exceeds 23 characters in length might cause a buffer overflow and the excess characters might be written in the subsequent character data. A simple boundary check will be necessary if you're receiving input from the user to prevent any vulnerabilities that cam stem from a buffer overflow.
