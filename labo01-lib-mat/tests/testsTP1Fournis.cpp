/**
 * @file TestsLabo1.cpp
 *
 * @brief Tests unitaires fournis pour la librairie d'algèbre linéaire.
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#include "../src/Matrix.h"
#include "../src/Operators.h"
#include "../src/Vector.h"

#include <chrono>
#include <gtest/gtest.h>

using namespace gti320;

// ===========================================================================================================
// ----- Implémentations naïves pour fin de tests, ne pas modifier! ------------------------------------------
// ===========================================================================================================
#pragma region // Implementations naives d'opérations

/** Multiplication  matrice * vecteur,  utilisant une implémentation naive */
template<typename _Scalar>
static inline Vector<_Scalar, Dynamic> naiveMatrixMult(const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& A, const Vector<_Scalar, Dynamic>& v)
{
    assert(A.cols() == v.rows());

    Vector<_Scalar, Dynamic> b(A.rows());
    assert(b.rows() == A.rows());

    for (int i = 0; i < A.rows(); ++i) {
        b(i) = 0.0;
        for (int j = 0; j < A.cols(); ++j) {
            b(i) += A(i, j) * v(j);
        }
    }

    return b;
}

/** Addition  matrice + matrice,  utilisant une implémentation naive */
template<typename _Scalar>
static inline Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage> naiveMatrixAddition(const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& B)
{
    assert(A.cols() == B.cols() && A.rows() == B.rows());

    Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage> C(A.rows(), A.cols());
    assert(C.rows() == A.rows() && C.cols() == A.cols());
    for (int i = 0; i < C.rows(); ++i) {
        for (int j = 0; j < C.cols(); ++j) {
            C(i, j) = A(i, j) + B(i, j);
        }
    }
    return C;
}

/** Multiplication  matrice * matrice,  utilisant une implémentation naive. */
template<typename _Scalar, int _Storage>
static inline Matrix<_Scalar, Dynamic, Dynamic, _Storage> naiveMatrixMult(const Matrix<_Scalar, Dynamic, Dynamic, _Storage>& A, const Matrix<_Scalar, Dynamic, Dynamic, _Storage>& B)
{
    assert(A.cols() == B.rows());
    Matrix<_Scalar, Dynamic, Dynamic> product(A.rows(), B.cols());
    for (int i = 0; i < A.rows(); ++i) {
        for (int j = 0; j < B.cols(); ++j) {
            for (int k = 0; k < A.cols(); ++k) {
                product(i, j) += A(i, k) * B(k, j);
            }
        }
    }
    return product;
}
#pragma endregion // Implementations naives d'opérations

// ===========================================================================================================
// ----- Tests fournis, ne pas modifier! ---------------------------------------------------------------------
// ===========================================================================================================
#pragma region // Tests Fournis

// Test les matrice avec redimensionnement dynamique
TEST(TestsTP1, DynamicMatrixTests)
{
    // Crée une matrice à taille dynamique
    // (note : les valeurs par défaut du patron de la classe `Matrix` mettent le
    // le nombre de ligne et de colonnes à `Dynamic`)
    Matrix<double> M(3, 5);
    EXPECT_EQ(M.cols(), 5);
    EXPECT_EQ(M.rows(), 3);

    // Redimensionne la matrice
    M.resize(100, 1000);
    EXPECT_EQ(M.cols(), 1000);
    EXPECT_EQ(M.rows(), 100);

    // Test - stockage par colonnes
    Matrix<double, Dynamic, Dynamic, ColumnStorage> ColM(100, 100);
    ColM.setZero();
    ColM(0, 0) = 1.0;
    ColM(99, 99) = 99.0;
    ColM(10, 33) = 5.0;
    EXPECT_EQ(ColM(0, 0), 1.0);
    EXPECT_EQ(ColM(10, 33), 5.0);
    EXPECT_EQ(ColM(99, 99), 99.0);

    // Test - stockage par lignes
    Matrix<double, Dynamic, Dynamic, RowStorage> RowM(5, 4);
    RowM.setZero();
    RowM(0, 0) = 2.1;
    RowM(3, 3) = -0.2;
    RowM(4, 3) = 1.2;
    EXPECT_EQ(RowM.rows(), 5);
    EXPECT_EQ(RowM.cols(), 4);
    EXPECT_DOUBLE_EQ(RowM(0, 0), 2.1);
    EXPECT_DOUBLE_EQ(RowM(3, 3), -0.2);
    EXPECT_DOUBLE_EQ(RowM(4, 3), 1.2);
    EXPECT_DOUBLE_EQ(RowM(3, 2), 0.0);

    // Transposée
    const auto RowMT = RowM.transpose();
    EXPECT_EQ(RowMT.rows(), 4);
    EXPECT_EQ(RowMT.cols(), 5);
    EXPECT_DOUBLE_EQ(RowMT(0, 0), 2.1);
    EXPECT_DOUBLE_EQ(RowMT(3, 3), -0.2);
    EXPECT_DOUBLE_EQ(RowMT(3, 4), 1.2);
    EXPECT_DOUBLE_EQ(RowMT(2, 3), 0.0);
}

/**
 * Test pour les vecteurs à taille dynamique
 */
TEST(TestsTP1, DynamicVectorSizeTest)
{
    Vector<double> v(5);
    v.setZero();

    EXPECT_EQ(v.rows(), 5);

    v.resize(3);
    EXPECT_EQ(v.rows(), 3);

    v(0) = 1.0;
    v(1) = 2.0;
    v(2) = 3.0;

    EXPECT_DOUBLE_EQ(v.norm(), 3.7416573867739413855837487323165);

    Vector<double, Dynamic> v2(3);
    v2.setZero();
    v2(1) = 2.0;

    EXPECT_DOUBLE_EQ(v2.dot(v), 4.0);
    EXPECT_DOUBLE_EQ(v2(0), 0.0);
    EXPECT_DOUBLE_EQ(v2(1), 2.0);
    EXPECT_DOUBLE_EQ(v2(2), 0.0);
}

/**
 * Test pour les matrice à taille fixe
 */
TEST(TestsTP1, Matrix4x4SizeTest)
{
    Matrix<double,4,4> M;
    M.setZero();

    EXPECT_EQ(M.cols(), 4);
    EXPECT_EQ(M.rows(), 4);
}

/**
 * Test pour les opérateurs d'arithmétique matricielle.
 */
TEST(TestsTP1, MatrixMatrixOperators)
{
    // Opérations arithmétiques avec matrices à taille dynamique
    {
        // Test : matrice identité
        Matrix<double> A(6, 6);
        A.setIdentity();
        EXPECT_DOUBLE_EQ(A(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(A(1, 1), 1.0);
        EXPECT_DOUBLE_EQ(A(2, 2), 1.0);
        EXPECT_DOUBLE_EQ(A(3, 3), 1.0);
        EXPECT_DOUBLE_EQ(A(4, 4), 1.0);
        EXPECT_DOUBLE_EQ(A(5, 5), 1.0);
        EXPECT_DOUBLE_EQ(A(0, 1), 0.0);
        EXPECT_DOUBLE_EQ(A(1, 0), 0.0);

        // Test : produit  scalaire * matrice
        const double alpha = 2.5;
        Matrix<double> B = alpha * A;
        EXPECT_DOUBLE_EQ(B(0, 0), alpha);
        EXPECT_DOUBLE_EQ(B(1, 1), alpha);
        EXPECT_DOUBLE_EQ(B(2, 2), alpha);
        EXPECT_DOUBLE_EQ(B(3, 3), alpha);
        EXPECT_DOUBLE_EQ(B(4, 4), alpha);
        EXPECT_DOUBLE_EQ(B(5, 5), alpha);
        EXPECT_DOUBLE_EQ(B(0, 1), 0.0);
        EXPECT_DOUBLE_EQ(B(1, 0), 0.0);

        // Test : produit  matrice * matrice
        Matrix<double> C = A * B;
        EXPECT_DOUBLE_EQ(C(0, 0), A(0, 0) * B(0, 0));
        EXPECT_DOUBLE_EQ(C(1, 1), A(1, 1) * B(1, 1));
        EXPECT_DOUBLE_EQ(C(2, 2), A(2, 2) * B(2, 2));
        EXPECT_DOUBLE_EQ(C(3, 3), A(3, 3) * B(3, 3));
        EXPECT_DOUBLE_EQ(C(4, 4), A(4, 4) * B(4, 4));
        EXPECT_DOUBLE_EQ(C(5, 5), A(5, 5) * B(5, 5));
        EXPECT_DOUBLE_EQ(C(0, 1), 0.0);
        EXPECT_DOUBLE_EQ(C(2, 3), 0.0);

        // Test : addition  matrice * matrice
        Matrix<double> A_plus_B = A + B;
        EXPECT_DOUBLE_EQ(A_plus_B(0, 0), A(0, 0) + B(0, 0));
        EXPECT_DOUBLE_EQ(A_plus_B(1, 1), A(1, 1) + B(1, 1));
        EXPECT_DOUBLE_EQ(A_plus_B(2, 2), A(2, 2) + B(2, 2));
        EXPECT_DOUBLE_EQ(A_plus_B(3, 3), A(3, 3) + B(3, 3));
        EXPECT_DOUBLE_EQ(A_plus_B(4, 4), A(4, 4) + B(4, 4));
        EXPECT_DOUBLE_EQ(A_plus_B(5, 5), A(5, 5) + B(5, 5));
        EXPECT_DOUBLE_EQ(A_plus_B(0, 1), 0.0);
        EXPECT_DOUBLE_EQ(A_plus_B(2, 3), 0.0);
    }

    // Opérations arithmétique avec matrices à stockage par lignes et par
    // colonnes.
    {
        // Création d'un matrice à stockage par lignes
        Matrix<double, Dynamic, Dynamic, RowStorage> A(5, 5);
        A(0, 0) = 0.8147;
        A(0, 1) = 0.0975;
        A(0, 2) = 0.1576;
        A(0, 3) = 0.1419;
        A(0, 4) = 0.6557;
        A(1, 0) = 0.9058;
        A(1, 1) = 0.2785;
        A(1, 2) = 0.9706;
        A(1, 3) = 0.4218;
        A(1, 4) = 0.0357;
        A(2, 0) = 0.1270;
        A(2, 1) = 0.5469;
        A(2, 2) = 0.9572;
        A(2, 3) = 0.9157;
        A(2, 4) = 0.8491;
        A(3, 0) = 0.9134;
        A(3, 1) = 0.9575;
        A(3, 2) = 0.4854;
        A(3, 3) = 0.7922;
        A(3, 4) = 0.9340;
        A(4, 0) = 0.6324;
        A(4, 1) = 0.9649;
        A(4, 2) = 0.8003;
        A(4, 3) = 0.9595;
        A(4, 4) = 0.6787;

        // Test : transposée (le résultat est une matrice à stockage par
        //        colonnes)
        Matrix<double, Dynamic, Dynamic, ColumnStorage> B = A.transpose();

        // Test : multiplication  matrix(ligne) * matrice(colonne)
        // Note : teste seulement la première et la dernière colonne
        const auto C = A * B;
        EXPECT_NEAR(C(0, 0), 1.14815820000000, 1e-3);
        EXPECT_NEAR(C(0, 4), 1.31659795000000, 1e-3);
        EXPECT_NEAR(C(1, 0), 1.00133748000000, 1e-3);
        EXPECT_NEAR(C(1, 4), 2.04727044000000, 1e-3);
        EXPECT_NEAR(C(2, 0), 0.99433707000000, 1e-3);
        EXPECT_NEAR(C(2, 4), 2.82896409000000, 1e-3);
        EXPECT_NEAR(C(3, 0), 1.63883925000000, 1e-3);
        EXPECT_NEAR(C(3, 4), 3.28401323000000, 1e-3);
        EXPECT_NEAR(C(4, 0), 1.31659795000000, 1e-3);
        EXPECT_NEAR(C(4, 4), 3.35271580000000, 1e-3);

        // Test : multiplication  matrice(colonne) * matrice(ligne)
        // Note : teste seulement la première et la dernière colonne
        const auto C2 = B * A;
        EXPECT_NEAR(C2(0, 0), 2.73456805000000, 1e-3);
        EXPECT_NEAR(C2(0, 4), 1.95669703000000, 1e-3);
        EXPECT_NEAR(C2(1, 0), 1.88593811000000, 1e-3);
        EXPECT_NEAR(C2(1, 4), 2.08742862000000, 1e-3);
        EXPECT_NEAR(C2(2, 0), 2.07860468000000, 1e-3);
        EXPECT_NEAR(C2(2, 4), 1.94727447000000, 1e-3);
        EXPECT_NEAR(C2(3, 0), 1.94434955000000, 1e-3);
        EXPECT_NEAR(C2(3, 4), 2.27675041000000, 1e-3);
        EXPECT_NEAR(C2(4, 0), 1.95669703000000, 1e-3);
        EXPECT_NEAR(C2(4, 4), 2.48517748000000, 1e-3);

        // Test : addition  matrice(ligne) + matrice(ligne)
        // Note : teste seulement la première et la dernière colonne
        const auto A_plus_A = A + A;
        EXPECT_DOUBLE_EQ(A_plus_A(0, 0), A(0, 0) + A(0, 0));
        EXPECT_DOUBLE_EQ(A_plus_A(0, 4), A(0, 4) + A(0, 4));
        EXPECT_DOUBLE_EQ(A_plus_A(1, 0), A(1, 0) + A(1, 0));
        EXPECT_DOUBLE_EQ(A_plus_A(1, 4), A(1, 4) + A(1, 4));
        EXPECT_DOUBLE_EQ(A_plus_A(2, 0), A(2, 0) + A(2, 0));
        EXPECT_DOUBLE_EQ(A_plus_A(2, 4), A(2, 4) + A(2, 4));
        EXPECT_DOUBLE_EQ(A_plus_A(3, 0), A(3, 0) + A(3, 0));
        EXPECT_DOUBLE_EQ(A_plus_A(3, 4), A(3, 4) + A(3, 4));
        EXPECT_DOUBLE_EQ(A_plus_A(4, 0), A(4, 0) + A(4, 0));
        EXPECT_DOUBLE_EQ(A_plus_A(4, 4), A(4, 4) + A(4, 4));

        // Test : addition  matrice(colonne) + matrice(colonne)
        // Note : teste seulement la première et la dernière colonne
        const auto B_plus_B = B + B;
        EXPECT_DOUBLE_EQ(B_plus_B(0, 0), B(0, 0) + B(0, 0));
        EXPECT_DOUBLE_EQ(B_plus_B(0, 4), B(0, 4) + B(0, 4));
        EXPECT_DOUBLE_EQ(B_plus_B(1, 0), B(1, 0) + B(1, 0));
        EXPECT_DOUBLE_EQ(B_plus_B(1, 4), B(1, 4) + B(1, 4));
        EXPECT_DOUBLE_EQ(B_plus_B(2, 0), B(2, 0) + B(2, 0));
        EXPECT_DOUBLE_EQ(B_plus_B(2, 4), B(2, 4) + B(2, 4));
        EXPECT_DOUBLE_EQ(B_plus_B(3, 0), B(3, 0) + B(3, 0));
        EXPECT_DOUBLE_EQ(B_plus_B(3, 4), B(3, 4) + B(3, 4));
        EXPECT_DOUBLE_EQ(B_plus_B(4, 0), B(4, 0) + B(4, 0));
        EXPECT_DOUBLE_EQ(B_plus_B(4, 4), B(4, 4) + B(4, 4));
    }
}

/**
 * Test pour la multiplication  matrice * vecteur
 */
TEST(TestsTP1, MatrixVectorOperators)
{
    // Vecteur à taille dynamique
    Vector<double> v(5);
    v(0) = 1.0;
    v(1) = 2.0;
    v(2) = 4.0;
    v(3) = 8.0;
    v(4) = 16.0;

    // Test : multiplication par la matrice identité
    {
        Matrix<double> M(5, 5);
        M.setIdentity();

        const auto b = M * v;
        EXPECT_DOUBLE_EQ(b(0), 1.0);
        EXPECT_DOUBLE_EQ(b(1), 2.0);
        EXPECT_DOUBLE_EQ(b(2), 4.0);
        EXPECT_DOUBLE_EQ(b(3), 8.0);
        EXPECT_DOUBLE_EQ(b(4), 16.0);
    }

    // Test : multiplication par une matrice à taille dynamique avec stockage par ligne.
    {
        Matrix<double, Dynamic, Dynamic, RowStorage> M(5, 5);
        M.setIdentity();
        M = 2.0 * M;

        Vector<double> b2 = M * v;
        EXPECT_DOUBLE_EQ(b2(0), 2.0);
        EXPECT_DOUBLE_EQ(b2(1), 4.0);
        EXPECT_DOUBLE_EQ(b2(2), 8.0);
        EXPECT_DOUBLE_EQ(b2(3), 16.0);
        EXPECT_DOUBLE_EQ(b2(4), 32.0);
    }
}

/**
 * Opérateurs d'arithmétique vectorielle
 */
TEST(TestsTP1, VectorOperators)
{
    Vector<double> v(5);
    v(0) = 0.1;
    v(1) = 0.2;
    v(2) = 0.4;
    v(3) = 0.8;
    v(4) = 1.6;

    // Test : multiplication  scalaire * vecteur
    const double alpha = 4.0;
    const auto v2 = alpha * v;
    EXPECT_DOUBLE_EQ(v2(0), alpha * v(0));
    EXPECT_DOUBLE_EQ(v2(1), alpha * v(1));
    EXPECT_DOUBLE_EQ(v2(2), alpha * v(2));
    EXPECT_DOUBLE_EQ(v2(3), alpha * v(3));
    EXPECT_DOUBLE_EQ(v2(4), alpha * v(4));

    // Test : addition  vecteur + vecteur
    const auto v3 = v + v2;
    EXPECT_DOUBLE_EQ(v3(0), v(0) + v2(0));
    EXPECT_DOUBLE_EQ(v3(1), v(1) + v2(1));
    EXPECT_DOUBLE_EQ(v3(2), v(2) + v2(2));
    EXPECT_DOUBLE_EQ(v3(3), v(3) + v2(3));
    EXPECT_DOUBLE_EQ(v3(4), v(4) + v2(4));
}

/**
 * Test des performance de la multiplication  matrice * vecteur
 * pour de grandes dimensions.
 */
TEST(TestsTP1, PerformanceMatrixVector)
{
    Matrix<double> A(16384, 16384); // grande matrice avec stockage colonne
    Vector<double> v(16384);        // grand vecteur

    using namespace std::chrono;
    // Test : multiplication avec l'algorithme naif.
    high_resolution_clock::time_point t = high_resolution_clock::now();
    naiveMatrixMult(A, v);
    const duration<double> naive_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);

    // Test : multiplication avec l'implémentation spécifique pour les matrices avec
    // stockage par colonnes.
    t = high_resolution_clock::now();
    A* v;
    const duration<double> optimal_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);

    EXPECT_TRUE(optimal_t < 0.4 * naive_t)
        << "Naive time: " << duration_cast<std::chrono::milliseconds>(naive_t).count() << " ms, "
        << "optimized time: " << duration_cast<std::chrono::milliseconds>(optimal_t).count() << " ms";
}

/**
 * Test des performances de l'addition  matrice + matrice
 * pour de grandes dimensions.
 */
TEST(TestsTP1, PerformanceLargeMatrixMatrix)
{
    // deux grandes matrices à stockage par colonnes
    Matrix<double> A(16384, 16384);
    Matrix<double> B(16384, 16384);

    using namespace std::chrono;
    high_resolution_clock::time_point t = high_resolution_clock::now();
    // Test : addition avec l'algorithme naif
    naiveMatrixAddition(A, B);
    const duration<double> naive_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);

    // Test : addition avec l'implémentation spécifique pour les matrices à
    // stockage par colonnes.
    t = high_resolution_clock::now();
    A + B;
    const duration<double> optimal_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);

    EXPECT_TRUE(optimal_t < 0.4 * naive_t);
}

#pragma endregion // Tests Fournis
