#pragma once
#include <string>
#include <string_view>
namespace uin {
/// Lit une ligne de texte (espaces compris).
std::string lireTexte(std::string_view invite);
/// Lit un entier compris entre min et max ; redemande tant que la saisie est
/// invalide.
int lireEntier(std::string_view invite, int min, int max);
/// Lit une note valide ; accepte la virgule décimale.
double lireNote(std::string_view invite);
} // namespace uin