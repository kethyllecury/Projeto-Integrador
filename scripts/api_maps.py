import requests


API_KEY = ""


ORIGEM = "Rua Alvaro da Boa Vista Maia, 811, Pernambuco, Brasil"


DESTINO = "Avenida Agamenom Magalhães, Pernambuco, Brasil"



def buscar_rota():
    """
    Função principal que monta e executa a requisição para a API do Google Routes
    e exibe o resultado formatado.
    """
    print(f"Buscando rotas de '{ORIGEM}' para '{DESTINO}'...")

    
    uri = "https://routes.googleapis.com/directions/v2:computeRoutes"

   
    headers = {
        "Content-Type": "application/json",
        "X-Goog-Api-Key": API_KEY,
        "X-Goog-FieldMask": "routes.legs.steps.transitDetails,routes.legs.steps.localizedValues",
    }

    
    body = {
        "origin": {"address": ORIGEM},
        "destination": {"address": DESTINO},
        "travelMode": "TRANSIT",
        "transitPreferences": {
            "allowedTravelModes": ["BUS"]
        },
    }

    
    try:
        
        response = requests.post(uri, json=body, headers=headers)

        
        response.raise_for_status()

        
        dados = response.json()
        
        
        
        routes = dados.get('routes', [])
        
        if not routes:
            print("Nenhuma rota encontrada.")
            return

        passo_de_onibus = None
        
        for step in routes[0].get('legs', [{}])[0].get('steps', []):
            if 'transitDetails' in step:
                passo_de_onibus = step
                break

        if passo_de_onibus:
           
            transit_details = passo_de_onibus.get('transitDetails', {})
            nome_da_parada = transit_details.get('stopDetails', {}).get('departureStop', {}).get('name')
            horario_saida = transit_details.get('localizedValues', {}).get('departureTime', {}).get('time', {}).get('text')
            linha_do_onibus = transit_details.get('transitLine', {}).get('name')
            parada_chegada = transit_details.get('stopDetails', {}).get('arrivalStop', {}).get('name')
            horario_chegada = transit_details.get('localizedValues', {}).get('arrivalTime', {}).get('time', {}).get('text')
            
            
            print("------------------------------------------------------------")
            print("✅ Rota encontrada com sucesso!")
            print(f" Linha: {linha_do_onibus}")
            print(f" Parada de Partida: {nome_da_parada}")
            print(f" Horário de Partida: {horario_saida}")
            print(f" Parada de Chegada: {parada_chegada}")
            print(f" Horário Previsto de Chegada: {horario_chegada}")
            print("------------------------------------------------------------")
        else:
            print("Nenhuma rota de ônibus encontrada para o trajeto especificado.")

    except requests.exceptions.HTTPError as http_err:
        print(f"❌ Ocorreu um erro HTTP: {http_err}")
        print(f"   Corpo do erro: {response.text}")
    except requests.exceptions.RequestException as err:
        print(f"❌ Ocorreu um erro na requisição: {err}")
    except Exception as e:
        print(f"❌ Ocorreu um erro inesperado: {e}")

if __name__ == "__main__":
    buscar_rota()