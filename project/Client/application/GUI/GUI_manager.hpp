#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>

#include "GUI_interface.hpp"
#include "GUI_event.hpp"
#include "eventManager.hpp"
#include "sharedContext.hpp"

using GUI_Frame = std::pair<std::shared_ptr<GUI_Interface>, std::vector<std::shared_ptr<GUI_Element>>>;
using GUI_Interfaces = std::unordered_map<std::string, GUI_Frame>;
using GUI_Container = std::unordered_map<StateType, GUI_Interfaces>;
using GUI_Events = std::unordered_map<StateType, std::vector<GUI_Event>>;
using GUI_Factory = std::unordered_map<GUI_ElementType,
        std::function<std::shared_ptr<GUI_Element>(std::string&)>>;
using GUI_ElemTypes = std::unordered_map <std::string, GUI_ElementType>;

struct SharedContext;
class GUI_Manager {
    friend class GUI_Interface;

public:
    GUI_Manager(std::weak_ptr<EventManager> l_eMgr, std::weak_ptr<SharedContext> l_shared);
    ~GUI_Manager();

    bool AddInterface(const StateType& l_state,
        const std::string& l_name);
    std::weak_ptr<GUI_Interface> GetInterface(const StateType& l_state,
        const std::string& l_name);
    bool RemoveInterface(const StateType& l_state,
        const std::string& l_name);

    bool LoadInterface(const StateType& l_state,
        const std::string& l_interface, const std::string& l_name);

    void SetCurrentState(const StateType& l_state);

    std::weak_ptr<SharedContext> GetContext();
    std::weak_ptr<GUI_Interface> GetFrame();

    void DefocusAllInterfaces();

    void HandleClick(EventDetails& l_details);
    void HandleRelease(EventDetails& l_details);
    void HandleTextEntered(EventDetails& l_details);

    void AddEvent(GUI_Event l_event);
    bool PollEvent(GUI_Event& l_event);

    void Update(float l_dT);
    void Render(std::weak_ptr<sf::RenderWindow> l_window);

private:
    std::shared_ptr<GUI_Element> createElement(const GUI_ElementType& l_id);

    GUI_ElementType stringToType(const std::string& l_string);
    bool loadStyle(const std::string& l_file, std::weak_ptr<GUI_Element> l_element);

    template<class T>
    void registerElement(const GUI_ElementType& l_id) {
        _factory[l_id] = [](const string& l_name) -> std::shared_ptr<GUI_Element> {
            return std::make_shared<T>(l_name);
        };
    }

private:
    GUI_Container _guiContainer;
    GUI_Events _events;
    StateType _currentState;
    GUI_Factory _factory;
    GUI_ElemTypes _elemTypes;

private:
    std::weak_ptr<SharedContext> _context;
    std::weak_ptr<EventManager> _eventMgr;
};
