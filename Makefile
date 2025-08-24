
clean:
	rm -rf *_copy.bmp *_lsbtoggle.bmp *_encoded.bmp bmp_copier lsb_toggle lsb_img_encoder lsb_img_decoder

all: lsb_img_encoder lsb_img_decoder lsb_toggle bmp_copier

lsb_img_encoder:lsb_img_encoder.c
	gcc -o $@ $^

lsb_img_decoder:lsb_img_decoder.c
	gcc -o $@ $^

lsb_toggle:lsb_toggle.c
	gcc -o $@ $^

bmp_copier:bmp_copier.c
	gcc -o $@ $^
