#ifndef WINDOWS_H
#define WINDOWS_H 1

#include "main.h"

#include <SFML/Graphics.hpp>

namespace N
{
  class WindowsAssert
  {
  public:
    WindowsAssert(const bool& p_assertion, const std::string& p_msg, const bool p_trigger = true);
    virtual ~WindowsAssert();

    void trigger();
    void setAssertion(const bool& p_assertion);

  protected:
    sf::RenderWindow render;
    sf::Text text;
    sf::Event event;
    sf::Font font;

    sf::Image bgImg;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    sf::Image img;
    sf::Texture texture;
    sf::Sprite sprite;

    bool assertion;
  };
}

#endif // WINDOWS_H
