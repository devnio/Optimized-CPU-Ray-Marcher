#include "baseline_test.h"
#include "utility.h"

SUITE(baseline_test){
    RUN_TEST(compare_scene0);
    RUN_TEST(compare_mandelbulb);
    RUN_TEST(compare_shape1);
    RUN_TEST(compare_shape2);
    RUN_TEST(compare_shape3);
    RUN_TEST(compare_shape4);
}

TEST compare_scene0()
{
    return compare_2_images("../../output/render_out/scene0.png", "../output_baseline/scene0.png");
}

TEST compare_mandelbulb()
{
    return compare_2_images("../../output/render_out/mandelbulb.png", "../output_baseline/mandelbulb.png");
}

TEST compare_shape0()
{
    return compare_2_images("../../output/render_out/shape0.png", "../output_baseline/shape0.png");
}

TEST compare_shape1()
{
    return compare_2_images("../../output/render_out/shape1.png", "../output_baseline/shape1.png");
}

TEST compare_shape2()
{
    return compare_2_images("../../output/render_out/shape2.png", "../output_baseline/shape2.png");
}

TEST compare_shape3()
{
    return compare_2_images("../../output/render_out/shape3.png", "../output_baseline/shape3.png");
}

TEST compare_shape4()
{
    return compare_2_images("../../output/render_out/shape4.png", "../output_baseline/shape4.png");
}


TEST compare_2_images(char* filename0, char* filename1)
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

    // Check that every pixel is equal
    for (int y = 0; y < height0; y++)
    {
        for (int x = 0; x < width0; x++)
        {
            ASSERT_EQ(image0[y * width0 * 4 + x * 4 + 0], image1[y * width0 * 4 + x * 4 + 0]);
            ASSERT_EQ(image0[y * width0 * 4 + x * 4 + 1], image1[y * width0 * 4 + x * 4 + 1]);
            ASSERT_EQ(image0[y * width0 * 4 + x * 4 + 2], image1[y * width0 * 4 + x * 4 + 2]);
            ASSERT_EQ(image0[y * width0 * 4 + x * 4 + 3], image1[y * width0 * 4 + x * 4 + 3]);
        }
    }

    free(image0);
    free(image1);
    PASS();
}
