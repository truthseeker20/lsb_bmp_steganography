
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

    if (argc!=2) {
        printf("Usage: %s <filename.bmp>.\n",argv[0]);
        return 1; 
    }
    
    FILE *fp;
    fp = fopen(argv[1],"rb");
    if (fp == NULL) {
        printf("Error: File can't be opened.\n");
        return 1;
    }

    unsigned char header[54];

    fread(header,54,1,fp);

    printf("bits per pixel: %d bits\n",header[28]);

    int file_size = header[2] + (header[3]<<8) + (header[4]<<16) + (header[5]<<24);
    printf("file size: %d bytes\n",file_size);

    int pixel_data_offset = header[10] + (header[11]<<8) + (header[12]<<16) + (header[13]<<24);
    printf("pixel data offset: %d bytes\n",pixel_data_offset);

    int pixel_data_size = file_size - pixel_data_offset;
    printf("pixel data size: %d bytes\n",pixel_data_size);

    unsigned char *pixel_data_ptr = (unsigned char*)malloc(pixel_data_size*(sizeof(unsigned char)));

    if (pixel_data_ptr == NULL) {
        printf("Memory Allocation Failed.\n");
        return 1;
    }

    fseek(fp,pixel_data_offset,SEEK_SET); //moves the pointer to beginning of file then move pixel_data_offset bytes
    fread(pixel_data_ptr,pixel_data_size,1,fp);


    //OUTPUT FILE NAMING
    
    char* last_dot_locator_ptr = strrchr(argv[1],'.');

    int ip_str_len = strlen(argv[1]); //input.bmp
    int op_str_len = ip_str_len + 5; //input_copy.bmp

    char* op_filename_ptr = (char*)malloc(sizeof(char)*op_str_len);
    if (op_filename_ptr == NULL) {
        printf("error: memory allocation for output filename failed.\n");
        return 1;
    }

    strncpy(op_filename_ptr, argv[1], ip_str_len-4);

    strcat(op_filename_ptr, "_copy.bmp");
    

    FILE* fnew = fopen(op_filename_ptr,"wb");

    fwrite(header,54,1,fnew);

    fwrite(pixel_data_ptr,pixel_data_size,1,fnew);

    fclose (fp);
    fclose(fnew);

    free(pixel_data_ptr);
    free(op_filename_ptr);

    return 0;

}
