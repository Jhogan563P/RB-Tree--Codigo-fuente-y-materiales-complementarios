#include <chrono>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <random>
#include "AVL.h"
#include "Splay.h" // tu implementación de splay_tree_implementation

using namespace std;
using namespace std::chrono;

int main() {
    vector<int> Ns = {1000, 10000, 100000, 1000000};

    // Archivo CSV de salida
    ofstream csv("benchmark_results.csv");
    csv << "N,Structure,Operation,Time_microseconds\n";

    mt19937 rng(42); // semilla fija para reproducibilidad
    uniform_int_distribution<int> dist(1, 1e7);

    for (int N : Ns) {
        cout << "\n===== Benchmark N = " << N << " =====\n";

        vector<int> data;
        data.reserve(N);
        for (int i = 0; i < N; ++i) data.push_back(dist(rng));

        // ======== std::set ========
        {
            set<int> st;

            // Insert
            auto start = high_resolution_clock::now();
            for (int v : data) st.insert(v);
            auto end = high_resolution_clock::now();
            double t_insert = duration_cast<duration<double, std::micro>>(end - start).count();
            csv << N << ",std::set,insert," << t_insert << "\n";

            // Search (evita optimización)
            volatile int dummy = 0;
            start = high_resolution_clock::now();
            for (int v : data) {
                auto it = st.find(v);
                if (it != st.end()) dummy += *it;
            }
            end = high_resolution_clock::now();
            double t_search = duration_cast<duration<double, std::micro>>(end - start).count();
            csv << N << ",std::set,search," << t_search << "\n";

            // Erase
            start = high_resolution_clock::now();
            for (int v : data) st.erase(v);
            end = high_resolution_clock::now();
            double t_erase = duration_cast<duration<double, std::micro>>(end - start).count();
            csv << N << ",std::set,erase," << t_erase << "\n";

            cout << "std::set → insert:" << t_insert
                 << " search:" << t_search
                 << " erase:" << t_erase << " µs\n";
        }


        // ======== AVL ========
        {
            AVL<int> avl;

            auto start = high_resolution_clock::now();
            for (int v : data) avl.insert(v);
            auto end = high_resolution_clock::now();
            double t_insert = duration_cast<duration<double, std::micro>>(end - start).count();
            csv << N << ",AVL,insert," << t_insert << "\n";

            start = high_resolution_clock::now();
            volatile int dummy2 = 0;
            for (int v : data) {
                dummy2 += avl.find(v) ? 1 : 0;
            }
            end = high_resolution_clock::now();
            double t_search = duration_cast<duration<double, std::micro>>(end - start).count();
            csv << N << ",AVL,search," << t_search << "\n";

            start = high_resolution_clock::now();
            for (int v : data) avl.remove(v);
            end = high_resolution_clock::now();
            double t_erase = duration_cast<duration<double, std::micro>>(end - start).count();
            csv << N << ",AVL,erase," << t_erase << "\n";

            cout << "AVL → insert:" << t_insert
                 << " search:" << t_search
                 << " erase:" << t_erase << " µs\n";
        }

        // ======== Splay ========
        {
            splay_tree_implementation splay;

            auto start = high_resolution_clock::now();
            for (int v : data) splay.insert(v);
            auto end = high_resolution_clock::now();
            double t_insert = duration_cast<duration<double, std::micro>>(end - start).count();
            csv << N << ",Splay,insert," << t_insert << "\n";

            start = high_resolution_clock::now();
            for (int v : data) splay.find(v);
            end = high_resolution_clock::now();
            double t_search = duration_cast<duration<double, std::micro>>(end - start).count();
            csv << N << ",Splay,search," << t_search << "\n";

            start = high_resolution_clock::now();
            for (int v : data) splay.remove(v);
            end = high_resolution_clock::now();
            double t_erase = duration_cast<duration<double, std::micro>>(end - start).count();
            csv << N << ",Splay,erase," << t_erase << "\n";

            cout << "Splay → insert:" << t_insert
                 << " search:" << t_search
                 << " erase:" << t_erase << " µs\n";
        }
    }

    csv.close();
    cout << "\n✅ Resultados guardados en 'benchmark_results.csv'\n";
    return 0;
}
