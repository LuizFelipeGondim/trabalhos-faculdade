import pandas as pd
import streamlit as st
import altair as alt

def estatisticas_bilhete():
  """
  Calcula e exibe as estatísticas baseadas na tabela bilhete.
  """

  df_bilhete = pd.read_csv("tables/csv_for_streamlit/bilhete.csv")

  bilhete_mais_caro = df_bilhete["valor_bilhete"].max()

  bilhete_mais_barato = df_bilhete["valor_bilhete"].min()

  percentual_medio_desconto = df_bilhete["percentual_desconto_aplicado"].mean()

  total_multas = df_bilhete["valor_multas"].sum()

  st.write(f"**Bilhete mais caro:** R$ {bilhete_mais_caro:.2f}")
  st.write(f"**Bilhete mais barato:** R$ {bilhete_mais_barato:.2f}")
  st.write(f"**Percentual médio de desconto aplicado:** {percentual_medio_desconto:.2f}%")
  st.write(f"**Total de multas aplicadas:** R$ {total_multas:.2f}")

  companhia_map = {
    1: "LATAM",
    2: "GOL",
    3: "AZUL"
  }

  df_bilhete["nome_companhia_aerea"] = df_bilhete["id_companhia_aerea"].map(companhia_map)

  distribuicao_companhia = (
    df_bilhete["nome_companhia_aerea"]
    .value_counts()
    .reset_index(name="quantidade")
    .rename(columns={"index": "nome_companhia_aerea"})
  )

  st.altair_chart(
    alt.Chart(distribuicao_companhia)
    .mark_bar()
    .encode(
      x=alt.X("nome_companhia_aerea:N", title="Companhia Aérea"),
      y=alt.Y("quantidade:Q", title="Quantidade"),
      tooltip=["nome_companhia_aerea", "quantidade"],
    )
    .properties(title="Distribuição de bilhetes por Companhia Aérea:", width=600)
  )
