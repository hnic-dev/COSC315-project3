#include <iostream>
#include <fstream>
#include <string>
using namespace std;
typedef int int32;
//typedef char byte;

class myFileSystem {
public:
  int create(char *name, int32 size);
  int del(char *name);
  void ls();
  int read(char *name, int blockNum, char *buf);
  int write(char *name, int blockNum, char *buf);
  string DNAME;
  void writeSuperBlock();
  myFileSystem(string diskName) {
   // Open the file with name diskName
    DNAME = diskName;
    this->writeSuperBlock();
   // Read the first 1KB and parse it to structs/objecs representing the super block
   // 	An easy way to work with the 1KB memory chunk is to move a pointer to a
   //	position where a struct/object begins. You can use the sizeof operator to help
   //	cleanly determine the position. Next, cast the pointer to a pointer of the
   //	struct/object type.

   // Be sure to close the file in a destructor or otherwise before
   // the process exits.
 }
private:
  /* The first 128 bytes stores the free block list. Each entry in this list
  indicates whether the corresponding block is free or in use (if the i-th
  byte is 0, it indicates that the block is free, else it is in use). Intially all
  blocks except the super block are free */
  char free_block_list[128] = {0};
  struct inode {
    char name; //file name
    int size; // file size (in number of blocks)
    char *blockPointers[8]; // direct block pointers
    int used; // 0 => inode is free; 1 => in use
  };
  inode inodes[16];
};

void myFileSystem::writeSuperBlock() {
     ofstream of(DNAME, ofstream::binary | ofstream::app);
   of.write(free_block_list, sizeof(free_block_list));
   for(int i=0; i<16; i++) {
     inode *in = &inodes[i];
     of.write((char*)&in->name, 8);
     of.write((char*)&in->size, sizeof(in->size));
     for(int i=0;i<8;i++)
       of.write((char*)&in->blockPointers[i], sizeof(in->blockPointers[i]));
     of.write((char*)&in->used, sizeof(in->used));
   }
   //pad with Zeros for the last 128 bytes
   int* padding = (int*)malloc(128);
   of.write((char*)&padding, 128);
   of.close();
} 

int myFileSystem::create(char *name, int size) {
  //create a file with this name and this size

  // high level pseudo code for creating a new file

  // Step 1: Look for a free inode by searching the collection of objects
  // representing inodes within the super block object.
  // If none exist, then return an error.
  // Also make sure that no other file in use with the same name exists.
  string file = name;
  int inodeCount = 1;
  for (int i=0; i<16; i++) {
    if (file == std::to_string(inodes[i].name)) {
      cout << "Error: filename already exists" << endl;
      return 0;
    }
    if (!inodes[i].used) break;
    else inodeCount++;
  }

  // Step 2: Look for a number of free blocks equal to the size variable
  // passed to this method. If not enough free blocks exist, then return an error.
  int blockCount = 0;
  for (int i=0; i<128; i++) {
    if (free_block_list[i] == 0) {
      for (int j=i; j<size; j++) {
	if (free_block_list[j] == 1) break;
	else if (j == size-1) blockCount = j+1;
	j++;
      }
    }
  }

  if (blockCount == 0) {
    cout << "No free space!" << endl;
    return 0;
  }

  // Step 3: Now we know we have an inode and free blocks necessary to
  // create the file. So mark the inode and blocks as used and update the rest of
  // the information in the inode.
  inodes[blockCount].name = *name;
  inodes[blockCount].size = size;
  inodes[blockCount].used = 1;
  for(int i=0; i<size; i++)
    inodes[blockCount].blockPointers[i] = &free_block_list[(i+blockCount)];

  //int start = blockCount - size;
  //for (int i=start; i < blockCount; i++)
  //  free_block_list[i] = 1;
  
  // Step 4: Write the entire super block back to disk.
  //	An easy way to do this is to seek to the beginning of the disk
  //	and write the 1KB memory chunk.
  this->writeSuperBlock();

  //Troubleshooting, print out free_block_list
  //for (int i=0; i<128; i++) {
  //  int Int32 = 0;
  //  Int32 = (Int32 << 8) + free_block_list[i];
  //  cout << Int32;
  //}
  return 1;
}

int myFileSystem::del(char *name) {
  // Delete the file with this name

  // Step 1: Look for an inode that is in use with given name
  // by searching the collection of objects
  // representing inodes within the super block object.
  // If it does not exist, then return an error.
  string file = name;
  int file_index = 16;
  for (int i=0; i<16; i++) {
    if (file == std::to_string(inodes[i].name)) {
      file_index = i;
      break;
    }
  }
  if (file_index == 16) {
    cout << "File does not exist" << endl;
    return 0;
  }

  // Step 2: Free blocks of the file being deleted by updating
  // the object representing the free block list in the super block object.
  for(int i=0; i<inodes[file_index].size; i++)
    inodes[file_index].blockPointers[i] = 0;

  // Step 3: Mark inode as free.
  inodes[file_index].used = 0;

  // Step 4: Write the entire super block back to disk.
  this->writeSuperBlock();
  return 1;
} // End Delete


void myFileSystem::ls() { 
  // List names of all files on disk

  // Step 1: Print the name and size fields of all used inodes.
  for (int i=0; i<16; i++) {
  if (inodes[i].used == 1)
    cout << "Inode [" << i << "] name: " << inodes[i].name << endl;
    cout << "Inode [" << i << "] size: " << inodes[i].size << endl;
  }
} // End ls

int myFileSystem::read(char *name, int blockNum, char *buf) {
   // read this block from this file
   // Return an error if and when appropriate. For instance, make sure
   // blockNum does not exceed size-1.

   // Step 1: Locate the inode for this file as in Step 1 of delete.

   // Step 2: Seek to blockPointers[blockNum] and read the block
   // from disk to buf.

} // End read


int myFileSystem::write(char *name, int blockNum, char *buf) {

   // write this block to this file
   // Return an error if and when appropriate.

   // Step 1: Locate the inode for this file as in Step 1 of delete.

   // Step 2: Seek to blockPointers[blockNum] and write buf to disk.
   
} // end write


int main(int argc, char** argv) {
  myFileSystem fs("disk0");
  char t = 't';
  fs.create((char*)&t, 8);
}
