#include "uincampus/regles.hpp"

namespace uin {

Mention mentionPour(double moyenne) {
    int rang{0};

    for (double seuil : seuilsMentions) {
        if (moyenne >= seuil) {
            ++rang;
        }
    }

    return static_cast<Mention>(rang);
}

std::string_view libelle(Mention mention) {
    switch (mention) {
    case Mention::Ajourne:
        return "Ajourné";
    case Mention::Passable:
        return "Passable";
    case Mention::AssezBien:
        return "Assez bien";
    case Mention::Bien:
        return "Bien";
    case Mention::TresBien:
        return "Très bien";
    }

    return "Mention inconnue";
}

} // namespace uin
