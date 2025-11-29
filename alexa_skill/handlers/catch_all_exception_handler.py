import logging
from ask_sdk_core.dispatch_components import AbstractExceptionHandler

logger = logging.getLogger(__name__)

class CatchAllExceptionHandler(AbstractExceptionHandler):
    def can_handle(self, handler_input, exception):
        return True

    def handle(self, handler_input, exception):
        logger.error("Erro na skill:", exc_info=exception)
        speak_output = "Desculpe, ocorreu um problema inesperado."
        return handler_input.response_builder.speak(speak_output).ask("Pode tentar novamente?").response
