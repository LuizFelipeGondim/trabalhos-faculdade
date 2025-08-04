import streamlit as st
import pandas as pd
from estatisticasOrgao import estatisticas_orgaos
from estatisticasBilhete import estatisticas_bilhete
from estatisticasCompanhia import estatisticas_companhia

csv_files = {
  "orgao": "tables/csv_for_streamlit/orgao.csv",
  "orgao_bilhete": "tables/csv_for_streamlit/orgao_bilhete.csv",
  "bilhete": "tables/csv_for_streamlit/bilhete.csv",
  "companhia_aerea": "tables/csv_for_streamlit/companhia_aerea.csv",
  "companhia_aerea_aviao": "tables/csv_for_streamlit/companhia_aerea_aviao.csv",
  "aviao": "tables/csv_for_streamlit/aviao.csv",
}

st.title("Visualize os dados das tabelas")

selected_table = st.selectbox("Selecione uma tabela:", list(csv_files.keys()))

if selected_table:
  file_path = csv_files[selected_table]
  try:
    df = pd.read_csv(file_path)

    st.sidebar.subheader("Filtro de dados")

    if selected_table == "orgao":
      filtro_nome = st.sidebar.text_input("Filtrar por nome do órgão:")
      filtro_superior = st.sidebar.text_input("Filtrar por código do órgão superior:")

      if filtro_nome:
        df = df[df["nome_orgao"].str.contains(filtro_nome, case=False, na=False)]

      if filtro_superior:
        df = df[df["codigo_orgao_superior"] == int(filtro_superior)]

    elif selected_table == "orgao_bilhete":
      filtro_localizador = st.sidebar.text_input("Filtrar por localizador:")
      filtro_codigo_orgao = st.sidebar.text_input("Filtrar por código do órgão:")

      if filtro_localizador:
        df = df[df["localizador"].str.contains(filtro_localizador, case=False, na=False)]

      if filtro_codigo_orgao:
        df = df[df["codigo_orgao"] == int(filtro_codigo_orgao)]

    elif selected_table == "bilhete":
      filtro_localizador = st.sidebar.text_input("Filtrar por localizador:")
      filtro_situacao = st.sidebar.selectbox("Filtrar por situação do bilhete:", ["", "Voado", "Cancelado", "Andamento"])
      
      if filtro_localizador:
        df = df[df["localizador"] == filtro_localizador]

      if filtro_situacao:
        df = df[df["situacao_bilhete"] == filtro_situacao]

    elif selected_table == "companhia_aerea":
      filtro_nome = st.sidebar.text_input("Filtrar por nome da companhia:")

      if filtro_nome:
        df = df[df["nome_companhia"].str.contains(filtro_nome, case=False, na=False)]

    elif selected_table == "companhia_aerea_aviao":
      filtro_id_companhia = st.sidebar.text_input("Filtrar por ID da companhia:")

      if filtro_id_companhia:
        df = df[df["id_companhia_aerea"] == int(filtro_id_companhia)]

    elif selected_table == "aviao":
      filtro_modelo = st.sidebar.text_input("Filtrar por modelo do avião:")
      filtro_selo = st.sidebar.selectbox("Filtrar por selo ANAC:", ["", 'A', 'B', 'C'])

      if filtro_modelo:
        df = df[df["modelo"].str.contains(filtro_modelo, case=False, na=False)]

      if filtro_selo:
        df = df[df["selo_anac"] == filtro_selo]

    num_rows = st.sidebar.selectbox("Quantidade de linhas:", [10, 20, 50, 100, "Todos"], index=4)
    
    if num_rows != "Todos":
      df = df.head(num_rows)

    st.subheader(f"Dados da tabela **{selected_table}**:")
    st.dataframe(df)

    if (selected_table == "orgao"):
      st.subheader("Informações importantes:")
      estatisticas_orgaos()
    elif (selected_table == "bilhete"):
      st.subheader("Informações importantes:")
      estatisticas_bilhete()
    elif (selected_table == "companhia_aerea" or selected_table == "aviao"):
      st.subheader("Informações importantes:")
      estatisticas_companhia()

  except Exception as e:
    st.error(f"Erro ao carregar o arquivo: {e}")



    