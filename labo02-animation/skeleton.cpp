/**
 * @file skeleton.cpp
 *
 * @brief GTI320 Labo 2 - Animation
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#include "skeleton.h"
#include "Math3D.h"
#include <array>
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"


using namespace gti320;

Bone::Bone( const char * name, int parentIndex, const Vector3d & start, const Vector3d & end, const Vector3d & offsetTrans, const Quat & offsetRot ) :
m_name(name),
m_hasGeometry(true),
m_parentIndex( parentIndex ),
m_offsetTrans( offsetTrans ),
m_offsetRot( offsetRot )
{
    m_animTrans(0) = 0.0; m_animTrans(1) = 0.0; m_animTrans(2) = 0.0;
    m_animRot(0) = 1.0; m_animRot(1) = 0.0; m_animRot(2) = 0.0; m_animRot(3) = 0.0;
    CreateBoneGeometry( start, end );
}

Bone::~Bone()
{
    if( m_hasGeometry )
        polyscope::removeSurfaceMesh( m_name, false );
}

/**
* Utiliser les information de transformation locale (offset et animation) contenue dans la classe
* et les combiner à la transformation du parent pour obtenir une matrice de transformation globale pour l’os.
*/
Matrix4d Bone::UpdateGlobalTransform( const Matrix4d & parentTransform )
{
    Matrix4d globalTransform;
    globalTransform.setIdentity();
    
    //Cree matrix de transformation pour l'anim et le offset
    Matrix4d animTransform = makeRigid(m_animTrans, m_animRot);
    Matrix4d offsetTransform = makeRigid(m_offsetTrans, m_offsetRot);
    
    //Calcul la matrix de transformation globale
    globalTransform = parentTransform * offsetTransform * animTransform;
    
    SetGlobalBoneMatrix( globalTransform );
    return globalTransform;
}

static const double lengthEpsilon = 0.001;

void Bone::CreateBoneGeometry( const Vector3d & start, const Vector3d & end )
{
    // Define radius of the bone (r) and height of the intermediate vertices (h)
    // ( r and h are fractions of the radius and height defined by start and end )
    static const double r = 0.1;
    static const double h = 0.2;
    
    Vector3d dir = end - start;
    double l = dir.norm();
    if( l < lengthEpsilon )
    {
        m_hasGeometry = false;
        return;
    }
    dir.normalize();
    Vector3d x; x(0) = 1.0; x(1) = 0.0; x(2) = 0.0;
    if( std::abs( dir(0) ) > std::abs(dir(1)) && std::abs(dir(0)) > std::abs(dir(2)) )  // si la direction du bone est principalement vers x, on utilise y comme vecteur arbitraire
    {
        x(0) = 0.0;
        x(1) = 1.0;
    }
    Vector3d rx = cross(dir,x);
    rx.normalize();
    Vector3d rp = cross(dir,rx);
    Vector3d pm = start + h * l * dir;
    Vector3d p1 = pm + r * l * rx;
    Vector3d p2 = pm + r * l * rp;
    Vector3d p3 = pm - r * l * rx;
    Vector3d p4 = pm - r * l * rp;
    
    // Create vertices array
    std::vector< std::array<double,3> > vertices;
    vertices.push_back(std::array<double,3>{start(0),start(1),start(2)});
    vertices.push_back(std::array<double,3>{p1(0),p1(1),p1(2)});
    vertices.push_back(std::array<double,3>{p2(0),p2(1),p2(2)});
    vertices.push_back(std::array<double,3>{p3(0),p3(1),p3(2)});
    vertices.push_back(std::array<double,3>{p4(0),p4(1),p4(2)});
    vertices.push_back(std::array<double,3>{end(0),end(1),end(2)});
    
    // Create face array
    std::vector< std::array<int,3> > faces{
        {0,2,1},
        {0,3,2},
        {0,4,3},
        {0,1,4},
        {1,2,5},
        {2,3,5},
        {3,4,5},
        {4,1,5}};
    
    // Create mesh
    polyscope::registerSurfaceMesh( m_name, vertices, faces);
}

void Bone::SetGlobalBoneMatrix( const Matrix4d & mat )
{
    if( m_hasGeometry )
    {
        glm::mat4 glmMat(mat(0,0), mat(1,0), mat(2,0), mat(3,0),   // col 0
                         mat(0,1), mat(1,1), mat(2,1), mat(3,1),   // col 1
                         mat(0,2), mat(1,2), mat(2,2), mat(3,2),   // col 2
                         mat(0,3), mat(1,3), mat(2,3), mat(3,3));  // col 3
        polyscope::getSurfaceMesh( m_name )->setTransform( glmMat );
    }
}

Skeleton::~Skeleton()
{
    for( Bone * b : m_bones )
        delete b;
}

void Skeleton::RecursiveUpdateBoneTransforms( const Matrix4d & parentTransform, int boneIndex )
{
    Matrix4d globalTransform = m_bones[boneIndex]->UpdateGlobalTransform( parentTransform );
    int nbChildren = m_bones[boneIndex]->GetNumberOfChildren();
    for( int i = 0; i < nbChildren; ++i )
    {
        RecursiveUpdateBoneTransforms( globalTransform, m_bones[boneIndex]->GetChildBoneIndex(i) );
    }
}

void Skeleton::UpdateGlobalBoneTransforms()
{
    // Don't try to update if there are no bones
    if(m_bones.size() == 0)
        return;
    
    // recursively update all bone matrices
    Matrix4d globalMat;
    globalMat.setIdentity();
    
    RecursiveUpdateBoneTransforms( globalMat, 0 );
    
}

void Skeleton::SetBoneLocalTransform( int boneIndex, const Vector3d & translation, const Quat & rotation )
{
    m_bones[boneIndex]->SetLocalTransform( translation, rotation );
}

int Skeleton::AddBone( const char * name, int parentIndex, const Vector3d & start, const Vector3d & end, const Vector3d & offsetTrans, const Quat & offsetRot )
{
    assert( parentIndex == -1 || ( parentIndex >= 0 && parentIndex < m_bones.size() ) );
    Bone * bone = new Bone( name, parentIndex, start, end, offsetTrans, offsetRot );
    int boneIndex = m_bones.size();
    m_bones.push_back( bone );
    if( parentIndex != -1 )
        m_bones[parentIndex]->AddChild(boneIndex);
    return boneIndex;
}
