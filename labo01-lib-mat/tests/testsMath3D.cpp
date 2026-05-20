/**
 * @file TestsMath3D.cpp
 *
 * @brief Tests unitaires fournis pour les fonctions de Math3D de la librairie matricielle.
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#include "../src/Math3D.h"

#include <gtest/gtest.h>

using namespace gti320;

/**
 * Mathématiques 3D
 */
TEST(TestsTP1, Math3D)
{
    // Test : norme d'un vecteur de dimension 3
    Vector3d v;
    v.setZero();
    v(1) = 2.0;
    EXPECT_EQ(v.rows(), 3);
    EXPECT_EQ(v.cols(), 1);
    EXPECT_DOUBLE_EQ(v(0), 0.0);
    EXPECT_DOUBLE_EQ(v(1), 2.0);
    EXPECT_DOUBLE_EQ(v(2), 0.0);
    EXPECT_DOUBLE_EQ(v.norm(), 2.0);

    // Test : calcul de la norme d'un deuxième vecteur 3D
    Vector3d v2;
    v2(0) = 4.0;
    v2(1) = 2.0;
    v2(2) = 5.0;
    EXPECT_EQ(v2.rows(), 3);
    EXPECT_EQ(v2.cols(), 1);
    EXPECT_DOUBLE_EQ(v2(0), 4.0);
    EXPECT_DOUBLE_EQ(v2(1), 2.0);
    EXPECT_DOUBLE_EQ(v2(2), 5.0);
    EXPECT_DOUBLE_EQ(v2.norm(), 6.7082039324993690892275210061938);

    // Test : produit scalaire
    EXPECT_DOUBLE_EQ(v.dot(v2), 4.0);

    // Test : matrice identité 4x4
    Matrix4d M;
    M.setIdentity();
    EXPECT_DOUBLE_EQ(M(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(M(0, 1), 0.0);
    EXPECT_DOUBLE_EQ(M(0, 2), 0.0);
    EXPECT_DOUBLE_EQ(M(1, 1), 1.0);
    EXPECT_DOUBLE_EQ(M(1, 0), 0.0);
    EXPECT_DOUBLE_EQ(M(1, 2), 0.0);
    EXPECT_DOUBLE_EQ(M(2, 0), 0.0);
    EXPECT_DOUBLE_EQ(M(2, 1), 0.0);
    EXPECT_DOUBLE_EQ(M(2, 2), 1.0);

    // Test : création d'une matrice de rotation de 45 degrés autour de l'axe des x
    const auto Rx = makeRotation<double>(M_PI / 4.0, 0, 0);
    EXPECT_NEAR(Rx(0, 0), 1, 1e-3);
    EXPECT_NEAR(Rx(0, 1), 0, 1e-3);
    EXPECT_NEAR(Rx(0, 2), 0, 1e-3);
    EXPECT_NEAR(Rx(1, 0), 0, 1e-3);
    EXPECT_NEAR(Rx(1, 1), 0.7071, 1e-3);
    EXPECT_NEAR(Rx(1, 2), -0.7071, 1e-3);
    EXPECT_NEAR(Rx(2, 0), 0, 1e-3);
    EXPECT_NEAR(Rx(2, 1), 0.7071, 1e-3);
    EXPECT_NEAR(Rx(2, 2), 0.7071, 1e-3);

    // Test : création d'une matrice de rotation de 45 degrés autour de l'axe des y
    const auto Ry = makeRotation<double>(0, M_PI / 4.0, 0);
    EXPECT_NEAR(Ry(0, 0), 0.7071, 1e-3);
    EXPECT_NEAR(Ry(0, 1), 0, 1e-3);
    EXPECT_NEAR(Ry(0, 2), 0.7071, 1e-3);
    EXPECT_NEAR(Ry(1, 0), 0, 1e-3);
    EXPECT_NEAR(Ry(1, 1), 1, 1e-3);
    EXPECT_NEAR(Ry(1, 2), 0, 1e-3);
    EXPECT_NEAR(Ry(2, 0), -0.7071, 1e-3);
    EXPECT_NEAR(Ry(2, 1), 0, 1e-3);
    EXPECT_NEAR(Ry(2, 2), 0.7071, 1e-3);

    // Test : création d'une matrice de rotation de 45 degrés autour de l'axe des z
    const auto Rz = makeRotation<double>(0, 0, M_PI / 4.0);
    EXPECT_NEAR(Rz(0, 0), 0.7071, 1e-3);
    EXPECT_NEAR(Rz(0, 1), -0.7071, 1e-3);
    EXPECT_NEAR(Rz(0, 2), 0, 1e-3);
    EXPECT_NEAR(Rz(1, 0), 0.7071, 1e-3);
    EXPECT_NEAR(Rz(1, 1), 0.7071, 1e-3);
    EXPECT_NEAR(Rz(1, 2), 0, 1e-3);
    EXPECT_NEAR(Rz(2, 0), 0, 1e-3);
    EXPECT_NEAR(Rz(2, 1), 0, 1e-3);
    EXPECT_NEAR(Rz(2, 2), 1, 1e-3);

    // Test : création d'une matrice de rotation quelconque.
    const auto Rxyz = makeRotation<double>(M_PI / 3.0, -M_PI / 6.0, M_PI / 4.0);
    EXPECT_NEAR(Rxyz(0, 0), 0.6124, 1e-3);
    EXPECT_NEAR(Rxyz(0, 1), -0.6597, 1e-3);
    EXPECT_NEAR(Rxyz(0, 2), 0.4356, 1e-3);
    EXPECT_NEAR(Rxyz(1, 0), 0.6124, 1e-3);
    EXPECT_NEAR(Rxyz(1, 1), 0.0474, 1e-3);
    EXPECT_NEAR(Rxyz(1, 2), -0.7891, 1e-3);
    EXPECT_NEAR(Rxyz(2, 0), 0.5, 1e-3);
    EXPECT_NEAR(Rxyz(2, 1), 0.75, 1e-3);
    EXPECT_NEAR(Rxyz(2, 2), 0.4330, 1e-3);

    // Test : création d'une transformation homogène via la sous-matrice 3x3 en
    // utilisant la fonction `block`
    M.block(0, 0, 3, 3) = Rxyz;
    M(0, 3) = -0.1;
    M(1, 3) = 1.0;
    M(2, 3) = 2.1;

    // Test : calcule l'inverse de la matrice M et vérifie que M^(-1) * M * v = v
    const Matrix4d Minv = M.inverse();
    const Vector3d v3 = Minv * (M * v2);
    EXPECT_DOUBLE_EQ(v3(0), v2(0));
    EXPECT_DOUBLE_EQ(v3(1), v2(1));
    EXPECT_DOUBLE_EQ(v3(2), v2(2));

    // Test : translation d'un vecteur 3D effectuée avec une matrice 4x4 en coordonnées homogènes
    Matrix4d T;
    T.setIdentity();
    T(0, 3) = 1.2;
    T(1, 3) = 2.5;
    T(2, 3) = -4.0;
    const Vector3d t = T * v3;
    EXPECT_DOUBLE_EQ(t(0), v3(0) + 1.2);
    EXPECT_DOUBLE_EQ(t(1), v3(1) + 2.5);
    EXPECT_DOUBLE_EQ(t(2), v3(2) - 4.0);

    // Test : inverse d'un matrice de rotation
    const Matrix3d Rinv = Rxyz.inverse();
    const Matrix3d RT = Rxyz.transpose<double, 3, 3, ColumnStorage>();
    EXPECT_DOUBLE_EQ(Rinv(0, 0), RT(0, 0));
    EXPECT_DOUBLE_EQ(Rinv(1, 1), RT(1, 1));
    EXPECT_DOUBLE_EQ(Rinv(0, 2), RT(0, 2));
}

TEST(TestsTP1, QuaternionSlerp)
{
    // Test : slerp with identity vectors (trivial)
    Quat q0a;
    q0a(0) = 1.0; q0a(1) = 0.0; q0a(2) = 0.0;  q0a(3) = 0.0;
    Quat q1a;
    q1a(0) = 1.0; q1a(1) = 0.0; q1a(2) = 0.0;  q1a(3) = 0.0;
    Quat qResa = slerp( q0a, q1a, 0.5 );
    EXPECT_NEAR( qResa(0), 1.0, 1e-3);
    EXPECT_NEAR( qResa(1), 0.0, 1e-3);
    EXPECT_NEAR( qResa(2), 0.0, 1e-3);
    EXPECT_NEAR( qResa(3), 0.0, 1e-3);
}

TEST(TestsTP1, EulerToQuat)
{
    // Test : rotation de 90 degres autour de l'axe x
    Vector3d eulerX90;
    eulerX90(0) = M_PI_2; eulerX90(1) = 0.0; eulerX90(2) = 0.0;
    Quat x90 = eulerToQuat( eulerX90 );
    EXPECT_NEAR( x90(0), std::cos(M_PI_4), 1e-3 );
    EXPECT_NEAR( x90(1), std::sin(M_PI_4), 1e-3 );
    EXPECT_NEAR( x90(2), 0.0, 1e-3 );
    EXPECT_NEAR( x90(3), 0.0, 1e-3 );
    
    // Test : rotation de 90 degres autour de l'axe y
    Vector3d eulerY90;
    eulerY90(0) = 0.0; eulerY90(1) = M_PI_2; eulerY90(2) = 0.0;
    Quat y90 = eulerToQuat( eulerY90 );
    EXPECT_NEAR( y90(0), std::cos(M_PI_4), 1e-3 );
    EXPECT_NEAR( y90(1), 0.0, 1e-3 );
    EXPECT_NEAR( y90(2), std::sin(M_PI_4), 1e-3 );
    EXPECT_NEAR( y90(3), 0.0, 1e-3 );
    
    // Test : rotation de 90 degres autour de l'axe z
    Vector3d eulerZ90;
    eulerZ90(0) = 0.0; eulerZ90(1) = 0.0; eulerZ90(2) = M_PI_2;
    Quat z90 = eulerToQuat( eulerZ90 );
    EXPECT_NEAR( z90(0), std::cos(M_PI_4), 1e-3 );
    EXPECT_NEAR( z90(1), 0.0, 1e-3 );
    EXPECT_NEAR( z90(2), 0.0, 1e-3 );
    EXPECT_NEAR( z90(3), std::sin(M_PI_4), 1e-3 );
    
    // Test plus realiste: x:90, y:45, z:30
    Vector3d eulerReal;
    eulerReal(0) = M_PI_2; eulerReal(1) = M_PI_4; eulerReal(2) = M_PI / 6.0;
    Quat quatReal = eulerToQuat( eulerReal );
    std::cout << quatReal(0) << " " << quatReal(1) << " " << quatReal(2) << " " << quatReal(3) << std::endl;
    EXPECT_NEAR( quatReal(0), 0.70105738, 1e-3 );
    EXPECT_NEAR( quatReal(1), 0.56098553, 1e-3 );
    EXPECT_NEAR( quatReal(2), 0.43045933, 1e-3 );
    EXPECT_NEAR( quatReal(3), -0.09229596, 1e-3 );
}

TEST(TestsTP1, MakeRigid)
{
    Vector3d trans;
    trans(0) = 1.0; trans(1) = -2.0; trans(2) = 0.5;
    
    // rotation euler: x:90, y:45, z:30
    Quat rot;
    rot(0) = 0.70105738; rot(1) = 0.56098553; rot(2) = 0.43045933; rot(3) = -0.09229596;
    Matrix4d mat = makeRigid( trans, rot );
    
    EXPECT_NEAR( mat(0,0), 6.12372436e-01, 1e-3 );
    EXPECT_NEAR( mat(0,1), 6.12372436e-01, 1e-3 );
    EXPECT_NEAR( mat(0,2), 5.00000000e-01, 1e-3 );
    EXPECT_NEAR( mat(0,3), 1.0, 1e-3 );
    
    EXPECT_NEAR( mat(1,0), 3.53553391e-01, 1e-3 );
    EXPECT_NEAR( mat(1,1), 3.53553391e-01, 1e-3 );
    EXPECT_NEAR( mat(1,2), -8.66025404e-01, 1e-3 );
    EXPECT_NEAR( mat(1,3), -2.0, 1e-3 );
    
    EXPECT_NEAR( mat(2,0), -7.07106781e-01, 1e-3 );
    EXPECT_NEAR( mat(2,1), 7.07106781e-01, 1e-3 );
    EXPECT_NEAR( mat(2,2), 5.55111512e-17, 1e-3 );
    EXPECT_NEAR( mat(2,3), 0.5, 1e-3 );
    
    EXPECT_NEAR( mat(3,0), 0.0, 1e-3 );
    EXPECT_NEAR( mat(3,1), 0.0, 1e-3 );
    EXPECT_NEAR( mat(3,2), 0.0, 1e-3 );
    EXPECT_NEAR( mat(3,3), 1.0, 1e-3 );
}
