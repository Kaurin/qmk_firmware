#include QMK_KEYBOARD_H
#include "version.h"

extern keymap_config_t keymap_config;

enum layers {
    _L0 = 0,
    _L1,
    _L2,
    _L3,
    _L4,
    _L5,
    _L6,
    _L7,
    _L8,
    _L9
};

///// TWO RULES HERE:
// * DYNAMIC_MACRO_RANGE needs to go last.
// * First enum needs to have = SAFE_RANGE
enum custom_keycodes {
    KC_REV = SAFE_RANGE,
    KC_VIQ, // Vim quit without changes
    KC_VWQ, // Vim quit while saving changes
    KC_VSE, // Vim quit while saving changes
    KC_TST,
    DYNAMIC_MACRO_RANGE
};

extern backlight_config_t backlight_config;

#include "dynamic_macro.h"
#define FN_ZERO LT(_L9, KC_KP_0)
#define KC_DMR1 DYN_REC_START1
#define KC_DMR2 DYN_REC_START2
#define KC_DMP1 DYN_MACRO_PLAY1
#define KC_DMP2 DYN_MACRO_PLAY2
#define KC_DMRS DYN_REC_STOP

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*## Layout Config:
    *     .-----------.  .-----------.  .-----------.  .-----------.  .-----------.
    *     |7  |8  |9  |  |VSE|   |VWQ|  |   |   |   |  |   |   |   |  |   |   |   |
    *     |-----------|  |-----------|  |-----------|  |-----------|  |-----------|
    *     |4  |5  |6  |  |   |VIQ|   |  |   |   |   |  |   |   |   |  |   |   |   |
    *     |-----------|  |-----------|  |-----------|  |-----------|  |-----------|
    *     |1  |2  |3  |  |   |TST|   |  |   |   |   |  |   |   |   |  |   |   |   |
    *     |-----------|  |-----------|  |-----------|  |-----------|  |-----------|
    *     |#L0|_L9/0  |  |#L1|_L9    |  |#L2|_L9    |  |#L3|_L9    |  |#L4|_L9    |
    *     *-----------*  *-----------*  *-----------*  *-----------*  *-----------*
    *     .-----------.  .-----------.  .-----------.  .-----------.  .-----------.
    *     |   |   |   |  |   |   |   |  |MP1|   |MP2|  |   |   |RST|  |_L6|_L7|_L8|
    *     |-----------|  |-----------|  |-----------|  |-----------|  |-----------|
    *     |   |   |   |  |   |   |   |  |   |MRS|   |  |   |   |   |  |_L3|_L4|_L5|
    *     |-----------|  |-----------|  |-----------|  |-----------|  |-----------|
    *     |   |   |   |  |   |   |   |  |MR1|   |MR2|  |   |REV|   |  |_L0|_L1|_L2|
    *     |-----------|  |-----------|  |-----------|  |-----------|  |-----------|
    *     |#L5|_L9    |  |#L6|_L9    |  |#L7|_L9    |  |#L8|_L9    |  |#L9|       |
    *     *-----------*  *-----------*  *-----------*  *-----------*  *-----------*
    */
    [_L0] = {{KC_KP_7, KC_KP_8, KC_KP_9}, {KC_KP_4, KC_KP_5, KC_KP_6}, {KC_KP_1, KC_KP_2, KC_KP_3}, {XXXXXXX, XXXXXXX, FN_ZERO}},
    [_L1] = {{KC_VSE , _______, KC_VWQ }, {_______, KC_VIQ , _______}, {_______, KC_TST, _______}, {XXXXXXX, XXXXXXX, MO(_L9)}},
    [_L2] = {{_______, _______, _______}, {_______, _______, _______}, {_______, _______, _______}, {XXXXXXX, XXXXXXX, MO(_L9)}},
    [_L3] = {{_______, _______, _______}, {_______, _______, _______}, {_______, _______, _______}, {XXXXXXX, XXXXXXX, MO(_L9)}},
    [_L4] = {{_______, _______, _______}, {_______, _______, _______}, {_______, _______, _______}, {XXXXXXX, XXXXXXX, MO(_L9)}},
    [_L5] = {{_______, _______, _______}, {_______, _______, _______}, {_______, _______, _______}, {XXXXXXX, XXXXXXX, MO(_L9)}},
    [_L6] = {{_______, _______, _______}, {_______, _______, _______}, {_______, _______, _______}, {XXXXXXX, XXXXXXX, MO(_L9)}},
    [_L7] = {{KC_DMP1, _______, KC_DMP2}, {_______, KC_DMRS, _______}, {KC_DMR1, _______, KC_DMR2}, {XXXXXXX, XXXXXXX, MO(_L9)}},
    [_L8] = {{_______, _______, RESET  }, {_______, _______, _______}, {_______, KC_REV , _______}, {XXXXXXX, XXXXXXX, MO(_L9)}},
    [_L9] = {{DF(_L6), DF(_L7), DF(_L8)}, {DF(_L3), DF(_L4), DF(_L5)}, {DF(_L0), DF(_L1), DF(_L2)}, {XXXXXXX, XXXXXXX, _______}},
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_REV:
            if (record->event.pressed) {
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " // " QMK_VERSION " // " QMK_BUILDDATE);
            }
            return false;
            break;
        case KC_VIQ: // VIM - quit without saving
            if (record->event.pressed) {
                SEND_STRING("\e:q!\n");
            }
            return false;
            break;
        case KC_VWQ: // VIM - quit and save
            if (record->event.pressed) {
                SEND_STRING("\e:wq\n");
            }
            return false;
            break;
        case KC_VSE: // VIM search
            if (record->event.pressed) {
                SEND_STRING("\e:%s///g");
                tap_code(KC_LEFT);
                tap_code(KC_LEFT);
                tap_code(KC_LEFT);
            }
            return false;
            break;
        case KC_TST:
            if (record->event.pressed) {
                SEND_STRING("Test Macro");
            }
            return false;
            break;
    }
    // Dynamic Macros.
    if (!process_record_dynamic_macro(keycode, record)) {
        return false;
    }
    return true;
}

void custom_backlight_level(uint8_t level) {
    if (level > BACKLIGHT_LEVELS)
        level = BACKLIGHT_LEVELS;
    backlight_config.level = level;
    backlight_config.enable = !!backlight_config.level;
    backlight_set(backlight_config.level);
}

void matrix_init_user(void) {
  #ifdef BACKLIGHT_ENABLE
    custom_backlight_level(0);
  #endif
}

void matrix_scan_user(void) {

}

uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
        case _L0:
            custom_backlight_level(0);
            break;
        case _L1:
            custom_backlight_level(1);
            break;
        case _L2:
            custom_backlight_level(2);
            break;
        case _L3:
            custom_backlight_level(3);
            break;
        case _L4:
            custom_backlight_level(4);
            break;
        case _L5:
            custom_backlight_level(5);
            break;
        case _L6:
            custom_backlight_level(6);
            break;
        case _L7:
            custom_backlight_level(6);
            break;
        case _L8:
            custom_backlight_level(6);
            break;
        case _L9:
            custom_backlight_level(0);
            break;
        default:
            custom_backlight_level(0);
            break;
    }
    return state;
}

void led_set_user(uint8_t usb_led) {

}
