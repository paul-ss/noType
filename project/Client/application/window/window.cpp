#include "window.hpp"

#define HEIGHT 700
#define WEIGHT 700
#define FPS 60

Window::Window() {
    setup("noType", sf::Vector2u(HEIGHT, WEIGHT));
}

Window::Window(const std::string& title, const sf::Vector2u& size) {
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
    _window.setFramerateLimit(FPS);

    _eventManager.SetFocus(true);
    // NOTE(vendroid): Нобходимо чтобы сигнатура функций совпадала, поэтому объявляем тут переменную пустышку
    auto lambdaClose = [this](EventDetails& details) { this->Close(); };
    _eventManager.AddCallback(StateType(0), "Window_close", lambdaClose);

    auto lambdaToggleFullscreen = [this](EventDetails& details) { this->ToggleFullscreen(); };
    _eventManager.AddCallback(StateType(0), "Fullscreen_toggle", lambdaToggleFullscreen);
    create();
}

void Window::create() {
    auto style = (_isFullScreen ? sf::Style::Fullscreen : sf::Style::Default);
    _window.create({ _windowSize.x, _windowSize.y, 32 }, _windowTitle, style);
}

void Window::destroy() {
    _window.close();
}

void Window::Update() {
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::LostFocus) {
            _isFocused = false;
            _eventManager.SetFocus(false);
        } else if (event.type == sf::Event::GainedFocus){
            _isFocused = true;
            _eventManager.SetFocus(true);
        }
        _eventManager.HandleEvent(event);
    }
    _eventManager.Update();
}

void Window::ToggleFullscreen() {
    _isFullScreen = !_isFullScreen;
    Close();
    create();
}

void Window::BeginDraw() {
    _window.clear(sf::Color::Black);
}

void Window::EndDraw() {
    _window.display();
}

bool Window::IsDone() {
    return _isDone;
}

bool Window::IsFullScreen() {
    return _isFullScreen;
}

sf::RenderWindow* Window::GetRenderWindow() {
    return &_window;
}

sf::Vector2u Window::GetWindowSize() {
    return _windowSize;
}

EventManager* Window::GetEventManager() {
    return &_eventManager;
}

void Window::Draw(sf::Drawable& drawable) {
    _window.draw(drawable);
}

void Window::Close() {
    _isDone = true;
}
