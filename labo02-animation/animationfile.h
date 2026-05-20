/**
 * @file animationfile.h
 *
 * @brief GTI320 Labo 2 - Animation
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#ifndef animationfile_h
#define animationfile_h

#include <stdio.h>
#include "BVHFormat.h"
#include "Math3D.h"

class Skeleton;

class AnimationFile
{
public:
    
    AnimationFile();
    ~AnimationFile();
    bool LoadFile( const char * filename );
    void BuildSkeleton( Skeleton & s );
    void AnimateLocalTransforms( Skeleton & s, int baseFrame, int nextFrame, double interval );
    int GetNumberOfBones() { if(m_header) return m_header->noofsegments; return 0; }
    gti320::Vector3d GetBoneTranslation( int boneIndex, int frame );
    gti320::Vector3d GetBoneRotationEuler( int boneIndex, int frame );
    const char * GetError();
    int GetNumberOfFrames() { if(m_header) return m_header->noofframes; return 0; }
    int GetFrameRate() { if(m_header) return m_header->datarate; return 0; }
    
private:

    void CreateBone( Skeleton & s, NODE * n, int boneIndex );
    void Clear();
    
    MOCAPHEADER * m_header = nullptr;
    MocapData * m_data = nullptr;

};


#endif
