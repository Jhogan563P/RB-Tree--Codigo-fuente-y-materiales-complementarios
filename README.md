# Benchmarking de Estructuras de Datos (AVL, Splay, std::set)

Este proyecto realiza un benchmarking de tres estructuras de datos de árbol: Árbol AVL, Árbol Splay y `std::set` (implementado como un árbol binario de búsqueda balanceado, generalmente un árbol rojo-negro). El objetivo es comparar su rendimiento en operaciones de inserción, búsqueda y eliminación para diferentes tamaños de conjuntos de datos.

## Estructuras de Datos Implementadas

*   **AVL.h**: Implementación de un Árbol AVL.
*   **Splay.h**: Implementación de un Árbol Splay.
*   **std::set**: Contenedor estándar de C++ que utiliza generlamente un RB Tree.

## Cómo Construir y Ejecutar

Este proyecto utiliza CMake para su construcción.

1.  **Clonar el repositorio:**
    ```bash
    git clone <URL_DEL_REPOSITORIO>
    cd Benchmarking_CLion
    ```

2.  **Crear el directorio de construcción y compilar:**
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```
    Esto generará el ejecutable `Benchmarking_CLion.exe` (o `Benchmarking_CLion` en sistemas Unix-like) en el directorio `build/`.

3.  **Ejecutar el benchmarking:**
    ```bash
    ./Benchmarking_CLion.exe
    ```
    La ejecución del programa generará un archivo `benchmark_results.csv` en el directorio `build/` con los tiempos de ejecución de las operaciones.

## Cómo Generar Gráficos de Resultados

El script de Python `plot_results.py` se utiliza para visualizar los datos del `benchmark_results.csv`.

1.  **Instalar dependencias de Python:**
    Asegúrate de tener `matplotlib` y `pandas` instalados.
    ```bash
    pip install matplotlib pandas
    ```

2.  **Ejecutar el script de gráficos:**
    ```bash
    python plot_results.py
    ```
    Esto mostrará una serie de gráficos comparando el rendimiento de las estructuras de datos para cada operación.

