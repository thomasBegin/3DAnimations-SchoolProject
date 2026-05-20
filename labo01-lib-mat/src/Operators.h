#pragma once

/**
 * @file Operators.h
 *
 * @brief Implémentation de divers opérateurs arithmétiques pour les matrices et les vecteurs.
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#include "Matrix.h"
#include "Vector.h"

namespace gti320 {

    /** Multiplication : Matrice * Matrice (générique) */
    template<typename _Scalar, int RowsA, int ColsA, int StorageA, int RowsB, int ColsB, int StorageB>
    Matrix<_Scalar, RowsA, ColsB> operator*(const Matrix<_Scalar, RowsA, ColsA, StorageA>& A, const Matrix<_Scalar, RowsB, ColsB, StorageB>& B)
    {
        //Creation d'une matrix de taille (A.rows, B.cols) pour le retour
        Matrix<_Scalar, RowsA, ColsB> multM = Matrix<_Scalar, RowsA, ColsB>(A.rows(), B.cols());
        multM.setZero();

        //Addition des produits des elements des rows de A et des cols de B
        for (int ia = 0; ia < A.rows(); ia++)
        {
            for (int jb = 0; jb < B.cols(); jb++)
            {
                for (int ja = 0; ja < A.cols(); ja++)
                {
                    multM(ia, jb) += A(ia, ja) * B(ja, jb);
                }
            }
        }

        return multM;
    }

    /**
     * Multiplication : Matrice (colonne) * Matrice (ligne)
     *
     * Spécialisation de l'opérateur de multiplication pour le cas où les matrices
     * ont un stockage à taille dynamique et où la matrice de gauche utilise un
     * stockage par colonnes et celle de droite un stockage par lignes.
     */
    template<typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic> operator*(const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& B)
    {
        //Creation d'une matrix de taille (A.rows, B.cols) pour le retour
        Matrix<_Scalar, Dynamic, Dynamic> multM = Matrix<_Scalar, Dynamic, Dynamic>(A.rows(), B.cols());
        multM.setZero();

        //Addition des produits des elements des rows de A et des cols de B
        //Parcours des matrix optimise pour A stocke en colonnes et B stocke en lignes
        for (int ib = 0; ib < B.rows(); ib++)
        {
            for (int jb = 0; jb < B.cols(); jb++)
            {
                for (int ia = 0; ia < A.rows(); ia++)
                {
                    multM(ia, jb) += A(ia, ib) * B(ib, jb);
                }
            }
        }

        return multM;
    }

    /**
     * Multiplication : Matrice (ligne) * Matrice (colonne)
     *
     * Spécialisation de l'opérateur de multiplication pour le cas où les matrices
     * ont un stockage à taille dynamique et où la matrice de gauche utilise un
     * stockage par lignes et celle de droite un stockage par colonnes.
     */
    template<typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic> operator*(const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& B)
    {
        //Creation d'une matrix de taille (A.rows, B.cols) pour le retour
        Matrix<_Scalar, Dynamic, Dynamic> multM = Matrix<_Scalar, Dynamic, Dynamic>(A.rows(), B.cols());
        multM.setZero();

        //Addition des produits des elements des rows de A et des cols de B
        //Parcours des matrix optimise pour A stocke en lignes et B stocke en colonnes
        for (int ia = 0; ia < A.rows(); ia++)
        {
            for (int jb = 0; jb < B.cols(); jb++)
            {
                for (int ja = 0; ja < A.cols(); ja++)
                {
                    multM(ia, jb) += A(ia, ja) * B(ja, jb);
                }
            }
        }

        return multM;
    }

    /** Addition : Matrice + Matrice (générique) */
    template<typename _Scalar, int Rows, int Cols, int StorageA, int StorageB>
    Matrix<_Scalar, Rows, Cols> operator+(const Matrix<_Scalar, Rows, Cols, StorageA>& A, const Matrix<_Scalar, Rows, Cols, StorageB>& B)
    {
        //Creation d'une matrix pour le retour
        Matrix<_Scalar, Rows, Cols> addM = Matrix<_Scalar, Rows, Cols>(A.rows(), A.cols());

        //Addition de chaque elemts de meme positions
        for (int j = 0; j < A.cols(); j++)
        {
            for (int i = 0; i < A.rows(); i++)
            {
                addM(i, j) = A(i, j) + B(i, j);
            }
        }

        return addM;
    }

    /**
     * Addition : Matrice (colonne) + Matrice (colonne)
     *
     * Spécialisation de l'opérateur d'addition pour le cas où les deux matrices
     * sont stockées par colonnes.
     */
    template<typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic> operator+(const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& B)
    {
        //Creation d'une matrix pour le retour
        Matrix<_Scalar, Dynamic, Dynamic> addM = Matrix<_Scalar, Dynamic, Dynamic>(A.rows(), B.rows());

        //Addition de chaque elemts de meme positions
        //Parcours des matrix optimise pour des matrix stocke en colonnes
        for (int j = 0; j < A.cols(); j++)
        {
            for (int i = 0; i < A.rows(); i++)
            {
                addM(i, j) = A(i, j) + B(i, j);
            }
        }

        return addM;
    }

    /**
     * Addition : Matrice (ligne) + Matrice (ligne)
     *
     * Spécialisation de l'opérateur d'addition pour le cas où les deux matrices
     * sont stockées par lignes.
     */
    template<typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic, RowStorage> operator+(const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& B)
    {
        //Creation d'une matrix pour le retour
        Matrix<_Scalar, Dynamic, Dynamic, RowStorage> addM = Matrix<_Scalar, Dynamic, Dynamic, RowStorage>(A.rows(), A.cols());

        //Addition de chaque elemts de meme positions
        //Parcours des matrix optimise pour des matrix stocke en colonnes
        for (int i = 0; i < A.rows(); i++)
        {
            for (int j = 0; j < A.cols(); j++)
            {
                addM(i, j) = A(i, j) + B(i, j);
            }
        }

        return addM;
    }

    /**
     * Multiplication  : Scalaire * Matrice (colonne)
     *
     * Spécialisation de l'opérateur de multiplication par un scalaire pour le
     * cas d'une matrice stockée par colonnes.
     */
    template<typename _Scalar, int _Rows, int _Cols>
    Matrix<_Scalar, _Rows, _Cols, ColumnStorage> operator*(const _Scalar& a, const Matrix<_Scalar, _Rows, _Cols, ColumnStorage>& A)
    {
        //Creation d'une matrix pour le retour
        Matrix<_Scalar, _Rows, _Cols, ColumnStorage> multScalaire = Matrix<_Scalar, _Rows, _Cols, ColumnStorage>(A.rows(), A.cols());

        //Multiplication de chaque elemts de la matrix par le scalaire
        //Parcours optimiser pour une matrix stocke en colonnes
        for (int j = 0; j < A.cols(); j++)
        {
            for (int i = 0; i < A.rows(); i++)
            {
                multScalaire(i, j) = A(i, j) * a;
            }
        }
        
        return multScalaire;
    }

    /**
     * Multiplication  : Scalaire * Matrice (ligne)
     *
     * Spécialisation de l'opérateur de multiplication par un scalaire pour le
     * cas d'une matrice stockée par lignes.
     */
    template<typename _Scalar, int _Rows, int _Cols>
    Matrix<_Scalar, _Rows, _Cols, RowStorage> operator*(const _Scalar& a, const Matrix<_Scalar, _Rows, _Cols, RowStorage>& A)
    {
        //Creation d'une matrix pour le retour
        Matrix<_Scalar, Dynamic, Dynamic, RowStorage> multScalaire = Matrix<_Scalar, Dynamic, Dynamic, RowStorage>(A.rows(), A.cols());

        //Multiplication de chaque elemts de la matrix par le scalaire
        //Parcours optimiser pour une matrix stocke en lignes
        for (int i = 0; i < A.rows(); i++)
        {
            for (int j = 0; j < A.cols(); j++)
            {
                multScalaire(i, j) = A(i, j) * a;
            }
        }

        return multScalaire;
    }

    /**
     * Multiplication : Matrice (ligne) * Vecteur
     *
     * Spécialisation de l'opérateur de multiplication matrice*vecteur pour le
     * cas où la matrice est représentée par lignes.
     */
    template<typename _Scalar, int _Rows, int _Cols>
    Vector<_Scalar, _Rows> operator*(const Matrix<_Scalar, _Rows, _Cols, RowStorage>& A, const Vector<_Scalar, _Cols>& v)
    {
        //Creation d'un vecteur pour le retour
        Vector<_Scalar, _Rows> multMV = Vector<_Scalar, _Rows>(A.cols());

        //Parcours optimiser pour une matrix stocke en ligne
        for (int i = 0; i < A.rows(); i++)
        {
            for (int j = 0; j < A.cols(); j++)
            {
                multMV(i) += A(i, j) * v(j);
            }
        }

        return multMV;
    }

    /**
     * Multiplication : Matrice (colonne) * Vecteur
     *
     * Spécialisation de l'opérateur de multiplication matrice*vecteur pour le
     * cas où la matrice est représentée par colonnes.
     */
    template<typename _Scalar, int _Rows, int _Cols>
    Vector<_Scalar, _Rows> operator*(const Matrix<_Scalar, _Rows, _Cols, ColumnStorage>& A, const Vector<_Scalar, _Cols>& v)
    {
        //Creation d'un vector pour le retour
        Vector<_Scalar, _Rows> multMV = Vector<_Scalar, _Rows>(A.cols());

        //Parcours optimiser pour une matrix stocke en colonne
        for (int j = 0; j < A.cols(); j++)
        {
            for (int i = 0; i < A.rows(); i++)
            {
                multMV(i) += A(i, j) * v(j);
            }   
        }
        
        return multMV;
    }

    /** Multiplication : Scalaire * Vecteur */
    template<typename _Scalar, int _Rows>
    Vector<_Scalar, _Rows> operator*(const _Scalar& a, const Vector<_Scalar, _Rows>& v)
    {
        //Creation d>un vector pour le retour
        Vector<_Scalar, _Rows> multSV = Vector<_Scalar, _Rows>(v.rows());

        //Multiplication du scalaire par chaque elemts du vector
        for (int i = 0; i < v.rows(); i++)
        {
            multSV(i) = a * v(i);
        }

        return multSV;
    }

    /** Addition : Vecteur + Vecteur */
    template<typename _Scalar, int _RowsA, int _RowsB>
    Vector<_Scalar, _RowsA> operator+(const Vector<_Scalar, _RowsA>& a, const Vector<_Scalar, _RowsB>& b)
    {
        //Creation d'un vector pour le retour
        Vector<_Scalar, _RowsA> addVV = Vector<_Scalar, _RowsA>(a.rows());

        //Addition de chaques elemts de meme position des deux vectors
        for (int i = 0; i < a.rows(); i++)
        {
            addVV(i) = a(i) + b(i);
        }

        return addVV;
    }

    /** Soustraction : Vecteur - Vecteur */
    template<typename _Scalar, int _RowsA, int _RowsB>
    Vector<_Scalar, _RowsA> operator-(const Vector<_Scalar, _RowsA>& a, const Vector<_Scalar, _RowsB>& b)
    {
        //Creation d'un vector pour le retour
        Vector<_Scalar, _RowsA> sousVV = Vector<_Scalar, _RowsA>(a.rows());

        //Soustraction de chaques elemts de meme position des deux vectors
        for (int i = 0; i < a.rows(); i++)
        {
            sousVV(i) = a(i) - b(i);
        }

        return sousVV;
    }
} // namespace gti320
