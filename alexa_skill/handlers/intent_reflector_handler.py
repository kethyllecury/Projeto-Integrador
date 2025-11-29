from ask_sdk_core.dispatch_components import AbstractRequestHandler
from ask_sdk_core.utils import is_request_type

class IntentReflectorHandler(AbstractRequestHandler):
    """Handler para refletir o intent disparado, útil para debug"""
    def can_handle(self, handler_input):
        return is_request_type("IntentRequest")(handler_input)

    def handle(self, handler_input):
        intent_name = handler_input.request_envelope.request.intent.name
        speak_output = f"Você acabou de disparar o intent {intent_name}."
        return handler_input.response_builder.speak(speak_output).response
