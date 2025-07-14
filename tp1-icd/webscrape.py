import pandas as pd
import os
from pathlib import Path
import time
from random import uniform

titles_12 = [
    'serie_A_overall',
    'serie_A_homeaway',
    'squad_standard_stats',
    'squad_standard_stats_opp',
    'squad_goalkeeping',
    'squad_goalkeeping_opp',
    'squad_advanced_goalkeeping',
    'squad_advanced_goalkeeping_opp',
    'squad_shooting',
    'squad_shooting_opp',
    'squad_passing',
    'squad_passing_opp',
    'squad_pass_types',
    'squad_pass_types_opp',
    'squad_goal_shot_creation',
    'squad_goal_shot_creation_opp',
    'squad_defensive_actions',
    'squad_defensive_actions_opp',
    'squad_possession',
    'squad_possession_opp',
    'squad_playing_time',
    'squad_playing_time_opp',
    'squad_miscellaneous',
    'squad_miscellaneous_opp',
]

titles_6 = titles_12[:6] + titles_12[8:10] + titles_12[20:]

titles_5 = titles_6[:6] + titles_6[8:]

os.makedirs('data/clubes/', exist_ok=True)
os.makedirs('data/league/', exist_ok=True)

def get_all_stats(selected_tables: list[str] | None = None):
    all_tables = {}

    for page in range(11):
        time.sleep(uniform(3, 20))
        year = 2024 - page
        url = f"https://fbref.com/en/comps/24/{year}/{year}-Serie-A-Stats"
        tables = pd.read_html(url)

        if year > 2018:
            titles = titles_12
        elif year > 2015:
            titles = titles_6
        else:
            titles = titles_5

        for i, title in enumerate(titles):
            table_name = f"{title}_{year}"

            # Se não passou filtro, pega todas as tabelas
            if selected_tables is None or title in selected_tables:
                df = tables[i]
                all_tables[table_name] = df
                df.to_csv(f"data/league/{table_name}.csv", index=False)

    return all_tables

## ID da página de acesso de cada clube
clubes_ids = {
    'Botafogo-RJ': {'id': 'd9fdd9d9'},
    'Palmeiras': {'id': 'abdce579'},
    'Flamengo': {'id': '639950ae'},
    'Fortaleza': {'id': 'a9d0ab0e'},
    'Internacional': {'id': '6f7e1f03'},
    'São-Paulo': {'id': '5f232eb1'},
    'Corinthians': {'id': 'bf4acd28'},
    'Bahia': {'id': '157b7fee'},
    'Cruzeiro': {'id': '03ff5eeb'},
    'Vasco-da-Gama': {'id': '83f55dbe'},
    'Vitória': {'id': '33f95fe0'},
    'Atlético-Mineiro': {'id': '422bb734'},
    'Fluminense': {'id': '84d9701c'},
    'Grêmio': {'id': 'd5ae3703'},
    'Juventude': {'id': 'd081b697'},
    'RB-Bragantino': {'id': 'f98930d1'},
    'Ath-Paranaense': {'id': '2091c619'},
    'Criciúma': {'id': '3f7595bb'},
    'Atl-Goianiense': {'id': '32d508ca'},
    'Cuiabá': {'id': 'f0e6fb14'},
    'Santos': {'id': '712c528f'},
    'Goiás': {'id': '78c617cc'},
    'Coritiba': {'id': 'd680d257'},
    'América-MG': {'id': '1f68d780'},
    'Ceará': {'id': '2f335e17'},
    'Avaí': {'id': 'f205258a'},
    'Sport-Recife': {'id': 'ece66b78'},
    'Chapecoense': {'id': 'baa296ad'},
    'CSA': {'id': '05aff519'},
    'Paraná': {'id': '2091c619'},
    'Ponte-Preta': {'id': 'b162ebe7'},
    'Figueirense': {'id': '0ce4436d'},
    'Santa-Cruz': {'id': 'ad0c1246'},
    'Joinville': {'id': 'da0666a2'},
}

def get_clubs_stats(selected_clubs: list[str] | None = None):
    if selected_clubs is None:
        selected_clubs = list(clubes_ids.keys())

    club_data = {}

    for clube in selected_clubs:
        if clube not in clubes_ids:
            print(f"[Aviso] Clube '{clube}' não encontrado em clubes_ids.")
            continue

        club_data[clube] = {}
        club_id = clubes_ids[clube]['id']

        for year in range(2024, 2013, -1):  # anos de 2024 a 2014
            url = f"https://fbref.com/en/squads/{club_id}/{year}/{clube}-Stats"
            try:
                print(f"Coletando {clube} - {year}")
                time.sleep(uniform(3, 8))
                tables = pd.read_html(url)

                # Corrigir múltiplos headers (MultiIndex)
                cleaned_tables = []
                for table in tables:
                    if isinstance(table.columns, pd.MultiIndex):
                        table.columns = table.columns.get_level_values(-1)
                    cleaned_tables.append(table)

                club_data[clube][year] = cleaned_tables

                # Salvar localmente
                club_dir = Path(f"data/clubes/{clube}")
                club_dir.mkdir(parents=True, exist_ok=True)
                for i, table in enumerate(cleaned_tables):
                    filename = club_dir / f"{year}_{i}.csv"
                    table.to_csv(filename, index=False)
                    print(f"Salvou: {filename}")

            except Exception as e:
                print(f"Erro em {clube} - {year}: {e}")
                continue

    return club_data

"""
def get_clubs_stats():
    for clube in clubes:
        clubes[clube]['years_SA'] = []

    # verifica se o time estava na serie A naquele ano e adiciona o ano na key 'years_SA'

    for i in range(11):
        df = pd.read_csv(f'data/league/serie_A_overall_{2024 - i}.csv')

        unique_values = df['Squad'].unique()

        for value in unique_values:
            value = value.replace(" ", "-").replace("(", "").replace(")", "")

            clubes[value]['years_SA'].append(2024 - i)

    for clube, dados in clubes.items():
        # Create club directory if it doesn't exist
        club_dir = Path(f'data/clubes/{clube}')
        club_dir.mkdir(parents=True, exist_ok=True)
        
        for year in dados['years_SA']:
            # Generate expected filenames pattern for this year
            existing_files = list(club_dir.glob(f"{year}_*.csv"))
            
            # Skip if we already have files for this year
            if existing_files:
                print(f"Skipping {clube} {year} - already downloaded")
                continue
            
            url = f"https://fbref.com/en/squads/{clubes[clube]['id']}/{year}/{clube}-Stats"

            try:
                # Add delay to avoid 429 errors
                time.sleep(uniform(3, 20))
                tables = pd.read_html(url)
                
                # Save each table
                for i, table in enumerate(tables):
                    filename = club_dir / f"{year}_{i}.csv"
                    table.to_csv(filename, index=False)
                    print(f"Saved {filename}")
                    
            except Exception as e:
                print(f"Failed to scrape {clube} {year}: {str(e)}")
                continue
"""