#ifndef CURSOR_H
#define CURSOR_H 1

#include "utils.h"
#include <mutex>

#define CURSOR_NORMAL {0, 0, 25, 26} //(px^4)
#define CURSOR_ONTEXT {25, 0, 10, 26} //(px^4)

namespace N
{
  enum CursorStatus
  {
    NORMAL, ONTEXT
  };

  class Cursor
  {
  private:
    static Cursor* instance;
    static std::mutex mutex;

  protected:
    Cursor();
    ~Cursor();

    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    CursorStatus status;

    void triggerNormal();
    void triggerOnText();

  public:
    Cursor(Cursor& other) = delete;
    void operator=(const Cursor&) = delete;

    static Cursor *GetInstance();

    sf::Sprite get() const;
    bool point(sf::Sprite other, bool isTextForm = false);
    void drawOnWindow(sf::RenderWindow* window);
    void reset();
  };
}

#endif // CURSOR_H
