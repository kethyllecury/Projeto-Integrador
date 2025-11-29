# main.py
from config import ORIGEM, DESTINO
from routes_api import buscar_rota

if __name__ == "__main__":
    buscar_rota(ORIGEM, DESTINO)
