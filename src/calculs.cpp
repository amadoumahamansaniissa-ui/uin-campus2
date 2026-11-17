#include "uincampus/calculs.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>

namespace uin {

double somme(std::span<const double> valeurs) {
    return std::accumulate(valeurs.begin(), valeurs.end(), 0.0);
}

double moyenne(std::span<const double> notes) {
    assert(!notes.empty());

    return somme(notes) / static_cast<double>(notes.size());
}

double moyenne(
    std::span<const double> notes,
    std::span<const double> coefficients
) {
    assert(!notes.empty());
    assert(notes.size() == coefficients.size());

    double totalPondere{0.0};
    double totalCoefficients{0.0};

    for (std::size_t i{0}; i < notes.size(); ++i) {
        totalPondere += notes[i] * coefficients[i];
        totalCoefficients += coefficients[i];
    }

    assert(totalCoefficients != 0.0);

    return totalPondere / totalCoefficients;
}

Statistiques calculerStatistiques(std::span<const double> notes) {
    assert(!notes.empty());

    const auto [minimum, maximum] = std::ranges::minmax(notes);
    const double moy = moyenne(notes);

    double sommeCarres{0.0};

    for (double note : notes) {
        const double ecart = note - moy;
        sommeCarres += ecart * ecart;
    }

    const double variance =
        sommeCarres / static_cast<double>(notes.size());

    const double ecartType = std::sqrt(variance);

    return {minimum, maximum, moy, ecartType};
}

int compterValidees(
    std::span<const double> notes,
    double seuil
) {
    return static_cast<int>(
        std::ranges::count_if(notes, [seuil](double note) {
            return note >= seuil;
        })
    );
}

double arrondir(double valeur, int decimales) {
    const double facteur = std::pow(10.0, decimales);

    return std::round(valeur * facteur) / facteur;
}

} // namespace uin
