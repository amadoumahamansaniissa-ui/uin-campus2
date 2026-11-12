#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

namespace carnet {

// Constantes
inline constexpr double noteMin = 0.0;
inline constexpr double noteMax = 20.0;
inline constexpr double seuilValidation = 10.0;

// Ordre de tri
enum class Ordre {
    Croissant,
    Decroissant
};

// Structure des statistiques
struct Statistiques {
    double minimum;
    double maximum;
    double moyenne;
    std::size_t nombreValides;
};

// Fonctions de lecture
[[nodiscard]] int lireEntier(
    std::string_view message,
    int minimum,
    int maximum
);

[[nodiscard]] double lireNote(int numero);

// Gestion des notes
void ajouterNote(std::vector<double>& notes);
void afficherNotes(const std::vector<double>& notes);
void modifierNote(std::vector<double>& notes);
void supprimerNote(std::vector<double>& notes);

// Tri
void trierNotes(std::vector<double>& notes, Ordre ordre);

// Calculs
[[nodiscard]] Statistiques calculerStatistiques(
    const std::vector<double>& notes
);

void appliquerBonus(std::vector<double>& notes, double bonus);

// Menu
void afficherMenu();
void afficherStatistiques(const std::vector<double>& notes);
void gererTri(std::vector<double>& notes);
void gererBonus(std::vector<double>& notes);
void executerMenu();

} // namespace carnet
