
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int lsb_modify(char* filename);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename.bmp>.\n",argv[0]);
        return 1;
    }
    return (lsb_modify(argv[1])) ? 1 : 0;
}

int lsb_modify(char* filename) {

    FILE *fp;
    fp = fopen(filename,"rb");
    if(fp==NULL) {
        printf("error: file opening failed.\n");
        return 1;
    }
    fseek(fp,2,SEEK_SET);

    unsigned char file_size_bytes[4];
    fread(&file_size_bytes,4,1,fp);
    int file_size = file_size_bytes[0] + (file_size_bytes[1]<<8) + (file_size_bytes[2]<<16) + (file_size_bytes[3]<<24);
    printf("file size: %d\n",file_size);

    fseek(fp,10,SEEK_SET);
    unsigned char pixel_data_offset_bytes[4];
    fread(pixel_data_offset_bytes,4,1,fp);
    int pixel_data_offset = pixel_data_offset_bytes[0] + (pixel_data_offset_bytes[1]<<8) + (pixel_data_offset_bytes[2]<<16) +(pixel_data_offset_bytes[3]<<24);
    printf("pixel data offset: %d\n",pixel_data_offset);

    int pixel_data_size = file_size - pixel_data_offset;

    unsigned char buffer[pixel_data_size];

    fseek(fp,pixel_data_offset,SEEK_SET);

    fread(buffer,pixel_data_size,1,fp);

    for (int i=0; i<pixel_data_size; i++) {
        buffer[i] = buffer[i] ^ 0x01;
    }

    int ip_str_len = strlen(filename); //input.bmp
    int op_str_len = ip_str_len + 10; // input_lsbtoggle.bmp

    char* output_file = (char*)malloc(sizeof(char)*op_str_len);

    if (output_file==NULL) {
        printf("error: memory allocation failed.\n");
        return 1;
    }

    strncpy(output_file,filename,ip_str_len-4);
    strcat(output_file,"_lsbtoggle.bmp");

    FILE *fnew = fopen(output_file,"wb");

    if (fnew==NULL) {
        printf("error: opening file failed.\n");
        return 1;
    }

    unsigned char header[54];

    rewind(fp);
    fread(header,54,1,fp);
    fwrite(header,54,1,fnew);
    fwrite(buffer,pixel_data_size,1,fnew);

    fclose(fp);
    fclose(fnew);

    return 0;

}
