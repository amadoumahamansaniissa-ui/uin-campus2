#pragma once

#include <array>
#include <string_view>

namespace uin {

// Constantes des notes
inline constexpr double noteMin = 0.0;
inline constexpr double noteMax = 20.0;
inline constexpr double seuilValidation = 10.0;

// Les mentions
enum class Mention {
    Ajourne,
    Passable,
    AssezBien,
    Bien,
    TresBien
};

// Seuils des mentions
inline constexpr std::array seuilsMentions{
    10.0, 12.0, 14.0, 16.0
};

// Vérification des constantes à la compilation
static_assert(seuilsMentions.size() == 4,
              "Un seuil par mention au-dessus d'Ajourné");

static_assert(seuilsMentions[0] == seuilValidation,
              "La mention Passable commence au seuil de validation");
// Vérifier si une note est valide
constexpr bool estNoteValide(double note)
{
    return note >= noteMin && note <= noteMax;
}

// Déclarations des fonctions
[[nodiscard]] Mention mentionPour(double note);

[[nodiscard]] std::string_view libelle(Mention mention);

} // namespace uin
