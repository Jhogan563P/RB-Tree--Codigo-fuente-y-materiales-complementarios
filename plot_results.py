import matplotlib.pyplot as plt
import pandas as pd

# === 1️⃣ Datos (leer del archivo CSV) ===
try:
    df = pd.read_csv("benchmark_results.csv")
except FileNotFoundError:
    print("Error: 'benchmark_results.csv' no encontrado. Asegúrate de ejecutar main.cpp primero.")
    exit()

# === 3️⃣ Traducción de operaciones ===
traducciones = {
    "insert": "Inserción",
    "search": "Búsqueda",
    "erase": "Eliminación"
}

# === 4️⃣ Graficar cada operación ===
operaciones = df["Operation"].unique()

for op in operaciones:
    df_op = df[df["Operation"] == op]
    plt.figure(figsize=(8, 5))

    for estructura in df_op["Structure"].unique():
        df_est = df_op[df_op["Structure"] == estructura]
        plt.plot(df_est["N"], df_est["Time_microseconds"], marker='o', label=estructura)

    # Usar la traducción en el título
    titulo = traducciones.get(op, op)
    plt.title(f"Tiempo de ejecución - {titulo}")
    plt.xlabel("Cantidad de datos (N)")
    plt.ylabel("Tiempo total [microsegundos]")
    plt.legend(title="Estructura")
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout()
    plt.show()

# === 5️⃣ (Opcional) Mostrar resumen general ===
print(df.groupby(["Structure", "Operation"])["Time_microseconds"].describe())
