#pragma once

enum class GUI_EventType { None, Click, Release, Hover, Leave };

struct ClickCoordinates {
    float x, y;
};

struct GUI_Event {
    GUI_EventType _type;
    const char* _element;
    const char* _interface;
    union {
        ClickCoordinates _clickCoords;
    };
};
