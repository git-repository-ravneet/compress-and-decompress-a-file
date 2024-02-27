# compress-and-decompress-a-file

This C program implements the Huffman compression technique using a combination of a Heap and Priority Queue. Huffman coding is a popular method for lossless data compression, creating a binary tree where characters with lower frequencies have higher priorities.

**Program Structure**
The program consists of several functions and structures:

**Node Structure (struct node):**
Represents a node in the Huffman tree.
Contains character information (ch), frequency (freq), and pointers to left and right children.

**Heap and Priority Queue:**
A priority queue is implemented using a heap.
The insertHeap function inserts a node into the heap while maintaining the heap property.

**Huffman Tree Construction:**
The program reads input from a file and constructs a Huffman tree based on character frequencies.
Priority queue and heap are utilized during the construction process.

**Code Generation:**
Generates Huffman codes for each character in the input file using a recursive approach.
Huffman codes are stored in the c_code array.

**Compression:**
Reads the input file, replaces characters with their Huffman codes, and writes the compressed data to an intermediate file (text2.txt).

**Decompression:**
Reads the compressed data, traverses the Huffman tree, and writes the decompressed data to an output file (text3.txt).

**Compile the program.**

1. Copy code
2. gcc huffman.c -o huffman
3. Run the program.

**Input Files:**
The program reads from text1.txt as the input file.

**Output Files:**
Compressed data is stored in text2.txt.
Decompressed data is stored in text3.txt.

**Note**
The program demonstrates the Huffman compression technique, and the output files (text2.txt and text3.txt) are generated based on the compression and decompression processes. The sample input and output are provided as illustrative examples.
