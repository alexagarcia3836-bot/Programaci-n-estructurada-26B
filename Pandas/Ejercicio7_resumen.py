import pandas as pd

df = pd.read_csv(r"C:\Users\carlos\Downloads\pacientes - pacientes.csv")

resumen = (
    df.groupby("servicio")
    .agg(
        pacientes=("id", "count"),
        temp_promedio=("temp_c", "mean"),
        fc_promedio=("fc_lpm", "mean")
    )
    .round(1)
    .sort_values("fc_promedio", ascending=False)
)

print(resumen)