#include "baseline_test.h"
#include "utility.h"

#define CONV_K_DIV_2 3 // 5/2 = 2, 2 left and 2 right
#define CONV_COEFF 0.0204081633 //1/49
#define TOL 6

SUITE(approx_baseline_test){
    RUN_TEST(approx_compare_scene0);
    RUN_TEST(approx_compare_mandelbulb);
    RUN_TEST(approx_compare_shape1);
    RUN_TEST(approx_compare_shape2);
    RUN_TEST(approx_compare_shape3);
    RUN_TEST(approx_compare_shape4);
}

TEST approx_compare_scene0()
{
    return approx_compare_2_images("../../output/render_out/scene0.png", "../output_baseline/scene0.png");
}

TEST approx_compare_mandelbulb()
{
    return approx_compare_2_images("../../output/render_out/mandelbulb.png", "../output_baseline/mandelbulb.png");
}

TEST approx_compare_shape0()
{
    return approx_compare_2_images("../../output/render_out/shape0.png", "../output_baseline/shape0.png");
}

TEST approx_compare_shape1()
{
    return approx_compare_2_images("../../output/render_out/shape1.png", "../output_baseline/shape1.png");
}

TEST approx_compare_shape2()
{
    return approx_compare_2_images("../../output/render_out/shape2.png", "../output_baseline/shape2.png");
}

TEST approx_compare_shape3()
{
    return approx_compare_2_images("../../output/render_out/shape3.png", "../output_baseline/shape3.png");
}

TEST approx_compare_shape4()
{
    return approx_compare_2_images("../../output/render_out/shape4.png", "../output_baseline/shape4.png");
}

// ================================================================================================

int comp_eq(const void *expd, const void *got, void *udata)
{
    void** uvalues = (void**)udata;

    unsigned int x = (unsigned int)uvalues[0];
    unsigned int y = (unsigned int)uvalues[1];

    unsigned int width = (unsigned int)uvalues[2];
    unsigned int height = (unsigned int)uvalues[3];

    unsigned char* image0 = (unsigned char*)uvalues[4];
    unsigned char* image1 = (unsigned char*)uvalues[5];

    int rgba = (int)uvalues[6];

    int l_x = ((int)x - CONV_K_DIV_2) > 0 ? ((int)x - CONV_K_DIV_2) : 0;
    int l_y = ((int)y - CONV_K_DIV_2) > 0 ? ((int)y - CONV_K_DIV_2) : 0;
    
    int r_x = ((int)x + CONV_K_DIV_2) < width ? ((int)x + CONV_K_DIV_2) : width;
    int r_y = ((int)y + CONV_K_DIV_2) < height ? ((int)y + CONV_K_DIV_2) : height;

    // Convolution
    double avg_expd = 0.0;
    double avg_got = 0.0;
    
    for (int i = l_y; i < r_y; i++)
    {
        for (int j = l_x; j < r_x; j++)
        {
            avg_expd += CONV_COEFF * image0[i * width * 4 + j * 4 + rgba];
            avg_got  += CONV_COEFF * image1[i * width * 4 + j * 4 + rgba];
        }   
    }    

    // pass values for printing
    double *a = malloc(sizeof(double));
    double *b = malloc(sizeof(double));
    *a = avg_expd;
    *b = avg_got;
    uvalues[7] = (void *)a;
    uvalues[8] = (void *)b;

    if (avg_expd > (avg_got - TOL) && avg_expd < (avg_got + TOL))
    {
        free(uvalues[7]);
        free(uvalues[8]);
        return 1;
    }
    else
    {
        return 0;
    }
}
/* Printing both pixel values */
int print_px(const void *t, void *udata)
{
    void** uvalues = (void**)udata;

    unsigned int x = (unsigned int)uvalues[0];
    unsigned int y = (unsigned int)uvalues[1];
    
    unsigned int width = (unsigned int)uvalues[2];
    unsigned int height = (unsigned int)uvalues[3];

    unsigned char* image0 = (unsigned char*)uvalues[4];
    unsigned char* image1 = (unsigned char*)uvalues[5];

    unsigned int r0 = image0[y * width * 4 + x * 4 + 0];
    unsigned int g0 = image0[y * width * 4 + x * 4 + 1];
    unsigned int b0 = image0[y * width * 4 + x * 4 + 2];
    unsigned int a0 = image0[y * width * 4 + x * 4 + 3];

    unsigned int r1 = image1[y * width * 4 + x * 4 + 0];
    unsigned int g1 = image1[y * width * 4 + x * 4 + 1];
    unsigned int b1 = image1[y * width * 4 + x * 4 + 2];
    unsigned int a1 = image1[y * width * 4 + x * 4 + 3];

    double a = *(double *)uvalues[7];
    double b = *(double *)uvalues[8];

    free(uvalues[7]);
    free(uvalues[8]);

    int rgba = (int)uvalues[6];
    char c = (rgba < 3) ? (rgba < 2) ? (rgba < 1) ? 'R' : 'G' : 'B' : 'A';
    
    unsigned int val = (unsigned int)t;
    return printf("[x=%u, y=%u] | [RGBA = %c] - EXPECTED: [%u, %u, %u, %u] | GOT: [%u, %u, %u, %u] | ERR: AVG_EXPD %f, AVG_GOT %f", x, y, c, r0, g0, b0, a0, r1, g1, b1, a1, a, b);
}

TEST approx_compare_2_images(char* filename0, char* filename1)
{   
    unsigned width0, height0, error0;
    unsigned char* image0 = decodeOneStep(filename0, &width0, &height0, &error0);

    if(error0) {
        printf("\n  - SKIP: image not found - path: %s   | status: ", filename0);
        SKIP();
    }
    
    unsigned width1, height1, error1;
    unsigned char* image1 = decodeOneStep(filename1, &width1, &height1, &error1);

    if(error1) {
        printf("\n  - SKIP: image not found - path: %s   | status: ", filename1);
        SKIP();
    }

    // Check that both images have same size first.
    ASSERT_EQ(width0, width1);
    ASSERT_EQ(height0, height1);

    greatest_type_info* gf = (greatest_type_info*)malloc(sizeof(greatest_type_info));
    gf->equal = comp_eq;
    gf->print = print_px;

    // Some data to pass
    void* uvalues[9];
    uvalues[2] = width0;
    uvalues[3] = height0;
    uvalues[4] = image0;
    uvalues[5] = image1;
    
    void* udata;
    udata = uvalues;

    // Check that every pixel is equal
    for (unsigned int y = 0; y < height0; y++)
    {
        for (unsigned int x = 0; x < width0; x++)
        {
            uvalues[0] = x;
            uvalues[1] = y;

            uvalues[6] = 0;
            ASSERT_EQUAL_T(image0[y * width0 * 4 + x * 4 + 0], image1[y * width0 * 4 + x * 4 + 0], gf, udata);
            uvalues[6] = 1;
            ASSERT_EQUAL_T(image0[y * width0 * 4 + x * 4 + 1], image1[y * width0 * 4 + x * 4 + 1], gf, udata);
            uvalues[6] = 2;
            ASSERT_EQUAL_T(image0[y * width0 * 4 + x * 4 + 2], image1[y * width0 * 4 + x * 4 + 2], gf, udata);
            uvalues[6] = 3;
            ASSERT_EQUAL_T(image0[y * width0 * 4 + x * 4 + 3], image1[y * width0 * 4 + x * 4 + 3], gf, udata);
        }
    }

    free(image0);
    free(image1);
    free(gf);
    PASS();
}
