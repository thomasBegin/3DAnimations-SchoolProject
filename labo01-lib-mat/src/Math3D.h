#pragma once

/**
 * @file Math3D.h
 *
 * @brief Fonctions pour l'initialisation et la manipulation de matrices de
 * rotation, de matrices de transformations en coordonnées homogènes et de
 * vecteurs 3D.
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#include "Matrix.h"
#include "Operators.h"
#include "Vector.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>

namespace gti320 {

    // Deux types de vecteurs 3D considérés ici
    typedef Vector<double, 3> Vector3d;
    typedef Vector<float, 3> Vector3f;

    // Quaternions
    typedef Vector<double,4> Quat;

    // Dans le cadre de ce projet, nous considérons seulement deux
    // cas :
    //
    //  - les rotations
    //  - les translations
    //
    // Deux types de matrices en coordonnées homogèes :
    typedef Matrix<double, 4, 4, ColumnStorage> Matrix4d;
    typedef Matrix<float, 4, 4, ColumnStorage> Matrix4f;
    //
    // Deux types de matrices pour les rotations
    typedef Matrix<double, 3, 3, ColumnStorage> Matrix3d;
    typedef Matrix<float, 3, 3, ColumnStorage> Matrix3f;

    /** Initialise et retourne la matrice identité */
    template<>
    inline void Matrix4d::setIdentity()
    {
        //Initialise la matrix a 0
        setZero();

        for (int i = 0; i < 4; i++)
        {
            //Set les elmts de la diago a 1.0
            (*this)(i, i) = 1.0;
        }
    }

    /**
     * Calcul de la matrice inverse SPÉCIALISÉ pour le cas
     * d'une matrice de transformation en coordonnées homogènes.
     */
    template<>
    inline Matrix4d Matrix4d::inverse() const
    {
        // Initialisation de la matrix de retour
        Matrix4d invM4d = Matrix4d(4,4);
        invM4d.setZero();

        //Initialisation de vecteurs pour les cols de la matrix
        Vector3d vCol1 = Vector3d(3);
        vCol1.setZero();
        Vector3d vCol2 = Vector3d(3);
        vCol2.setZero();
        Vector3d vCol3 = Vector3d(3);
        vCol3.setZero();
        Vector3d vCol4 = Vector3d(3);
        vCol4.setZero();

        for (int col1 = 0; col1 < 3; col1++)
        {
            vCol1(col1) = (*this)(col1, 0);
        }
        for (int col2 = 0; col2 < 3; col2++)
        {
            vCol2(col2) = (*this)(col2, 1);
        }
        for (int col3 = 0; col3 < 3; col3++)
        {
            vCol3(col3) = (*this)(col3, 2);
        }
        for (int col4 = 0; col4 < 3; col4++)
        {
            vCol4(col4) = (*this)(col4, 3);
        }


        //On transpose seulement les 3 premieres colonnes et lignes
        for (int j = 0; j < 3; j++)
        {
            for (int i = 0; i < 3; i++)
            {
                //Set chaque element i,j de la matrix a l'element j,i de l'inverse
                invM4d(j, i) = (*this)(i, j);
            }
        }

        //Set les elements de la dernieres colonne
        invM4d(0, 3) = vCol1.dot(vCol4) * -1;
        invM4d(1, 3) = vCol2.dot(vCol4) * -1;
        invM4d(2, 3) = vCol3.dot(vCol4) * -1;
        invM4d(3, 3) = 1.0;

        return invM4d;
    }

    /**
     * Calcul de la matrice inverse SPÉCIALISÉ pour le cas
     * d'une matrice de rotation.
     */
    template<>
    inline Matrix3d Matrix3d::inverse() const
    {
        // Initialisation de la matrix de retour
        Matrix<double, 3, 3, ColumnStorage> invM3d = Matrix(3, 3);
        invM3d.setZero();

        for (int j = 0; j < 3; j++)
        {
            for (int i = 0; i < 3; i++)
            {
                //Set chaque element i,j de la matrix a l'element j,i de l'inverse
                invM3d(j, i) = (*this)(i, j);
            }
        }

        //L'inverse d'une matrix de rotation est sa transpose
        return invM3d;
    }

    /**
     * Multiplication d'une matrice 4x4 avec un vecteur 3D où la matrice
     * représente une transformation en coordonnées homogène.
     */
    template<typename _Scalar>
    Vector<_Scalar, 3> operator*(const Matrix<_Scalar, 4, 4, ColumnStorage>& A, const Vector<_Scalar, 3>& v)
    {
        //Initialise un vector 3d pour le retour
        Vector<_Scalar, 3> multM4dV3d = Vector<_Scalar, 3>(3);
        multM4dV3d.setZero();

        //Initialise un vector 4d pour la multiplication
        Vector<_Scalar, 4> copyV = Vector<_Scalar, 4> (4);
        copyV.setZero();

        //Set les valeurs de copyV aux valeurs de v
        for (int k = 0; k < 3; k++)
        {
            copyV(k) = v(k);
        }
        //Et set le dernier elmt du vector a 1.0
        copyV(3) = 1.0;

        //Multiplication Matrix vector optimisee pour une matrice colonne
        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                multM4dV3d(i) += A(i, j) * copyV(j);
            }
        }

        return multM4dV3d;
    }

    /**
     * Créer et retourne une matrice de rotation définie par les angles
     * d'Euler XYZ exprimés en radians.
     *
     * La matrice doit correspondre au produit : Rz * Ry * Rx.
     */
    template<typename _Scalar>
    static Matrix<_Scalar, 3, 3> makeRotation(_Scalar x, _Scalar y, _Scalar z)
    {
        //Initialisation de la matrix de retour
        Matrix<_Scalar, 3, 3> rotM = Matrix<_Scalar, 3, 3>(3, 3);

        //Initialisaton des valeurs utilises dans le calcul
        _Scalar cosX = _CMATH_::cos(x);
        _Scalar sinX = _CMATH_::sin(x);
        _Scalar cosY = _CMATH_::cos(y);
        _Scalar sinY = _CMATH_::sin(y);
        _Scalar cosZ = _CMATH_::cos(z);
        _Scalar sinZ = _CMATH_::sin(z);

        //Calcul de la rotation avec angles d'Euler
        rotM(0, 0) = cosY * cosZ;
        rotM(1, 0) = cosY * sinZ;
        rotM(2, 0) = sinY * -1;
        rotM(0, 1) = (cosX * sinZ * -1) + (sinX * sinY * cosZ);
        rotM(1, 1) = (cosX * cosZ) + (sinX * sinY * sinZ);
        rotM(2, 1) = sinX * cosY;
        rotM(0, 2) = (sinX * sinZ) + (cosX * sinY * cosZ);
        rotM(1, 2) = (sinX * cosZ * -1) + (cosX * sinY * sinZ);
        rotM(2, 2) = cosX * cosY;

        return rotM;
    }

    /**
     *  Cree une matrice de transformation rigide a partir d'une translation (Vector3) et d'une rotation (Quat)
     */
    static Matrix4d makeRigid( const Vector3d & translation, const Quat & rotation )
    {
        //Initialisation de la matrix de retour
        Matrix4d mat;
        mat.setZero();

        //Inititialisation de var pour les valeurs des translation et rotation
        double qw = rotation(0);
        double qx = rotation(1);
        double qy = rotation(2);
        double qz = rotation(3);

        double tx = translation(0);
        double ty = translation(1);
        double tz = translation(2);

        /*
            Conversion du quaternion en matrice de rotation pour la submatix 3x3
            Set Translation a la derniere colonne de la matrix
            Set la dernier rangee a [0,0,0,1]

            L'ordre des operations est optimise pour une matrice colonne
        */
        mat(0, 0) = pow(qw, 2) + pow(qx, 2) - pow(qy, 2) - pow(qz, 2);
        mat(1, 0) = (2 * qx * qy) + (2 * qw * qz);
        mat(2, 0) = (2 * qx * qz) - (2 * qw * qy);
        mat(3, 0) = 0.0;
        mat(0, 1) = (2 * qx * qy) - (2 * qw * qz);
        mat(1, 1) = pow(qw, 2) - pow(qx, 2) + pow(qy, 2) - pow(qz, 2);
        mat(2, 1) = (2 * qy * qz) + (2 * qw * qx);
        mat(3, 1) = 0.0;
        mat(0, 2) = (2 * qx * qz) + (2 * qw * qy);
        mat(1, 2) = (2 * qy * qz) - (2 * qw * qx);
        mat(2, 2) = pow(qw, 2) - pow(qx, 2) - pow(qy, 2) + pow(qz, 2);
        mat(3, 2) = 0.0;
        mat(0, 3) = tx;
        mat(1, 3) = ty;
        mat(2, 3) = tz;
        mat(3, 3) = 1.0;

        return mat;
    }

    /**
     *  Interpole entre 2 Vector selon un ratio t et retourne le résultat
     */
    template<typename _Scalar, int _Size>
    static Vector<_Scalar, _Size> lerp(const Vector<_Scalar, _Size>& v0, const Vector<_Scalar, _Size>& v1, double t)
    {
        //Initialisation du vector de retour
        Vector<_Scalar, _Size> res;
        res.setZero();

        //Calcul de l'interporation
        for (int i = 0; i < res.size(); i++)
        {
            res(i) = ((1 - t) * v0(i)) + (t * v1(i));
        }

        return res;
    }

    /**
     *  Interpolation sphérique entre 2 quaternions selon un ratio t et retourne le résultat
     */
    static const double dotTolerance = 0.9995;
    static Quat slerp(const Quat& q0, const Quat& q1, double t)
    {
        //Initialisation du Quaternion de retour
        Quat res;
        res.setZero();

        //Initialisation des var utilises dans le calcul
        double theta = _CMATH_::acos(q0.dot(q1));
        if (theta == 0) theta = 1 - dotTolerance;

        double thetaSin = _CMATH_::sin(theta);
        double q0Sin = _CMATH_::sin(theta * (1 - t));
        double q1Sin = _CMATH_::sin(theta * t);

        //Calcul de l'interpolation lisse
        for (int i = 0; i < 4; i++)
        {
            res(i) = ( (q0Sin * q0(i)) + (q1Sin * q1(i)) ) / thetaSin;
        }

        return res;
    }

    /**
     *  Transforme les angles d'Euler dans le vecteur en quaternion et retourne le résultat
     *   (les angles d'Euler sont spécifiés dans l'ordre x, y, z)
     */
    static Quat eulerToQuat(const Vector3d& euler)
    {
        //Initialisation du Quaternion de retour
        Quat res;
        res.setZero();

        //Initialisation des var utilises dans le calcul
        double cosX = _CMATH_::cos(euler(0) / 2);
        double sinX = _CMATH_::sin(euler(0) / 2);
        double cosY = _CMATH_::cos(euler(1) / 2);
        double sinY = _CMATH_::sin(euler(1) / 2);
        double cosZ = _CMATH_::cos(euler(2) / 2);
        double sinZ = _CMATH_::sin(euler(2) / 2);

        //Conversion d'angles d'eulers -> Quaternion
        res(0) = (cosX * cosY * cosZ) + (sinX * sinY * sinZ);
        res(1) = (sinX * cosY * cosZ) - (cosX * sinY * sinZ);
        res(2) = (cosX * sinY * cosZ) + (sinX * cosY * sinZ);
        res(3) = (cosX * cosY * sinZ) - (sinX * sinY * cosZ);

        return res;
    }

    /**
    * Calcule le produit vectoriel a x b et retourne le resultat
    */
    template<typename _Scalar>
    Vector<_Scalar, 3> cross( const Vector<_Scalar, 3>& a, const Vector<_Scalar, 3>& b )
    {
        //Initialisation du Vector de retour
        Vector<_Scalar,3> res;
        res.setZero();

        //Calcul du produit vectoriel
        res(0) = (a(1) * b(2)) - (a(2) * b(1));
        res(1) = (a(2) * b(0)) - (a(0) * b(2));
        res(2) = (a(0) * b(1)) - (a(1) * b(0));

        return res;
    }

} // namespace gti320
