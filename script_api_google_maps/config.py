# config.py
from dotenv import load_dotenv
import os

# Carrega variáveis do .env
load_dotenv()

API_KEY = os.getenv("GOOGLE_API_KEY")
ORIGEM = os.getenv("ORIGEM")
DESTINO = os.getenv("DESTINO")
