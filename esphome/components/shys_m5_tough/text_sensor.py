import esphome.codegen as cg
import esphome.config_validation as cv
#import voluptuous as vol
from esphome.components import text_sensor
from esphome.const import CONF_ID, CONF_NAME

# POSSIBLE VALUES
VALUE_BUTTON_TYPES = {
    "x": ("X-Position", "px"),
    "y": ("Y-Position", "px"),
    "width": ("Breite", "px"),
    "height": ("Höhe", "px"),
    "text": ("Label", ""),
    "fontsize": ("Schriftgröße", "")
}


# PARAMETER
CONF_INIT_SOUND = "init_sound"
CONF_LOGIN_ENABLED = "login_enabled"
CONF_BUTTONS = "buttons"
CONF_BUTTON_X = "x"
CONF_BUTTON_Y = "y"
CONF_BUTTON_WIDTH = "width"
CONF_BUTTON_HEIGHT = "height"
CONF_BUTTON_FONTSIZE = "fontsize"
CONF_BUTTON_TEXT = "text"


# DEFAULTS
DEFAULT_NAME = "M5 Stack Tough"
DEFAULT_INIT_SOUND = False
DEFAULT_LOGIN_ENABLED = True


empty_text_sensor_ns = cg.esphome_ns.namespace(
    'shys_m5_tough')
ShysM5Tough = empty_text_sensor_ns.class_(
    'ShysM5Tough', text_sensor.TextSensor, cg.Component)

CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ShysM5Tough),
    cv.Optional(CONF_NAME, default=DEFAULT_NAME): cv.string,
    cv.Optional(CONF_INIT_SOUND, default=DEFAULT_INIT_SOUND): cv.boolean,
    cv.Optional(CONF_LOGIN_ENABLED, default=DEFAULT_LOGIN_ENABLED): cv.boolean,
    cv.Optional(CONF_BUTTONS, default=dict): cv.All([dict({
        cv.Required("x"): cv.int_range(0, 240),
        cv.Required("y"): cv.int_range(0, 320),
        cv.Required("width"): cv.int_range(0, 240),
        cv.Required("height"): cv.int_range(0, 320),
        cv.Required("text"): cv.string,
        cv.Required("fontsize"): cv.int_range(0, 20)
    })])
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield text_sensor.register_text_sensor(var, config)
    yield cg.register_component(var, config)

    if CONF_NAME in config:
        confInitSound = config[CONF_NAME]
        cg.add(var.set_init_sound_enabled(confInitSound))

    if CONF_INIT_SOUND in config:
        confInitSound = config[CONF_INIT_SOUND]
        cg.add(var.set_init_sound_enabled(confInitSound))

    if CONF_LOGIN_ENABLED in config:
        confSecured = config[CONF_LOGIN_ENABLED]
        cg.add(var.set_login_enabled(confSecured))

    if CONF_BUTTONS in config:
        confButtons = config[CONF_BUTTONS]
        for btn in confButtons:
            cg.add(var.createButton(btn[CONF_BUTTON_X],
                                    btn[CONF_BUTTON_Y],
                                    btn[CONF_BUTTON_WIDTH],
                                    btn[CONF_BUTTON_HEIGHT],
                                    btn[CONF_BUTTON_TEXT],
                                    btn[CONF_BUTTON_FONTSIZE]))
    else:
        cg.add(var.createButton(10,
                                10,
                                220,
                                300,
                                "KLICK",
                                5))
