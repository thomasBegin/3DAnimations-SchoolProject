#pragma once

/**
 * @file Vector.h
 *
 * @brief Implémentation de vecteurs simples
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#include "MatrixBase.h"

namespace gti320 {

    /**
     * Classe vecteur générique.
     *
     * Cette classe réutilise la classe `MatrixBase` et ses spécialisations de
     * templates pour les manipulation bas niveau.
     */
    template<typename _Scalar = double, int _Rows = Dynamic>
    class Vector : public MatrixBase<_Scalar, _Rows, 1> {
    public:
        /** Constructeur par défaut */
        Vector() : MatrixBase<_Scalar, _Rows, 1>() {}

        /** Contructeur à partir d'un taille (rows). */
        explicit Vector(int rows) : MatrixBase<_Scalar, _Rows, 1>(rows, 1) {}

        /** Constructeur de copie */
        Vector(const Vector& other) : MatrixBase<_Scalar, _Rows, 1>(other) {}

        /** Destructeur */
        ~Vector() {}

        /** Opérateur de copie */
        Vector& operator=(const Vector& other)
        {
            //Resize le vector s'ils ne sont pas du même size
            if (this->rows() != other.rows())
            {
                this->resize(other.rows());
            }

            this->m_storage = other.m_storage;
            return *this;
        }

        /** Accesseur à une entrée du vecteur (lecture seule) */
        _Scalar operator()(int i) const
        {
            return this->data()[i];
        }

        /** Accesseur à une entrée du vecteur (lecture et écriture) */
        _Scalar& operator()(int i)
        {
            return this->m_storage.data()[i];
        }

        /** Modifie le nombre de lignes du vecteur */
        void resize(int _rows)
        {
            MatrixBase<_Scalar, _Rows, 1>::resize(_rows, 1);
        }

        /** Produit scalaire de *this et other */
        inline _Scalar dot(const Vector& other) const
        {
            //Additionne les produit des elements aux memes positions dans les deux vectors

            _Scalar dotp = 0;

            for (int i = 0; i < this->rows(); ++i)
            {
                dotp += (*this)(i) * other(i);
            }

            return dotp;
        }

        /** Retourne la norme euclidienne du vecteur */
        inline _Scalar norm() const
        {
            //Additione le carre de tout les elements du vectors et retourne la racine carre

            _Scalar powAdd = 0;

            for (int i = 0; i < rows(); ++i)
            {
                powAdd += pow((*this)(i), 2);
            }

            return sqrt(powAdd);
        }
        
        void normalize()
        {
            _Scalar f = 1.0 / this->norm();
            for (int i = 0; i < this->rows(); ++i)
                (*this)(i) *= f;
        }
    };
} // namespace gti320
