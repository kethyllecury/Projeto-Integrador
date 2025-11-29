import logging
from pymongo import MongoClient
from config import MONGO_URI, DB_NAME, COLLECTION_NAME

logger = logging.getLogger(__name__)

# Conexão com MongoDB
client = MongoClient(MONGO_URI)
db = client[DB_NAME]
collection = db[COLLECTION_NAME]

def obter_ultimo_uid():
    """Retorna o último UID registrado no MongoDB"""
    try:
        ultimo = collection.find_one(sort=[("_id", -1)])
        if ultimo:
            return ultimo.get("uid", "desconhecido")
        return None
    except Exception as e:
        logger.error("Erro ao acessar MongoDB: %s", e)
        return None
