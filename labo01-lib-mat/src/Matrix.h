#pragma once

/**
 * @file Matrix.h
 *
 * @brief Implémentation de matrices simples.
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#include "MatrixBase.h"

namespace gti320 {
    enum StorageType { ColumnStorage = 0,
                       RowStorage = 1 };

    // Déclaration avancée
    template<typename _Scalar, int _RowsAtCompile, int _ColsAtCompile, int _StorageType>
    class SubMatrix;

    /**
     * Classe Matrix spécialisé pour le cas générique. (defaut par colonne)
     *
     * Note :
     * Le cas d'un stockage par ligne fait l'objet d'une spécialisation de patron. (voir plus bas)
     */
    template<typename _Scalar = double, int _RowsAtCompile = Dynamic, int _ColsAtCompile = Dynamic, int _StorageType = ColumnStorage>
    class Matrix : public MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile> {
    public:
        /** Constructeur par défaut */
        Matrix() :
            MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>() {}

        /** Constructeur de copie */
        Matrix(const Matrix& other) :
            MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>(other) {}

        /** Constructeur avec spécification du nombre de ligne et de colonnes */
        explicit Matrix(int _rows, int _cols) :
            MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>(_rows, _cols) {}

        /** Destructeur */
        ~Matrix() {}

        /**
         * Opérateur de copie à partir d'une sous-matrice.
         * Exemple :
         * Matrix<...> A(...);
         * Matrix<...> B(...);
         * B = A.block(i,j,m,n);
         */
        template<typename _OtherScalar, int OtherRows, int _OtherCols, int _OtherStorage>
        Matrix& operator=(const SubMatrix<_OtherScalar, OtherRows, _OtherCols, _OtherStorage>& submatrix)
        {
            //Resize this si submatrix n'est pas du meme size
            if (this->cols() != submatrix.cols() || this->rows() != submatrix.rows()) 
            {
                this->resize(submatrix.rows(), submatrix.rows())
            }

            //Optimise le remplacement pour une matrix stockee en colonnes
            for (int j = 0; j < this->cols(); j++)
            {
                for (int i = 0; i < this->rows(); i++) 
                {
                    (*this)(i, j) = submatrix(i, j);
                }
            }

            return *this;
        }

        /** Accesseur à une entrée de la matrice (lecture seule) */
        _Scalar operator()(int i, int j) const
        {
            //Matrix stokee en colonne, on multiplie donc j par rows() pour acceder a la bonne colonne
            return this->data()[((j * this->rows()) + i)];
        }

        /** Accesseur à une entrée de la matrice (lecture ou écriture) */
        _Scalar& operator()(int i, int j)
        {
            //Matrix stokee en colonne, on multiplie donc j par rows() pour acceder a la bonne colonne
            return this->m_storage.data()[((j * this->rows()) + i)];
        }

        /** Créer une sous-matrice de taille (rows, cols) à partir de l'index (i,j) */
        SubMatrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType> block(int i, int j, int rows, int cols) const
        {
            return SubMatrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType>(*this, i, j, rows, cols);
        }

        /** Calcule l'inverse de la matrice */
        Matrix inverse() const
        {
            // Ne rien faire.
            return *this;
        }

        /** Retourne la transposée de la matrice */
        template<typename _OtherScalar, int _OtherRows, int _OtherCols, int _OtherStorage>
        Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage> transpose() const
        {
            //Creation d'une matrix de taille inverse a this (this.rows = tempCopy.cols, etc...)
            Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage> tempCopy = Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage>(this->cols(), this->rows());
            tempCopy.setZero();

            //Chaque element (i,j) de this sera egal a (j,i) de son inverse
            //Parcours de la matrix optimise pour une matrix en colonne
            for (int j = 0; j < this->cols(); j++)
            {
                for (int i = 0; i < this->rows(); i++)
                {
                    tempCopy(j, i) = (*this)(i, j);
                }
            }

            return tempCopy; 
        }

        /** Affecte l'identité à la matrice */
        inline void setIdentity()
        {
            //On met tout les elements de this a 0
            setZero();

            //On prend le plus petit des rows() ou cols() au cas ou notre matrix n'est pas carre
            int minSize = this->rows() <= this->cols() ? this->rows() : this->cols();

            //On met chaque valeur de la diagonale a 1.0
            for (int i = 0; i < minSize; i++)
            {
                (*this)(i, i) = 1.0;
            }
        }
    };

    /** Classe Matrix spécialisée pour un stockage par lignes */
    template<typename _Scalar, int _RowsAtCompile, int _ColsAtCompile>
    class Matrix<_Scalar, _RowsAtCompile, _ColsAtCompile, RowStorage> : public MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile> {
    public:
        /** Constructeur par défaut */
        Matrix() :
            MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>() {}

        /** Constructeur de copie */
        Matrix(const Matrix& other) :
            MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>(other) {}

        /** Constructeur avec spécification du nombre de ligne et de colonnes */
        explicit Matrix(int rows, int cols) :
            MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>(rows, cols) {}

        /** Destructeur */
        ~Matrix() {}

        /**
         * Opérateur de copie à partir d'une sous-matrice.
         * Exemple :
         * Matrix<...> A(...);
         * Matrix<...> B(...);
         * B = A.block(i,j,m,n);
         */
        template<typename _OtherScalar, int OtherRows, int _OtherCols, int _OtherStorage>
        Matrix& operator=(const SubMatrix<_OtherScalar, OtherRows, _OtherCols, _OtherStorage>& submatrix)
        {
            //Resize this si submatrix n'est pas du meme size
            if (this->cols() != submatrix.cols() || this->rows() != submatrix.rows())
            {
                this->resize(submatrix.rows(), submatrix.rows())
            }

            //Optimise le remplacement pour un matrix stocker en lignes
            for (int i = 0; i < this->rows(); i++)
            {
                for (int j = 0; j < this->cols(); j++)
                {
                    (*this)(i, j) = submatrix(i, j);
                }
            }

            return *this;
        }

        /** Accesseur à une entrée de la matrice (lecture seule) */
        _Scalar operator()(int i, int j) const
        {
            //Matrix stokee en rangee, on multiplie donc i par cols() pour acceder a la bonne rangee
            return this->data()[((i * this->cols()) + j)];
        }

        /** Accesseur à une entrée de la matrice (lecture ou écriture) */
        _Scalar& operator()(int i, int j)
        {
            //Matrix stokee en rangee, on multiplie donc i par cols() pour acceder a la bonne rangee
            return this->m_storage.data()[((i * this->cols()) + j)];
        }

        /** Créer une sous-matrice de taille (rows, cols) à partir de l'index (i,j) */
        SubMatrix<_Scalar, _RowsAtCompile, _ColsAtCompile, RowStorage> block(int i, int j, int rows, int cols) const
        {
            return SubMatrix<_Scalar, _RowsAtCompile, _ColsAtCompile, RowStorage>(*this, i, j, rows, cols);
        }

        /** Calcule l'inverse de la matrice */
        Matrix inverse() const
        {
            // Ne rien faire.
            return *this;
        }

        /** Retourne la transposée de la matrice */
        Matrix<_Scalar, _ColsAtCompile, _RowsAtCompile, ColumnStorage> transpose() const
        {
            //Creation d'une matrix de taille inverse a this (this.rows = tempCopy.cols, etc...)
            Matrix<_Scalar, _ColsAtCompile, _RowsAtCompile, ColumnStorage> tempCopy = Matrix<_Scalar, _ColsAtCompile, _RowsAtCompile, ColumnStorage>(this->cols(), this->rows());
            tempCopy.setZero();

            //Chaque element (i,j) de this sera egal a (j,i) de son inverse
            //Parcours de la matrix optimise pour une matrix en rangee
            for (int i = 0; i < this->rows(); i++)
            {
                for (int j = 0; j < this->cols(); j++)
                {
                    tempCopy(j, i) = (*this)(i, j);
                }
            }

            return tempCopy;
        }

        /** Affecte l'identité à la matrice */
        inline void setIdentity()
        {
            //On met tout les elements de this a 0
            setZero();

            //On prend le plus petit des rows() ou cols() au cas ou notre matrix n'est pas carre
            int minSize = this->rows() <= this->cols() ? this->rows() : this->cols();

            //On met chaque valeur de la diagonale a 1.0
            for (int i = 0; i < minSize; i++)
            {
                (*this)(i, i) = 1.0;
            }
        }
    };

    /**
     * Classe pour accéder à une sous-matrice.
     *
     * Note :
     * Un sous-matrice ne copie pas les données.
     * Elle conserve plutôt une référence à la matrice originale.
     */
    template<typename _Scalar, int _RowsAtCompile, int _ColsAtCompile, int _StorageType>
    class SubMatrix {
    private:
        // Référence à la matrice originale
        Matrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType>& m_matrix;

        // Constructeur par défaut (privé)
        SubMatrix() {}

        // (m_i, m_j) est le coin supérieur gauche de la sous-matrice dans la matrice originale.
        int m_i; // Décalage en ligne
        int m_j; // Décalage en colonne

        // La sous-matrice est de dimension : m_rows x m_cols
        int m_rows; // Hauteur de la sous-matrice (nombre de lignes)
        int m_cols; // Largeur de la sous-matrice (nombre de colonnes)

    public:
        /** Constructeur à partir d'une référence en lecture seule à une matrice. */
        SubMatrix(const Matrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType>& _matrix, int _i, int _j, int _rows, int _cols) :
            m_matrix(const_cast<Matrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType>&>(_matrix)),
            m_i(_i),
            m_j(_j),
            m_rows(_rows),
            m_cols(_cols)
        {
        }

        /** Constructeur à partir d'une référence en lecture et écriture à une matrice. */
        explicit SubMatrix(Matrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType>& _matrix, int _i, int _j, int _rows, int _cols) :
            m_matrix(_matrix),
            m_i(_i),
            m_j(_j),
            m_rows(_rows),
            m_cols(_cols)
        {
        }

        /** Constructeur de copie */
        SubMatrix(const SubMatrix& other) :
            m_matrix(other.m_matrix),
            m_i(other.m_i),
            m_j(other.m_j),
            m_rows(other.m_rows),
            m_cols(other.m_cols)
        {
        }

        /** Destructeur */
        ~SubMatrix() {}

        /** Opérateur de copie (à partir d'une matrice) */
        template<typename _OtherScalar, int _OtherRows, int _OtherCols, int _OtherStorage>
        SubMatrix& operator=(const Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage>& matrix)
        {

            //retourne immediatement this si les dimension de la matrix ne sont pas les memes que la sous-matrix
            if (_OtherCols != m_cols || _OtherRows != m_rows) 
            {
                return *this;
            }

            //Par defaut, une matrix est stocke en colonne, le parcours de la matix est donc optimise pour les colonnes
            for (int j = m_j; j < m_cols; j++)
            {
                for (int i = m_i; i < m_rows; i++)
                {
                    m_matrix(i, j) = matrix(i - m_i, j - m_j);
                }
            }

            return *this;
        }

        /** Accesseur aux entrées de la sous-matrice (lecture seule) */
        _Scalar operator()(int i, int j) const
        {
            return m_matrix.data()[i + m_i, j + m_j];
        }

        /** Accesseur aux entrées de la sous-matrice (lecture et écriture) */
        _Scalar& operator()(int i, int j)
        {
            //Retourne un pointeur null si les indices ne respectent pas la taille de la sous-matrice.
            if (i > m_rows || j > m_cols)
            {
                return nullptr;
            }

            return m_matrix.m_storage.data()[i + m_i, j + m_j];
        }

        /** Retourne la transposée de la sous-matrice sous la forme d'une matrice. */
        template<typename _OtherScalar, int _OtherRows, int _OtherCols, int _OtherStorage>
        Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage> transpose() const
        {
            //Creation d'une matrix pour le retour
            Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage> tempCopy = Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage>();

            //Chaque element (i,j) de this sera egal a (j,i) de son inverse
            //Par defaut, une matrix est stocke en colonne, le parcours de la matix est donc optimise pour les colonnes
            for (int j = 0; j < m_cols; j++)
            {
                for (int i = 0; i < m_rows; i++)
                {
                    tempCopy(j, i) = (*this)(i, j);
                }
            }

            return tempCopy;
        }

        inline int rows() const { return m_rows; }
        inline int cols() const { return m_cols; }
    };

} // namespace gti320
