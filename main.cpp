#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <numeric>

std::mutex mtx;

void MapFunction(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end, std::map<std::string, int>& localMap) {
    for (auto it = start; it != end; ++it) {
        localMap[*it]++;
    }
}

void ReduceFunction(std::map<std::string, int>& globalMap, const std::map<std::string, int>& localMap) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& pair : localMap) {
        globalMap[pair.first] += pair.second;
    }
}

int main() {
    int n;
    std::vector<std::string> palabras;
    std::cout << "Ingrese la cantidad de palabras: ";
    std::cin >> n;


    std::cout << "Ingrese las palabras:\n";
    for (int i = 0; i < n; i++) {
        std::string palabra;
        std::cin >> palabra;
        palabras.push_back(palabra);
    }


    int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 2; // En caso de que no pueda detectar los núcleos


    std::vector<std::thread> threads;
    std::vector<std::map<std::string, int>> localMaps(num_threads);
    int blockSize = palabras.size() / num_threads;

    for (int i = 0; i < num_threads; i++) {
        auto start = palabras.begin() + i * blockSize;
        auto end = (i == num_threads - 1) ? palabras.end() : start + blockSize;
        threads.push_back(std::thread(MapFunction, start, end, std::ref(localMaps[i])));
    }


    for (auto& thread : threads) {
        thread.join();
    }


    std::map<std::string, int> globalMap;
    for (const auto& localMap : localMaps) {
        ReduceFunction(globalMap, localMap);
    }


    std::cout << "------Frecuencia de palabras----\n";
    for (const auto& pair : globalMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
