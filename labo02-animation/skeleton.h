/**
 * @file skeleton.h
 *
 * @brief GTI320 Labo 2 - Animation
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#ifndef skeleton_h
#define skeleton_h

#include <vector>
#include <string>
#include "Math3D.h"

class Bone
{
public:
    Bone( const char * name, int parentIndex,
         const gti320::Vector3d & start,
         const gti320::Vector3d & end,
         const gti320::Vector3d & offsetTrans,
         const gti320::Quat & offsetRot );
    ~Bone();
    void AddChild( int childIndex ) { m_childrenIndices.push_back(childIndex); }
    int GetNumberOfChildren() { return m_childrenIndices.size(); }
    int GetChildBoneIndex( int childIndex ) { return m_childrenIndices[childIndex]; }
    gti320::Matrix4d UpdateGlobalTransform( const gti320::Matrix4d & parentTransform );
    void SetLocalTransform( const gti320::Vector3d & translation, const gti320::Quat & rotation ) { m_animTrans = translation; m_animRot = rotation; }
    
private:
    void CreateBoneGeometry( const gti320::Vector3d & start, const gti320::Vector3d & end);
    void SetGlobalBoneMatrix( const gti320::Matrix4d & mat );
    
    std::string m_name;
    bool m_hasGeometry;
    int m_parentIndex;
    std::vector<int> m_childrenIndices;
    
    gti320::Vector3d m_offsetTrans;
    gti320::Quat m_offsetRot;
    gti320::Vector3d m_animTrans;
    gti320::Quat m_animRot;
};

class Skeleton
{
public:
    
    Skeleton() {}
    ~Skeleton();
    int AddBone( const char * name, int parentIndex,
                  const gti320::Vector3d & start,
                  const gti320::Vector3d & end,
                  const gti320::Vector3d & offsetTrans,
                  const gti320::Quat & offsetRot );
    void UpdateGlobalBoneTransforms();
    void SetBoneLocalTransform( int boneIndex, const gti320::Vector3d & translation, const gti320::Quat & rotation );

private:
    
    void RecursiveUpdateBoneTransforms( const gti320::Matrix4d & parentTransform, int boneIndex );
    
    std::vector<Bone*> m_bones;
    
};

#endif
