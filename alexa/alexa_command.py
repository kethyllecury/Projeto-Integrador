import logging
import json
import requests
from ask_sdk_core.skill_builder import SkillBuilder
from ask_sdk_core.dispatch_components import AbstractRequestHandler, AbstractExceptionHandler
from ask_sdk_core.handler_input import HandlerInput
from ask_sdk_core.utils import is_intent_name, is_request_type
from ask_sdk_model import Response
from dotenv import load_dotenv
import os

load_dotenv()

api_key = os.getenv("api_key")

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)


class LaunchRequestHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_request_type("LaunchRequest")(handler_input)

    def handle(self, handler_input):
        speak_output = "Bem-vindo ao Alexaiot. Você pode dizer: consultar UID."
        return handler_input.response_builder.speak(speak_output).ask("O que deseja fazer?").response


class GetUIDIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_intent_name("GetUIDIntent")(handler_input)

    def handle(self, handler_input):
        speak_output = "Não consegui acessar a API."
        url = "https://projeto-integrador-f86b.onrender.com/api/readings/latest"
        headers = {
            "x-api-key": api_key
        }

        try:
            response = requests.get(url, headers=headers, timeout=5)
            response.raise_for_status()
            dados = response.json()

            if isinstance(dados, list) and len(dados) > 0:
                # Pega o registro mais recente (primeiro da lista)
                ultimo = dados[0]
                partes = []

                if "uid" in ultimo:
                    partes.append(f"UID {ultimo['uid']}")
                if "tag_type" in ultimo:
                    partes.append(f"tipo {ultimo['tag_type']}")
                if "rssi" in ultimo:
                    partes.append(f"sinal {ultimo['rssi']}")
                if "timestamp" in ultimo:
                    partes.append(f"registrado em {ultimo['timestamp']}")

                speak_output = "O registro mais recente é: " + ", ".join(partes)
            else:
                speak_output = "Não encontrei nenhum dado."

        except requests.RequestException as e:
            speak_output = f"Erro ao acessar a API: {e}"
        except Exception as e:
            speak_output = f"Ocorreu um erro interno: {e}"

        logger.info("Resposta para Alexa: %s", speak_output)
        return handler_input.response_builder.speak(speak_output).set_should_end_session(False).response



class HelloWorldIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_intent_name("HelloWorldIntent")(handler_input)

    def handle(self, handler_input):
        return handler_input.response_builder.speak("Hello World!").response


class HelpIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_intent_name("AMAZON.HelpIntent")(handler_input)

    def handle(self, handler_input):
        return handler_input.response_builder.speak("Você pode me pedir para consultar o UID.").ask("O que deseja fazer?").response


class CancelOrStopIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_intent_name("AMAZON.CancelIntent")(handler_input) or is_intent_name("AMAZON.StopIntent")(handler_input)

    def handle(self, handler_input):
        return handler_input.response_builder.speak("Até logo!").response


class FallbackIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_intent_name("AMAZON.FallbackIntent")(handler_input)

    def handle(self, handler_input):
        return handler_input.response_builder.speak("Desculpe, não entendi. Você pode dizer: consultar UID.").ask("O que deseja fazer?").response


class SessionEndedRequestHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_request_type("SessionEndedRequest")(handler_input)

    def handle(self, handler_input):
        logger.info("Session ended.")
        return handler_input.response_builder.response


class IntentReflectorHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_request_type("IntentRequest")(handler_input)

    def handle(self, handler_input):
        intent_name = handler_input.request_envelope.request.intent.name
        return handler_input.response_builder.speak(f"Você acabou de disparar o intent {intent_name}.").response


class CatchAllExceptionHandler(AbstractExceptionHandler):
    def can_handle(self, handler_input, exception):
        return True

    def handle(self, handler_input, exception):
        logger.error("Erro na skill:", exc_info=exception)
        return handler_input.response_builder.speak("Desculpe, ocorreu um problema inesperado.").ask("Pode tentar novamente?").response


sb = SkillBuilder()
sb.add_request_handler(LaunchRequestHandler())
sb.add_request_handler(GetUIDIntentHandler())
sb.add_request_handler(HelloWorldIntentHandler())
sb.add_request_handler(HelpIntentHandler())
sb.add_request_handler(CancelOrStopIntentHandler())
sb.add_request_handler(FallbackIntentHandler())
sb.add_request_handler(SessionEndedRequestHandler())
sb.add_request_handler(IntentReflectorHandler())
sb.add_exception_handler(CatchAllExceptionHandler())

lambda_handler = sb.lambda_handler()
