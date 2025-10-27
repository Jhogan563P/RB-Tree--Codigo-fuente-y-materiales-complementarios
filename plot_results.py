import matplotlib.pyplot as plt
import pandas as pd

try:
    df = pd.read_csv("sample_data/benchmark_results.csv")
    print("✅ Archivo cargado exitosamente")
    print(f"Total de registros: {len(df)}")
    print(f"\nEstructuras disponibles: {df['Structure'].unique()}")
    print(f"Operaciones disponibles: {df['Operation'].unique()}")
except FileNotFoundError:
    print("❌ Error: 'benchmark_results.csv' no encontrado.")
    print("   Asegúrate de ejecutar main_simple.cpp primero.")
    exit()

traducciones_operaciones = {
    "insert": "Inserción",
    "search": "Búsqueda",
    "erase": "Eliminación"
}

colores = {
    'std::set': '#1f77b4',
    'AVL': '#ff7f0e',
    'Splay': '#2ca02c',
    'RedBlackTree': '#d62728'
}

def plot_operation(df, operation):
    """Grafica una operación específica"""
    df_op = df[df["Operation"] == operation]

    plt.figure(figsize=(10, 6))

    for estructura in df_op["Structure"].unique():
        df_est = df_op[df_op["Structure"] == estructura]

        plt.plot(
            df_est["N"],
            df_est["Time_microseconds"],
            marker='o',
            label=estructura,
            color=colores.get(estructura, 'black'),
            linewidth=2,
            markersize=8
        )

    titulo_op = traducciones_operaciones.get(operation, operation)
    plt.title(f'Tiempo de ejecución - {titulo_op}', fontsize=14, fontweight='bold')
    plt.xlabel("Cantidad de datos (N)", fontsize=11)
    plt.ylabel("Tiempo total [microsegundos]", fontsize=11)
    plt.legend(title="Estructura", fontsize=10)
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout()

    filename = f"benchmark_{operation}.png"
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    print(f"✅ Gráfica guardada: {filename}")
    plt.show()

def plot_all_operations(df):
    """Grafica las 3 operaciones lado a lado"""
    operations = df["Operation"].unique()

    fig, axes = plt.subplots(1, 3, figsize=(18, 6))
    fig.suptitle('Comparación de Operaciones', fontsize=16, fontweight='bold')

    for idx, operation in enumerate(operations):
        ax = axes[idx]
        df_op = df[df["Operation"] == operation]

        for estructura in df_op["Structure"].unique():
            df_est = df_op[df_op["Structure"] == estructura]

            ax.plot(
                df_est["N"],
                df_est["Time_microseconds"],
                marker='o',
                label=estructura,
                color=colores.get(estructura, 'black'),
                linewidth=2,
                markersize=8
            )

        titulo_op = traducciones_operaciones.get(operation, operation)
        ax.set_title(titulo_op, fontsize=13, fontweight='bold')
        ax.set_xlabel("Cantidad de datos (N)", fontsize=11)
        ax.set_ylabel("Tiempo [microsegundos]", fontsize=11)
        ax.legend(title="Estructura", fontsize=9)
        ax.grid(True, linestyle="--", alpha=0.6)

    plt.tight_layout()
    filename = "benchmark_all_operations.png"
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    print(f"✅ Gráfica guardada: {filename}")
    plt.show()

def create_comparison_table(df, N_value):
    """Crea una tabla comparativa para un N específico"""
    df_filtered = df[df["N"] == N_value]

    if df_filtered.empty:
        print(f"⚠️  No hay datos para N={N_value}")
        return

    print(f"\n{'='*70}")
    print(f"TABLA COMPARATIVA - N={N_value:,}")
    print(f"{'='*70}")
    print(f"{'Operación':<15} {'Estructura':<15} {'Tiempo [µs]':<15}")
    print(f"{'-'*70}")

    for operation in ["insert", "search", "erase"]:
        df_op = df_filtered[df_filtered["Operation"] == operation]
        op_traducida = traducciones_operaciones.get(operation, operation)

        print(f"\n{op_traducida}")

        # Ordenar por tiempo
        df_op = df_op.sort_values('Time_microseconds')

        for _, row in df_op.iterrows():
            print(f"{'':15} {row['Structure']:<15} {row['Time_microseconds']:>12,.2f}")

    print(f"{'='*70}\n")

if __name__ == "__main__":
    print("\n" + "="*80)
    print("GENERANDO GRÁFICAS Y ANÁLISIS")
    print("="*80 + "\n")

    # Graficar cada operación
    operations = df["Operation"].unique()
    for op in operations:
        print(f"\n📊 Graficando operación: {traducciones_operaciones.get(op, op)}")
        plot_operation(df, op)

    # Comparación general
    print("\n📊 Graficando comparación general")
    plot_all_operations(df)

    # Tablas comparativas
    print("\n📊 Generando tablas comparativas\n")
    available_ns = sorted(df["N"].unique())

    # Mostrar tablas para algunos valores de N
    if len(available_ns) >= 3:
        ns_to_show = [available_ns[len(available_ns)//3],
                      available_ns[2*len(available_ns)//3],
                      available_ns[-1]]
    else:
        ns_to_show = available_ns

    for N_val in ns_to_show:
        create_comparison_table(df, N_val)

    # Resumen estadístico
    print("\n" + "="*80)
    print("RESUMEN ESTADÍSTICO GENERAL")
    print("="*80 + "\n")

    summary = df.groupby(["Structure", "Operation"])["Time_microseconds"].agg([
        'count', 'mean', 'min', 'max'
    ]).round(2)
    print(summary)

    print("\n✅ Análisis completado exitosamente!")
    print("="*80 + "\n")