/**
 * @file animationfile.cpp
 *
 * @brief GTI320 Labo 2 - Animation
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#include "animationfile.h"
#include "skeleton.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>


using namespace gti320;

AnimationFile::AnimationFile()
{
}

AnimationFile::~AnimationFile()
{
    Clear();
}

bool AnimationFile::LoadFile( const char * filename )
{
    Clear();
    
    m_header = new MOCAPHEADER;
    m_data = new BVHFormat(m_header);
    if(!m_data->ImportData(filename))
        return false;
    return true;
}

void AnimationFile::CreateBone( Skeleton & s, NODE * n, int parentIndex )
{
    // Create bone geometry
    Vector3d start; start(0) = 0.0; start(1) = 0.0; start(2) = 0.0;
    Vector3d end; end(0) = n->length[0]; end(1) = n->length[1]; end(2) = n->length[2];
    Vector3d offsetTrans; offsetTrans(0) = n->offset[0]; offsetTrans(1) = n->offset[1]; offsetTrans(2) = n->offset[2];
    Quat offsetRot; offsetRot(0) = 1.0; offsetRot(1) = 0.0; offsetRot(2) = 0.0; offsetRot(3) = 0.0;  // TODO: Convert Euler offset to Quat
    int boneIndex = s.AddBone( n->name, parentIndex, start, end, offsetTrans, offsetRot );
    
    // Process all children bones
    for( int i = 0; i < n->noofchildren; ++i )
        CreateBone( s, n->children[i], boneIndex );
}

void AnimationFile::BuildSkeleton( Skeleton & s )
{
    NODE * root = m_data->GetRootNode();
    CreateBone( s, root, -1 );
}

/**
* Pour chaque os du squelette, cette fonction doit récupérer la rotation et la translation
* des frames adjacents dans l’animation, les interpoler pour le temps courant et stocker
* le vecteur et le quaternion qui représentent cette transformation dans l’os correspondant
* dans la classe Skeleton. Utilisez les fonction GetBoneTranslation et GetBoneRotationEuler
* pour récupérer les information d'animation
*/
void AnimationFile::AnimateLocalTransforms( Skeleton & s, int baseFrame, int nextFrame, double interval )
{
    //Pour chaque os
    for( int i = 0; i < GetNumberOfBones(); ++i )
    {   
        //Get vector de translation des frame adjacents
        Vector3d transBase = GetBoneTranslation(i, baseFrame);
        Vector3d transNext = GetBoneTranslation(i, nextFrame);

        //Get vector de rotation (euler) des frames adjecents
        Vector3d rotEulBase = GetBoneRotationEuler(i, baseFrame);
        Vector3d rotEulNext = GetBoneRotationEuler(i, nextFrame);

        //Transformation des rotation Euler en quaternion pour l'interpolation
        Quat rotBase = eulerToQuat(rotEulBase);
        Quat rotNext = eulerToQuat(rotEulNext);

        //Appel des methode d'interpolation de Math3D
        Vector3d trans = lerp(transBase, transNext, interval);
        Quat rot = slerp(rotBase, rotNext, interval);
    
        // Assigner rotation et translation a l'os du squelette
        s.SetBoneLocalTransform( i , trans, rot );
    }
}

/**
*  Recuperer le vecteur de translation de l'os boneIndex au frame indique
*/
Vector3d AnimationFile::GetBoneTranslation( int boneIndex, int frame )
{
    NODE * n = m_data->GetNodeList()[boneIndex];
    float * transPtr = n->froset[frame];
    Vector3d trans;
    trans(0) = transPtr[0];
    trans(1) = transPtr[1];
    trans(2) = transPtr[2];
    return trans;
}

static double DegreesToRadian( double degrees )
{
    double rad = M_PI * degrees / 180.0;
    return rad;
}

/**
*  Recuperer le vecteur des angles d'Euler de l'os boneIndex au frame indique
*/
Vector3d AnimationFile::GetBoneRotationEuler( int boneIndex, int frame )
{
    NODE * n = m_data->GetNodeList()[boneIndex];
    float * eulerPtr = n->freuler[frame];
    Vector3d euler;
    if( m_header->degrees )
    {
        euler(0) = DegreesToRadian( static_cast<double>(eulerPtr[0]) );
        euler(1) = DegreesToRadian( static_cast<double>(eulerPtr[1]) );
        euler(2) = DegreesToRadian( static_cast<double>(eulerPtr[2]) );
    }
    else
    {
        euler(0) = eulerPtr[0];
        euler(1) = eulerPtr[1];
        euler(2) = eulerPtr[2];
    }
    return euler;
}

const char * AnimationFile::GetError()
{
    if( m_data )
        return m_data->GetError();
    return "";
}

void AnimationFile::Clear()
{
    delete m_data;
    m_data = nullptr;
    delete m_header;
    m_header = nullptr;
}

