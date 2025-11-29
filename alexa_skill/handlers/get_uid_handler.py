import logging
from ask_sdk_core.dispatch_components import AbstractRequestHandler
from ask_sdk_core.utils import is_intent_name
from db import obter_ultimo_uid
from google_transit import buscar_onibus_proximo

logger = logging.getLogger(__name__)

class GetUIDIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_intent_name("GetUIDIntent")(handler_input)

    def handle(self, handler_input):
        uid = obter_ultimo_uid()
        if uid:
            fala_onibus = buscar_onibus_proximo()
            speak_output = f"Cartão Livre Acesso detectado. {fala_onibus}"
        else:
            speak_output = "Não detectei nenhum cartão livre acesso, por favor aproxime o cartão VEM."
        
        logger.info("Resposta para Alexa: %s", speak_output)
        return handler_input.response_builder.speak(speak_output).set_should_end_session(False).response
