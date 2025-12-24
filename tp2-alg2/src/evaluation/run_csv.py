import os
import sys
import json
from pathlib import Path
from summarize import summarize_results_to_csv
from aggregate import aggregate_all_summaries

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.abspath(os.path.join(CURRENT_DIR, "../.."))
SRC_DIR = os.path.join(ROOT_DIR, "src")

sys.path.append(ROOT_DIR)
sys.path.append(SRC_DIR)

from core.clustering import run_experiments
from load.load_dataset import load_csv_dataset


"""
Processa todos os CSVs de cada pasta informada.
Para cada conjunto de dados, gera um summary individual.
Ao final de cada pasta, gera um summary global daquela categoria.
"""
def process_all_csv(folders):
    if isinstance(folders, str):
        folders = [folders]

    for input_folder in folders:
        input_path = Path(f"data/{input_folder}")
        csv_files = list(input_path.glob("*.csv"))

        if not csv_files:
            print(f"Nenhum arquivo CSV encontrado em {input_folder}. Pulando...")
            continue

        # Cria diretório de saída caso não exista
        output_dir = Path(f"results/{input_folder}")
        output_dir.mkdir(parents=True, exist_ok=True)

        # Processa cada CSV individualmente
        for csv_file in csv_files:
            print(f"Processando dataset: {csv_file.name}")

            X, y, k = load_csv_dataset(csv_file)

            results = run_experiments(
                X=X,
                y_true=y,
                p_values=[1, 2],
                use_mahalanobis=True,
                algorithms=["farthest", "interval", "kmeans"],
                width_fracs=[0.01, 0.05, 0.1, 0.15, 0.25],
                runs=15
            )

            dataset_name = csv_file.stem
            output_csv = output_dir / f"{dataset_name}_summary.csv"
            summarize_results_to_csv(results, str(output_csv))

        # Gera summary agregado da pasta inteira
        aggregate_all_summaries(
            summary_folder=str(output_dir),
            output_csv=str(output_dir / "global_summary.csv")
        )


if __name__ == "__main__":
    process_all_csv(["real", "synthetic_1", "synthetic_2"])
