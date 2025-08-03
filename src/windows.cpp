#include "windows.h"

#include "event_manager.h"
#include "cursor.h"

#include <windows.h>

namespace N
{
  WindowsAssert::WindowsAssert(const bool& p_assertion, const std::string& p_msg, const bool p_trigger) : assertion(p_assertion)
  {
    if (!font.loadFromFile("assets/fonts/consola.ttf")) throw;

    if (!bgImg.loadFromFile("assets/images/error_background.jpg")) throw;

    if (!img.loadFromFile("assets/images/error.png")) throw;

    bgTexture.loadFromImage(bgImg);
    bgSprite.setTexture(bgTexture, true);

    texture.loadFromImage(img);
    sprite.setTexture(texture, true);
    sprite.setPosition((bgSprite.getLocalBounds().width - sprite.getLocalBounds().width) / 2.f, 15.f);

    text = sf::Text(p_msg, font, 18);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color(255, 255, 0));
    text.setPosition(15.f, sprite.getPosition().y + sprite.getLocalBounds().height + 15.f);

    if (p_trigger) trigger();
  }

  WindowsAssert::~WindowsAssert() {}

  void WindowsAssert::setAssertion(const bool& p_assertion)
  {
    assertion = p_assertion;
  }

  void WindowsAssert::trigger()
  {
    if (!assertion)
    {
      render.create(sf::VideoMode(858,570), "Narwhal Bug");
      render.requestFocus();

      while (render.isOpen())
      {
        EventManager::GetInstance()->bind(&render);
        if (EventManager::GetInstance()->isCloseTriggered() || EventManager::GetInstance()->isKeyPressed(sf::Keyboard::Escape, true))
        {
          render.close();
        }

        render.clear();
        render.draw(bgSprite);
        render.draw(sprite);
        render.draw(text);

        Cursor::GetInstance()->drawOnWindow(&render);
        render.display();
      }
    }
  }
}
