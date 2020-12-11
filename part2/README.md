# Part 2

For the file system, we chose to use c++ to take advantage of the OOP features that are not included with C. The inode is implemented as a struct, and the free block list is an array of chars (which are bytes).
The code closely follows the boilerplate code and comments that was provided, so it should be easy to see what we implemented for each step.

Things that are tested working:
- The superblock is created corrected, at the correct size as per the specification. It is even padded with zeroes to fill exactly 1kb.
- The create method has been tested and correctly finds free inodes and blocks.

Due to time constraints, which were exasperated by weird type casting issues and pointer problems, we were not able to completely test:
- read
- write

As such these methods are incomplete, but we feel we have the knowledge to implement them accordingly.

Since read and write do not work correctly, when running the test file, the output is a bit odd. The program at least runs without seg faulting!
