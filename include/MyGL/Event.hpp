/** @file */
#ifndef MY_EVENT
#define MY_EVENT

#include "mygl_export.h"
#include <glm/glm.hpp>

namespace my
{

    /**
     * @brief Types that an event can have
    */
    enum class EventType {
        /**
         * @brief Event triggered when a key is pressed
         * 
         * This event is triggered when a keyboard key (including modifier keys) has
         * been pressed. It will set the event.type to keyPressed, event.keyCode to the
         * corresponding key code and provide information about the modifier keys' state
        */
        keyPressed,

        /**
         * @brief Event triggered when a key is released
         *
         * This event is triggered when a keyboard key (including modifier keys) was pressed and has
         * been released. It will set the event.type to keyReleased, event.keyCode to the
         * corresponding key code and provide information about the modifier keys' state
        */
        keyReleased,

        /**
         * @brief Event triggered when a key is held down
         *
         * This event is triggered when a keyboard key (including modifier keys) is held
         * down longer than the os' repeat delay, and it is triggered until the key is released.
         * It will set the event.type to keyRepeated, event.keyCode to the corresponding
         * key code and provide information about the modifier keys' state
        */
        keyRepeated,

        /**
         * @brief Event triggered when the mouse cursor moves inside the window
         * 
         * This event is triggered when the cursor's position changes. It sets event.type to
         * mouseMoved, event.mouse.pos to the new x and y positions relative to the window's
         * bottom left corner, and does not provide information about the modifier keys
        */
        mouseMoved,

        /**
         * @brief Event triggered when a mouse button is pressed
         * 
         * This event is triggered when a mouse button has been pressed. It sets event.type
         * to mouseButtonPressed, event.mouseButton to one of 'left', 'right' or 'middle' for
         * traditionnal mouse buttons, and one of 'extra_1' to 'extra_5' for additional mouse
         * buttons. It provides information about the modifier keys' state
        */
        mouseButtonPressed,

        /**
         * @brief Event triggered when a mouse button is released
         *
         * This event is triggered when a mouse button is released after being pressed. It sets
         * event.type to mouseButtonReleased, event.mouseButton to one of 'left', 'right' or 'middle'
         * for traditionnal mouse buttons, and one of 'extra_1' to 'extra_5' for additional mouse
         * buttons. It provides information about the modifier keys' state
        */
        mouseButtonReleased,

        /**
         * @brief Event triggered when scrolling (mouse wheel movement or touchpad gesture)
         * 
         * This event is triggered when the mouse wheel has been scrolled or a touchpad scrolling
         * gesture happened. It sets event.type to mouseScrolled, event.scrollOffset to the number
         * of ticks that has been scrolled verticaly (y) and horizontaly (x). It does not provide
         * information about the modifier keys
        */
        mouseScrolled,

        /**
         * @brief Event triggered when the cursor enters the window's content area
         * 
         * This event is triggered when the cursor enters the window's content area.
         * It sets event.type to cursorEntered, event.keyCode to unknown as it does
         * not provide any additional informations, and does not give information
         * about the modifier keys
        */
        cursorEntered,

        /**
         * @brief Event triggered when the cursor leaves the window's content area
         *
         * This event is triggered when the cursor leaves the window's content area.
         * It sets event.type to cursorLeft, event.keyCode to unknown as it does
         * not provide any additional informations, and does not give information
         * about the modifier keys
        */
        cursorLeft,

        /**
         * @brief Event triggered when the window is resized
        */
        windowResized,

        /**
         * @brief Event triggered when the user tries to close the window
        */
        windowShouldClose,

        /**
         * @brief Special value used to signal that an event variable is not associated to
         *        any event
        */
        unknown
    };

    /**
     * @brief Key codes for every key of a fullsize US qwerty layout
     * 
     * The my::Key enum is used in keyboard events to indicate the involved key,
     * the keys correspond to the ones of a full size US layout keyboard.
     * - Keys situated on the number pad have the 'np' prefix. ('np0' to 'np9', 'np_divide', ...)
     * - Duplicated keys such as alt or shift are prefixed with their location 
     * (e.g. 'left_shift', 'right_alt').
     * - Function keys have the 'f' prefix ('f1', f2', ...)
     * - Number keys of the main area have the 'n' prefix ('n0' to 'n9')
    */
    enum class Key {
        unknown = -1, space = 32, apostrophe = 39, comma = 44, minus, period, slash,
        n0, n1, n2, n3, n4, n5, n6, n7, n8, n9,
        semicolon = 59, equal = 61,
        a = 65, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
        left_bracket, backslash, right_bracket, grave_accent = 96, non_us1 = 161, non_us2,
        escape = 256, enter, tab, backspace, insert, del,
        right, left, down, up,
        page_up, page_down, home, end, caps_lock = 280, scroll_lock, num_lock, print_screen, pause,
        f1 = 290, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25,
        np0 = 320, np1, np2, np3, np4, np5, np6, np7, np8, np9, np_dot, np_divide, np_multiply, np_substract, np_add, np_enter, np_equal,
        left_shift = 340, left_ctrl, left_alt, left_os, right_shift, right_ctrl, right_alt, right_os, menu
    };

    /**
     * @brief Codes for mouse buttons (used in mouseButtonPressed / Released events)
    */
    enum class MouseButton {
        left, right, middle, extra_1, extra_2, extra_3, extra_4, extra_5, none
    };

    /**
     * @brief Mouse inforamtions for events involving mouse buttons 
     * 
     * This structure is used to store the informations of mouse events. It
     * holds the button that was pressed in case of a mouseButtonPressed/
     * released event and the cursor's position.
    */
    struct MouseInfo {
        MouseButton button;
        glm::vec2 pos;
    };

    /**
     * @brief Struct for representing events and informations associated with them
     * 
     * This structure holds information about an event, it is composed of a type, an
     * information and the state of the modifier keys.
     * 
     * @warning As the information is coded using an union, even though the event has 
     * multiple fields, only one is active at a time, so reading from the others is undefined.
     * For example reading the mouse value of a keyPressed event is undefined behaviour.
     * The information for each type of event is documented in the EventType enum documentation.
    */
    struct MYGL_EXPORT Event {

        /**
         * @brief A struct for storing the state of the keyboard modifier keys
        */
        struct Modifiers {
            bool shift = false;
            bool ctrl = false;
            bool alt = false;
            bool os = false;
            bool caps_lock = false;
            bool num_lock = false;

            Modifiers() = default;
        };

        /**
         * @brief The event's type
        */
        EventType type;

        union {
            /**
             * @brief The key code associated with a keyPressed, keyReleased or 
             *        keyRepeated event
            */
            Key keyCode;
            /**
             * @brief The mouse informations associated with mouse events
            */
            MouseInfo mouse;
            /**
             * @brief The scroll wheel offset in a mouseScrolled event
             * 
             * 'x' is the horizontal offset, and 'y' the vertical offset (a positive
             * value indicates an upward movement).
            */
            glm::ivec2 scrollOffset;
            /**
             * @brief The new size of a window which have been resized
             * 
             * 'x' is the new width and 'y' the new height.
            */
            glm::ivec2 windowSize;
        };

        /**
         * @brief A variable storing the state of the modifier keys
         * 
         * This variable stores the state of the modifer keys (alt, shift, ctr, ...)
         * when the event was triggered. Note that not every type of event reports the
         * modifier keys' state
        */
        Modifiers mods;

        Event();
    };
}

#endif // MY_EVENT