
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _BV(x) (1<<x)

int msg_encode(char* filename, char* message);
unsigned char change_lsb(unsigned char pixel_data, int bit_to_hide);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename.bmp> <message>.\n",argv[0]);
        return 1;
    }
    return (msg_encode(argv[1],argv[2])) ? 1 : 0;
}

int msg_encode(char* filename, char* message) {

    //FINDING PIXEL DATA SIZE AND CREATING BUFFER 

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

    unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char)*pixel_data_size);

    if (buffer==NULL) {
        printf("error: memory allocation failed.\n");
        fclose(fp);
        return 1;
    }

    fseek(fp,pixel_data_offset,SEEK_SET);

    fread(buffer,pixel_data_size,1,fp);


    //MESSAGE ENCODING ---------------------------------

    int message_len = strlen(message);
    int pixel_data_index=0;
    printf("msg_len: %d, pixel_data_size: %d\n ",message_len,pixel_data_size);
    if (pixel_data_size < (message_len+1)*8) {
        printf("error: image is not large enough to hide the message.\n");
        fclose(fp);
        return 1;
    }

    for (int i=0; i<=message_len; i++) {
        for (int j=7; j>=0; j--) //first decode msb 
        {
            int bit_to_hide = (message[i]>>j) & 1;
            buffer[pixel_data_index] = change_lsb(buffer[pixel_data_index],bit_to_hide);
            pixel_data_index++;
        }
    } 

    //OUTPUT FILENAME MAKING ---------------------------

    int ip_str_len = strlen(filename); //input.bmp
    int op_str_len = ip_str_len + 8; // input_encoded.bmp

    char* output_file = (char*)malloc(sizeof(char)*op_str_len);

    if (output_file==NULL) {
        printf("error: memory allocation failed.\n");
        fclose(fp);
        return 1;
    }

    strncpy(output_file,filename,ip_str_len-4);
    output_file[ip_str_len-4] = '\0';
    strcat(output_file,"_encoded.bmp");

    FILE *fnew = fopen(output_file,"wb");

    if (fnew==NULL) {
        printf("error: opening file failed.\n");
        fclose(fp);
        return 1;
    }

    // WRITING HEADER AND BUFFER TO OUTPUT FILE ---------
    
    unsigned char header[54];

    rewind(fp);
    fread(header,54,1,fp);
    fwrite(header,54,1,fnew);
    fwrite(buffer,pixel_data_size,1,fnew);

    free(buffer);
    free(output_file);
    fclose(fp);
    fclose(fnew);

    return 0;

}

unsigned char change_lsb(unsigned char pixel_data, int bit_to_hide) {

    return (pixel_data & 0xfe) | bit_to_hide;

}
