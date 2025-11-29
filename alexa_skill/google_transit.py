import logging
import requests
from config import GOOGLE_API_KEY, ORIGEM, DESTINO

logger = logging.getLogger(__name__)

def buscar_onibus_proximo():
    """Consulta a API do Google Transit para obter o próximo ônibus disponível"""
    try:
        url = "https://routes.googleapis.com/directions/v2:computeRoutes"
        headers = {
            "Content-Type": "application/json",
            "X-Goog-Api-Key": GOOGLE_API_KEY,
            "X-Goog-FieldMask": "routes.legs.steps.transitDetails,routes.legs.steps.localizedValues",
        }
        body = {
            "origin": {"address": ORIGEM},
            "destination": {"address": DESTINO},
            "travelMode": "TRANSIT",
            "transitPreferences": {"allowedTravelModes": ["BUS"]},
        }

        response = requests.post(url, json=body, headers=headers, timeout=10)
        response.raise_for_status()
        dados = response.json()

        routes = dados.get("routes", [])
        if not routes:
            return "Nenhuma rota de ônibus encontrada."

        for step in routes[0].get("legs", [{}])[0].get("steps", []):
            if "transitDetails" in step:
                transit = step["transitDetails"]
                linha = transit.get("transitLine", {}).get("name", "desconhecida")
                dep_time = transit.get("localizedValues", {}).get("departureTime", {}).get("time", {}).get("text", "desconhecido")
                arr_time = transit.get("localizedValues", {}).get("arrivalTime", {}).get("time", {}).get("text", "desconhecido")
                return f"O próximo ônibus é da linha {linha}, saindo às {dep_time} e chegando às {arr_time}."
        
        return "Não há ônibus disponíveis no momento."

    except Exception as e:
        logger.error("Erro ao buscar ônibus: %s", e)
        return "Não consegui encontrar informações sobre o ônibus mais próximo."
