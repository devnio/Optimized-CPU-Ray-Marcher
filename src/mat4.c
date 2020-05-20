#include "mat4.h"

/*
 * Function: create_mat4_id
 * ----------------------------
 *   Creates 4x4 identity matrix
 *
 *   returns: new 4x4 identity matrix
 */
// Mat4 create_mat4_id()
// {

//     Mat4 mat;

//     for (int i = 0; i < 4; i++)
//     {
//         for (int j = 0; j < 4; j++)
//         {
//             if (i == j)
//             {
//                 mat.m[i][j] = 1.;
//             }
//             else
//             {
//                 mat.m[i][j] = 0.;
//             }
//         }
//     }

//     return mat;
// }

/*
 * Function: create_mat4
 * ----------------------------
 *   Creates a matrix with the specified doubles values in row-major
 *
 *   m00: double value 
 *   ...
 *   m33: double value
 *
 *   returns: a new matrix initialized with the given values
 */
// Mat4 create_mat4(double m00, double m01, double m02, double m03,
//                  double m10, double m11, double m12, double m13,
//                  double m20, double m21, double m22, double m23,
//                  double m30, double m31, double m32, double m33)
// {
//     Mat4 mat;

//     mat.m[0][0] = m00;
//     mat.m[0][1] = m01;
//     mat.m[0][2] = m02;
//     mat.m[0][3] = m03;
//     mat.m[1][0] = m10;
//     mat.m[1][1] = m11;
//     mat.m[1][2] = m12;
//     mat.m[1][3] = m13;
//     mat.m[2][0] = m20;
//     mat.m[2][1] = m21;
//     mat.m[2][2] = m22;
//     mat.m[2][3] = m23;
//     mat.m[3][0] = m30;
//     mat.m[3][1] = m31;
//     mat.m[3][2] = m32;
//     mat.m[3][3] = m33;

//     return mat;
// }

/*
 * Function: transpose
 * ----------------------------
 *   Creates the transpose of the given matrix
 *
 *   mat: of type Mat4
 *
 *   returns: creates a new matrix that is the transpose of mat
 */
// Mat4 transpose(Mat4 *mat)
// {
//     return create_mat4(mat->m[0][0], mat->m[1][0], mat->m[2][0], mat->m[3][0],
//                        mat->m[0][1], mat->m[1][1], mat->m[2][1], mat->m[3][1],
//                        mat->m[0][2], mat->m[1][2], mat->m[2][2], mat->m[3][2],
//                        mat->m[0][3], mat->m[1][3], mat->m[2][3], mat->m[3][3]);
// }

/*
 * Function: mult_mat
 * ----------------------------
 *   Computes the matrix multiplication between m1 and m2
 *
 *   mat1: of type Mat4
 *   mat2: of type Mat4
 *
 *   returns: creates a new matrix with the multiplication result of m1 and m2
 */
// Mat4 mult_mat(Mat4 *m1, Mat4 *m2)
// {
//     Mat4 mat;
//     for (int i = 0; i < 4; i++)
//         for (int j = 0; j < 4; j++)
//             mat.m[i][j] = m1->m[i][0] * m2->m[0][j] +
//                           m1->m[i][1] * m2->m[1][j] +
//                           m1->m[i][2] * m2->m[2][j] +
//                           m1->m[i][3] * m2->m[3][j];
//     return mat;
// }

/*
 * Function: mult_vec_matrix_no_homo
 * ----------------------------
 *   Computes matrix-vector multiplication ignoring 4 homeogeneous value column, thus no translation component for view matrix component
 *
 *   returns: vector resulting from matrix multiplication
 */
// void mult_vec_matrix_no_homo(Mat4 *m, double v[NR_VEC_ELEMENTS], double res[NR_VEC_ELEMENTS])
// {
//     double x, y, z, w;
//     res[0] = v[0] * m->m[0][0] + v[1] * m->m[0][1] + v[2] * m->m[0][2]; // xxxx
//     res[1] = v[0] * m->m[1][0] + v[1] * m->m[1][1] + v[2] * m->m[1][2]; // yyyy
//     res[2] = v[0] * m->m[2][0] + v[1] * m->m[2][1] + v[2] * m->m[2][2]; // zzzz
// }

/*
 * Function: mult_vec_matrix
 * ----------------------------
 *   Computes matrix-vector multiplication, Assumes homogenous value w of v is 1
 *
 *   returns: vector resulting from matrix multiplication
 */
// Vec3 mult_vec_matrix(Mat4 *m, Vec3 v)
// {

//     Vec3 res;

//     double x, y, z, w;

//     x = v[0] * m->m[0][0] + v[1] * m->m[0][1] + v[2] * m->m[0][2] + m->m[0][3];
//     y = v[0] * m->m[1][0] + v[1] * m->m[1][1] + v[2] * m->m[1][2] + m->m[1][3];
//     z = v[0] * m->m[2][0] + v[1] * m->m[2][1] + v[2] * m->m[2][2] + m->m[2][3];
//     w = v[0] * m->m[3][0] + v[1] * m->m[3][1] + v[2] * m->m[3][2] + m->m[3][3];

//     //From homogenenous coordinates back to normal coords
//     double inv = 1 / w;
//     res[0] = x * inv;
//     res[1] = y * inv;
//     res[2] = z * inv;

//     return res;
// }

/*
 * Function: look_at
 * ----------------------------
 * 
 */
Mat4 look_at(float pos[NR_VEC_ELEMENTS], float dir[NR_VEC_ELEMENTS], float up[NR_VEC_ELEMENTS])
{

    Mat4 viewMatrix;
    //fourth column of view matrix
    viewMatrix.m[0][3] = pos[0];
    viewMatrix.m[1][3] = pos[1];
    viewMatrix.m[2][3] = pos[2];
    viewMatrix.m[3][3] = 1;

    float tmp_vcross[NR_VEC_ELEMENTS]; 
    vec_cross(up, dir, tmp_vcross);
    vec_normalize(tmp_vcross);
    float *cRight = tmp_vcross;
    float cUp[NR_VEC_ELEMENTS];
    vec_cross(dir, cRight, cUp);

    viewMatrix.m[0][0] = cRight[0];
    viewMatrix.m[1][0] = cRight[1];
    viewMatrix.m[2][0] = cRight[2];
    viewMatrix.m[3][0] = 0.0;
    viewMatrix.m[0][1] = cUp[0];
    viewMatrix.m[1][1] = cUp[1];
    viewMatrix.m[2][1] = cUp[2];
    viewMatrix.m[3][1] = 0.0;
    viewMatrix.m[0][2] = dir[0];
    viewMatrix.m[1][2] = dir[1];
    viewMatrix.m[2][2] = dir[2];
    viewMatrix.m[3][2] = 0.0;

    return viewMatrix;
}