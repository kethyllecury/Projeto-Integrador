from ask_sdk_core.dispatch_components import AbstractRequestHandler
from ask_sdk_core.utils import is_intent_name

class FallbackIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_intent_name("AMAZON.FallbackIntent")(handler_input)

    def handle(self, handler_input):
        speak_output = "Desculpe, não entendi. Você pode dizer: consultar UID."
        return handler_input.response_builder.speak(speak_output).ask("O que deseja fazer?").response
