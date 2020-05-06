#pragma once

#include <unordered_map>
#include <functional>

#include <SFML/Graphics.hpp>

#include "GUI_interface.hpp"
#include "GUI_event.hpp"
#include "eventManager.hpp"
#include "sharedContext.hpp"

using GUI_Interfaces = std::unordered_map<std::string, GUI_Interface*>;
using GUI_Container = std::unordered_map<StateType, GUI_Interfaces>;
using GUI_Events = std::unordered_map<StateType, std::vector<GUI_Event>>;
using GUI_Factory = std::unordered_map<GUI_ElementType, std::function<GUI_Element*(GUI_Interface*)>>;
using GUI_ElemTypes = std::unordered_map <std::string, GUI_ElementType>;

struct SharedContext;
class GUI_Manager {
    friend class GUI_Interface;

    public:
        explicit GUI_Manager(EventManager* evMgr, SharedContext* shared);
        ~GUI_Manager();

        bool AddInterface(const StateType& state,
            const std::string& name);
        GUI_Interface* GetInterface(const StateType& state,
            const std::string& name);
        bool RemoveInterface(const StateType& state,
            const std::string& name);

        bool LoadInterface(const StateType& state,
            const std::string& interface, const std::string& name);

        void SetCurrentState(const StateType& state);

        SharedContext* GetContext();

        void DefocusAllInterfaces();

        void HandleClick(EventDetails* details);
        void HandleRelease(EventDetails* details);
        void HandleTextEntered(EventDetails* details);

        void AddEvent(GUI_Event event);
        bool PollEvent(GUI_Event& event);

        void Update(float dT);
        void Render(sf::RenderWindow* window);

    private:
        GUI_Element* createElement(const GUI_ElementType& id, GUI_Interface* owner);
        GUI_ElementType stringToType(const std::string& string);
        bool loadStyle(const std::string& file, GUI_Element* element);

        template<class T>
        void registerElement(const GUI_ElementType& id) {
            _factory[id] = [](GUI_Interface* owner) -> GUI_Element* {
                return new T("", owner);
            };
        }

    private:
        GUI_Container _interfaces;
        GUI_Events _events;
        SharedContext* _context;
        EventManager* _eventMgr;
        StateType _currentState;
        GUI_Factory _factory;
        GUI_ElemTypes _elemTypes;
};
