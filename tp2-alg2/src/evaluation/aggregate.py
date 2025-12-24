import os
import csv
import numpy as np
from pathlib import Path
from collections import defaultdict

"""
Lê todos os *_summary.csv da pasta de resultados e gera um CSV único
com as médias agregadas entre todos os datasets.
"""
def aggregate_all_summaries(summary_folder, output_csv):
    summary_folder = Path(summary_folder)
    files = list(summary_folder.glob("*_summary.csv"))

    if not files:
        print("Nenhum summary encontrado.")
        return

    # Estrutura: chave → lista de dicts com valores
    groups = defaultdict(list)

    for file in files:
        with open(file, "r") as f:
            reader = csv.DictReader(f)
            for row in reader:

                # chave lógica de agrupamento
                key = (
                    row["algorithm"],
                    row["metric"],
                    row["p"],
                    row["width_frac"]
                )

                groups[key].append(row)

    def safe_float(v):
        try:
            return float(v)
        except:
            return None

    def mean(values):
        vals = [safe_float(v) for v in values if v not in [None, "", "None"]]
        return float(np.mean(vals)) if vals else None

    rows_out = []

    for key, entries in groups.items():
        alg, metric, p, width_frac = key

        def collect(col):
            return [e[col] for e in entries]

        row = {
            "algorithm": alg,
            "metric": metric,
            "p": p,
            "width_frac": width_frac,

            "radius_mean": mean(collect("radius_mean")),
            "radius_std": mean(collect("radius_std")),

            "silhouette_mean": mean(collect("silhouette_mean")),
            "silhouette_std": mean(collect("silhouette_std")),

            "ari_mean": mean(collect("ari_mean")),
            "ari_std": mean(collect("ari_std")),

            "time_mean": mean(collect("time_mean")),
            "time_std": mean(collect("time_std")),

            "num_datasets": len(entries)
        }

        rows_out.append(row)

    os.makedirs(os.path.dirname(output_csv), exist_ok=True)

    fieldnames = [
        "algorithm", "metric", "p", "width_frac",
        "radius_mean", "radius_std",
        "silhouette_mean", "silhouette_std",
        "ari_mean", "ari_std",
        "time_mean", "time_std",
        "num_datasets"
    ]

    with open(output_csv, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows_out)

    print(f"Tabela geral criada em: {output_csv}")
