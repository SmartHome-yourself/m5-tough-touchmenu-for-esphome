import esphome.codegen as cg
import esphome.config_validation as cv
# import voluptuous as vol
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
CONF_BUTTON_COLOR = "color"
CONF_BUTTON_TEXT_COLOR = "textcolor"
CONF_BUTTON_BG_COLOR = "bgcolor"


# DEFAULTS
DEFAULT_NAME = "M5 Stack Tough"
DEFAULT_INIT_SOUND = False
DEFAULT_LOGIN_ENABLED = True
DEFAULT_BUTTON_COLOR = "TFT_BLUE"
DEFAULT_BUTTON_TEXT_COLOR = "TFT_WHITE"
DEFAULT_BUTTON_BG_COLOR = "TFT_DARKGREY"

empty_text_sensor_ns = cg.esphome_ns.namespace(
    'shys_m5_tough')
ShysM5Tough = empty_text_sensor_ns.class_(
    'ShysM5Tough', text_sensor.TextSensor, cg.Component)

CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ShysM5Tough),
    cv.Optional(CONF_NAME, default=DEFAULT_NAME): cv.string,
    cv.Optional(CONF_INIT_SOUND, default=DEFAULT_INIT_SOUND): cv.boolean,
    cv.Optional(CONF_LOGIN_ENABLED, default=DEFAULT_LOGIN_ENABLED): cv.boolean,
    cv.Optional(CONF_BUTTONS, default=[]): cv.All([dict({
        cv.Required(CONF_BUTTON_X): cv.int_range(0, 240),
        cv.Required(CONF_BUTTON_Y): cv.int_range(0, 320),
        cv.Required(CONF_BUTTON_WIDTH): cv.int_range(0, 240),
        cv.Required(CONF_BUTTON_HEIGHT): cv.int_range(0, 320),
        cv.Required(CONF_BUTTON_TEXT): cv.string,
        cv.Optional(CONF_BUTTON_FONTSIZE, default=3): cv.int_range(0, 10),
        cv.Optional(CONF_BUTTON_COLOR, default=DEFAULT_BUTTON_COLOR): cv.string,
        cv.Optional(CONF_BUTTON_TEXT_COLOR, default=DEFAULT_BUTTON_TEXT_COLOR): cv.string,
        cv.Optional(CONF_BUTTON_BG_COLOR, default=DEFAULT_BUTTON_BG_COLOR): cv.string,

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
                                    btn[CONF_BUTTON_FONTSIZE],
                                    btn[CONF_BUTTON_COLOR],
                                    btn[CONF_BUTTON_TEXT_COLOR],
                                    btn[CONF_BUTTON_BG_COLOR]))
    else:
        cg.add(var.createButton(10,
                                10,
                                220,
                                300,
                                "KLICK",
                                5))
