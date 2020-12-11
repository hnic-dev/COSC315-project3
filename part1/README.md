# COSC315-project3
## Part 1
## December 10th

- Nathan Wickenheiser
- Patrick Mahler
- Aaron Mahnic
- Eric Achenbach

For part 1 of the project we implemented a program to calculate the page number and offset of a given virtual address. The algorithm first extracts the first two lines of the input file which are n (n lowest significant bits that represent the offset) and m (the next m bits that represent the page number). It then loops through each line of the file and calculates the page number and offset. We used bitwise operators as recommended in the instructions, which allowed us to focus on only the bits we needed for the answer. We also chose to use c for this part as we were more familiar using it. 
