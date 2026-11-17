#include "uincampus/calculs.hpp"
#include "uincampus/carnet.hpp"
#include "uincampus/regles.hpp"
#include "uincampus/saisie.hpp"
#include "uincampus/version.hpp"

#include <cstddef>
#include <cstdlib>
#include <string_view>
#include <iostream>
#include <print>
#include <span>
#include <string>
#include <vector>

namespace {

enum class Suite {
    Continuer,
    Terminer,
    Erreur,
    Carnet
};

void afficherAide() {
    std::println("UIN-Campus - Système de gestion académique");
    std::println("Utilisation : uin-campus [option]");
    std::println("");
    std::println("Options disponibles :");
    std::println("  --version  Afficher la version");
    std::println("  --aide     Afficher cette aide");
    std::println("  --carnet   Ouvrir le carnet de notes TP4");
}

Suite traiterOptions(std::span<char*> arguments) {
    if (arguments.size() <= 1) {
        return Suite::Continuer;
    }

    const auto options = arguments.subspan(1);
    const std::string_view option{options.front()};

    if (option == "--version") {
        std::println("Version {}", uin::version());
        return Suite::Terminer;
    }

    if (option == "--aide") {
        afficherAide();
        return Suite::Terminer;
    }

    if (option == "--carnet") {
        return Suite::Carnet;
    }

    std::cerr << "Option inconnue : " << option << '\n';
    std::cerr << "Utilisez --aide pour afficher les options.\n";

    return Suite::Erreur;
}

} // namespace

int main(int argc, char* argv[]) {
    const Suite suite = traiterOptions(
        std::span{argv, static_cast<std::size_t>(argc)}
    );

    if (suite == Suite::Terminer) {
        return EXIT_SUCCESS;
    }

    if (suite == Suite::Erreur) {
        return EXIT_FAILURE;
    }

    if (suite == Suite::Carnet) {
        carnet::executerMenu();
        return EXIT_SUCCESS;
    }

    std::println("===== GESTION DES NOTES UNIVERSITAIRES =====");

    const std::string nom = uin::lireTexte("Nom de l'etudiant : ");

    const int nombreUE =
        uin::lireEntier("Nombre d'UE : ", 1, 20);

    std::vector<double> notes;
    std::vector<double> coefficients;

    notes.reserve(static_cast<std::size_t>(nombreUE));
    coefficients.reserve(static_cast<std::size_t>(nombreUE));

    for (int i = 0; i < nombreUE; ++i) {
        std::println("\nUE {}", i + 1);

        const double note =
            uin::lireNote("Note sur 20 : ");

        const int coefficient =
            uin::lireEntier("Coefficient : ", 1, 10);

        notes.push_back(note);
        coefficients.push_back(static_cast<double>(coefficient));
    }

    const double moyenne =
        uin::moyenne(notes, coefficients);

    const uin::Statistiques statistiques =
        uin::calculerStatistiques(notes);

    const int validees =
        uin::compterValidees(notes);

    const uin::Mention mention =
        uin::mentionPour(moyenne);

    std::println("\n========== RESULTATS ==========");
    std::println("Etudiant : {}", nom);
    std::println("Nombre d'UE : {}", nombreUE);
    std::println("Moyenne generale : {:.2f}/20", moyenne);
    std::println("Note minimale : {:.2f}", statistiques.minimum);
    std::println("Note maximale : {:.2f}", statistiques.maximum);
    std::println("Ecart-type : {:.2f}", statistiques.ecartType);
    std::println("UE validees : {}", validees);
    std::println("Mention : {}", uin::libelle(mention));
    std::println("===============================");

    return EXIT_SUCCESS;
}
