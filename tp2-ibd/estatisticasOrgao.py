import pandas as pd
import streamlit as st
import altair as alt

def estatisticas_orgaos():
  st.write("**Quantidade de solicitações por órgão**")
  solicitacoes_por_orgao()

  st.write("**Quantidade de órgãos subordinados solicitantes por orgão superior**")
  grafico_orgaos_superiores()


def solicitacoes_por_orgao():
  """
  Exibe uma tabela mostrando a quantidade de solicitações por orgão.
  """
  df_orgao = pd.read_csv("tables/csv_for_streamlit/orgao.csv")
  df_orgao_bilhete = pd.read_csv("tables/csv_for_streamlit/orgao_bilhete.csv")

  solicitacoes_por_orgao = df_orgao_bilhete["codigo_orgao"].value_counts().reset_index()
  solicitacoes_por_orgao.columns = ["codigo_orgao", "quantidade_solicitacoes"]

  solicitacoes_com_nomes = solicitacoes_por_orgao.merge(
      df_orgao, how="left", left_on="codigo_orgao", right_on="codigo_orgao"
  )

  solicitacoes_com_nomes = solicitacoes_com_nomes.sort_values(
    by="quantidade_solicitacoes", ascending=False
  )

  st.dataframe(solicitacoes_com_nomes[["nome_orgao", "quantidade_solicitacoes"]])



def grafico_orgaos_superiores():
    """
    Exibe um gráfico mostrando os órgãos superiores e a quantidade de órgãos subordinados solicitantes.
    """

    df_orgao = pd.read_csv("tables/csv_for_streamlit/orgao.csv")

    orgaos_superiores = df_orgao[df_orgao["codigo_orgao_superior"].isna()]

    subordinados = (
      df_orgao.groupby("codigo_orgao_superior")
      .size()
      .reset_index(name="quantidade_subordinados")
    )

    resultado = orgaos_superiores.merge(
      subordinados, how="left", left_on="codigo_orgao", right_on="codigo_orgao_superior"
    )
    resultado["quantidade_subordinados"].fillna(0, inplace=True)

    chart = (
      alt.Chart(resultado)
      .mark_bar()
      .encode(
        x=alt.X("nome_orgao:N", title="Órgãos Superiores"),
        y=alt.Y("quantidade_subordinados:Q", title="Quantidade de Subordinados"),
        tooltip=["nome_orgao", "quantidade_subordinados"],
      )
    )

    st.altair_chart(chart, use_container_width=True)