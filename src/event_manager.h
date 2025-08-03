#ifndef EVENT_H
#define EVENT_H 1

#include "main.h"
#include <mutex>

namespace N
{
  class EventManager
  {
  private:
    static EventManager* instance;
    static std::mutex mutex;

  protected:
    EventManager();
    ~EventManager();

    sf::Event event;

    bool* key;
    bool* keypressed;

    bool* button;
    bool* buttonpressed;

    bool closetriggered;
    bool textentered;
    bool textenteredonce;
    bool unknownevent;

  public:
    EventManager(EventManager& other) = delete;
    void operator=(const EventManager&) = delete;

    static const constexpr int keyCount = sf::Keyboard::KeyCount;
    static const constexpr int buttonCount = sf::Mouse::ButtonCount;

    static EventManager* GetInstance();

    void bind(sf::RenderWindow* window);

    bool isKeyPressed(int keycode, bool once = false);
    bool isButtonPressed(int buttoncode, bool once = false);
    bool isCloseTriggered() const;
    bool isTextEntered(bool once = false);
    bool aKeyPressedButEscape() const;
    bool isUnknownEvent() const;

    sf::Uint32 lastUnicode() const;
    sf::Uint32 lastKeycode() const;
  };
}

#endif // EVENT_H
