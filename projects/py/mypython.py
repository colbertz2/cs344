#! /usr/bin/python3

"""
Generates 3 random strings and 2 random ints

Sends 3 lines of 10 characters each (plus newline) to stdout, then to 3 files in
the same directory as this file. Then, sends 2 random ints [1, 42] to stdout,
followed by their product.

Returns:
    3 lines of 10-character random strings (lowercase alpha characters only)
    2 lines, each with a random int in the range [1, 42]
    1 line with the product of the 2 above ints

Yields:
    1 file (pyfile.1, pyfile.2, pyfile.3) for each of the random strings

Notes:
    For CS 344, W2020 (online)

Warnings:
    Output files are overwritten each execution. If files already exist with the
    names "pyfile.1", "pyfile.2", "pyfile.3" they will be overwritten!

Author:
    Zach Colbert (colbertz@oregonstate.edu)
"""

import sys
import os.path as path
import random as rand

########################
# FUNCTION DEFINITIONS #
########################

def RandString(size=10):
    """
    Returns a string of random characters of the specified length.

    Keyword arguments:
        size : int
            Length of the string to return.

    Returns:
        String of random characters.

    Notes:
        Make sure to set the seed for the random library before calling!
        random.seed()
    """

    # Initialize an array of possible output characters
    alpha = [None] * 26
    for i in range(0, 26):
        # Offset i by 97 to get into the Unicode range of lowercase letters
        u = i + 97

        # Get the Unicode character at u and insert into array
        alpha[i] = chr(u)

    # Use the random library to select characters from alpha
    result = [None] * size
    for i in range(0, size):
        result[i] = rand.choice(alpha)

    # Stringify the array of characters and return it
    return ("".join(result))


################
# MAIN ROUTINE #
################
def main():
    # Set seed for random numbers with the current system time
    rand.seed()

    # Working directory is defined as the dir in which this file lives
    wd = path.split(path.abspath(__file__))[0]

    # Random characters will be written to these files
    fnames = ['pyfile.1', 'pyfile.2', 'pyfile.3']

    # Join working dir and filename for absolute path to each file
    outpaths = [None] * 3
    for i, f in enumerate(fnames):
        outpaths[i] = path.join(wd, f)

    # For each output file...
    for p in outpaths:
        # Open file stream for writing 
        f = open(outpaths[i], 'w+')

        # try block acts like a trap for SIGINT
        try:
            # Generate random string
            rstr = RandString()

            # Print to stdout
            sys.stdout.write("%s\n" % rstr)

            # Write string to file
            f.write("%s\n" % rstr)

        except KeyboardInterrupt:
            # Raise the KeyboardInterrupt exception anyway
            # Continue to next block to close file stream
            raise
        finally:
            f.close()

    intmin = 1
    intmax = 42
    rints = [None] * 2
    # Generate 2 random ints in the range [1, 42]
    rints[0] = rand.randrange(intmin, intmax+1)
    rints[1] = rand.randrange(intmin, intmax+1)

    # Write each int to stdout
    sys.stdout.write("%d\n" % rints[0])
    sys.stdout.write("%d\n" % rints[1])

    # Write product of random ints
    prod = rints[0] * rints[1]
    sys.stdout.write("%d\n" % prod)

    return 0

# This runs the function main() when this script is being executed
# (Not called or imported by another script)
if __name__ == "__main__":
    main()