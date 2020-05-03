#include "mat4.h"

/*
 * Function: create_mat4_id
 * ----------------------------
 *   Creates 4x4 identity matrix
 *
 *   returns: new 4x4 identity matrix
 */
Mat4 create_mat4_id()
{

    Mat4 mat;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                mat.m[i][j] = 1.;
            }
            else
            {
                mat.m[i][j] = 0.;
            }
        }
    }

    return mat;
}

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
Mat4 create_mat4(double m00, double m01, double m02, double m03,
                 double m10, double m11, double m12, double m13,
                 double m20, double m21, double m22, double m23,
                 double m30, double m31, double m32, double m33)
{
    Mat4 mat;

    mat.m[0][0] = m00;
    mat.m[0][1] = m01;
    mat.m[0][2] = m02;
    mat.m[0][3] = m03;
    mat.m[1][0] = m10;
    mat.m[1][1] = m11;
    mat.m[1][2] = m12;
    mat.m[1][3] = m13;
    mat.m[2][0] = m20;
    mat.m[2][1] = m21;
    mat.m[2][2] = m22;
    mat.m[2][3] = m23;
    mat.m[3][0] = m30;
    mat.m[3][1] = m31;
    mat.m[3][2] = m32;
    mat.m[3][3] = m33;

    return mat;
}

/*
 * Function: transpose
 * ----------------------------
 *   Creates the transpose of the given matrix
 *
 *   mat: of type Mat4
 *
 *   returns: creates a new matrix that is the transpose of mat
 */
Mat4 transpose(Mat4 *mat)
{
    return create_mat4(mat->m[0][0], mat->m[1][0], mat->m[2][0], mat->m[3][0],
                       mat->m[0][1], mat->m[1][1], mat->m[2][1], mat->m[3][1],
                       mat->m[0][2], mat->m[1][2], mat->m[2][2], mat->m[3][2],
                       mat->m[0][3], mat->m[1][3], mat->m[2][3], mat->m[3][3]);
}

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
Mat4 mult_mat(Mat4 *m1, Mat4 *m2)
{
    Mat4 mat;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat.m[i][j] = m1->m[i][0] * m2->m[0][j] +
                          m1->m[i][1] * m2->m[1][j] +
                          m1->m[i][2] * m2->m[2][j] +
                          m1->m[i][3] * m2->m[3][j];
    return mat;
}

/*
 * Function: mult_vec_matrix_no_homo
 * ----------------------------
 *   Computes matrix-vector multiplication ignoring 4 homeogeneous value column, thus no translation component for view matrix component
 *
 *   returns: vector resulting from matrix multiplication
 */
Vec3 mult_vec_matrix_no_homo(Mat4 *m, Vec3 v)
{

    Vec3 res;

    res.x = v.x * m->m[0][0] + v.y * m->m[0][1] + v.z * m->m[0][2];
    res.y = v.x * m->m[1][0] + v.y * m->m[1][1] + v.z * m->m[1][2];
    res.z = v.x * m->m[2][0] + v.y * m->m[2][1] + v.z * m->m[2][2];

    return res;
}

/*
 * Function: mult_vec_matrix
 * ----------------------------
 *   Computes matrix-vector multiplication, Assumes homogenous value w of v is 1
 *
 *   returns: vector resulting from matrix multiplication
 */
Vec3 mult_vec_matrix(Mat4 *m, Vec3 v)
{

    Vec3 res;

    double x, y, z, w;

    x = v.x * m->m[0][0] + v.y * m->m[0][1] + v.z * m->m[0][2] + m->m[0][3];
    y = v.x * m->m[1][0] + v.y * m->m[1][1] + v.z * m->m[1][2] + m->m[1][3];
    z = v.x * m->m[2][0] + v.y * m->m[2][1] + v.z * m->m[2][2] + m->m[2][3];
    w = v.x * m->m[3][0] + v.y * m->m[3][1] + v.z * m->m[3][2] + m->m[3][3];

    //From homogenenous coordinates back to normal coords
    double inv = 1 / w;
    res.x = x * inv;
    res.y = y * inv;
    res.z = z * inv;

    return res;
}

/*
 * Function: look_at
 * ----------------------------
 *   TODO
 *
 *   returns: TODO
 */
Mat4 look_at(Vec3 pos, Vec3 dir, Vec3 up)
{

    Mat4 viewMatrix;
    //fourth column of view matrix
    viewMatrix.m[0][3] = pos.x;
    viewMatrix.m[1][3] = pos.y;
    viewMatrix.m[2][3] = pos.z;
    viewMatrix.m[3][3] = 1;

    Vec3 cRight = vec_normalized(vec_cross(up, dir));
    Vec3 cUp = vec_cross(dir, cRight);

    viewMatrix.m[0][0] = cRight.x;
    viewMatrix.m[1][0] = cRight.y;
    viewMatrix.m[2][0] = cRight.z;
    viewMatrix.m[3][0] = 0.0;
    viewMatrix.m[0][1] = cUp.x;
    viewMatrix.m[1][1] = cUp.y;
    viewMatrix.m[2][1] = cUp.z;
    viewMatrix.m[3][1] = 0.0;
    viewMatrix.m[0][2] = dir.x;
    viewMatrix.m[1][2] = dir.y;
    viewMatrix.m[2][2] = dir.z;
    viewMatrix.m[3][2] = 0.0;

    return viewMatrix;
}