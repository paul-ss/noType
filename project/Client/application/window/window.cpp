#include "window.hpp"

#define GAME_NAME "noType"
#define HEIGHT 640
#define WEIGHT 640
#define FPS 60
#define BITS_PER_PIXEL 32

Window::Window() :
        _window(std::make_shared<sf::RenderWindow>()),
        _eventManager(std::make_shared<EventManager>()) {

    setup("noType", sf::Vector2u(HEIGHT, WEIGHT));
}

Window::Window(const std::string& title, const sf::Vector2u& size) :
        _window(std::make_shared<sf::RenderWindow>()),
        _eventManager(std::make_shared<EventManager>()) {

    setup(title, size);
}

Window::~Window() {
    destroy();
}

void Window::setup(const std::string& title, const sf::Vector2u& size) {
    _isFocused = true;
    _windowTitle = title;
    _windowSize = size;
    _isFullScreen = false;
    _isDone = false;
    _window->setFramerateLimit(FPS);

    auto lambdaClose = [this](EventDetails& details) { this->Close(); };
    _eventManager->AddCallback(StateType(0), "Window_close", lambdaClose);

    auto lambdaToggleFullscreen = [this](EventDetails& details) { this->ToggleFullscreen(); };
    _eventManager->AddCallback(StateType(0), "Fullscreen_toggle", lambdaToggleFullscreen);
    create();
}

void Window::create() {
    auto style = (_isFullScreen ? sf::Style::Fullscreen : sf::Style::Default);
    if (style == sf::Style::Default) {
        _window->create({ _windowSize.x, _windowSize.y, BITS_PER_PIXEL },
                _windowTitle, style);
    } else {
        _window->create(sf::VideoMode::getFullscreenModes()[0], _windowTitle, sf::Style::Fullscreen);
    }
}

void Window::destroy() {
    _window->close();
}

void Window::Update() {
    sf::Event event;
    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::LostFocus) {
            _isFocused = false;
            _eventManager->SetFocus(false);
        } else if (event.type == sf::Event::GainedFocus){
            _isFocused = true;
            _eventManager->SetFocus(true);
        }
        _eventManager->HandleEvent(event);
    }
    _eventManager->Update();
}

void Window::ToggleFullscreen() {
    _isFullScreen = !_isFullScreen;
    _window->close();
    create();
}

void Window::BeginDraw() {
    _window->clear(sf::Color::Black);
}

void Window::EndDraw() {
    _window->display();
}

bool Window::IsDone() {
    return _isDone;
}

bool Window::IsFullScreen() {
    return _isFullScreen;
}

std::weak_ptr<sf::RenderWindow> Window::GetRenderWindow() {
    return _window;
}

sf::Vector2u Window::GetWindowSize() {
    return _windowSize;
}

std::weak_ptr<EventManager> Window::GetEventManager() {
    return _eventManager;
}

void Window::Draw(sf::Drawable& l_drawable) {
    _window->draw(l_drawable);
}

void Window::Close() {
    _isDone = true;
}
