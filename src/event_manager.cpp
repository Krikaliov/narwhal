#include "event_manager.h"
#include <iostream>

namespace N
{
  EventManager* EventManager::instance{nullptr};
  std::mutex EventManager::mutex;

  EventManager::EventManager() : closetriggered(false), textentered(false), textenteredonce(false), unknownevent(false)
  {
    key = new bool[sf::Keyboard::KeyCount];
    keypressed = new bool[sf::Keyboard::KeyCount];
    button = new bool[sf::Mouse::ButtonCount];
    buttonpressed = new bool[sf::Mouse::ButtonCount];

    for (int i = 0 ; i < sf::Keyboard::KeyCount ; i++)
    {
      key[i] = false;
      keypressed[i] = false;
    }

    for (int i = 0 ; i < sf::Mouse::ButtonCount ; i++)
    {
      button[i] = false;
      buttonpressed[i] = false;
    }
  }

  EventManager::~EventManager() 
  {
    delete key;
    delete keypressed;
    delete button;
    delete buttonpressed;
  }

  EventManager* EventManager::GetInstance()
  {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr)
    {
      instance = new EventManager();
    }
    return instance;
  }

  void EventManager::bind(sf::RenderWindow* window)
  {
    closetriggered = false;
    textentered = false;
    unknownevent = false;

    while (window->pollEvent(event))
    {
      switch (event.type)
      {
        case sf::Event::Closed:
          closetriggered = closetriggered || true;
          break;
        case sf::Event::TextEntered:
          textentered = textentered || true;
          break;
        case sf::Event::KeyPressed:
          key[event.key.code] = true;
          break;
        case sf::Event::KeyReleased:
          key[event.key.code] = false;
          keypressed[event.key.code] = false;
          break;
        case sf::Event::MouseButtonPressed:
          button[event.mouseButton.button] = true;
          break;
        case sf::Event::MouseButtonReleased:
          button[event.mouseButton.button] = false;
          buttonpressed[event.mouseButton.button] = false;
          break;
        default:
          unknownevent = unknownevent || true;
          break;
      }
    }
  }

  bool EventManager::isKeyPressed(int keycode, bool once)
  {
    bool wasPressed = keypressed[keycode];
    keypressed[keycode] = true;
    return (once) ? (key[keycode] && !wasPressed) : key[keycode];
  }

  bool EventManager::isButtonPressed(int buttoncode, bool once)
  {
    bool wasPressed = buttonpressed[buttoncode];
    buttonpressed[buttoncode] = true;
    return (once) ? (button[buttoncode] && !wasPressed) : button[buttoncode];
  }

  bool EventManager::isCloseTriggered() const
  {
    return closetriggered;
  }

  bool EventManager::isTextEntered(bool once)
  {
    bool wasTextEntered = textenteredonce;
    textenteredonce = true;
    return (once) ? (textentered && !wasTextEntered) : textentered;
  }

  bool EventManager::aKeyPressedButEscape() const
  {
    return event.type == sf::Event::KeyPressed && event.key.code != sf::Keyboard::Escape;
  }

  bool EventManager::isUnknownEvent() const
  {
    return unknownevent;
  }

  sf::Uint32 EventManager::lastUnicode() const
  {
    return event.text.unicode;
  }

  sf::Uint32 EventManager::lastKeycode() const
  {
    return event.key.code;
  }
}
