
## Reference Material

    1. BMP:
`https://www.youtube.com/watch?v=kpHFFFu9qeU` - How BMP works?
    `https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm` - File Structure Documentation

    2. Let us C by Yashwant Kanetkar

## Concepts Involved

1. BMP (bitmap) file format understanding (reference link above)
2. File Handling
3. Bit Manipulation
4. Command Line Arguments
5. String Manipulation
6. Ternary/ Conditional Operator
7. Dynamic Memory Allocation
8. make 

## Required Installations

1. gcc compiler
2. make
   
## How to use this repo

1. clean and build source files using make
`make clean all`

Example: (You can pass your own files and/or secret message)

2. to make a copy of input.bmp file -> input_copy.bmp file will be created
`./bmp_copier input.bmp`

3. to toggle least significant bits of all pixel data bytes of input.bmp file -> input_lsbtoggle.bmp file will be created
`./lsb_toggle input.bmp`

4. to encode 'oho_sanskar' into input.bmp -> input_encoded.bmp file will be created
`./lsb_img_encoder input.bmp oho_sanskar`

5. to decode encoded message from input_encoded.bmp file -> prints encoded message in terminal
`./lsb_img_decoder input_encoded.bmp`


