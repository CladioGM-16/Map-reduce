#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

std::map<std::string, int> MapFunction(const std::vector<std::string>& words) {
    std::map<std::string, int> wordCount;
    for (const auto& word : words) {
        wordCount[word]++;
    }
    return wordCount;
}

std::map<std::string, int> ReduceFunction(const std::map<std::string, int>& a, const std::map<std::string, int>& b) {
    std::map<std::string, int> result = a;
    for (const auto& pair : b) {
        result[pair.first] += pair.second;
    }
    return result;
}

int main() {
    int n;
    std::vector<std::string> palabras;

    // Solicitar cantidad de palabras
    std::cout << "Ingrese la cantidad de palabras: ";
    std::cin >> n;

    // Leer palabras
    std::cout << "Ingrese las palabras:\n";
    for (int i = 0; i < n; i++) {
        std::string palabra;
        std::cin >> palabra;
        palabras.push_back(palabra);
    }


    std::map<std::string, int> mapaPalabras = MapFunction(palabras);

    std::map<std::string, int> resultado = std::accumulate(mapaPalabras.begin(), mapaPalabras.end(), std::map<std::string, int>(),
        [](std::map<std::string, int> acc, const std::pair<std::string, int>& word) {
            acc[word.first] += word.second;
            return acc;
        });


    std::cout << "Frecuencia de palabras:\n";
    for (const auto& pair : resultado) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
