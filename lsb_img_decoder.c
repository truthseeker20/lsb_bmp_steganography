
#include <stdio.h>
#include <stdlib.h>

char* msg_decode(char* filename);
int get_lsb(unsigned char pixel_data);

int main(int argc, char* argv[]) {

    if(argc!=2) {
        printf("Usage: %s <filename.bmp>.\n",argv[0]);
        return 1;
    }

    char* decoded_message = msg_decode(argv[1]);

    if (decoded_message == NULL) {
        printf("message decoding failed.\n");
        return 1;
    }
    else {
        printf("Decoded Message: %s\n",decoded_message);
    }

    free(decoded_message);

    return 0;

}

char* msg_decode(char* filename) {

    //FINDING PIXEL DATA SIZE AND CREATING BUFFER 

    FILE *fp;
    fp = fopen(filename,"rb");
    if(fp==NULL) {
        printf("error: file opening failed.\n");
        return NULL;
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
    printf("pixel data size: %d\n", pixel_data_size);

    unsigned char* buffer = (unsigned char*)malloc(pixel_data_size);
    if (buffer==NULL) {
        printf("error: memory allocation failed.\n");
        fclose(fp);
        return NULL;
    }

    fseek(fp,pixel_data_offset,SEEK_SET);

    fread(buffer,pixel_data_size,1,fp);


    // MESSAGE DECODING -----------------------------------------
    int message_len = 0;
    unsigned char* message = (unsigned char*)malloc(1);
    message[0] = '\0';
    int pixel_data_index=0;

    while(1) {

        unsigned char current_char = 0;

        for (int j=7; j>=0; j--) {

            if (pixel_data_index > pixel_data_size) {
                printf("error: reached end of image before finding message.\n");
                free(message);
                fclose(fp);
                return NULL;
            }

            int lsb_bit = get_lsb(buffer[pixel_data_index]);
            pixel_data_index++;
            //printf("%c",current_char);
            current_char = current_char | (lsb_bit<<j);
        }

        if (current_char == '\0') {
            break;
        }

        message_len++;

        unsigned char* tmp_ptr = (unsigned char*)realloc(message, message_len+1);

        if (tmp_ptr == NULL) {
            printf("error: memory allocation failed.\n");
            free(message);
            fclose(fp);
            return NULL;
        }
        message = tmp_ptr;
        
        message[message_len - 1] = current_char;
        message[message_len] = '\0';

    }

    //FREE MEMORY AND CLOSE FILES
    free(buffer);
    fclose(fp);

    return message;

}

int get_lsb(unsigned char pixel_data) {

    int lsb_bit = (pixel_data & 0x01);
    return lsb_bit;

}

