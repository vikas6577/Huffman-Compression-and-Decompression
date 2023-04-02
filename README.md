# Huffman-Compression-and-Decompression

Compression and Decompression:

To compress a file, run the following commands:
g++ -o compressing HuffCompress.cpp
compressing <input_file_name>

This will create a compressed file with the name <input_file_name>_compressed.bin.

To decompress a file, run the following commands:
g++ -o decompressing HuffDecompress.cpp
decompressing <compressed_file_name>

This will create a decompressed file with the name <compressed_file_name>_decompressed.txt.

Format of .bin file: 

  3txt (3 is the size of the type, and txt is the type of file)
  a0011NULL 
  b0101NULL
  c0011NULL
  d1111NULL  (d is character, 1111 is variable length code, NULL is to know that one character has ended)
  NULL      (If two NULL present together means codes are ended)
  padding   (padding given to make the input divisible by 8)
  NULL      (NULL to know padding is over)
  000000000 11111111 00000000 11111111 01010101 (The data including the padding divided into 8 bits, making a single character)
