import logging
from ask_sdk_core.skill_builder import SkillBuilder

# Handlers
from handlers.launch_handler import LaunchRequestHandler
from handlers.get_uid_handler import GetUIDIntentHandler
from handlers.help_handler import HelpIntentHandler
from handlers.cancel_stop_handler import CancelOrStopIntentHandler
from handlers.fallback_handler import FallbackIntentHandler
from handlers.session_ended_handler import SessionEndedRequestHandler
from handlers.hello_world_handler import HelloWorldIntentHandler
from handlers.intent_reflector_handler import IntentReflectorHandler
from handlers.catch_all_exception_handler import CatchAllExceptionHandler

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

sb = SkillBuilder()

handlers = [
    LaunchRequestHandler(),
    GetUIDIntentHandler(),
    HelloWorldIntentHandler(),
    HelpIntentHandler(),
    CancelOrStopIntentHandler(),
    FallbackIntentHandler(),
    SessionEndedRequestHandler(),
    IntentReflectorHandler()
]

for handler in handlers:
    sb.add_request_handler(handler)

sb.add_exception_handler(CatchAllExceptionHandler())

lambda_handler = sb.lambda_handler()
