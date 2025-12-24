import pandas as pd
import numpy as np

"""
Faz o pre-processamento do arquivo.
"""
def load_csv_dataset(path):

    df = pd.read_csv(path)

    # Testa se TODAS as colunas das features (exceto a última) são numéricas
    # A última coluna pode ser string (label).
    try:
        df.iloc[:, :-1].astype(float)
        has_header = True
    except:
        has_header = False

    # Se não tinha header → relê sem header
    if not has_header:
        df = pd.read_csv(path, header=None)

        # Agora as features são todas exceto a última coluna
        try:
            df.iloc[:, :-1].astype(float)
        except:
            raise ValueError(
                f"Arquivo {path}: alguma coluna contém strings que não são numéricas."
            )

    data = df.values
    y = data[:, -1]

    # Se labels não forem numéricos → converte para inteiros (0,1,2,...)
    if not np.issubdtype(y.dtype, np.number):
        try:
            y = pd.factorize(y)[0]
        except:
            raise ValueError(
                f"Labels do arquivo {path} não são numéricos e não podem ser convertidos."
            )

    y = y.astype(int)

    X = data[:, :-1]

    # Converte features para float
    try:
        X = X.astype(float)
    except:
        raise ValueError(
            f"Arquivo {path}: alguma coluna contém strings que não são numéricas."
        )

    # Quantidade de clusters/classes
    k = len(np.unique(y))

    return X, y, k