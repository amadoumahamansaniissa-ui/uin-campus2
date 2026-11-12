#include "uincampus/carnet.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

namespace carnet {

namespace {

// Lecture sécurisée d'un nombre décimal
[[nodiscard]] double lireDecimal(
    std::string_view message,
    double minimum,
    double maximum
) {
    while (true) {
        std::cout << message;

        std::string ligne;
        if (!std::getline(std::cin, ligne)) {
            return minimum;
        }

        // Accepter la virgule comme séparateur décimal
        std::replace(ligne.begin(), ligne.end(), ',', '.');

        std::istringstream flux(ligne);
        double valeur{};
        char reste{};

        if (flux >> valeur &&
            !(flux >> reste) &&
            std::isfinite(valeur) &&
            valeur >= minimum &&
            valeur <= maximum) {
            return valeur;
        }

        std::cout << "Valeur invalide. Recommencez.\n";
    }
}

} // namespace

// Lecture d'un entier dans un intervalle
int lireEntier(
    std::string_view message,
    int minimum,
    int maximum
) {
    while (true) {
        std::cout << message;

        std::string ligne;
        if (!std::getline(std::cin, ligne)) {
            return minimum;
        }

        std::istringstream flux(ligne);
        int valeur{};
        char reste{};

        if (flux >> valeur &&
            !(flux >> reste) &&
            valeur >= minimum &&
            valeur <= maximum) {
            return valeur;
        }

        std::cout << "Choix invalide. Recommencez.\n";
    }
}

// Lecture d'une note entre 0 et 20
double lireNote(int numero) {
    const std::string message =
        "Saisir la note numero " + std::to_string(numero) + " (0 a 20) : ";

    return lireDecimal(message, noteMin, noteMax);
}

// Ajouter une note
void ajouterNote(std::vector<double>& notes) {
    const double note = lireNote(
        static_cast<int>(notes.size()) + 1
    );

    notes.push_back(note);
    std::cout << "Note ajoutee avec succes.\n";
}

// Afficher toutes les notes
void afficherNotes(const std::vector<double>& notes) {
    if (notes.empty()) {
        std::cout << "Aucune note enregistree.\n";
        return;
    }

    std::cout << "\n--- LISTE DES NOTES ---\n";

    for (std::size_t i = 0; i < notes.size(); ++i) {
        std::cout << i + 1 << ". " << notes[i] << "/20\n";
    }
}

// Modifier une note
void modifierNote(std::vector<double>& notes) {
    if (notes.empty()) {
        std::cout << "Aucune note a modifier.\n";
        return;
    }

    afficherNotes(notes);

    const int position = lireEntier(
        "Numero de la note a modifier : ",
        1,
        static_cast<int>(notes.size())
    );

    notes[static_cast<std::size_t>(position - 1)] = lireNote(position);

    std::cout << "Note modifiee avec succes.\n";
}

// Supprimer une note
void supprimerNote(std::vector<double>& notes) {
    if (notes.empty()) {
        std::cout << "Aucune note a supprimer.\n";
        return;
    }

    afficherNotes(notes);

    const int position = lireEntier(
        "Numero de la note a supprimer : ",
        1,
        static_cast<int>(notes.size())
    );

    notes.erase(notes.begin() + (position - 1));

    std::cout << "Note supprimee avec succes.\n";
}

// Trier les notes
void trierNotes(std::vector<double>& notes, Ordre ordre) {
    if (ordre == Ordre::Croissant) {
        std::ranges::sort(notes, [](double a, double b) {
            return a < b;
        });
    } else {
        std::ranges::sort(notes, [](double a, double b) {
            return a > b;
        });
    }
}

// Calculer les statistiques sans afficher de texte
Statistiques calculerStatistiques(
    const std::vector<double>& notes
) {
    if (notes.empty()) {
        return {0.0, 0.0, 0.0, 0};
    }

    const auto [minimum, maximum] =
        std::ranges::minmax(notes);

    const double somme =
        std::accumulate(notes.begin(), notes.end(), 0.0);

    const double moyenne =
        somme / static_cast<double>(notes.size());

    const auto nombreValides = static_cast<std::size_t>(
        std::ranges::count_if(notes, [](double note) {
            return note >= seuilValidation;
        })
    );

    return {minimum, maximum, moyenne, nombreValides};
}

// Appliquer un bonus sans effectuer d'affichage
void appliquerBonus(std::vector<double>& notes, double bonus) {
    std::ranges::for_each(notes, [bonus](double& note) {
        note = std::clamp(note + bonus, noteMin, noteMax);
    });
}

// Afficher le menu principal
void afficherMenu() {
    std::cout << "\n====== CARNET DE NOTES ======\n"
              << "1. Ajouter une note\n"
              << "2. Afficher les notes\n"
              << "3. Modifier une note\n"
              << "4. Supprimer une note\n"
              << "5. Trier les notes\n"
              << "6. Afficher les statistiques\n"
              << "7. Appliquer un bonus\n"
              << "0. Quitter\n"
              << "============================\n";
}

// Afficher les statistiques avec une liaison structuree
void afficherStatistiques(const std::vector<double>& notes) {
    if (notes.empty()) {
        std::cout << "Aucune note disponible pour les statistiques.\n";
        return;
    }

    const auto [minimum, maximum, moyenne, nombreValides] =
        calculerStatistiques(notes);

    std::cout << "\n--- STATISTIQUES ---\n"
              << "Note minimale : " << minimum << '\n'
              << "Note maximale : " << maximum << '\n'
              << "Moyenne : " << moyenne << "/20\n"
              << "Nombre de notes validees : "
              << nombreValides << '\n';
}

// Gérer le choix du tri
void gererTri(std::vector<double>& notes) {
    if (notes.empty()) {
        std::cout << "Aucune note a trier.\n";
        return;
    }

    std::cout << "\n1. Ordre croissant\n"
              << "2. Ordre decroissant\n";

    const int choix = lireEntier("Votre choix : ", 1, 2);

    const Ordre ordre = (choix == 1)
        ? Ordre::Croissant
        : Ordre::Decroissant;

    trierNotes(notes, ordre);
    std::cout << "Notes triees avec succes.\n";
    afficherNotes(notes);
}

// Gérer l'application d'un bonus
void gererBonus(std::vector<double>& notes) {
    if (notes.empty()) {
        std::cout << "Aucune note pour appliquer un bonus.\n";
        return;
    }

    const double bonus = lireDecimal(
        "Montant du bonus (0 a 20) : ",
        0.0,
        20.0
    );

    appliquerBonus(notes, bonus);

    std::cout << "Bonus applique. Les notes ne depassent pas 20/20.\n";
}

// Exécuter le menu principal
void executerMenu() {
    std::vector<double> notes;
    int choix{};

    do {
        afficherMenu();
        choix = lireEntier("Votre choix : ", 0, 7);

        switch (choix) {
            case 1:
                ajouterNote(notes);
                break;

            case 2:
                afficherNotes(notes);
                break;

            case 3:
                modifierNote(notes);
                break;

            case 4:
                supprimerNote(notes);
                break;

            case 5:
                gererTri(notes);
                break;

            case 6:
                afficherStatistiques(notes);
                break;

            case 7:
                gererBonus(notes);
                break;

            case 0:
                std::cout << "Fermeture du carnet de notes. Au revoir !\n";
                break;
        }

    } while (choix != 0);
}

} // namespace carnet
