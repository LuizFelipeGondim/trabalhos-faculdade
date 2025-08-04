import pandas as pd
import streamlit as st

def estatisticas_companhia():
  """
  Calcula e exibe as estatísticas baseadas na tabela companhia e aviões.
  """

  df_companhia_aerea = pd.read_csv("tables/csv_for_streamlit/companhia_aerea.csv")
  df_companhia_aviao = pd.read_csv("tables/csv_for_streamlit/companhia_aerea_aviao.csv")
  df_aviao = pd.read_csv("tables/csv_for_streamlit/aviao.csv")

  df_companhias = pd.merge(
    df_companhia_aerea,
    df_companhia_aviao,
    on="id_companhia_aerea"
  )

  df_companhias["ativos"] = df_companhias["ativos"].fillna(0)

  df_companhias["inativos"] = df_companhias["inativos"].fillna(0)
  df_total = (
    df_companhias.groupby("nome_companhia")[["ativos", "inativos"]]
    .sum()
    .reset_index()
  )
  df_total["total_avioes"] = df_total["ativos"] + df_total["inativos"]

  st.write("Total de aviões por companhia aérea")
  st.dataframe(df_total)

  df_ativos_detalhes = pd.merge(
    df_companhias[df_companhias["ativos"] > 0],
    df_aviao,
    on="id_aviao"
  )

  df_media_assentos = (
    df_ativos_detalhes.groupby("nome_companhia")["assentos"]
    .mean()
    .reset_index()
    .rename(columns={"assentos": "media_assentos"})
  )

  st.write("Média de assentos dos aviões ativos por companhia")
  st.dataframe(df_media_assentos)

