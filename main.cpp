#include <chrono>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <random>
#include <numeric>
#include "AVL.h"
#include "Splay.h"
#include "RB_tree.h"

using namespace std;
using namespace std::chrono;

int main() {
    const int NUM_ITERATIONS = 5;
    vector<int> Ns = {10, 100, 1000, 10000, 100000, 1000000};

    // Archivo CSV de salida
    ofstream csv("benchmark_results.csv");
    csv << "N,Structure,Operation,Time_microseconds\n";

    mt19937 rng(42); // Semilla fija para reproducibilidad
    uniform_int_distribution<int> dist(1, 1e7);

    for (int N : Ns) {
        cout << "\n===== Benchmark N = " << N << " =====\n";


        // ======== AVL ========
        {
            vector<double> times_insert, times_search, times_erase;

            for (int iter = 0; iter < NUM_ITERATIONS; ++iter) {
                vector<int> data;
                data.reserve(N);
                for (int i = 0; i < N; ++i) data.push_back(dist(rng));

                AVL<int> avl;
                auto start = high_resolution_clock::now();
                for (int v : data) avl.insert(v);
                auto end = high_resolution_clock::now();
                times_insert.push_back(duration_cast<duration<double, micro>>(end - start).count());

                volatile int dummy2 = 0;
                start = high_resolution_clock::now();
                for (int v : data) dummy2 += avl.find(v) ? 1 : 0;
                end = high_resolution_clock::now();
                times_search.push_back(duration_cast<duration<double, micro>>(end - start).count());

                start = high_resolution_clock::now();
                for (int v : data) avl.remove(v);
                end = high_resolution_clock::now();
                times_erase.push_back(duration_cast<duration<double, micro>>(end - start).count());
            }

            double avg_insert = accumulate(times_insert.begin(), times_insert.end(), 0.0) / NUM_ITERATIONS;
            double avg_search = accumulate(times_search.begin(), times_search.end(), 0.0) / NUM_ITERATIONS;
            double avg_erase = accumulate(times_erase.begin(), times_erase.end(), 0.0) / NUM_ITERATIONS;

            csv << N << ",AVL,insert," << avg_insert << "\n";
            csv << N << ",AVL,search," << avg_search << "\n";
            csv << N << ",AVL,erase," << avg_erase << "\n";

            cout << "AVL → insert:" << avg_insert
                 << " search:" << avg_search
                 << " erase:" << avg_erase << " µs\n";
        }

        // ======== Splay ========
        {
            vector<double> times_insert, times_search, times_erase;

            for (int iter = 0; iter < NUM_ITERATIONS; ++iter) {
                vector<int> data;
                data.reserve(N);
                for (int i = 0; i < N; ++i) data.push_back(dist(rng));

                splay_tree_implementation splay;
                auto start = high_resolution_clock::now();
                for (int v : data) splay.insert(v);
                auto end = high_resolution_clock::now();
                times_insert.push_back(duration_cast<duration<double, micro>>(end - start).count());

                start = high_resolution_clock::now();
                for (int v : data) splay.find(v);
                end = high_resolution_clock::now();
                times_search.push_back(duration_cast<duration<double, micro>>(end - start).count());

                start = high_resolution_clock::now();
                for (int v : data) splay.remove(v);
                end = high_resolution_clock::now();
                times_erase.push_back(duration_cast<duration<double, micro>>(end - start).count());
            }

            double avg_insert = accumulate(times_insert.begin(), times_insert.end(), 0.0) / NUM_ITERATIONS;
            double avg_search = accumulate(times_search.begin(), times_search.end(), 0.0) / NUM_ITERATIONS;
            double avg_erase = accumulate(times_erase.begin(), times_erase.end(), 0.0) / NUM_ITERATIONS;

            csv << N << ",Splay,insert," << avg_insert << "\n";
            csv << N << ",Splay,search," << avg_search << "\n";
            csv << N << ",Splay,erase," << avg_erase << "\n";

            cout << "Splay → insert:" << avg_insert
                 << " search:" << avg_search
                 << " erase:" << avg_erase << " µs\n";
        }

        // ======== Red-Black Tree ========
        {
            vector<double> times_insert, times_search, times_erase;

            for (int iter = 0; iter < NUM_ITERATIONS; ++iter) {
                vector<int> data;
                data.reserve(N);
                for (int i = 0; i < N; ++i) data.push_back(dist(rng));

                RB_tree<int> rb;
                auto start = high_resolution_clock::now();
                for (int v : data) rb.add_leaf(v);
                auto end = high_resolution_clock::now();
                times_insert.push_back(duration_cast<duration<double, micro>>(end - start).count());

                volatile int dummy3 = 0;
                start = high_resolution_clock::now();
                for (int v : data) dummy3 += rb.find(v) ? 1 : 0;
                end = high_resolution_clock::now();
                times_search.push_back(duration_cast<duration<double, micro>>(end - start).count());

                start = high_resolution_clock::now();
                for (int v : data) rb.delete_leaf(v);
                end = high_resolution_clock::now();
                times_erase.push_back(duration_cast<duration<double, micro>>(end - start).count());
            }

            double avg_insert = accumulate(times_insert.begin(), times_insert.end(), 0.0) / NUM_ITERATIONS;
            double avg_search = accumulate(times_search.begin(), times_search.end(), 0.0) / NUM_ITERATIONS;
            double avg_erase = accumulate(times_erase.begin(), times_erase.end(), 0.0) / NUM_ITERATIONS;

            csv << N << ",RedBlackTree,insert," << avg_insert << "\n";
            csv << N << ",RedBlackTree,search," << avg_search << "\n";
            csv << N << ",RedBlackTree,erase," << avg_erase << "\n";

            cout << "Red-Black Tree → insert:" << avg_insert
                 << " search:" << avg_search
                 << " erase:" << avg_erase << " µs\n";
        }
    }

    csv.close();
    cout << "\n✅ Resultados guardados en 'benchmark_results.csv'\n";
    cout << "   (Promedio de " << NUM_ITERATIONS << " iteraciones por configuración)\n";
    return 0;
}