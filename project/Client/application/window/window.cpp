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

    // NOTE(vendroid): Нобходимо чтобы сигнатура функций совпадала, поэтому объявляем тут переменную пустышку
    auto lambdaClose = [this](std::shared_ptr<EventDetails> details) { this->close(); details = nullptr; };
    _eventManager.addCallback("Window_close", lambdaClose);

    auto lambdaToggleFullscreen = [this](std::shared_ptr<EventDetails> details) { this->toggleFullscreen(details); };
    _eventManager.addCallback("Fullscreen_toggle", lambdaToggleFullscreen);
    create();
}

void Window::create() {
    auto style = (_isFullScreen ? sf::Style::Fullscreen : sf::Style::Default);
    _window.create({ _windowSize.x, _windowSize.y, 32 }, _windowTitle, style);
}

void Window::destroy() {
    _window.close();
}

void Window::update() {
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::LostFocus) {
            _isFocused = false;
            _eventManager.setFocus(false);
        } else if (event.type == sf::Event::GainedFocus){
            _isFocused = true;
            _eventManager.setFocus(true);
        }
        _eventManager.handleEvent(event);
    }
    _eventManager.update();
}

void Window::toggleFullscreen(EventDetails* details = nullptr) {
    details = nullptr;
}

void Window::beginDraw() {
    _window.clear(sf::Color::Black);
}

void Window::endDraw() {
    _window.display();
}

bool Window::isDone() {
    return _isDone;
}

bool Window::isFullScreen() {
    return _isFullScreen;
}

sf::Vector2u Window::getWindowSize() {
    return _windowSize;
}

void Window::draw(sf::Drawable& drawable) {
    _window.draw(drawable);
}

void Window::close() {
    _isDone = true;
}
