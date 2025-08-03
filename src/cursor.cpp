#include "cursor.h"
#include <windows.h>

namespace N
{
  Cursor* Cursor::instance{nullptr};
  std::mutex Cursor::mutex;

  Cursor::Cursor() : status(CursorStatus::NORMAL)
  {
    image.loadFromFile("assets/images/cursor.png");
    image.createMaskFromColor(sf::Color(000,255,000));

    texture.loadFromImage(image);

    sprite.setTexture(texture);
    sprite.setTextureRect(CURSOR_NORMAL);
  }

  Cursor::~Cursor() {}

  Cursor* Cursor::GetInstance()
  {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr)
    {
      instance = new Cursor();
    }
    return instance;
  }

  void Cursor::triggerNormal()
  {
    sprite.setTextureRect(CURSOR_NORMAL);
    status = CursorStatus::NORMAL;
  }

  void Cursor::triggerOnText()
  {
    sprite.setTextureRect(CURSOR_ONTEXT);
    status = CursorStatus::ONTEXT;
  }

  void Cursor::reset() { triggerNormal(); }

  sf::Sprite Cursor::get() const
  {
    return sprite;
  }

  bool Cursor::point(sf::Sprite other, bool isTextForm)
  {
    bool point = contact(sprite.getPosition(), sf::FloatRect(1.0f,1.0f,1.0f,1.0f), other.getPosition(), other.getLocalBounds());
    if (isTextForm)
    {
      if (point)
      {
        triggerOnText();
      }
      else
      {
        triggerNormal();
      }
    }
    else
    {
      if (point)
      {
        triggerNormal();
      }
    }
    return point;
  }

  void Cursor::drawOnWindow(sf::RenderWindow* window)
  {
    sprite.setPosition((sf::Vector2f)sf::Mouse::getPosition(*window));
    window->draw(sprite);
  }
}
