flag=-Wall

build: tema3.c
	gcc $(flag) tema3.c -o image_processing -lm
run: image_processing
	./image_processing

clean: 
	rm -rf image_processing
