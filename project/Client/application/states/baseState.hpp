#pragma once

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

struct SharedContext;
class StateManager;
class Window;
class EventManager;
enum class ElementName;
class BaseElement;
class StateManger;
namespace Network {
    enum class Status;
    class INetworkManager;
    namespace Client {
        class IQueueManager;
    }
}

class BaseState {
    friend class StateManager;

public:
    explicit BaseState(std::weak_ptr<SharedContext> l_context);

    virtual ~BaseState() = default;

    virtual void OnCreate() = 0;
    virtual void OnDestroy() = 0;

    virtual void Activate() = 0;
    virtual void Deactivate() = 0;

    virtual void Update(const sf::Time& l_time) = 0;
    virtual void Draw() = 0;

public:
    void SetTransparent(const bool& l_transparent);
    bool IsTransparent() const;

    void SetTranscendent(const bool& l_transcendence);
    bool IsTranscendent() const;

    std::shared_ptr<SharedContext> GetSharedContext();
    std::shared_ptr<StateManager> GetStateManager();
    std::shared_ptr<EventManager> GetEventManager();
    std::shared_ptr<Window> GetWindow();
    std::shared_ptr<sf::RenderWindow> GetRenderWindow();
    std::shared_ptr<Network::Client::IQueueManager> GetQueueManager();
    std::shared_ptr<Network::INetworkManager> GetNetworkManager();

protected:
    void checkNetStatus(const Network::Status l_status, const std::string& l_error);
    void drawElement(ElementName l_name);

protected:
    bool _transparent;
    bool _transcendent;
    std::weak_ptr<SharedContext> _context;
    std::unordered_map<ElementName, std::shared_ptr<BaseElement>> _elements;
};
