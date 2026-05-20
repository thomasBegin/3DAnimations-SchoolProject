/**
 * @file TestsEtudiants.cpp
 *
 * @brief Tests unitaires faits par l'étudiant pour la librairie d'algèbre linéaire.
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
// ----- Tests supplémentaires évalués, à compléter! ---------------------------------------------------------
// ===========================================================================================================
#pragma region // Tests évalués

/**
 * TODO:
 * Écrivez trois tests supplémentaires pertinents, complets et bien commentés.
 * Seuls les trois tests de cette section seront évalués.
 *
 * Vous pouvez changer leurs noms, mais pas celui de la suite de tests.
 * Les tests évalués doivent être dans "TestsTP1_Evalue".
 *
 * Attention :
 * Le non-respect de ces consignes entraînera une note de zéro pour les tests.
 *
 * Documentation :
 * https://google.github.io/googletest/primer.html
 */

TEST(TestsTP1_Evalue, TestEvalue1)
{
    // TODO remplacez le code avec votre propre tests ici.
    EXPECT_TRUE(false);
}

TEST(TestsTP1_Evalue, TestEvalue2)
{
    // TODO remplacez le code avec votre propre tests ici.
    EXPECT_TRUE(false);
}

TEST(TestsTP1_Evalue, TestEvalue3)
{
    // TODO remplacez le code avec votre propre tests ici.
    EXPECT_TRUE(false);
}

#pragma endregion // Tests évalués

// ===========================================================================================================
// ----- Tests supplémentaires (Facultatif) -----------------------------------------------------------------
// ===========================================================================================================
#pragma region // Tests supplémentaires

/**
 * TODO: (Facultatif)
 * Vous pouvez utiliser cette section pour créer des tests supplémentaires.
 * Aucun de ces tests ne seront évalués.
 *
 * Vous pouvez changer leurs noms, mais pas celui de la suite de tests.
 * Les tests supplémentaires doivent être dans "TestLabo1_Etudiant".
 *
 * Documentation :
 * https://google.github.io/googletest/primer.html
 */

TEST(TestsTP1_Etudiant, Supplementaire1)
{
    EXPECT_TRUE(false);
}

#pragma endregion // Tests Supplémentaires
