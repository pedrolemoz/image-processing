# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct {
	unsigned char blue, green, red;
} PIXEL;

typedef struct {
	int width, height, padding;
	PIXEL **pixels;
	unsigned char header[54];
} BMP;

void show_image(BMP image);
void grayscale(PIXEL pixel);
void save_to_file(BMP image);
BMP BGR_to_grayscale(BMP image);
BMP load_image(char filename[60]);
BMP rotate_90_clockwise(BMP image);
BMP rotate_90_counterclockwise(BMP image);
BMP copy_image(BMP image);
BMP invert_horizontally(BMP image);
BMP invert_vertically(BMP image);

void main(){
	BMP modified_image;
	int option;

	printf("Image Processing for BMP files without padding\n\n");

	BMP image = load_image("images/bmp_4x4.bmp");

	printf("\nImage info:\nWidth: %d\nHeight: %d\nPadding: %d\nImage pixels:\n\n", image.width, image.height, image.padding);
	show_image(image);
	printf("\nChoose an option\n\n[1] - Copy image\n[2] - Rotate 90 degrees right\n[3] - Rotate 90 degrees left\n[4] - Invert horizontally\n[5] - Invert vertically\n[6] - Convert to grayscale\n[0] - Exit program\n>>> ");
	scanf("%d", &option);
	fflush(stdin);

	switch(option){
		case 0:
			printf("\nExiting...\n");
			exit(1);
		case 1:
			modified_image = copy_image(image);
			show_image(modified_image);
			save_to_file(modified_image);
			break;
		case 2:
			modified_image = rotate_90_clockwise(image);
			show_image(modified_image);
			save_to_file(modified_image);
			break;
		case 3:
			modified_image = rotate_90_counterclockwise(image);
			show_image(modified_image);
			save_to_file(modified_image);
			break;
		case 4:
			modified_image = invert_horizontally(image);
			show_image(modified_image);
			save_to_file(modified_image);
			break;
		case 5:
			modified_image = invert_vertically(image);
			show_image(modified_image);
			save_to_file(modified_image);
			break;
		case 6:
			modified_image = BGR_to_grayscale(image);
			show_image(modified_image);
			save_to_file(modified_image);
			break;
	}
}

BMP load_image(char filename[60]){
	FILE *file;
	BMP image;
	file = fopen(filename, "rb");

	if (file == NULL){
		printf("Cannot load the image.\n\nPossible reasons:\n  - File or directory doesn't exists\n  - Denied permissions\n");
		exit(1);
	}
	else {
		printf("Image loaded sucessfully.\n");
	}

	unsigned char header[54];
	
	fread(header, sizeof(unsigned char), 54, file);
	
	int width = header[18];
	int height = header[22];

	int padding = 0;
	while ((width + padding) % 4 != 0){
		padding++;
	}

	PIXEL **pixels = (PIXEL **) malloc(height * sizeof(PIXEL *));
    
	for (int i = 0; i < height; i++){
		pixels[i] = (PIXEL *) malloc(width * sizeof(PIXEL));
	}

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			fread(&pixels[i][j].blue, 1, 1, file);
			fread(&pixels[i][j].green, 1, 1, file);
			fread(&pixels[i][j].red, 1, 1, file);
		}
	}

	image.width = width;
	image.height = height;
	image.padding = padding;
	image.pixels = pixels;
	strcpy(image.header, header);

	fclose(file);

	return image;
}

void save_to_file(BMP image){
	int option;
	FILE *image_file;

	printf("\nDo you wanna save the file?\n\n[1] - Yes\n[2] - No\n>>> ");
	scanf("%d", &option);

	switch (option){
		case 1:
			image_file = fopen("modified.bmp", "wb");

			for (int i = 0; i < 54; i++){
				fputc(image.header[i], image_file);
			}

			for (int i = 0; i < image.width; i++){
				for (int j = 0; j < image.width; j++){
					fputc(image.pixels[i][j].blue, image_file);
					fputc(image.pixels[i][j].green, image_file);
					fputc(image.pixels[i][j].red, image_file);
				}
			}

			fclose(image_file);

			printf("\nSaved sucessfully");
			break;
		case 2:
			break;
		default:
			printf("\nInvalid option\n");
	}

}

void show_image(BMP image){
	printf("\n");
	for (int i = 0; i < image.height; i++){
		for (int j = 0; j < image.width; j++){
			printf("|%d, %d, %d", image.pixels[i][j].blue, image.pixels[i][j].green, image.pixels[i][j].red);
		}
		printf("|\n");
	}
}

BMP rotate_90_clockwise(BMP image){
	PIXEL **rotated_pixels = (PIXEL **) malloc(image.height * sizeof(PIXEL));

	for (int i = 0; i < image.height; i++){
		rotated_pixels[i] = (PIXEL *) malloc(image.width * sizeof(PIXEL));
	}
	
	for (int i = 0; i < image.height; i++){
		for (int j = 0; j < image.width; j++){
			rotated_pixels[i][j] = image.pixels[image.width - 1 - j][i];
		}
	}

	BMP rotated_image = {image.height, image.width, image.padding, rotated_pixels};
	strcpy(rotated_image.header, image.header);
	
	return rotated_image;
}

BMP rotate_90_counterclockwise(BMP image){
	PIXEL **rotated_pixels = (PIXEL **) malloc(image.height * sizeof(PIXEL));

	for (int i = 0; i < image.height; i++){
		rotated_pixels[i] = (PIXEL *) malloc(image.width * sizeof(PIXEL));
	}
	
	for (int i = 0; i < image.height; i++){
		for (int j = 0; j < image.width; j++){
			rotated_pixels[i][j] = image.pixels[j][image.height - 1 - i];
		}
	}

	BMP rotated_image = {image.height, image.width, image.padding, rotated_pixels};
	strcpy(rotated_image.header, image.header);

	return rotated_image;
}

BMP invert_horizontally(BMP image){
	PIXEL **inverted_pixels = (PIXEL **) malloc(image.height * sizeof(PIXEL));

	for (int i = 0; i < image.height; i++){
		inverted_pixels[i] = (PIXEL *) malloc(image.width * sizeof(PIXEL));
	}
	
	for (int i = 0; i < image.height; i++){
		for (int j = 0; j < image.width; j++){
			inverted_pixels[i][j] = image.pixels[i][image.width - 1 - j];
		}
	}

	BMP inverted_image = {image.height, image.width, image.padding, inverted_pixels};
	strcpy(inverted_image.header, image.header);

	return inverted_image;
}

BMP invert_vertically(BMP image){
	PIXEL **inverted_pixels = (PIXEL **) malloc(image.height * sizeof(PIXEL));

	for (int i = 0; i < image.height; i++){
		inverted_pixels[i] = (PIXEL *) malloc(image.width * sizeof(PIXEL));
	}
	
	for (int i = 0; i < image.height; i++){
		for (int j = 0; j < image.width; j++){
			inverted_pixels[i][j] = image.pixels[image.height - 1 - i][j];
		}
	}

	BMP inverted_image = {image.height, image.width, image.padding, inverted_pixels};
	strcpy(inverted_image.header, image.header);

	return inverted_image;
}

BMP copy_image(BMP image){
	BMP copy;

	copy.height = image.height;
	copy.width = image.width;
	copy.padding = image.padding;
	copy.pixels = image.pixels;
	copy.padding = image.padding;
	strcpy(copy.header, image.header);
	
	return copy;
}

BMP BGR_to_grayscale(BMP image){
	PIXEL **grayscale_pixels = (PIXEL **) malloc(image.height * sizeof(PIXEL));
	int grayscale_color;

	for (int i = 0; i < image.height; i++){
		grayscale_pixels[i] = (PIXEL *) malloc(image.width * sizeof(PIXEL));
	}

	for (int i = 0; i < image.width; i++){
		for (int j = 0; j < image.width; j++){
			grayscale_color = ((0.3 * image.pixels[i][j].red) + (0.59 * image.pixels[i][j].green) + (0.11 * image.pixels[i][j].blue));
			grayscale_pixels[i][j].red = grayscale_pixels[i][j].green = grayscale_pixels[i][j].blue = grayscale_color;
		}
	}

	BMP grayscale_image = {image.width, image.height, image.padding, grayscale_pixels};
	strcpy(grayscale_image.header, image.header);

	return grayscale_image;
}
