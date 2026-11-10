#include <array>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

// Constantes
constexpr double noteMin = 0.0;
constexpr double noteMax = 20.0;
constexpr double seuilValidation = 10.0;
constexpr std::size_t nbMentions = 5;

// Énumération des mentions
enum class Mention { Ajourne, Passable, AssezBien, Bien, TresBien };

// Déterminer la mention
Mention mentionPour(double note) {
  if (note < seuilValidation) {
    return Mention::Ajourne;
  }
  if (note < 12.0) {
    return Mention::Passable;
  }
  if (note < 14.0) {
    return Mention::AssezBien;
  }
  if (note < 16.0) {
    return Mention::Bien;
  }
  return Mention::TresBien;
}

// Nom de la mention
std::string_view libelle(Mention m) {
  switch (m) {
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
  return "Inconnue";
}

// Lire un entier valide
int lireEntier(std::string_view message, int minimum, int maximum) {
  int valeur{};

  while (true) {
    std::cout << message;

    std::string ligne;
    std::getline(std::cin, ligne);

    std::istringstream flux(ligne);
    char reste{};

    if ((flux >> valeur) && !(flux >> reste) && valeur >= minimum &&
        valeur <= maximum) {
      return valeur;
    }

    std::cout << "Valeur invalide. Reessayez.\n";
  }
}

// Lire une note valide
double lireNote(int numero) {
  while (true) {
    std::cout << "Note de l'etudiant " << numero << " (0 a 20) : ";

    std::string ligne;
    std::getline(std::cin, ligne);

    for (char &caractere : ligne) {
      if (caractere == ',') {
        caractere = '.';
      }
    }

    std::istringstream flux(ligne);
    double note{};
    char reste{};

    if ((flux >> note) && !(flux >> reste) && note >= noteMin &&
        note <= noteMax) {
      return note;
    }

    std::cout << "Note invalide. Entrez une note entre 0 et 20.\n";
  }
}

int main() {
  std::string nomUE;

  std::cout << "Nom de l'UE : ";
  std::getline(std::cin, nomUE);

  const int nombreEtudiants =
      lireEntier("Nombre d'etudiants (1 a 200) : ", 1, 200);

  std::vector<double> notes;
  notes.reserve(static_cast<std::size_t>(nombreEtudiants));

  for (int i = 1; i <= nombreEtudiants; ++i) {
    notes.push_back(lireNote(i));
  }

  // Calcul de la note minimale et maximale
  double minimum = notes[0];
  double maximum = notes[0];
  double somme = 0.0;

  int admis = 0;
  // Comptage des mentions
  std::array<int, nbMentions> effectifs{};

  for (double note : notes) {
    const Mention mention = mentionPour(note);
    const std::size_t indice = static_cast<std::size_t>(mention);
    ++effectifs[indice];
  }
  for (double note : notes) {
    if (note < minimum) {
      minimum = note;
    }

    if (note > maximum) {
      maximum = note;
    }

    somme += note;

    if (note >= seuilValidation) {
      ++admis;
    }
  }

  // Calcul de la moyenne
  const double moyenne = somme / static_cast<double>(nombreEtudiants);

  // Calcul de l'ecart-type
  double sommeCarres = 0.0;

  for (double note : notes) {
    const double difference = note - moyenne;
    sommeCarres += difference * difference;
  }

  const double ecartType =
      std::sqrt(sommeCarres / static_cast<double>(nombreEtudiants));

  // Calcul du pourcentage de reussite
  const double pourcentage =
      static_cast<double>(admis) * 100.0 / static_cast<double>(nombreEtudiants);

  // Affichage des resultats
  std::cout << "\n========== RESULTATS ==========\n";
  std::cout << "UE : " << nomUE << '\n';
  std::cout << "Nombre d'etudiants : " << nombreEtudiants << '\n';
  std::cout << "Note minimale : " << minimum << '\n';
  std::cout << "Note maximale : " << maximum << '\n';
  std::cout << "Moyenne : " << moyenne << '\n';
  std::cout << "Ecart-type : " << ecartType << '\n';
  std::cout << "Nombre d'admis : " << admis << '\n';
  std::cout << "Pourcentage de reussite : " << pourcentage << " %\n";
  std::cout << "\n========== HISTOGRAMME DES MENTIONS ==========\n";

  for (std::size_t i = 0; i < nbMentions; ++i) {
    const Mention mention = static_cast<Mention>(i);

    std::cout << libelle(mention) << " : " << effectifs[i] << " ";

    for (int j = 0; j < effectifs[i]; ++j) {
      std::cout << '*';
    }

    std::cout << '\n';
  }

  return 0;
}