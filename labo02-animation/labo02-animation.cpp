/**
 * @file labo02-animation.cpp
 *
 * @brief GTI320 Labo 2 - Animation
 *
 * Nom: Thomas Bégin
 * Code permanent : BEGT09079800
 * Email : thomas.begin.1@ens.etsmtl.ca
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <filesystem>

#include "polyscope/polyscope.h"
#include "skeleton.h"
#include "animationfile.h"
#include "animationtimer.h"
#include "bvhfolder.h"

// paths des fichiers d'animation
namespace fs = std::filesystem;
int currentAnimationIndex = 0;
std::vector<fs::path> animationFilePaths;

// variables globales des objets principaux
AnimationTimer * mainTimer = nullptr;
AnimationFile * animFile = nullptr;
Skeleton * skeleton = nullptr;

bool LoadAnimation( const char * filename )
{
    // Detruire l'animation et le skelette precedents
    if( animFile )
    {
        delete animFile;
        animFile = nullptr;
    }
    if( skeleton )
    {
        delete skeleton;
        skeleton = nullptr;
    }
    if( mainTimer )
    {
        delete mainTimer;
        mainTimer = nullptr;
    }
    
    // Essayer de lire le nouveau fichier d'animation
    animFile = new AnimationFile;
    if( !animFile->LoadFile( filename ) )
    {
        return false;
    }
    
    // Creer et demarrer un timer qui suit la progression de l'animation
    mainTimer = new AnimationTimer( animFile->GetNumberOfFrames(), animFile->GetFrameRate() );
    mainTimer->Start();
    
    polyscope::options::automaticallyComputeSceneExtents = true;
    
    // Construire le squelette a partir du nouveau fichier d'animation
    skeleton = new Skeleton;
    animFile->BuildSkeleton( *skeleton );
    skeleton->UpdateGlobalBoneTransforms();
    
    polyscope::options::automaticallyComputeSceneExtents = false;
    
    return true;
}

void UpdateGUI()
{
    static ImGuiComboFlags flags = 0;
    int prevAnimationIndex = currentAnimationIndex;
    if (ImGui::BeginCombo("animation", animationFilePaths[currentAnimationIndex].filename().string().c_str(), flags)	)
    {
        for( int i = 0; i < animationFilePaths.size(); ++i )
        {
            bool isSelected = (currentAnimationIndex == i);
            if( ImGui::Selectable( animationFilePaths[i].filename().string().c_str(), isSelected ) )
                currentAnimationIndex = i;
            if( isSelected )
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if( prevAnimationIndex != currentAnimationIndex )
    {
        LoadAnimation( animationFilePaths[currentAnimationIndex].string().c_str() );
    }
}

void UpdateAnim()
{
    // Mettre a jour l'interface graphique
    UpdateGUI();
    
    // Compute animation time
    int baseFrame = 0;
    int nextFrame = 1;
    double interval = 0.0;
    mainTimer->GetAnimationTime( baseFrame, nextFrame, interval );
    
    // Interpolate local transforms for every bone
    animFile->AnimateLocalTransforms( *skeleton, baseFrame, nextFrame, interval );
    
    // Update global transform for every bone
    skeleton->UpdateGlobalBoneTransforms();
}

int main(int argc, char** argv)
{
    std::string bvhFolder = BVH_FOLDER;
    
    // S'il y a un argument sur la ligne de commande, on assume que c'est
    // pour specifier une autre repertoire pour les fichiers d'animation
    if( argc > 1 )
        bvhFolder = argv[1];
    
    // Verifier que le repertoire d'animation existe
    fs::path dirPath(bvhFolder);
    if( !exists(dirPath) )
    {
        std::cerr << "Le repertoire d'animation n'existe pas." << std::endl;
        exit(1);
    }
    
    // Lire le nom des fichiers d'animation
    for( const auto & entry : fs::directory_iterator(dirPath) )
    {
        if( entry.path().extension() == ".bvh" )
            animationFilePaths.push_back( entry.path() );
    }
    if( animationFilePaths.size() == 0 )
    {
        std::cerr << "Le repertoire d'animation ne contient aucun fichier valide." << std::endl;
        exit(1);
    }
    
    // Initialiser Polyscope et choisir les options par defaut
    polyscope::options::programName = "Labo02 - Animation";
    polyscope::options::giveFocusOnShow = true;
    polyscope::options::alwaysRedraw = true;
    polyscope::view::upDir = polyscope::UpDir::ZUp;
    polyscope::init();
    polyscope::state::userCallback = UpdateAnim;
    
    // Lire le fichier d'animation et construire le squelette
    if( !LoadAnimation( animationFilePaths[0].string().c_str() ) )
    {
        std::cout << "Le fichier d'animation " << animationFilePaths[0].c_str() << " n'a pas pu etre lu: " << animFile->GetError() << std::endl;
        exit(1);
    }
    
    // Demarrer la boucle principale
    polyscope::show();
    
    // Relacher la memoire utiliser par les principaux objets
    delete mainTimer;
    if( animFile )
        delete animFile;
    if( skeleton )
        delete skeleton;

    return 0;
}
