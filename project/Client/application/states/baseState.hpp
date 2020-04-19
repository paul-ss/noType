#include <memory>

#include <SFML/System.hpp>

class StateManager;

class BaseState {
    friend class StateManager;
    public:
        BaseState(std::shared_ptr<StateManager> stateManager) :
                _stateMgr(stateManager),
                _transparent(false),
                _transcendent(false) {}

        virtual ~BaseState() = default;
        virtual void onCreate() = 0;
        virtual void onDestroy() = 0;
        virtual void activate() = 0;
        virtual void deactivate() = 0;
        virtual void update(const sf::Time& time) = 0;
        virtual void draw() = 0;
        void setTransparent(const bool& transparent) {
            _transparent = transparent;
        }
        bool isTransparent()const {
            return _transparent;
        }
        void setTranscendent(const bool& transcendence) {
        _transcendent = transcendence;
        }
        bool isTranscendent() const {
            return _transcendent;
        }
        std::shared_ptr<StateManager> getStateManager() {
            return _stateMgr;
        }

    protected:
        std::shared_ptr<StateManager> _stateMgr;
        bool _transparent;
        bool _transcendent;
};
