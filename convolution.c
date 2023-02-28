#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int **value;
    int M;
    int N;
    int IS_IMG_EXIST;

} image_struct;

image_struct get_image()
{
    image_struct image;
    // Allocating memory for img
    int **img = (int **)malloc(10000 * sizeof(int *));
    // for matrix allocate 10000 * array (rows)

    for (int i = 0; i < 10000; i++)
    {
        img[i] = (int *)malloc(10000 * sizeof(int));
        // for every array allocate 10000 integer (cols)
    }

    // Get filename of image from user
    printf("Input image filename: ");
    char filename[100];
    char buffer[10000];
    scanf("%s", filename);
    FILE *F = fopen(filename, "r");
    if (F == NULL)
    {
        // If file not found
        printf("Image doesn't exist\n");
        image.IS_IMG_EXIST = 0;
    }
    else
    {
        fgets(buffer, 10000, F);
        char *token = strtok(buffer, ",");
        int M = atoi(token);
        token = strtok(NULL, ","); // read new token after the ","
        int N = atoi(token);
        printf("M: %i, N: %i\n", M, N);
        image.M = M;
        image.N = N;

        int rows = 0;
        int cols = 0;
        int num;
        while (fgets(buffer, 10000, F) != NULL)
        {
            char *token = strtok(buffer, ",");
            // call it just once to read one line
            while (cols < N)
            {
                num = atoi(token);
                img[rows][cols] = num;
                token = strtok(NULL, ",");
                // read next value beside ","
                cols += 1;
            }
            cols = 0;
            rows += 1;
        }
        image.IS_IMG_EXIST = 1;
    }
    image.value = img;
    return image;
}

float **get_kernel()
{
    // Allocate memory for kernel
    float **old_kernel = (float **)malloc(3 * sizeof(float *));
    // For matrix allocate 3 * array (rows)

    for (int i = 0; i < 3; i++)
    {
        old_kernel[i] = (float *)malloc(3 * sizeof(float));
        // For every array allocate 3 integer (cols)
    }

    // Allocate memory for kernel
    float **new_kernel = (float **)malloc(3 * sizeof(float *));
    // For matrix allocate 3 * array (rows)

    for (int i = 0; i < 3; i++)
    {
        new_kernel[i] = (float *)malloc(3 * sizeof(float));
        // For every array allocate 3 integer (cols)
    }

    printf("Input Kernel:\n");
    scanf("%f %f %f", &old_kernel[0][0], &old_kernel[0][1], &old_kernel[0][2]);
    scanf("%f %f %f", &old_kernel[1][0], &old_kernel[1][1], &old_kernel[1][2]);
    scanf("%f %f %f", &old_kernel[2][0], &old_kernel[2][1], &old_kernel[2][2]);

    int i2 = 0;
    int j2 = 0;
    for (int i = 2; i >= 0; i--)
    {
        for (int j = 2; j >= 0; j--)
        {
            new_kernel[i2][j2] = old_kernel[i][j];
            j2 += 1;
        }
        i2 += 1;
        j2 = 0;
    }

    return new_kernel;
}

int **convolution_2D(image_struct image, float **new_kernel, int padding)
{
    int M = image.N;
    int N = image.N;
    int **img = image.value;

    int length_kernel = 3;
    int K = length_kernel / 2;
    int L = K;
    int sum;
    float **kernel = new_kernel;

    // Allocate memory for result
    int **result = (int **)malloc(10000 * sizeof(int *));
    // For matrix allocate 10000 * array (rows)

    for (int i = 0; i < 10000; i++)
    {
        result[i] = (int *)malloc(10000 * sizeof(int));
        // For every array allocate 1000 integer (cols)
    }

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            sum = 0;
            for (int a = -K; a <= K; a++)
            {
                for (int b = -L; b <= L; b++)
                {
                    if (i + a < 0 || j + b < 0 || i + a > M || j + b > N)
                    {
                        sum += kernel[a + K][b + L] * padding;
                    }
                    else
                    {
                        sum += kernel[a + K][b + L] * img[i + a][j + b];
                    }
                }
            }
            result[i][j] = sum;
        }
    }

    return result;
}

void save_image(int **result, int M, int N)
{
    char filename[100];
    printf("Input output file: ");
    scanf("%s", filename);
    FILE *F = fopen(filename, "a");
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            fprintf(F, "%i", result[i][j]);
            fprintf(F, "%c", ',');
        }
        fprintf(F, "%c", '\n');
    }
}

int main()
{
    image_struct image = get_image();
    int **img = image.value;
    int M = image.M;
    int N = image.N;
    int IS_IMG_EXIST = image.IS_IMG_EXIST;
    if (IS_IMG_EXIST == 1)
    {
        float **new_kernel = get_kernel();
        int **result = convolution_2D(image, new_kernel, 0);

        printf("New kernel: \n");
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                printf("%f ", new_kernel[i][j]);
            }
            printf("\n");
        }

        printf("Before Convolution: \n");
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%i\t", img[i][j]);
            }
            printf("\n");
        }

        printf("After Convolution: \n");

        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%i\t", result[i][j]);
            }
            printf("\n");
        }

        save_image(result, M, N);
        free(img);
        free(result);
        free(new_kernel);
    }
}
