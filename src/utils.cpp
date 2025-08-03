#include "utils.h"
#include "strutils.h"

using namespace std;
using namespace sf;

namespace N
{
  Image grayscale(Image src)
  {
    Image grayscaled;

    int x = src.getSize().x;
    int y = src.getSize().y;

    grayscaled.create(x, y);

    for (int i = 0 ; i < x ; i++)
    {
      for (int j = 0 ; j < y ; j++)
      {
        Color o = src.getPixel(i, j);
        Uint8 c = (o.b + o.g + o.r) / 3;
        // Preserve the original alpha value because
        // it is not taken account on grayscaling.
        grayscaled.setPixel(i, j, Color(c, c, c, o.a));
      }
    }
    return grayscaled;
  }

  string keystring(unsigned int k)
  {
    switch (k)
    {
      case Keyboard::A:
        return "A";
        break;
      case Keyboard::B:
        return "B";
        break;
      case Keyboard::C:
        return "C";
        break;
      case Keyboard::D:
        return "D";
        break;
      case Keyboard::E:
        return "E";
        break;
      case Keyboard::F:
        return "F";
        break;
      case Keyboard::G:
        return "G";
        break;
      case Keyboard::H:
        return "H";
        break;
      case Keyboard::I:
        return "I";
        break;
      case Keyboard::J:
        return "J";
        break;
      case Keyboard::K:
        return "K";
        break;
      case Keyboard::L:
        return "L";
        break;
      case Keyboard::M:
        return "M";
        break;
      case Keyboard::N:
        return "N";
        break;
      case Keyboard::O:
        return "O";
        break;
      case Keyboard::P:
        return "P";
        break;
      case Keyboard::Q:
        return "Q";
        break;
      case Keyboard::R:
        return "R";
        break;
      case Keyboard::S:
        return "S";
        break;
      case Keyboard::T:
        return "T";
        break;
      case Keyboard::U:
        return "U";
        break;
      case Keyboard::V:
        return "V";
        break;
      case Keyboard::W:
        return "W";
        break;
      case Keyboard::X:
        return "X";
        break;
      case Keyboard::Y:
        return "Y";
        break;
      case Keyboard::Z:
        return "Z";
        break;
      case Keyboard::Add:
        return "+";
        break;
      case Keyboard::BackSpace:
        return "Backspace";
        break;
      case Keyboard::Comma:
        return ",";
        break;
      case Keyboard::BackSlash:
        return "\\";
        break;
      case Keyboard::Delete:
        return "Delete";
        break;
      case Keyboard::Divide:
        return "/";
        break;
      case Keyboard::Down:
        return "Down";
        break;
      case Keyboard::Equal:
        return "=";
        break;
      case Keyboard::Insert:
        return "Insert";
        break;
      case Keyboard::Up:
        return "Up";
        break;
      case Keyboard::Tilde:
        return "%";
        break;
      case Keyboard::Tab:
        return "Tab";
        break;
      case Keyboard::Subtract:
        return "-";
        break;
      case Keyboard::Multiply:
        return "*";
        break;
      case Keyboard::Return:
        return "Return";
        break;
      case Keyboard::Num0:
        return "0";
        break;
      case Keyboard::Num1:
        return "1";
        break;
      case Keyboard::Num2:
        return "2";
        break;
      case Keyboard::Num3:
        return "3";
        break;
      case Keyboard::Num4:
        return "4";
        break;
      case Keyboard::Num5:
        return "5";
        break;
      case Keyboard::Num6:
        return "6";
        break;
      case Keyboard::Num7:
        return "7";
        break;
      case Keyboard::Num8:
        return "8";
        break;
      case Keyboard::Num9:
        return "9";
        break;
      case Keyboard::PageDown:
        return "Page down";
        break;
      case Keyboard::PageUp:
        return "Page up";
        break;
      case Keyboard::Numpad0:
        return "NUMPAD 0";
        break;
      case Keyboard::Numpad1:
        return "NUMPAD 1";
        break;
      case Keyboard::Numpad2:
        return "NUMPAD 2";
        break;
      case Keyboard::Numpad3:
        return "NUMPAD 3";
        break;
      case Keyboard::Numpad4:
        return "NUMPAD 4";
        break;
      case Keyboard::Numpad5:
        return "NUMPAD 5";
        break;
      case Keyboard::Numpad6:
        return "NUMPAD 6";
        break;
      case Keyboard::Numpad7:
        return "NUMPAD 7";
        break;
      case Keyboard::Numpad8:
        return "NUMPAD 8";
        break;
      case Keyboard::Numpad9:
        return "NUMPAD 9";
        break;
      case Keyboard::Period:
        return ";";
        break;
      case Keyboard::Slash:
        return ":";
        break;
      case Keyboard::Dash:
        return "Dash";
        break;
      case Keyboard::SemiColon:
        return "Semicolon";
        break;
      case Keyboard::Right:
        return "Right";
        break;
      case Keyboard::Left:
        return "Left";
        break;
      case Keyboard::Space:
        return "Space";
        break;
      case Keyboard::End:
        return "End";
        break;
      case Keyboard::F1:
        return "F1";
        break;
      case Keyboard::F2:
        return "F2";
        break;
      case Keyboard::F3:
        return "F3";
        break;
      case Keyboard::F4:
        return "F4";
        break;
      case Keyboard::F5:
        return "F5";
        break;
      case Keyboard::F6:
        return "F6";
        break;
      case Keyboard::F7:
        return "F7";
        break;
      case Keyboard::F8:
        return "F8";
        break;
      case Keyboard::F9:
        return "F9";
        break;
      case Keyboard::F10:
        return "F10";
        break;
      case Keyboard::F11:
        return "F11";
        break;
      case Keyboard::F12:
        return "F12";
        break;
      case Keyboard::Quote:
        return "²";
        break;
      case Keyboard::Unknown:
        return "None";
        break;
      default:
        return "None";
        break;
    }
  }

  int numberLength(int n)
  {
    return (n < 0)
      ? (int)log10(max(1,abs(n)))+2
      : (int)log10(max(1,abs(n)))+1;
  }

  Int32 nextMonsterSpawnDate(Int32 numberSpawned)
  {
    /// The first arrives 3 seconds after the launch of the game.
    /// The next takes 5 milliseconds less long to spawn than the previous.
    return (3000-5*numberSpawned)*(numberSpawned+1);
  }

  Int32 nextBonusSpawnDate(Int32 numberSpawned)
  {
    return (30000-30*numberSpawned)*(numberSpawned+1);
  }

  Int32 nextEpolarSpawnDate(Int32 numberSpawned)
  {
    return (44000-50*numberSpawned)*(numberSpawned+1);
  }

  float objectif_y(Vector2f posA, Vector2f posB, float x)
  {
    float a = (posB.y - posA.y) / (posB.x - posA.x);
    float b = posA.y - a*posA.x;
    return a*x + b;
  }

  bool contact(Vector2f posA, FloatRect boundsA, Vector2f posB, FloatRect boundsB, float margin)
  {
    float _x_a = posA.x;
    float _x_b = posB.x;
    float _y_a = posA.y;
    float _y_b = posB.y;

    float _w_a = boundsA.width;
    float _w_b = boundsB.width;
    float _h_a = boundsA.height;
    float _h_b = boundsB.height;

    return (_x_a + _w_a >= _x_b + margin &&
            _x_a + margin <= _x_b + _w_b &&
            _y_a + _h_a >= _y_b + margin &&
            _y_a + margin <= _y_b + _h_b);
  }
}
