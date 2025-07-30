#include "utils.h"
#include "strutils.h"

using namespace sf;
using namespace std;

int main()
{
  /// Network Management and File Transfer Protocol
  ifstream _sb_amont; // This is also used in order to get monster sprites and to get settings
  ofstream _sb_aval;
  TcpSocket _socket;
  IpAddress _gtaddress;
  Ftp _client;
  bool _connected(true);
  if (_socket.connect("62.4.29.249", 21) != Socket::Done)
    _connected = false;
  else
    _gtaddress = _socket.getRemoteAddress();
  /// Event Manager
  bool* _key = (bool*) malloc (sizeof(bool) * Keyboard::KeyCount);
  for ( int i = 0 ; i < Keyboard::KeyCount ; ++i) _key[i] = false;
  bool* _keypressed = (bool*) malloc (sizeof(bool) * Keyboard::KeyCount
                                     );
  for ( int i = 0 ; i < Keyboard::KeyCount ; ++i) _keypressed[i] = false
        ;
  bool* _button = (bool*) malloc (sizeof(bool) * Mouse::ButtonCount);
  for ( int i = 0 ; i < Mouse::ButtonCount ; ++i) _button[i] = false;
  bool* _buttonpressed = (bool*) malloc (sizeof(bool) * Mouse::
                                         ButtonCount);
  for ( int i = 0 ; i < Mouse::ButtonCount ; ++i) _buttonpressed[i] =
      false;
  bool _textEntered(false);
  Event _event;
  srand(static_cast<unsigned int>(time(NULL)));
  bool _playerIsInLauncher(true);
  bool _playerIsInGame(false);
  bool _playerIsInMenu(false);
  bool _playerIsInOption(false);
  bool _gameHasToReboot(true);
  vector<unsigned int> _keyid(NBR_KEYS, 0);
  /// Time Manager
  Int32 _t_fps = seconds(WIN_TICK).asMilliseconds();
  Int32 _t_passedInOption = 0;
  Int32 _t_passedInGame = 0;
  Int32 _t_passedInMenu = 0;
  Int32 _t_passedInLauncher = 0;
  Int32 _lasttime = 0; // Last duration of the loop
  /// Fonts
  // Initialization
  Font _policeFont;
  if (!_policeFont.loadFromFile("assets/fonts/angelina.ttf"))
  {
    return 1;
  }
  Font _basicFont;
  if (!_basicFont.loadFromFile("assets/fonts/consola.ttf"))
  {
    return 1;
  }
  Font _narwhalFont;
  if (!_narwhalFont.loadFromFile("assets/fonts/narwhal.ttf"))
  {
    return 1;
  }
  /// Error
  Image _errorBgdImg;
  Texture _errorBgdTxt;
  Sprite _errorBgd;
  _errorBgdImg.loadFromFile("assets/images/error_background.jpg");
  _errorBgdTxt.loadFromImage(_errorBgdImg);
  _errorBgd.setTexture(_errorBgdTxt, true);
  Image _errorImg;
  Texture _errorTxt;
  Sprite _error;
  _errorImg.loadFromFile("assets/images/error.png");
  _errorTxt.loadFromImage(_errorImg);
  _error.setTexture(_errorTxt, true);
  _error.setPosition((_errorBgd.getLocalBounds().width - _error.
                      getLocalBounds().width) /2.00f,
                     15.0f);
  Text _errorText("Impossible de jouer sans entrer de\npseudonyme !",
                  _basicFont, 18);
  _errorText.setStyle(Text::Bold);
  _errorText.setFillColor(Color(255,255,100));
  _errorText.setPosition(15.0f,
                         _error.getPosition().y + _error.getLocalBounds
                         ().height + 15.0f);
  RenderWindow _windowError;
  /// Cursor
  Image _cursorImg;
  Texture _cursorTxt;
  Sprite _cursor;
  _cursorImg.loadFromFile("assets/images/cursor.png");
  _cursorImg.createMaskFromColor(Color(000,255,000));
  _cursorTxt.loadFromImage(_cursorImg);
  _cursor.setTexture(_cursorTxt);
  _cursor.setTextureRect(CURSOR_NORMAL);
  ShowCursor(FALSE);
  /// Background
  Image _backgroundImg;
  if (!_backgroundImg.loadFromFile("assets/images/background3d.jpg"))
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger la texture de fond !"
                        );
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) // Keyboard
        {
          if (_event.type == Event::KeyPressed && _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased&& _event.key.
              code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type == Event::
              MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type == Event::
              MouseButtonReleased&& _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError
                                                      ));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  IntRect _backgroundRect(0, 0, 1366, 768);
  Texture _backgroundText;
  int _backgroundLight = 255;
  Int32 _backgroundLightNext = 0;
  _backgroundText.loadFromImage(_backgroundImg);
  Sprite _background(_backgroundText, _backgroundRect);
  Image _backgroundRocksImg;
  Texture _backgroundRocksText;
  Sprite *_backgroundRocks[MAX_ROCKS] = {0};
  vector<int> _backgroundRocksInt({0, 70, 146, 223, 300, 380, 458});
  int _backgroundRocksIndex = 0;
  Int32 _backgroundNextRock = 0;
  IntRect _backgroundRocksRect({_backgroundRocksInt[_backgroundRocksIndex], 0, _backgroundRocksInt[_backgroundRocksIndex+1],400});
  _backgroundRocksImg.loadFromFile("assets/images/rocks.bmp");
  _backgroundRocksImg.createMaskFromColor(Color(000,255,000));
  _backgroundRocksImg = grayscale(_backgroundRocksImg);
  _backgroundRocksText.loadFromImage(_backgroundRocksImg);
  Image _mysteryBoatImg;
  Texture _mysteryBoatText;
  Int32 _mysteryBoatNext = 30000;
  _mysteryBoatImg.loadFromFile("assets/images/mysteryboat.bmp");
  _mysteryBoatImg.createMaskFromColor(Color(000,255,000));
  _mysteryBoatImg = grayscale(_mysteryBoatImg);
  _mysteryBoatText.loadFromImage(_mysteryBoatImg);
  Sprite _mysteryBoat(_mysteryBoatText);
  _mysteryBoat.setColor(Color(000,000,127));
  _mysteryBoat.move(-_mysteryBoat.getLocalBounds().width, -_mysteryBoat
                    .getLocalBounds().height);
  Image _launcher_backgroundImg;
  Texture _launcher_backgroundTxt;
  Sprite _launcher_background;
  _launcher_backgroundImg.loadFromFile(
    "assets/images/launcher_background.png");
  _launcher_backgroundTxt.loadFromImage(_launcher_backgroundImg);
  _launcher_background.setTexture(_launcher_backgroundTxt, true);
  /// Open Launcher
  Text _signGame("Narwhal", _narwhalFont, 144);
  _signGame.setStyle(Text::Bold);
  _signGame.setFillColor(Color(199,199,255));
  _signGame.setPosition((WIN_W - _signGame.getLocalBounds().width) /
                        2.00f, (WIN_H - _signGame.getLocalBounds().height)/
                        5.00f);
  Image _formulaireImg;
  Texture _formulaireTxt;
  Sprite _formulaire;
  _formulaireImg.loadFromFile("assets/images/formulaire.bmp");
  _formulaireTxt.loadFromImage(_formulaireImg);
  _formulaire.setTexture(_formulaireTxt, true);
  _formulaire.setPosition((WIN_W - _formulaire.getLocalBounds().width) /
                          2.00f, (WIN_H - _formulaire.getLocalBounds().height)/
                          1.62f);
  Text _enterYourPseudo("Chargement du jeu en cours...", _basicFont, 32);
  _enterYourPseudo.setFillColor(Color(255,255,255));
  _enterYourPseudo.setPosition(_formulaire.getPosition().x + 7.0f,
                               _formulaire.getPosition().y -
                               _enterYourPseudo.getLocalBounds().height - 15.0f);
  Image _windowIcon;
  _windowIcon.loadFromFile("assets/images/narwhal_icon.ico");
  RenderWindow _windowLauncher(VideoMode(WIN_W,WIN_H), "Narwhal Launcher");
  _windowLauncher.setIcon(32, 32, _windowIcon.getPixelsPtr());
  _windowLauncher.requestFocus();
  _windowLauncher.draw(_launcher_background);
  _windowLauncher.draw(_signGame);
  _windowLauncher.draw(_enterYourPseudo);
  _windowLauncher.display();
  /// Bonus
  Image _bonusImg;
  if (!_bonusImg.loadFromFile("assets/images/bonus.png"))
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger le sprite des bonus");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) // Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased &&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased&& _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  _bonusImg.createMaskFromColor(Color(000,255,000));
  Texture _bonusText;
  _bonusText.loadFromImage(_bonusImg);
  _bonusText.setSmooth(true);
  IntRect _bonusRect(32, 0, 32, 32);
  Clock _bonusTime;
  int _bonusSect[5] = {0, 32, 64, 96, 128};
  int _bonusActualSect(1);
  int _bonusCount = 0;
  int _bonusLastSpawnPosX = 0;
  int _bonusLastSpawnPosY = 0;
  Int32 * _bonusLife[MAX_BONUS] = {0};
  Sprite * _bonus[MAX_BONUS] = {0};
  /// Bluko
  Image _dragonImg; // Image
  bool _dragonFaceToEnd = true; // Is the dragon facing to the end ?
  bool _dragonFacedToEnd = true; // Was the dragon facing to the end ?
  bool _dragonIsFlying(true); // Is the dragon flying in the air ?
  bool _dragonIsAlive(true); // Is the dragon living ?
  bool _dragonFireballPowered(false); // Is the dragon powered by fireball bonus ?
  bool _dragonHasBeenKilled(false);
  Texture _dragonText; // Texture
  IntRect _dragonRect(239, 0, 239, 200); // Initial dragon section sprite
  Clock _dragonTime; // For animation
  Int32 _dragonGotPowered = 0; // When has the dragon drunk the potion ?
  int _dragonSect[11] = {2, 242, 481, 719, 959, 1195, 1434, 1673, 1912,
                         2151, 2390
                        }; // Limit pixels between each dragon
  int _dragonActualSect(1); // 1 -> 10 (different dragon sprite sections)
  float _dragonSpeed(0.0f);
  int _dragonAnimSpeed = NORMAL_SPEED_ANIM;
  int _dragonKill(0);
  if (!_dragonImg.loadFromFile("assets/images/narwhal.bmp"))
    // Load Image
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger Narwhal ! :'(");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased&&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased&& _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  _dragonImg.createMaskFromColor(Color(000,255,000)); // Mask green
  _dragonText.loadFromImage(_dragonImg); // Load Texture
  _dragonText.setSmooth(true);
  Sprite _dragon(_dragonText, _dragonRect); // Load Sprite with initial dragon section sprite
  /// Fireball Sprite
  Image _fireballImg; // Image
  Texture _fireballText; // Texture
  IntRect _fireballSect[2] = {{0, 0, 94, 47},
    {95, 0, 94, 47}
  }; // Limit pixels between each fireball
  bool _fireballThrown(false); // Check if a fireball has been thrown by Bluko
  if (!_fireballImg.loadFromFile("assets/images/bouledefeu.bmp"))
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger le sprite des cornes");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased&&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased&& _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  _fireballImg.createMaskFromColor(Color(000,255,000)); // Mask green
  _fireballText.loadFromImage(_fireballImg);
  Sprite *_fireball[MAX_FIREBALL] = {0};
  /// Monster Sprite
  Image _monsterImg;
  Texture _monsterText;
  // get monster sprites
  if (_connected)
  {
    _client.connect(_gtaddress, 21).getMessage();
    if (!_client.login("297899_narwhal141", "rt29xk").isOk())
    {
      _connected = false;
      _windowError.create(VideoMode(858,570), "Narwhal Bug");
      _errorText.setString("La version de votre jeu est trop obsolète pour enregistrer votre score\nou le service Narwhal a fermé ses portes (date de fermeture : 13/07/2022");
      _windowError.requestFocus();
      while (_windowError.isOpen())
      {
        // Key binding
        if (_windowError.pollEvent(_event))
        {
          if (_event.type == Event::Closed)
          {
            _windowError.close();
          }
          for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
          {
            if (_event.type == Event::KeyPressed &&
                _event.key.code == i)
            {
              _key[i] = true;
            }
            if (_event.type == Event::KeyReleased&&
                _event.key.code == i)
            {
              _key[i] = false;
              _keypressed[i] = false;
            }
            if (i < Mouse::ButtonCount && _event.type ==
                Event::MouseButtonPressed && _event.mouseButton.button == i)
            {
              _button[i] = true;
            }
            if (i < Mouse::ButtonCount && _event.type ==
                Event::MouseButtonReleased&& _event.mouseButton.button == i)
            {
              _button[i] = false;
              _buttonpressed[i] = false;
            }
          }
        }
        // Key activate
        if (_key[Keyboard::Escape] &&
            !_keypressed[Keyboard::Escape])
        {
          _windowError.close();
          _keypressed[Keyboard::Escape] = true;
        }
        _windowError.clear();
        _windowError.draw(_errorBgd);
        _windowError.draw(_error);
        _windowError.draw(_errorText);
        _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
        _windowError.draw(_cursor);
        _windowError.display();
      }
    }
    else
    {
      _client.download("hanouna.png", "assets/images").getMessage();
      _client.download("hanouna.txt", "assets/images",
                       Ftp::Ascii).getMessage();
      _client.disconnect().getMessage();
    }
  }
  else
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("La connexion au serveur est impossible ! Vous pouvez quand même\njouer au jeu mais votre score ne sera pas enregistré.\nFermez cette fenêtre pour accéder au launcher.");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased&&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased&& _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  // Récupérer les données concernant les sprites des monstres
  unsigned int _monsterNarwhals = 8;
  int _monsterSect[9] = {1,1,2,3,4,5,6,7,8};
  _sb_amont.open("assets/images/hanouna.txt");
  if (!_sb_amont)
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger les informations de monstres");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased&&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased&& _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  else
  {
    _sb_amont >> _monsterNarwhals;
    for (unsigned int i = 0 ; i <= _monsterNarwhals ; i++)
    {
      _sb_amont >> _monsterSect[i];
    }
    _sb_amont.close();
  }
  // Charger l'image
  if (!_monsterImg.loadFromFile("assets/images/hanouna.png"))
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger les têtes de monstre");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased&&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased&& _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  _monsterImg.createMaskFromColor(Color(000,255,000));
  // Charger la texture
  _monsterText.loadFromImage(_monsterImg);
  _monsterText.setSmooth(true);
  // Initialiser la texture sur le premier monstre
  IntRect _monsterRect(0, 0, 103, 112);
  // Nouvelle section et dernier endroit de spawn du monstre
  int _monsterNewSect = 0;
  int _monsterLastSpawnPos = 0;
  // Sprites
  Sprite *_monster[MAX_MONSTER] = {0};
  // Vitesse
  float _monsterSpeed = MONSTER_INIT_SPEED;
  // Compteurs
  int _monsterCount = 0;
  int _monsterTotal = 0; // Augmente avec la fonction nextMonsterSpawnDate(t)
  /// Epolar Sprite
  Image _epolarImg;
  Texture _epolarText;
  // Chargement de l'image
  if (!_epolarImg.loadFromFile("assets/images/epolar.png"))
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger le sprite d'Epolar");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased &&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased && _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  _epolarImg.createMaskFromColor(Color(000,255,000));
  // Chargement de la texture
  _epolarText.loadFromImage(_epolarImg);
  _epolarText.setSmooth(true);
  // Chargement des positions
  IntRect _epolarRect(262, 0, 262, 134);
  bool _epolarFaceToEnd = true; // Is the epolar facing to the end ?
  bool _epolarFacedToEnd = true; // Was the epolar facing to the end ?
  bool _epolarIsFlying = true;
  // L'animation
  int _epolarSect[11] = {0, 262, 524, 786, 1048, 1310, 1572, 1834, 2096,
                         2358, 2620
                        }; // Limit pixels between each dragon
  int _epolarActualSect(1); // 1 -> 10 (different dragon sprite sections)
  float _epolarSpeed(EPOLAR_INIT_SPEED);
  // Décompte et temps
  int _epolarCount(0);
  int _epolarTotalCount(0);
  Clock _epolarTime;
  // Comportement et sprites
  bool _epolarStart[MAX_EPOLAR] = {true};
  int *_epolarHealth[MAX_EPOLAR] = {0};
  int *_epolarHealthMax[MAX_EPOLAR] = {0};
  Int32 _epolarPhases[MAX_EPOLAR][EPOLAR_PHASES] = {{0}};
  Int32 _epolarPhasesTime[EPOLAR_PHASES] = {0};
  _epolarPhasesTime[0] = EPOLAR_PHASETIME_0;
  _epolarPhasesTime[1] = EPOLAR_PHASETIME_1;
  _epolarPhasesTime[2] = EPOLAR_PHASETIME_2;
  _epolarPhasesTime[3] = EPOLAR_PHASETIME_3;
  _epolarPhasesTime[4] = EPOLAR_PHASETIME_4;
  Sprite *_epolar[MAX_EPOLAR] = {0};
  bool _epolarGoesUp[MAX_EPOLAR] = {false};
  float _epolarYDiff = 0.0f;
  // Jauge de vie
  RectangleShape *_epolarHealthBarDyn[MAX_EPOLAR] = {0};
  Sprite *_epolarHealthBar[MAX_EPOLAR] = {0};
  Image _epolarHealthBarImg;
  Texture _epolarHealthBarText;
  if (!_epolarHealthBarImg.loadFromFile("assets/images/epolar_health_bar.png"))
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger la jauge de vie d'Epolar");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased&&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased&& _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  _epolarHealthBarImg.createMaskFromColor(Color(000,255,000));
  _epolarHealthBarText.loadFromImage(_epolarHealthBarImg);
  _epolarHealthBarText.setSmooth(true);
  /// ATH
  /// Score (en haut à gauche)
  Text _score;
  char _texte[10] = "";
  string _texteS = "Score : 0";
  _score.setFont(_policeFont);
  _score.setStyle(Text::Style::Bold);
  _score.setCharacterSize(40);
  _score.setFillColor(Color(255,255,0));
  _score.move(8,0);
  /// Game Over Message (en bas à gauche)
  Text _gameOver;
  Text _gameOverSubtitle;
  _gameOver.setFont(_narwhalFont);
  _gameOver.setCharacterSize(132);
  _gameOver.setFillColor(Color::Red);
  _gameOver.setString("GAME OVER");
  _gameOver.move((WIN_WIDTH - _gameOver.getLocalBounds().width) /2.00f,
                 (WIN_HEIGHT- _gameOver.getLocalBounds().height)/2.88f);
  _gameOverSubtitle.setFont(_basicFont);
  _gameOverSubtitle.setCharacterSize(40);
  _gameOverSubtitle.setFillColor(Color(200,200,200));
  _gameOverSubtitle.setString("Appuyez sur [Échap]");
  _gameOverSubtitle.move((WIN_WIDTH -
                          _gameOverSubtitle.getLocalBounds().width) /2.00f,
                         _gameOver.getPosition().y +
                         _gameOver.getLocalBounds().height + 200.0f);
  /// Credits (en haut à droite)
  string _creditsTexte = "Programmé par HarryBreak";
  Text _credits;
  _credits.setFont(_policeFont);
  _credits.setStyle(Text::Style::Bold);
  _credits.setCharacterSize(40);
  _credits.setFillColor(Color(255,255,0));
  _credits.setString(_creditsTexte);
  _credits.move(WIN_WIDTH - _credits.getLocalBounds().width,0);
  /// FPS (en dessous du score)
  bool _showFPS = false; // [U] --> Show/Hide FPS countdown
  Int32 _fpscount = 0; /// THIS IS ALSO THE LAST FPS REGISTERED
  char _fpschr[20] = "";
  string _fpsstr = "FPS : 0";
  Text _fps;
  _fps.setFont(_policeFont);
  _fps.setStyle(Text::Style::Bold);
  _fps.setCharacterSize(32);
  _fps.setFillColor(Color(255,255,0));
  _fps.setString(_fpsstr);
  _fps.move(8, 32);
  /// Scoreboard
  vector<int> _sb_scores(5,0);
  char _sb_scores_chr[5][10];
  vector<string> _sb_scores_str(5);
  vector<string> _sb_players(5);
  Text _sb;
  _sb.setFont(_policeFont);
  _sb.setStyle(Text::Style::Bold);
  _sb.setCharacterSize(40);
  _sb.setFillColor(Color(255,255,255));
  _sb.move(64,64);
  /// Music Manager
  Music _music;
  float _music_volume = 45.0f;
  vector<string> _music_list = browseMusic();
  if (_music_list.size() == 0)
  {
    _music.openFromFile("assets/audio/default.ogg");
  }
  else
  {
    _music.openFromFile("assets/audio/musiques/"+_music_list[0]);
  }
  _music.setVolume(_music_volume);
  _music.setLoop(true);
  bool _music_repeatfolder(false);
  bool _music_randomfolder(false);
  /// Sound/Noise Manager
  SoundBuffer _firehorn_buffer;
  if (!_firehorn_buffer.loadFromFile("assets/audio/launch.ogg"))
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger les fichiers de son");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased &&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased && _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  Sound *_firehorn[MAX_SOUNDS] = {0};
  float _firehorn_volume = 30.0f;
  /// Parameters File
  _sb_amont.open("options");
  if (!_sb_amont)
  {
    _sb_aval.open("options");
    // Forward Key, Backward key, Godown Key, Goup Key, Fireball key, Accelerate Key, fpsshow Key, volume_music, volume_sound
    _keyid[KEYB_FORWARD] = Keyboard::D;
    _sb_aval << _keyid[KEYB_FORWARD];
    _sb_aval << " ";
    _keyid[KEYB_BACKWARD] = Keyboard::Q;
    _sb_aval << _keyid[KEYB_BACKWARD];
    _sb_aval << " ";
    _keyid[KEYB_GODOWN] = Keyboard::S;
    _sb_aval << _keyid[KEYB_GODOWN];
    _sb_aval << " ";
    _keyid[KEYB_GOUP] = Keyboard::Z;
    _sb_aval << _keyid[KEYB_GOUP];
    _sb_aval << " ";
    _keyid[KEYB_FIREBALL] = Keyboard::M;
    _sb_aval << _keyid[KEYB_FIREBALL];
    _sb_aval << " ";
    _keyid[KEYB_ACCELERATE] = Keyboard::K;
    _sb_aval << _keyid[KEYB_ACCELERATE];
    _sb_aval << " ";
    _keyid[KEYB_FPSDISP] = Keyboard::U;
    _sb_aval << _keyid[KEYB_FPSDISP];
    _sb_aval << " ";
    _sb_aval << _music_volume;
    _sb_aval << " ";
    _sb_aval << _firehorn_volume;
    _sb_aval.close();
  }
  else
  {
    _sb_amont >> _keyid[KEYB_FORWARD];
    _sb_amont >> _keyid[KEYB_BACKWARD];
    _sb_amont >> _keyid[KEYB_GODOWN];
    _sb_amont >> _keyid[KEYB_GOUP];
    _sb_amont >> _keyid[KEYB_FIREBALL];
    _sb_amont >> _keyid[KEYB_ACCELERATE];
    _sb_amont >> _keyid[KEYB_FPSDISP];
    _sb_amont >> _music_volume;
    _sb_amont >> _firehorn_volume;
    _sb_amont.close();
  }
  /// Menu Managment
  Image _menuBarreImg;
  Texture _menuBarreText;
  // Loading...
  if (!_menuBarreImg.loadFromFile("assets/images/barre_menu.png"))
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger les sprites de menu");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased &&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased && _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  _menuBarreImg.createMaskFromColor(Color(000,255,000));
  _menuBarreText.loadFromImage(_menuBarreImg);
  _menuBarreText.setSmooth(true);
  // Jauges et Boutons
  IntRect *_menuBarreRect[NBR_BUTTONS] = {0};
  Sprite *_menuBarre[NBR_BUTTONS] = {0};
  IntRect *_menuJaugeRect[NBR_JAUGES] = {0};
  Sprite *_menuJaugeFond[NBR_JAUGES] = {0};
  Sprite *_menuJaugeCursor[NBR_JAUGES] = {0};
  Text *_menuTxtBarre[NBR_BUTTONS] = {0};
  Text *_menuTxtJauge[NBR_JAUGES] = {0};
  for (unsigned int i = 0 ; i < NBR_BUTTONS ; i++)
  {
    _menuBarreRect[i] = new IntRect(0, 0, 729, 76);
    _menuBarre[i] = new Sprite(_menuBarreText, *_menuBarreRect[i]);
    _menuTxtBarre[i] = new Text("", _basicFont, 32);
  }
  for (unsigned int i = 0 ; i < NBR_JAUGES ; i++)
  {
    _menuJaugeRect[i] = new IntRect(833, 0, 729, 76);
    _menuJaugeFond[i] = new Sprite(_menuBarreText,
                                   *_menuJaugeRect[i]);
    *_menuJaugeRect[i] = {805, 0, 28, 76};
    _menuJaugeCursor[i] = new Sprite(_menuBarreText,
                                     *_menuJaugeRect[i]);
    _menuTxtJauge[i] = new Text("", _basicFont, 32);
  }
  // Listes
  Image _listeImg;
  Texture _listeText;
  if (!_listeImg.loadFromFile("assets/images/liste_menu.png"))
  {
    _windowError.create(VideoMode(858,570), "Narwhal Bug");
    _errorText.setString("Impossible de charger les sprites de liste");
    _windowError.requestFocus();
    while (_windowError.isOpen())
    {
      // Key binding
      if (_windowError.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowError.close();
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased &&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased && _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activate
      if (_key[Keyboard::Escape] && !_keypressed[Keyboard::Escape])
      {
        _windowError.close();
        _keypressed[Keyboard::Escape] = true;
      }
      _windowError.clear();
      _windowError.draw(_errorBgd);
      _windowError.draw(_error);
      _windowError.draw(_errorText);
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
      _windowError.draw(_cursor);
      _windowError.display();
    }
  }
  _listeImg.createMaskFromColor(Color(000,255,000));
  _listeText.loadFromImage(_listeImg);
  _listeText.setSmooth(true);
  unsigned int _listeContenance = 3;
  unsigned int *_listeCurseur[NBR_LISTES] = {0};
  unsigned int *_listeSelected[NBR_LISTES] = {0};
  Text *_listeNomTxt[NBR_LISTES] = {0};
  IntRect *_listeRect[NBR_LISTES] = {0};
  Sprite *_listeFlecheHaut[NBR_LISTES] = {0};
  Sprite *_listeFlecheBas[NBR_LISTES] = {0};
  Sprite *_listeNom[NBR_LISTES] = {0};
  vector<string> _listeChoixStr[NBR_LISTES] = {vector<string>(0)};
  Text *_listeChoixTxt0[NBR_LISTES] = {0};
  Text *_listeChoixTxt1[NBR_LISTES] = {0};
  Text *_listeChoixTxt2[NBR_LISTES] = {0};
  Sprite *_listeChoix0[NBR_LISTES] = {0};
  Sprite *_listeChoix1[NBR_LISTES] = {0};
  Sprite *_listeChoix2[NBR_LISTES] = {0};
  for (unsigned int i = 0 ; i < NBR_LISTES ; i++)
  {
    _listeRect[i] = new IntRect(0, 0, 76, 76);
    _listeFlecheHaut[i] = new Sprite(_listeText, *_listeRect[i]);
    *_listeRect[i] = {0, 76, 729, 76};
    _listeChoix0[i] = new Sprite(_listeText, *_listeRect[i]);
    _listeChoix1[i] = new Sprite(_listeText, *_listeRect[i]);
    _listeChoix2[i] = new Sprite(_listeText, *_listeRect[i]);
    *_listeRect[i] = {76, 0, 577, 76};
    _listeNom[i] = new Sprite(_listeText, *_listeRect[i]);
    *_listeRect[i] = {577+76, 0, 76, 76};
    _listeFlecheBas[i] = new Sprite(_listeText, *_listeRect[i]);
    _listeChoixTxt0[i] = new Text("", _basicFont, 28);
    _listeChoixTxt1[i] = new Text("", _basicFont, 28);
    _listeChoixTxt2[i] = new Text("", _basicFont, 28);
    _listeNomTxt[i] = new Text("Liste sans nom", _policeFont, 48);
    _listeCurseur[i] = new unsigned int(0);
    _listeSelected[i] = new unsigned int(0);
  }
  // Jauges
  _menuJaugeFond[JAUGE_SON]->setPosition(
    (WIN_WIDTH-_menuJaugeFond[JAUGE_SON]->getLocalBounds().width)/2,
    (WIN_HEIGHT-_menuJaugeFond[JAUGE_SON]->getLocalBounds().height)/4);
  _menuJaugeCursor[JAUGE_SON]->setPosition(_menuJaugeFond[JAUGE_SON]->getPosition().x + _firehorn_volume*(729.0f - _menuJaugeCursor[JAUGE_SON]->getLocalBounds().width)/100.0f, _menuJaugeFond[JAUGE_SON]->getPosition().y);
  _menuTxtJauge[JAUGE_SON]->setString("Volume du son : "+intToString(static_cast<int>(_firehorn_volume))+" %");
  _menuTxtJauge[JAUGE_SON]->setPosition(_menuJaugeFond[JAUGE_SON]->getPosition().x + (_menuJaugeFond[JAUGE_SON]->getLocalBounds().width - _menuTxtJauge[JAUGE_SON]->getLocalBounds().width )/2, _menuJaugeFond[JAUGE_SON]->getPosition().y + (_menuJaugeFond[JAUGE_SON]->getLocalBounds().height-_menuTxtJauge[JAUGE_SON]->getLocalBounds().height)/4);
  _menuJaugeFond[JAUGE_MUSIQUE]->setPosition(
    (WIN_WIDTH-_menuJaugeFond[JAUGE_MUSIQUE]->getLocalBounds().width)/2,
    _menuJaugeFond[JAUGE_SON]->getPosition().y -_menuJaugeFond[JAUGE_MUSIQUE]->getLocalBounds().height - 15);
  _menuJaugeCursor[JAUGE_MUSIQUE]->setPosition(
    _menuJaugeFond[JAUGE_MUSIQUE]->getPosition().x + _music_volume*(729.0f - _menuJaugeCursor[JAUGE_MUSIQUE]->getLocalBounds().width)/100.0f,
    _menuJaugeFond[JAUGE_MUSIQUE]->getPosition().y);
  _menuTxtJauge[JAUGE_MUSIQUE]->setString("Volume de la musique :"+intToString(static_cast<int>(_music_volume))+" %");
  _menuTxtJauge[JAUGE_MUSIQUE]->setPosition(_menuJaugeFond[JAUGE_MUSIQUE]->getPosition().x + (_menuJaugeFond[JAUGE_MUSIQUE]->getLocalBounds().width - _menuTxtJauge[JAUGE_MUSIQUE]->getLocalBounds().width )/2,
      _menuJaugeFond[JAUGE_MUSIQUE]->getPosition().y +
      (_menuJaugeFond[JAUGE_MUSIQUE]->getLocalBounds().height-
       _menuTxtJauge[JAUGE_MUSIQUE]->getLocalBounds().height)/4);
  // Listes (quand le curseur du joueur se trouve sur un bouton, changer la couleur du sprite entier)
  _listeNom[LISTE_MUSIQUES]->setPosition((WIN_WIDTH -
                                          _listeNom[LISTE_MUSIQUES]->getLocalBounds().width)/2,
                                         _menuJaugeFond[JAUGE_SON]->getPosition().y +
                                         _menuJaugeFond[JAUGE_SON]->getLocalBounds().height + 15);
  _listeNomTxt[LISTE_MUSIQUES]->setString("Liste des musiques");
  _listeNomTxt[LISTE_MUSIQUES]->setPosition(_listeNom[LISTE_MUSIQUES]->getPosition().x + (_listeNom[LISTE_MUSIQUES]->getLocalBounds().width -
      _listeNomTxt[LISTE_MUSIQUES]->getLocalBounds().width )/2,
      _listeNom[LISTE_MUSIQUES]->getPosition().y -
      (_listeNom[LISTE_MUSIQUES]->getLocalBounds().height-
       _listeNomTxt[LISTE_MUSIQUES]->getLocalBounds().height)/8);
  _listeFlecheBas[LISTE_MUSIQUES]->setPosition(_listeNom[LISTE_MUSIQUES]->getPosition().x - _listeFlecheBas[LISTE_MUSIQUES]->getLocalBounds().width,
      _listeNom[LISTE_MUSIQUES]->getPosition().y);
  _listeFlecheHaut[LISTE_MUSIQUES]->setPosition(_listeNom[LISTE_MUSIQUES]->getPosition().x +_listeNom[LISTE_MUSIQUES]->getLocalBounds().width,
      _listeNom[LISTE_MUSIQUES]->getPosition().y);
  _listeChoix0[LISTE_MUSIQUES]->setPosition(_listeFlecheBas[LISTE_MUSIQUES]->getPosition().x,
      _listeFlecheBas[LISTE_MUSIQUES]->getPosition().y + 76*1);
  _listeChoix1[LISTE_MUSIQUES]->setPosition(_listeFlecheBas[LISTE_MUSIQUES]->getPosition().x,
      _listeFlecheBas[LISTE_MUSIQUES]->getPosition().y + 76*2);
  _listeChoix2[LISTE_MUSIQUES]->setPosition(_listeFlecheBas[LISTE_MUSIQUES]->getPosition().x,
      _listeFlecheBas[LISTE_MUSIQUES]->getPosition().y + 76*3);
  _listeChoixTxt0[LISTE_MUSIQUES]->setPosition(_listeChoix0[LISTE_MUSIQUES]->getPosition().x + 10,
      _listeChoix0[LISTE_MUSIQUES]->getPosition().y +
      (_listeChoix0[LISTE_MUSIQUES]->getLocalBounds().height-
       _listeChoixTxt0[LISTE_MUSIQUES]->getLocalBounds().height)/4);
  _listeChoixTxt1[LISTE_MUSIQUES]->setPosition(_listeChoix1[LISTE_MUSIQUES]->getPosition().x + 10,
      _listeChoix1[LISTE_MUSIQUES]->getPosition().y +
      (_listeChoix1[LISTE_MUSIQUES]->getLocalBounds().height-
       _listeChoixTxt1[LISTE_MUSIQUES]->getLocalBounds().height)/4);
  _listeChoixTxt2[LISTE_MUSIQUES]->setPosition(_listeChoix2[LISTE_MUSIQUES]->getPosition().x + 10,
      _listeChoix2[LISTE_MUSIQUES]->getPosition().y +
      (_listeChoix2[LISTE_MUSIQUES]->getLocalBounds().height-
       _listeChoixTxt2[LISTE_MUSIQUES]->getLocalBounds().height)/4);
  _listeChoixStr[LISTE_MUSIQUES] = _music_list;
  for (unsigned int i = 0 ; i < _listeChoixStr[LISTE_MUSIQUES].size() ;
       i++)
  {
    _listeChoixStr[LISTE_MUSIQUES][i] =
      _listeChoixStr[LISTE_MUSIQUES][i].substr(0, 47);
  }
  // Boutons
  _menuBarre[BUT_MUSIC_REPEAT]->setPosition((WIN_WIDTH -
      _menuBarre[BUT_MUSIC_REPEAT]->getLocalBounds().width) /2.00f,
      _listeChoix2[LISTE_MUSIQUES]->getPosition().y +
      _listeChoix2[LISTE_MUSIQUES]->getLocalBounds().height + 15.0f);
  _menuTxtBarre[BUT_MUSIC_REPEAT]->setString("Répétition : musique sélectionnée");
  _menuTxtBarre[BUT_MUSIC_REPEAT]->setPosition(_menuBarre[BUT_MUSIC_REPEAT]->getPosition().x + (_menuBarre[BUT_MUSIC_REPEAT]->getLocalBounds().width -
      _menuTxtBarre[BUT_MUSIC_REPEAT]->getLocalBounds().width )/2,
      _menuBarre[BUT_MUSIC_REPEAT]->getPosition().y +
      (_menuBarre[BUT_MUSIC_REPEAT]->getLocalBounds().height-
       _menuTxtBarre[BUT_MUSIC_REPEAT]->getLocalBounds().height)/4);
  _menuBarre[BUT_MUSIQUE]->setPosition((WIN_WIDTH
                                        -_menuBarre[BUT_MUSIQUE]->getLocalBounds().width) /2,
                                       (WIN_HEIGHT-_menuBarre[BUT_MUSIQUE]->getLocalBounds().height)/2);
  _menuTxtBarre[BUT_MUSIQUE]->setString("Sons et musique");
  _menuTxtBarre[BUT_MUSIQUE]->setPosition(_menuBarre[BUT_MUSIQUE]->getPosition().x + (_menuBarre[BUT_MUSIQUE]->getLocalBounds().width -
                                          _menuTxtBarre[BUT_MUSIQUE]->getLocalBounds().width )/2,
                                          _menuBarre[BUT_MUSIQUE]->getPosition().y +
                                          (_menuBarre[BUT_MUSIQUE]->getLocalBounds().height-
                                              _menuTxtBarre[BUT_MUSIQUE]->getLocalBounds().height)/4);
  _menuBarre[BUT_RETOUR_JEU]->setPosition((WIN_WIDTH
                                          -_menuBarre[BUT_RETOUR_JEU]->getLocalBounds().width) /2,
                                          _menuBarre[BUT_MUSIQUE]->getPosition().y -
                                          _menuBarre[BUT_RETOUR_JEU]->getLocalBounds().height - 15);
  _menuTxtBarre[BUT_RETOUR_JEU]->setString("Retour au jeu");
  _menuTxtBarre[BUT_RETOUR_JEU]->setPosition(_menuBarre[BUT_RETOUR_JEU]->getPosition().x + (_menuBarre[BUT_RETOUR_JEU]->getLocalBounds().width -
      _menuTxtBarre[BUT_RETOUR_JEU]->getLocalBounds().width )/2,
      _menuBarre[BUT_RETOUR_JEU]->getPosition().y +
      (_menuBarre[BUT_RETOUR_JEU]->getLocalBounds().height-
       _menuTxtBarre[BUT_RETOUR_JEU]->getLocalBounds().height)/4);
  _menuBarre[BUT_TOUCHES]->setPosition((WIN_WIDTH
                                        -_menuBarre[BUT_TOUCHES]->getLocalBounds().width) /2,
                                       _menuBarre[BUT_MUSIQUE]->getPosition().y +
                                       _menuBarre[BUT_MUSIQUE]->getLocalBounds().height + 15);
  _menuTxtBarre[BUT_TOUCHES]->setString("Touches");
  _menuTxtBarre[BUT_TOUCHES]->setPosition(_menuBarre[BUT_TOUCHES]->getPosition().x + (_menuBarre[BUT_TOUCHES]->getLocalBounds().width -
                                          _menuTxtBarre[BUT_TOUCHES]->getLocalBounds().width )/2,
                                          _menuBarre[BUT_TOUCHES]->getPosition().y +
                                          (_menuBarre[BUT_TOUCHES]->getLocalBounds().height-
                                              _menuTxtBarre[BUT_TOUCHES]->getLocalBounds().height)/4);
  _menuBarre[BUT_RETOUR_MAIN]->setPosition((WIN_WIDTH
      -_menuBarre[BUT_RETOUR_MAIN]->getLocalBounds().width) /2,
      WIN_HEIGHT - 95);
  _menuTxtBarre[BUT_RETOUR_MAIN]->setString("Retour au menu principal");
  _menuTxtBarre[BUT_RETOUR_MAIN]->setPosition(_menuBarre[BUT_RETOUR_MAIN]->getPosition().x + (_menuBarre[BUT_RETOUR_MAIN]->getLocalBounds().width -
      _menuTxtBarre[BUT_RETOUR_MAIN]->getLocalBounds().width )/2,
      _menuBarre[BUT_RETOUR_MAIN]->getPosition().y +
      (_menuBarre[BUT_RETOUR_MAIN]->getLocalBounds().height-
       _menuTxtBarre[BUT_RETOUR_MAIN]->getLocalBounds().height)/4);
  _menuBarre[BUT_KEY_FORWARD]->setPosition((WIN_WIDTH
      -_menuBarre[BUT_KEY_FORWARD]->getLocalBounds().width) /2,
      (WIN_HEIGHT-_menuBarre[BUT_KEY_FORWARD]->getLocalBounds().height)/2 - 40);
  _menuTxtBarre[BUT_KEY_FORWARD]->setString("Go right : "+keystring(_keyid[KEYB_FORWARD]));
  _menuTxtBarre[BUT_KEY_FORWARD]->setPosition(_menuBarre[BUT_KEY_FORWARD]->getPosition().x + (_menuBarre[BUT_KEY_FORWARD]->getLocalBounds().width -
      _menuTxtBarre[BUT_KEY_FORWARD]->getLocalBounds().width )/2,
      _menuBarre[BUT_KEY_FORWARD]->getPosition().y +
      (_menuBarre[BUT_KEY_FORWARD]->getLocalBounds().height-
       _menuTxtBarre[BUT_KEY_FORWARD]->getLocalBounds().height)/4);
  _menuBarre[BUT_KEY_GODOWN]->setPosition((WIN_WIDTH
                                          -_menuBarre[BUT_KEY_GODOWN]->getLocalBounds().width) /2,
                                          _menuBarre[BUT_KEY_FORWARD]->getPosition().y -
                                          _menuBarre[BUT_KEY_GODOWN]->getLocalBounds().height - 15);
  _menuTxtBarre[BUT_KEY_GODOWN]->setString("Go down : "+keystring(_keyid[KEYB_GODOWN]));
  _menuTxtBarre[BUT_KEY_GODOWN]->setPosition(_menuBarre[BUT_KEY_GODOWN]->getPosition().x + (_menuBarre[BUT_KEY_GODOWN]->getLocalBounds().width -
      _menuTxtBarre[BUT_KEY_GODOWN]->getLocalBounds().width )/2,
      _menuBarre[BUT_KEY_GODOWN]->getPosition().y +
      (_menuBarre[BUT_KEY_GODOWN]->getLocalBounds().height-
       _menuTxtBarre[BUT_KEY_GODOWN]->getLocalBounds().height)/4);
  _menuBarre[BUT_KEY_GOUP]->setPosition((WIN_WIDTH
                                         -_menuBarre[BUT_KEY_GOUP]->getLocalBounds().width) /2,
                                        _menuBarre[BUT_KEY_GODOWN]->getPosition().y -
                                        _menuBarre[BUT_KEY_GOUP]->getLocalBounds().height - 15);
  _menuTxtBarre[BUT_KEY_GOUP]->setString("Go up : "+keystring(_keyid[KEYB_GOUP]));
  _menuTxtBarre[BUT_KEY_GOUP]->setPosition(_menuBarre[BUT_KEY_GOUP]->getPosition().x + (_menuBarre[BUT_KEY_GOUP]->getLocalBounds().width -
      _menuTxtBarre[BUT_KEY_GOUP]->getLocalBounds().width )/2,
      _menuBarre[BUT_KEY_GOUP]->getPosition().y +
      (_menuBarre[BUT_KEY_GOUP]->getLocalBounds().height-
       _menuTxtBarre[BUT_KEY_GOUP]->getLocalBounds().height)/4);
  _menuBarre[BUT_KEY_BACKWARD]->setPosition((WIN_WIDTH
      -_menuBarre[BUT_KEY_BACKWARD]->getLocalBounds().width) /2,
      _menuBarre[BUT_KEY_GOUP]->getPosition().y -
      _menuBarre[BUT_KEY_BACKWARD]->getLocalBounds().height - 15);
  _menuTxtBarre[BUT_KEY_BACKWARD]->setString("Go left : "+keystring(_keyid[KEYB_BACKWARD]));
  _menuTxtBarre[BUT_KEY_BACKWARD]->setPosition(_menuBarre[BUT_KEY_BACKWARD]->getPosition().x + (_menuBarre[BUT_KEY_BACKWARD]->getLocalBounds().width -
      _menuTxtBarre[BUT_KEY_BACKWARD]->getLocalBounds().width )/2,
      _menuBarre[BUT_KEY_BACKWARD]->getPosition().y +
      (_menuBarre[BUT_KEY_BACKWARD]->getLocalBounds().height-
       _menuTxtBarre[BUT_KEY_BACKWARD]->getLocalBounds().height)/4);
  _menuBarre[BUT_KEY_ACCERELATE]->setPosition((WIN_WIDTH
      -_menuBarre[BUT_KEY_ACCERELATE]->getLocalBounds().width) /2,
      _menuBarre[BUT_KEY_FORWARD]->getPosition().y +
      _menuBarre[BUT_KEY_FORWARD]->getLocalBounds().height + 15);
  _menuTxtBarre[BUT_KEY_ACCERELATE]->setString("Accelerate : "+keystring(_keyid[KEYB_ACCELERATE]));
  _menuTxtBarre[BUT_KEY_ACCERELATE]->setPosition(_menuBarre[BUT_KEY_ACCERELATE]->getPosition().x +
      (_menuBarre[BUT_KEY_ACCERELATE]->getLocalBounds().width -
       _menuTxtBarre[BUT_KEY_ACCERELATE]->getLocalBounds().width )/2,
      _menuBarre[BUT_KEY_ACCERELATE]->getPosition().y +
      (_menuBarre[BUT_KEY_ACCERELATE]->getLocalBounds().height-
       _menuTxtBarre[BUT_KEY_ACCERELATE]->getLocalBounds().height)/4);
  _menuBarre[BUT_KEY_FIREBALL]->setPosition((WIN_WIDTH
      -_menuBarre[BUT_KEY_FIREBALL]->getLocalBounds().width) /2,
      _menuBarre[BUT_KEY_ACCERELATE]->getPosition().y +
      _menuBarre[BUT_KEY_ACCERELATE]->getLocalBounds().height + 15);
  _menuTxtBarre[BUT_KEY_FIREBALL]->setString("Fire : "+keystring(_keyid[KEYB_FIREBALL]));
  _menuTxtBarre[BUT_KEY_FIREBALL]->setPosition(_menuBarre[BUT_KEY_FIREBALL]->getPosition().x + (_menuBarre[BUT_KEY_FIREBALL]->getLocalBounds().width -
      _menuTxtBarre[BUT_KEY_FIREBALL]->getLocalBounds().width )/2,
      _menuBarre[BUT_KEY_FIREBALL]->getPosition().y +
      (_menuBarre[BUT_KEY_FIREBALL]->getLocalBounds().height-
       _menuTxtBarre[BUT_KEY_FIREBALL]->getLocalBounds().height)/4);
  _menuBarre[BUT_KEY_FPSDISP]->setPosition((WIN_WIDTH
      -_menuBarre[BUT_KEY_FPSDISP]->getLocalBounds().width) /2,
      _menuBarre[BUT_KEY_FIREBALL]->getPosition().y +
      _menuBarre[BUT_KEY_FIREBALL]->getLocalBounds().height + 15);
  _menuTxtBarre[BUT_KEY_FPSDISP]->setString("Afficher les FPS : "+keystring(_keyid[KEYB_FIREBALL]));
  _menuTxtBarre[BUT_KEY_FPSDISP]->setPosition(_menuBarre[BUT_KEY_FPSDISP]->getPosition().x + (_menuBarre[BUT_KEY_FPSDISP]->getLocalBounds().width -
      _menuTxtBarre[BUT_KEY_FPSDISP]->getLocalBounds().width )/2,
      _menuBarre[BUT_KEY_FPSDISP]->getPosition().y +
      (_menuBarre[BUT_KEY_FPSDISP]->getLocalBounds().height-
       _menuTxtBarre[BUT_KEY_FPSDISP]->getLocalBounds().height)/4);
  int _menu_actuel = MENU_MAIN;
  int _menu_prochain = MENU_MAIN;
  /// Launcher (without bgd)
  bool _formulaireActivated(true);
  vector<char> _formulaireVecchar(0);
  string _formulaireStr("");
  Text _formulaireText("", _basicFont, 64);
  _formulaireText.setFillColor(Color::Black);
  _formulaireText.setPosition(_formulaire.getPosition().x + 10.0f,
                              _formulaire.getPosition().y - 2.0f);
  _enterYourPseudo.setString("Entrez votre pseudonyme :");
  Image _jouerBoutonImg;
  Texture _jouerBoutonTxt;
  Sprite _jouerBouton;
  _jouerBoutonImg.loadFromFile("assets/images/play_button.bmp");
  _jouerBoutonTxt.loadFromImage(_jouerBoutonImg);
  _jouerBouton.setTexture(_jouerBoutonTxt, true);
  _jouerBouton.setPosition((WIN_W - _jouerBouton.getLocalBounds().width)
                           /2.00f, (WIN_H -
                                    _jouerBouton.getLocalBounds().height)/1.22f);
  /// Loading finished
  while (_windowLauncher.isOpen())
  {
    for (Clock _chrono ; _playerIsInLauncher ;)
    {
      _chrono.restart();
      // Key binding
      if (_windowLauncher.pollEvent(_event))
      {
        if (_event.type == Event::Closed)
        {
          _windowLauncher.close();
          _playerIsInLauncher = false;
        }
        for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
        {
          if (_event.type == Event::KeyPressed &&
              _event.key.code == i)
          {
            _key[i] = true;
          }
          if (_event.type == Event::KeyReleased&&
              _event.key.code == i)
          {
            _key[i] = false;
            _keypressed[i] = false;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonPressed && _event.mouseButton.button == i)
          {
            _button[i] = true;
          }
          if (i < Mouse::ButtonCount && _event.type ==
              Event::MouseButtonReleased&& _event.mouseButton.button == i)
          {
            _button[i] = false;
            _buttonpressed[i] = false;
          }
        }
      }
      // Key activation
      if (_formulaireActivated)
      {
        if (_key[Keyboard::Escape] &&
            !_keypressed[Keyboard::Escape])
        {
          _formulaireActivated = false;
          _keypressed[Keyboard::Escape] = true;
        }
        else if (_event.type == Event::TextEntered &&
                 !_textEntered)
        {
          _textEntered = true;
          switch (_event.text.unicode)
          {
            // Backspace
            case 8:
              if (_formulaireVecchar.size() > 0)
                _formulaireVecchar.pop_back();
              break;
            case 13:
              // If player didn't enter any pseudo
              _keypressed[Keyboard::Return] = true;
              if (_formulaireStr.size() == 0)
              {
                _errorText.setString("Impossible de jouer sans entrer son pseudonyme !");
                _windowError.create(VideoMode(858,570), "Narwhal Bug");
                _windowError.requestFocus();
                while (_windowError.isOpen())
                {
                  // Key binding
                  if (_windowError.pollEvent(_event))
                  {
                    if (_event.type == Event::Closed)
                    {
                      _windowError.close();
                    }
                    for (int i = 0 ; i <
                         Keyboard::KeyCount ; ++i) // Keyboard
                    {
                      if (_event.type ==
                          Event::KeyPressed && _event.key.code == i)
                      {
                        _key[i] = true;
                      }
                      if (_event.type ==
                          Event::KeyReleased&& _event.key.code == i)
                      {
                        _key[i] = false;
                        _keypressed[i] = false;
                      }
                      if (i < Mouse::ButtonCount &&
                          _event.type == Event::MouseButtonPressed && _event.mouseButton.button ==
                          i)
                      {
                        _button[i] = true;
                      }
                      if (i < Mouse::ButtonCount &&
                          _event.type == Event::MouseButtonReleased && _event.mouseButton.button ==
                          i)
                      {
                        _button[i] = false;
                        _buttonpressed[i] = false;
                      }
                    }
                  }
                  // Key activate
                  if (_key[Keyboard::Escape] &&
                      !_keypressed[Keyboard::Escape])
                  {
                    _windowError.close();
                    _keypressed[Keyboard::Escape] =
                      true;
                  }
                  _windowError.clear();
                  _windowError.draw(_errorBgd);
                  _windowError.draw(_error);
                  _windowError.draw(_errorText);
                  _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
                  _windowError.draw(_cursor);
                  _windowError.display();
                }
              }
              else
              {
                _playerIsInLauncher = false;
                _playerIsInGame = true;
                _windowLauncher.close();
              }
              break;
            default:
              if (_formulaireVecchar.size() <
                  PSEUDO_MAXSIZE)
                _formulaireVecchar.push_back(static_cast<char>(_event.text.unicode));
              break;
          }
          _formulaireStr = charListToString(_formulaireVecchar);
          _formulaireText.setString(_formulaireStr);
        }
        else if (_event.type != Event::TextEntered)
        {
          _textEntered = false;
        }
      }
      // Mouse activation
      // Play Button
      if (contact(_cursor.getPosition(), _cursor.getLocalBounds(),
                  _jouerBouton.getPosition(), _jouerBouton.getLocalBounds()))
      {
        _jouerBouton.setColor(Color(255,166,166,244));
        if (_button[Mouse::Left] && !_buttonpressed[Mouse::Left])
        {
          _buttonpressed[Mouse::Left] = true;
          // If player didn't enter any pseudo
          if (_formulaireStr.size() == 0)
          {
            _errorText.setString("Impossible de jouer sans entrer son pseudonyme !");
            _windowError.create(VideoMode(858,570), "Narwhal Bug");
            _windowError.requestFocus();
            while (_windowError.isOpen())
            {
              // Key binding
              if (_windowError.pollEvent(_event))
              {
                if (_event.type == Event::Closed)
                {
                  _windowError.close();
                }
                for (int i = 0 ; i < Keyboard::KeyCount ;
                     ++i) // Keyboard
                {
                  if (_event.type == Event::KeyPressed
                      && _event.key.code == i)
                  {
                    _key[i] = true;
                  }
                  if (_event.type ==
                      Event::KeyReleased&& _event.key.code == i)
                  {
                    _key[i] = false;
                    _keypressed[i] = false;
                  }
                  if (i < Mouse::ButtonCount &&
                      _event.type == Event::MouseButtonPressed && _event.mouseButton.button ==
                      i)
                  {
                    _button[i] = true;
                  }
                  if (i < Mouse::ButtonCount &&
                      _event.type == Event::MouseButtonReleased&& _event.mouseButton.button ==
                      i)
                  {
                    _button[i] = false;
                    _buttonpressed[i] = false;
                  }
                }
              }
              // Key activate
              if (_key[Keyboard::Escape] &&
                  !_keypressed[Keyboard::Escape])
              {
                _windowError.close();
                _keypressed[Keyboard::Escape] = true;
              }
              _windowError.clear();
              _windowError.draw(_errorBgd);
              _windowError.draw(_error);
              _windowError.draw(_errorText);
              _cursor.setPosition((Vector2f)Mouse::getPosition(_windowError));
              _windowError.draw(_cursor);
              _windowError.display();
            }
          }
          else
          {
            _playerIsInLauncher = false;
            _playerIsInGame = true;
            _windowLauncher.close();
          }
        }
      }
      else
      {
        _jouerBouton.setColor(Color(255,255,255,255));
      }
      // Form
      if (contact(_cursor.getPosition(), _cursor.getLocalBounds(),
                  _formulaire.getPosition(), _formulaire.getLocalBounds()))
      {
        _cursor.setTextureRect(CURSOR_ONTEXT);
        if (_button[Mouse::Left] && !_buttonpressed[Mouse::Left])
        {
          _buttonpressed[Mouse::Left] = true;
          _formulaireActivated = true;
        }
      }
      else
      {
        _cursor.setTextureRect(CURSOR_NORMAL);
        if (_button[Mouse::Left] && !_buttonpressed[Mouse::Left])
        {
          _buttonpressed[Mouse::Left] = true;
          _formulaireActivated = false;
        }
      }
      // Calculus
      // Drawing
      _windowLauncher.clear();
      _windowLauncher.draw(_launcher_background);
      _windowLauncher.draw(_signGame);
      _windowLauncher.draw(_enterYourPseudo);
      if (_formulaireActivated)
        _formulaire.setColor(Color(255,255,211,233));
      else
        _formulaire.setColor(Color(255,255,255,255));
      _windowLauncher.draw(_formulaire);
      _windowLauncher.draw(_jouerBouton);
      _windowLauncher.draw(_formulaireText);
      // Cursor always last !
      _cursor.setPosition((Vector2f)Mouse::getPosition(_windowLauncher));
      _windowLauncher.draw(_cursor);
      /// Monitoring
      // Control FPS
      sf::sleep(milliseconds(_t_fps - _chrono.getElapsedTime().asMilliseconds()));
      _lasttime = _chrono.getElapsedTime().asMilliseconds();
      // Display
      _windowLauncher.display();
      // Updating time passed
      _t_passedInLauncher +=
        _chrono.getElapsedTime().asMilliseconds();
    }
  }
  _windowLauncher.close();
  /// Strecth pseudonyme
  string _pseudo = _formulaireStr;
  _socket.disconnect();
  _cursor.setTextureRect(CURSOR_NORMAL);
  RenderWindow _window;
  if (!(!_playerIsInGame && !_playerIsInLauncher && !_playerIsInMenu &&
        !_playerIsInOption))
  {
    /// Rendering Window
    // Création de la fenêtre
    _window.create(VideoMode(WIN_WIDTH,WIN_HEIGHT), "Narwhal");
    _window.setIcon(32, 32, _windowIcon.getPixelsPtr());
    // Play music
    _music.play();
    /// Main Loop
    _window.requestFocus();
  }
  while (_window.isOpen())
  {
    /// InGame Loop
    if (_playerIsInGame)
    {
      /// REFRESH EVERYTHING BEFORE IT REBOOTS
      if (_gameHasToReboot)
      {
        _dragon.setPosition((WIN_WIDTH-_dragon.getLocalBounds().width)/2,
                            (WIN_HEIGHT-_dragon.getLocalBounds().height)/2);
        _epolarPhasesTime[0] = EPOLAR_PHASETIME_0;
        _epolarPhasesTime[1] = EPOLAR_PHASETIME_1;
        _epolarPhasesTime[2] = EPOLAR_PHASETIME_2;
        _epolarPhasesTime[3] = EPOLAR_PHASETIME_3;
        _epolarPhasesTime[4] = EPOLAR_PHASETIME_4;
        _dragonFireballPowered = false;
        _backgroundRect.left = 0;
        _backgroundRect.top = 0;
        // ROCKS
        for (unsigned int i = 0 ; i < MAX_ROCKS ; ++i)
        {
          if (_backgroundRocks[i] != 0)
          {
            delete _backgroundRocks[i];
            _backgroundRocks[i] = 0;
          }
        }
        // FIREBALLS
        for (unsigned int i = 0 ; i < MAX_FIREBALL ; ++i)
        {
          if (_fireball[i] != 0)
          {
            delete _fireball[i];
            _fireball[i] = 0;
          }
        }
        // MONSTER SPRITES
        for (unsigned int i = 0 ; i < MAX_MONSTER ; ++i)
        {
          if (_monster[i] != 0)
          {
            delete _monster[i];
            _monster[i] = 0;
          }
        }
        // SOUNDS
        for (unsigned int i = 0 ; i < MAX_SOUNDS ; ++i)
        {
          if (_firehorn[i] != 0)
          {
            delete _firehorn[i];
            _firehorn[i] = 0;
          }
        }
        // BONUS
        for (unsigned int i = 0 ; i < MAX_BONUS ; ++i)
        {
          if (_bonus[i] != 0)
          {
            delete _bonus[i];
            _bonus[i] = 0;
            delete _bonusLife[i];
            _bonusLife[i] = 0;
          }
        }
        // EPOLARS
        for (unsigned int i = 0 ; i < MAX_EPOLAR ; ++i)
        {
          if (_epolar[i] != 0)
          {
            delete _epolar[i];
            _epolar[i] = 0;
          }
          if (_epolarHealth[i] != 0)
          {
            delete _epolarHealth[i];
            _epolarHealth[i] = 0;
          }
          if (_epolarHealthMax[i] != 0)
          {
            delete _epolarHealthMax[i];
            _epolarHealthMax[i] = 0;
          }
          if (_epolarHealthBar[i] != 0)
          {
            delete _epolarHealthBar[i];
            _epolarHealthBar[i] = 0;
          }
          if (_epolarHealthBarDyn[i] != 0)
          {
            delete _epolarHealthBarDyn[i];
            _epolarHealthBarDyn[i] = 0;
          }
        }
        _gameHasToReboot = false;
      }
      // Classic For Loop in Videogame Window Management
      for (Clock _chrono ; _playerIsInGame && _window.isOpen() ; )
      {
        /// ALWAYS REFRESH START TIME BEFORE ENTERING ANY MENU
        _chrono.restart();
        /// Event Managing InGame
        // Event Binding
        if (_window.pollEvent(_event))
        {
          if (_event.type == Event::Closed) //Red Cross
          {
            _window.close();
            _playerIsInGame = false;
          }
          for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
          {
            if (_event.type == Event::KeyPressed &&
                _event.key.code == i)
            {
              _key[i] = true;
            }
            if (_event.type == Event::KeyReleased &&
                _event.key.code == i)
            {
              _key[i] = false;
              _keypressed[i] = false;
            }
          }
        }
        // Key Checking
        if (_key[Keyboard::Escape] &&
            !_keypressed[Keyboard::Escape]) /// [Escape] -> Quit the game
        {
          _playerIsInGame = false;
          if (_dragonIsAlive)
            _playerIsInOption = true;
          else
            _playerIsInMenu = true;
          _keypressed[Keyboard::Escape] = true;
        }
        if (_key[_keyid[KEYB_BACKWARD]] && _dragonIsAlive)
          /// BACKWARD -> Backward
        {
          _dragon.move(-_dragonSpeed,0);
          _dragonFaceToEnd = false;
        }
        if (_key[_keyid[KEYB_FORWARD]] && _dragonIsAlive)
          /// FORWARD -> Forward
        {
          _dragon.move(_dragonSpeed,0);
          _dragonFaceToEnd = true;
        }
        if (_key[_keyid[KEYB_GODOWN]] && _dragonIsAlive)
          /// GO_DOWN -> Go down
        {
          _dragon.move(0,_dragonSpeed);
        }
        if (_key[_keyid[KEYB_GOUP]] && _dragonIsAlive)
          /// GO_UP -> Fly up
        {
          _dragon.move(0,-_dragonSpeed);
        }
        if (_key[_keyid[KEYB_ACCELERATE]])
          /// Hold ACCELERATE -> Speed up
        {
          _dragonSpeed = 20.0f;
          _dragonAnimSpeed = HIGH_SPEED_ANIM;
        }
        if (!_key[_keyid[KEYB_ACCELERATE]])
          /// Release ACCELERATE -> Slow down
        {
          _dragonSpeed = 7.0f;
          _dragonAnimSpeed = NORMAL_SPEED_ANIM;
        }
        if (_key[_keyid[KEYB_FIREBALL]] && _dragonIsAlive)
          /// FIREBALL -> Throw ONE Fireball
        {
          for (unsigned int i = 0 ; i < MAX_SOUNDS ; ++i)
          {
            if (_firehorn[i] == 0 && !_fireballThrown)
            {
              _firehorn[i] = new Sound(_firehorn_buffer);
              _firehorn[i]->setVolume(_firehorn_volume);
              _firehorn[i]->play();
              break;
            }
          }
          for (unsigned int i = 0 ; i < MAX_FIREBALL ; ++i)
          {
            if (_fireball[i] == 0 && !_fireballThrown) // Bluko can spam while being boosted
            {
              if (!_dragonFireballPowered)
              {
                _fireball[i] = (_dragonFaceToEnd) ? new
                               Sprite(_fireballText, _fireballSect[0]) : new
                               Sprite(_fireballText, _fireballSect[1]);
                if (_dragonFaceToEnd)
                  _fireball[i]->move(_dragon.getPosition().x + 150,
                                     _dragon.getPosition().y);
                else
                  _fireball[i]->move(_dragon.getPosition().x, _dragon.getPosition().y);
                _fireballThrown = true;
                break;
              }
              else
              {
                _fireball[i] = new Sprite(_fireballText,
                                          _fireballSect[0]);
                _fireball[i]->move(_dragon.getPosition().x
                                   + 150, _dragon.getPosition().y);
                for (unsigned int j = i+1 ; j <
                     MAX_FIREBALL ; ++j)
                {
                  if (_fireball[j] == 0)
                  {
                    _fireball[j] = new
                    Sprite(_fireballText, _fireballSect[1]);
                    _fireball[j]->move(_dragon.getPosition().x, _dragon.getPosition().y);
                    break;
                  }
                }
                break;
              }
            }
          }
          _keypressed[_keyid[KEYB_FIREBALL]] = true;
        }
        if (!_key[_keyid[KEYB_FIREBALL]])
          /// Release FIREBALL -> Allow Bluko to throw an another fireball :)
        {
          _fireballThrown = false;
        }
        if (_key[_keyid[KEYB_FPSDISP]] &&
            !_keypressed[_keyid[KEYB_FPSDISP]]) /// Press FPS_DISP -> Show/Hide FPS Count
        {
          if (_showFPS)
            _showFPS = false;
          else
            _showFPS = true;
          _keypressed[_keyid[KEYB_FPSDISP]] = true;
        }
        /***************\
        ---> CALCULS <---
        \***************/
        /// Fireballs
        for (unsigned int i = 0 ; i < MAX_FIREBALL ; ++i)
        {
          if (_fireball[i] != 0) /// If the fireball exists !
          {
            if (_fireball[i]->getTextureRect().left == 0)
            {
              _fireball[i]->move(FIREBALL_SPEED,0);
            }
            else
            {
              _fireball[i]->move(-FIREBALL_SPEED,0);
            }
            if (_fireball[i]->getPosition().x > WIN_WIDTH ||
                _fireball[i]->getPosition().x + 202 < 0)
            {
              delete _fireball[i];
              _fireball[i] = 0;
            }
          }
        }
        /// Monsters
        // Spawning
        if (_t_passedInGame >= nextMonsterSpawnDate(_monsterTotal))
        {
          for (unsigned int i = 0 ; i < MAX_MONSTER ; ++i)
          {
            if (_monster[i] == 0)
            {
              _monsterCount++;
              _monsterTotal++;
              _monsterNewSect = rand() % _monsterNarwhals;
              _monsterRect.top =
                _monsterSect[_monsterNewSect];
              _monsterRect.height =
                _monsterSect[_monsterNewSect+1] - _monsterSect[_monsterNewSect];
              _monster[i] = new Sprite(_monsterText,
                                       _monsterRect);
              _monsterLastSpawnPos = rand() %
                                     MONSTER_SPAWN_NBR;
              if (_monsterLastSpawnPos == 0)
                _monster[i]->move(0 -
                                  _monster[i]->getLocalBounds().width, (WIN_HEIGHT -
                                      _monster[i]->getLocalBounds().height)/2);
              else if (_monsterLastSpawnPos == 1)
                _monster[i]->move(0 -
                                  _monster[i]->getLocalBounds().width, 0 -
                                  _monster[i]->getLocalBounds().height);
              else if (_monsterLastSpawnPos == 2)
                _monster[i]->move((WIN_WIDTH -
                                   _monster[i]->getLocalBounds().width)/2, 0 -
                                  _monster[i]->getLocalBounds().height);
              else if (_monsterLastSpawnPos == 3)
                _monster[i]->move(WIN_WIDTH,
                                  0 -
                                  _monster[i]->getLocalBounds().height);
              else if (_monsterLastSpawnPos == 4)
                _monster[i]->move(WIN_WIDTH,
                                  (WIN_HEIGHT -
                                   _monster[i]->getLocalBounds().height)/2);
              else if (_monsterLastSpawnPos == 5)
                _monster[i]->move(WIN_WIDTH,
                                  WIN_HEIGHT);
              else if (_monsterLastSpawnPos == 6)
                _monster[i]->move((WIN_WIDTH -
                                   _monster[i]->getLocalBounds().width)/2, WIN_HEIGHT);
              else
                _monster[i]->move(0,
                                  WIN_HEIGHT);
              break;
            }
          }
        }
        /// Acceleration
        _monsterSpeed = MONSTER_INIT_SPEED + MONSTER_ACCELERATION
                        * _monsterTotal; // +2%
        /// Drawing
        for (unsigned int i = 0 ; i < MAX_MONSTER ; ++i)
        {
          if (_monster[i] != 0) /// If the monster exists !
          {
            // Hunting Head Movement System by HarryBreak
            // Hanouna must go to dragon center and not dragon origine position given by SFML System
            _monster[i]->move((_dragon.getPosition().x +
                               _dragon.getLocalBounds().width/2 -
                               _monster[i]->getPosition().x)*_monsterSpeed,
                              (_dragon.getPosition().y +
                               _dragon.getLocalBounds().height/2-
                               _monster[i]->getPosition().y)*_monsterSpeed);
            for (unsigned int j = 0 ; j < MAX_FIREBALL ; ++j)
            {
              if (_fireball[j] != 0) /// If the fireball exists !
              {
                if (contact(_monster[i]->getPosition(),
                            _monster[i]->getLocalBounds(), _fireball[j]->getPosition(),
                            _fireball[j]->getLocalBounds(), 15.0f))
                {
                  delete _monster[i];
                  _monster[i] = 0;
                  _monsterCount--;
                  delete _fireball[j];
                  _fireball[j] = 0;
                  _dragonKill++;
                  break;
                }
              }
            }
          }
        }
        /// Is Bluko killed by Hanouna ? :(
        for (unsigned int i = 0 ; i < MAX_MONSTER ; i++)
        {
          if (_monster[i] != 0)
          {
            if (contact(_dragon.getPosition(),
                        _dragon.getLocalBounds(), _monster[i]->getPosition(),
                        _monster[i]->getLocalBounds(), 50.0f))
            {
              _dragonIsAlive = false;
            }
          }
        }
        /// Bonus Apparition
        // Spawning
        if (_t_passedInGame >= nextBonusSpawnDate(_bonusCount))
        {
          for (unsigned int i = 0 ; i < MAX_BONUS ; ++i)
          {
            if (_bonus[i] == 0)
            {
              _bonus[i] = new Sprite(_bonusText,
                                     _bonusRect);
              _bonusLastSpawnPosX = rand() % (WIN_WIDTH -
                                              (int)_bonus[i]->getLocalBounds().width);
              _bonusLastSpawnPosY = rand() % (WIN_HEIGHT-
                                              (int)_bonus[i]->getLocalBounds().height);
              _bonus[i]->move((float)_bonusLastSpawnPosX,
                              (float)_bonusLastSpawnPosY);
              _bonusLife[i] = new Int32(5000);
              _bonusCount++;
              break;
            }
          }
        }
        // Bonus death
        for (unsigned int i = 0 ; i < MAX_BONUS ; ++i)
        {
          if (_bonus[i] != 0)
          {
            *_bonusLife[i] -= _lasttime;
            if (*_bonusLife[i] < 0)
            {
              delete _bonusLife[i];
              delete _bonus[i];
              _bonusLife[i] = 0;
              _bonus[i] = 0;
            }
          }
        }
        // Contact with Dragon
        for (unsigned int i = 0 ; i < MAX_BONUS ; ++i)
        {
          if (_bonus[i] != 0)
          {
            if (contact(_dragon.getPosition(),
                        _dragon.getLocalBounds(), _bonus[i]->getPosition(),
                        _bonus[i]->getLocalBounds(), 6.0f))
            {
              delete _bonusLife[i];
              _bonusLife[i] = 0;
              delete _bonus[i];
              _bonus[i] = 0;
              _dragonFireballPowered = true;
              _dragonGotPowered = _t_passedInGame;
            }
          }
        }
        // Stop it when it's over
        if (_t_passedInGame - _dragonGotPowered >=
            FBEFFECT_DURATION) _dragonFireballPowered = false;
        /// Epolar Boss
        // Spawning
        if (_t_passedInGame >= nextEpolarSpawnDate(_epolarTotalCount))
        {
          for (unsigned int i = 0 ; i < MAX_EPOLAR ; i++)
          {
            if (_epolar[i] == 0)
            {
              _epolarTotalCount++;
              _epolarCount++;
              // New Sprite
              _epolar[i] = new Sprite(_epolarText,
                                      _epolarRect);
              _epolarHealth[i] = new int(EPOLAR_INIT_HEALTH
                                         + (_epolarTotalCount-1) * EPOLAR_GAIN_HEALTH);
              _epolarHealthMax[i] = new
              int(EPOLAR_INIT_HEALTH + (_epolarTotalCount-1) * EPOLAR_GAIN_HEALTH);
              _epolar[i]->move(WIN_WIDTH, 0);
              // New Health Bar
              _epolarHealthBar[i] = new
              Sprite(_epolarHealthBarText);
              _epolarHealthBarDyn[i] = new
              RectangleShape(Vector2f(180.0f, 7.0f));
              _epolarHealthBarDyn[i]->setFillColor(Color::Red);
              _epolarHealthBarDyn[i]->move(_epolarHealthBar[i]->getPosition().x + 49,
                                           _epolarHealthBar[i]->getPosition().y + 4);
              _epolarPhases[i][0] = _t_passedInGame;
              for (unsigned int j = 1 ; j < EPOLAR_PHASES ; ++j)
                _epolarPhases[i][j] = 0;
              for (unsigned int j = 1 ; j < EPOLAR_PHASES ; ++j)
                _epolarPhasesTime[j] = static_cast<Int32>(_epolarPhasesTime[i] * EPOLAR_PHASETIME_ACC); // Accurate * 0.92
              _epolarStart[i] = false;
              _epolarFaceToEnd = false;
              break;
            }
          }
        }
        // Drawing and Hit by Bluko
        for (unsigned int i = 0 ; i < MAX_EPOLAR ; i++)
        {
          if (_epolar[i] != 0)
          {
            if (!_epolarStart[i]) // Introduction of the Epolar
            {
              _epolar[i]->move((-_epolar[i]->getLocalBounds().width/_fpscount)*(1000/_epolarPhasesTime[0]), 0);  // 1 unit of epolar in 1 second
              if (_t_passedInGame - _epolarPhases[i][0] > _epolarPhasesTime[0])
              {
                _epolarStart[i] = true;
                _epolarPhases[i][0] = 0;
                _epolarPhases[i][1] = _t_passedInGame;
              }
            }
            else // THE IA OF THE EPOLARS IS HERE !!
            {
              if (_epolarPhases[i][1] != 0)
              {
                if (_epolarGoesUp[i])
                {
                  _epolar[i]->move(0,-_epolarSpeed);
                  if (_epolar[i]->getPosition().y < 0)
                    _epolarGoesUp[i] = false;
                }
                else
                {
                  _epolar[i]->move(0, _epolarSpeed);
                  if (_epolar[i]->getPosition().y >
                      WIN_HEIGHT - _epolar[i]->getLocalBounds().height)
                    _epolarGoesUp[i] = true;
                }
                if (_t_passedInGame - _epolarPhases[i][1] > _epolarPhasesTime[1])
                {
                  _epolarYDiff =
                    objectif_y(_epolar[i]->getPosition(),_dragon.getPosition(),0) -
                    _epolar[i]->getPosition().y;
                  _epolarPhases[i][1] = 0;
                  _epolarPhases[i][2] = _t_passedInGame;
                }
              }
              if (_epolarPhases[i][2] != 0)
              {
                _epolar[i]->move((-WIN_WIDTH/_fpscount)*(1000.0f/_epolarPhasesTime[2]),
                                 (_epolarYDiff/_fpscount)*(1000.0f/_epolarPhasesTime[2]));
                if (_t_passedInGame - _epolarPhases[i][2] > _epolarPhasesTime[2])
                {
                  _epolarFaceToEnd = true;
                  _epolarPhases[i][2] = 0;
                  _epolarPhases[i][3] = _t_passedInGame;
                }
              }
              if (_epolarPhases[i][3] != 0)
              {
                if (_epolarGoesUp[i])
                {
                  _epolar[i]->move(0,-_epolarSpeed);
                  if (_epolar[i]->getPosition().y < 0)
                    _epolarGoesUp[i] = false;
                }
                else
                {
                  _epolar[i]->move(0, _epolarSpeed);
                  if (_epolar[i]->getPosition().y >
                      WIN_HEIGHT - _epolar[i]->getLocalBounds().height)
                    _epolarGoesUp[i] = true;
                }
                if (_t_passedInGame - _epolarPhases[i][3] > _epolarPhasesTime[3])
                {
                  _epolarYDiff =
                    objectif_y(_epolar[i]->getPosition(),_dragon.getPosition(),WIN_WIDTH -
                               _epolar[i]->getLocalBounds().width) - _epolar[i]->getPosition().y;
                  _epolarPhases[i][3] = 0;
                  _epolarPhases[i][4] = _t_passedInGame;
                }
              }
              if (_epolarPhases[i][4] != 0)
              {
                _epolar[i]->move((WIN_WIDTH/_fpscount)*(1000.0f/_epolarPhasesTime[4]), (_epolarYDiff/_fpscount)*(1000.0f/_epolarPhasesTime[4]));
                if (_t_passedInGame - _epolarPhases[i][4] > _epolarPhasesTime[4])
                {
                  _epolarFaceToEnd = false;
                  _epolarPhases[i][4] = 0;
                  _epolarPhases[i][1] = _t_passedInGame;
                }
              }
            }
            // Health bar
            _epolarHealthBar[i]->setPosition(_epolar[i]->getPosition() + Vector2f(0,20));
            _epolarHealthBarDyn[i]->setPosition(_epolarHealthBar[i]->getPosition() + Vector2f(49,4));
            // Hit
            for (unsigned int j = 0 ; j < MAX_FIREBALL &&
                 _epolarStart[i] ; ++j)
            {
              if (_fireball[j] != 0) /// If the fireball exists !
              {
                if (contact(_epolar[i]->getPosition(),
                            _epolar[i]->getLocalBounds(), _fireball[j]->getPosition(),
                            _fireball[j]->getLocalBounds(), 15.0f))
                {
                  *_epolarHealth[i] -= 1;
                  _epolarHealthBarDyn[i]->setSize(Vector2f(180 * ((float)(*_epolarHealth[i])
                                                  / (float)(*_epolarHealthMax[i])), 7));
                  if (*_epolarHealth[i] == 0)
                  {
                    delete _epolar[i];
                    _epolar[i] = 0;
                    delete _epolarHealth[i];
                    _epolarHealth[i] = 0;
                    delete _epolarHealthBar[i];
                    _epolarHealthBar[i] = 0;
                    delete _epolarHealthMax[i];
                    _epolarHealthBar[i] = 0;
                    delete _epolarHealthBarDyn[i];
                    _epolarHealthBarDyn[i] = 0;
                    _dragonKill += 5;
                    _epolarCount--;
                  }
                  delete _fireball[j];
                  _fireball[j] = 0;
                  break;
                }
              }
            }
          }
        }
        // Epolar Acceleration
        _epolarSpeed = EPOLAR_INIT_SPEED + EPOLAR_ACCELERATION *
                       _epolarTotalCount; //+16%
        // Is Bluko killed by Epolar ? :(
        for (unsigned int i = 0 ; i < MAX_EPOLAR ; i++)
        {
          if (_epolar[i] != 0)
          {
            if (contact(_dragon.getPosition(),
                        _dragon.getLocalBounds(), _epolar[i]->getPosition(),
                        _epolar[i]->getLocalBounds(), 50.0f))
            {
              _dragonIsAlive = false;
            }
          }
        }
        /// Dragon Animation
        if (_dragonIsAlive)
        {
          if (_dragonTime.getElapsedTime().asMilliseconds() >
              _dragonAnimSpeed || _dragonFacedToEnd != _dragonFaceToEnd)
          {
            if (_dragonFaceToEnd)
            {
              switch (_dragonActualSect)
              {
                case 1:
                  _dragonActualSect++;
                  _dragonIsFlying = true;
                  break;
                case 2:
                  if (_dragonIsFlying)
                    _dragonActualSect++;
                  else
                    _dragonActualSect--;
                  break;
                case 3:
                  if (_dragonIsFlying)
                    _dragonActualSect++;
                  else
                    _dragonActualSect--;
                  break;
                case 4:
                  _dragonActualSect--;
                  _dragonIsFlying = false;
                  break;
                case 5:
                  _dragonActualSect = 2;
                  _dragonIsFlying = true;
                  break;
                case 6:
                  if (_dragonIsFlying)
                    _dragonActualSect = 3;
                  else
                    _dragonActualSect = 1;
                  break;
                case 7:
                  if (_dragonIsFlying)
                    _dragonActualSect = 4;
                  else
                    _dragonActualSect = 2;
                  break;
                case 8:
                  _dragonActualSect = 3;
                  _dragonIsFlying = false;
                default:
                  _dragonActualSect = 1;
                  _dragonIsFlying = true;
                  break;
              }
              _dragonFacedToEnd = _dragonFaceToEnd;
            }
            else
            {
              switch (_dragonActualSect)
              {
                case 1:
                  _dragonActualSect = 6;
                  _dragonIsFlying = true;
                  break;
                case 2:
                  if (_dragonIsFlying)
                    _dragonActualSect = 7;
                  else
                    _dragonActualSect = 5;
                  break;
                case 3:
                  if (_dragonIsFlying)
                    _dragonActualSect = 8;
                  else
                    _dragonActualSect = 6;
                  break;
                case 4:
                  _dragonActualSect = 7;
                  _dragonIsFlying = false;
                case 5:
                  _dragonActualSect++;
                  _dragonIsFlying = true;
                  break;
                case 6:
                  if (_dragonIsFlying)
                    _dragonActualSect++;
                  else
                    _dragonActualSect--;
                  break;
                case 7:
                  if (_dragonIsFlying)
                    _dragonActualSect++;
                  else
                    _dragonActualSect--;
                  break;
                case 8:
                  _dragonActualSect--;
                  _dragonIsFlying = false;
                  break;
                default:
                  _dragonActualSect = 5;
                  _dragonIsFlying = true;
                  break;
              }
              _dragonFacedToEnd = _dragonFaceToEnd;
            }
            _dragonTime.restart();
          }
        }
        else
        {
          _dragonActualSect = (_dragonFaceToEnd) ? 9 : 10;
        }
        // Update dragon sprite
        if (_dragonFireballPowered && _dragonIsAlive)
        {
          _dragonRect.left = _dragonSect[_dragonActualSect-1] +
                             _dragonSect[10];
          _dragonRect.width = _dragonSect[_dragonActualSect] -
                              _dragonSect[_dragonActualSect-1];
          _dragon.setTextureRect(_dragonRect);
          _dragonAnimSpeed = HIGH_SPEED_ANIM;
        }
        else
        {
          _dragonRect.left = _dragonSect[_dragonActualSect-1];
          _dragonRect.width = _dragonSect[_dragonActualSect] -
                              _dragonSect[_dragonActualSect-1];
          _dragon.setTextureRect(_dragonRect);
        }
        /// Any Epolar Animation
        if (_epolarTime.getElapsedTime().asMilliseconds() >
            NORMAL_SPEED_ANIM || _epolarFacedToEnd != _epolarFaceToEnd)
        {
          if (!_epolarFaceToEnd)
          {
            switch (_epolarActualSect)
            {
              case 1:
                _epolarActualSect++;
                _epolarIsFlying = true;
                break;
              case 2:
                if (_epolarIsFlying)
                  _epolarActualSect++;
                else _epolarActualSect--;
                break;
              case 3:
                if (_epolarIsFlying)
                  _epolarActualSect++;
                else _epolarActualSect--;
                break;
              case 4:
                if (_epolarIsFlying)
                  _epolarActualSect++;
                else _epolarActualSect--;
                break;
              case 5:
                _epolarActualSect--;
                _epolarIsFlying = false;
                break;
              case 6:
                _epolarActualSect = 4;
                _epolarIsFlying = false;
                break;
              case 7:
                if (_epolarIsFlying)
                  _epolarActualSect = 5;
                else _epolarActualSect = 3;
                break;
              case 8:
                if (_epolarIsFlying)
                  _epolarActualSect = 4;
                else _epolarActualSect = 2;
                break;
              case 9:
                if (_epolarIsFlying)
                  _epolarActualSect = 3;
                else _epolarActualSect = 1;
                break;
              default:
                _epolarActualSect = 2;
                _epolarIsFlying = true;
                break;
            }
            _epolarFacedToEnd = _epolarFaceToEnd;
          }
          else
          {
            switch (_epolarActualSect)
            {
              case 1:
                _epolarActualSect = 9;
                _epolarIsFlying = true;
                break;
              case 2:
                if (_epolarIsFlying)
                  _epolarActualSect = 8;
                else _epolarActualSect = 10;
                break;
              case 3:
                if (_epolarIsFlying)
                  _epolarActualSect = 7;
                else _epolarActualSect = 9;
                break;
              case 4:
                if (_epolarIsFlying)
                  _epolarActualSect = 6;
                else _epolarActualSect = 8;
                break;
              case 5:
                _epolarActualSect = 7;
                _epolarIsFlying = false;
                break;
              case 6:
                _epolarActualSect++;
                _epolarIsFlying = false;
              case 7:
                if (_epolarIsFlying)
                  _epolarActualSect--;
                else _epolarActualSect++;
                break;
              case 8:
                if (_epolarIsFlying)
                  _epolarActualSect--;
                else _epolarActualSect++;
                break;
              case 9:
                if (_epolarIsFlying)
                  _epolarActualSect--;
                else _epolarActualSect++;
                break;
              default:
                _epolarActualSect = 9;
                _epolarIsFlying = true;
                break;
            }
            _epolarFacedToEnd = _epolarFaceToEnd;
          }
          _epolarTime.restart();
        }
        // Setting Epolars Sprite IntRect
        for (unsigned int i = 0 ; i < MAX_EPOLAR ; i++)
        {
          if (_epolar[i] != 0)
          {
            _epolarRect.left =
              _epolarSect[_epolarActualSect-1];
            _epolarRect.width = _epolarSect[_epolarActualSect]
                                - _epolarSect[_epolarActualSect-1];
            _epolar[i]->setTextureRect(_epolarRect);
          }
        }
        /// Bonus Animation
        if (_bonusTime.getElapsedTime().asMilliseconds() >
            HIGH_SPEED_ANIM)
        {
          switch (_bonusActualSect)
          {
            case 4:
              _bonusActualSect = 1;
              break;
            default:
              _bonusActualSect++;
              break;
          }
          _bonusTime.restart();
        }
        // Update bonus sprite
        _bonusRect.left = _bonusSect[_bonusActualSect-1];
        _bonusRect.width = _bonusSect[_bonusActualSect] -
                           _bonusSect[_bonusActualSect-1];
        for (unsigned int i = 0 ; i < MAX_BONUS ; i++)
        {
          if (_bonus[i] != 0)
          {
            _bonus[i]->setTextureRect(_bonusRect);
          }
        }
        /// Finally, death of Bluko
        if (!_dragonIsAlive)
        {
          if (!_dragonHasBeenKilled)
          {
            _dragon.move(0,120);
            _dragonHasBeenKilled = true;
          }
          _dragon.move(0,1);
        }
        /// ATH
        sprintf_s(_texte, "Score : %d", _dragonKill);
        _texteS = _texte;
        _score.setString(_texteS);
        /// Background Advancing
        if (_backgroundRect.top >= MAX_BACKGROUND_ADVANCEMENT)
        {
          if (_t_passedInGame >= _backgroundLightNext)
          {
            _backgroundLightNext = _t_passedInGame + 96;
            _backgroundLight--;
          }
          _background.setColor(Color(max(0,_backgroundLight),max(0,_backgroundLight)
                                     ,max(101,_backgroundLight)));
          if (_t_passedInGame >= _backgroundNextRock)
          {
            for (unsigned int i = 0 ; i < MAX_ROCKS ; i++)
            {
              if (_backgroundRocks[i] == 0)
              {
                _backgroundNextRock = _t_passedInGame +
                                      (Uint32)(100000/(log(_t_passedInGame)/log(1.5))) + (rand() % 1000);
                _backgroundRocksIndex = rand() % 6;
                _backgroundRocksRect =
                {
                  _backgroundRocksInt[_backgroundRocksIndex], 0,
                  _backgroundRocksInt[_backgroundRocksIndex+1]-_backgroundRocksInt[_backgroundRocksIndex], 400
                };
                _backgroundRocks[i] = new
                Sprite(_backgroundRocksText, _backgroundRocksRect);
                _backgroundRocks[i]->scale({2.0f, 1.0f});
                _backgroundRocks[i]->move(WIN_WIDTH,
                                          WIN_HEIGHT - 180);
                break;
              }
            }
          }
        }
        else
        {
          _backgroundRect.top = min(1300, _backgroundRect.top
                                    + 1);
          _backgroundRect.left = min(1918, _backgroundRect.left
                                     + 1);
        }
        _background.setTextureRect(_backgroundRect);
        // Mystery Boat
        if (_t_passedInGame >= _mysteryBoatNext)
        {
          _mysteryBoatNext = _t_passedInGame + 100000;
          _mysteryBoat.setPosition(WIN_WIDTH, WIN_HEIGHT -
                                   260);
        }
        _mysteryBoat.move(-0.89f,-0.11f);
        /// Drawing and Rendering
        _window.clear(Color(255,255,255));
        _window.draw(_background);
        _window.draw(_mysteryBoat);
        // ROCKS
        for (unsigned int i = 0 ; i < MAX_ROCKS ; ++i)
        {
          if (_backgroundRocks[i] != 0)
          {
            _backgroundRocks[i]->move(-1.49f,-0.2f);
            _backgroundRocks[i]->setColor(Color(max(40,_backgroundLight-100),max(40,_backgroundLight-100),max(40,_backgroundLight-100)));
            if (_backgroundRocks[i]->getPosition().x + 458 <
                0)
            {
              delete _backgroundRocks[i];
              _backgroundRocks[i] = 0;
            }
            else _window.draw(*_backgroundRocks[i]);
          }
        }
        _window.draw(_dragon);
        // FIREBALLS
        for (unsigned int i = 0 ; i < MAX_FIREBALL ; ++i)
        {
          if (_fireball[i] != 0)
            _window.draw(*_fireball[i]);
        }
        // MONSTER
        for (unsigned int i = 0 ; i < MAX_MONSTER ; ++i)
        {
          if (_monster[i] != 0)
            _window.draw(*_monster[i]);
        }
        // BONUS
        for (unsigned int i = 0 ; i < MAX_BONUS ; ++i)
        {
          if (_bonus[i] != 0)
            _window.draw(*_bonus[i]);
        }
        // EPOLAR (sprite and textbar)
        for (unsigned int i = 0 ; i < MAX_EPOLAR ; ++i)
        {
          if (_epolar[i] != 0)
            _window.draw(*_epolar[i]);
          if (_epolarHealthBarDyn[i] != 0)
            _window.draw(*_epolarHealthBarDyn[i]);
          if (_epolarHealthBar[i] != 0)
            _window.draw(*_epolarHealthBar[i]);
        }
        _window.draw(_score);
        _window.draw(_credits);
        if (!_dragonIsAlive)
        {
          _window.draw(_gameOver);
          _window.draw(_gameOverSubtitle);
        }
        /// Volume Updating
        _music.setVolume(_music_volume);
        if (_music_repeatfolder)
        {
          if (_music.getDuration().asSeconds() <=
              _music.getPlayingOffset().asSeconds() + 0.5f)
          {
            *_listeSelected[LISTE_MUSIQUES] =
              (_music_randomfolder) ? rand() % _music_list.size() :
              (*_listeSelected[LISTE_MUSIQUES] + 1) % _music_list.size();
            _music.stop();
            _music.openFromFile("assets/audio/musiques/"+_music_list.at(*_listeSelected[LISTE_MUSIQUES]));
            _music.play();
          }
        }
        /// Sound Updating : is any sound over ?
        for (unsigned int i = 0 ; i < MAX_SOUNDS ; i++)
        {
          if (_firehorn[i] != 0 &&
              _firehorn[i]->getPlayingOffset().asSeconds() > 1.6f)
          {
            delete _firehorn[i];
            _firehorn[i] = 0;
          }
        }
        /// FPS Controller
        sf::sleep(milliseconds(_t_fps -
                               _chrono.getElapsedTime().asMilliseconds()));
        /// FPS F3
        _lasttime = _chrono.getElapsedTime().asMilliseconds();
        _fpscount = 1000 / _lasttime;
        sprintf_s(_fpschr, "FPS : %d", _fpscount);
        _fpsstr = _fpschr;
        _fps.setString(_fpsstr);
        if (_showFPS)
          _window.draw(_fps);
        /// Displaying the final rendering
        _cursor.setPosition((Vector2f)Mouse::getPosition(_window));
        _window.draw(_cursor);
        _window.display();
        /// Updating Time Passed In Game
        _t_passedInGame +=
          _chrono.getElapsedTime().asMilliseconds();
      }
    }
    /// OPTION MENU LOOP
    if (_playerIsInOption)
    {
      _menu_prochain = MENU_MAIN;
      for (Clock _chrono ; _playerIsInOption && _window.isOpen() ;)
      {
        /// ALWAYS REFRESH START TIME BEFORE ENTERING ANY MENU
        _chrono.restart();
        // Refresh menu id
        _menu_actuel = _menu_prochain;
        /// Event Managing Options
        // Event Binding
        if (_window.pollEvent(_event))
        {
          if (_event.type == Event::Closed)
          {
            _window.close();
            _playerIsInOption = false;
          } // Red Cross
          for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
          {
            if (_event.type == Event::KeyPressed &&
                _event.key.code == i)
            {
              _key[i] = true;
            }
            if (_event.type == Event::KeyReleased &&
                _event.key.code == i)
            {
              _key[i] = false;
              _keypressed[i] = false;
            }
            if (i < Mouse::ButtonCount && _event.type ==
                Event::MouseButtonPressed && _event.mouseButton.button == i)
            {
              _button[i] = true;
            }
            if (i < Mouse::ButtonCount && _event.type ==
                Event::MouseButtonReleased && _event.mouseButton.button == i)
            {
              _button[i] = false;
              _buttonpressed[i] = false;
            }
          }
        }
        /// Mouse binding
        switch (_menu_actuel)
        {
          case MENU_MAIN:
            // Retour au jeu quand on est dans le main
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuBarre[BUT_RETOUR_JEU]->getPosition(),
                        _menuBarre[BUT_RETOUR_JEU]->getLocalBounds()))
            {
              _menuBarreRect[BUT_RETOUR_JEU]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _playerIsInGame = true;
                _playerIsInOption = false;
                _buttonpressed[Mouse::Left] = true;
              }
            }
            else
            {
              _menuBarreRect[BUT_RETOUR_JEU]->top =
                BUT_MOUSEOUT;
            }
            // Retour au jeu imminent
            if (_key[Keyboard::Escape] &&
                !_keypressed[Keyboard::Escape])
            {
              _playerIsInGame = true;
              _playerIsInOption = false;
              _keypressed[Keyboard::Escape] = true;
            }
            // Sons et musique quand on est dans le main
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f), _menuBarre[BUT_MUSIQUE]->getPosition()
                        , _menuBarre[BUT_MUSIQUE]->getLocalBounds()))
            {
              _menuBarreRect[BUT_MUSIQUE]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menu_prochain = MENU_MUSIQUE;
                _buttonpressed[Mouse::Left] = true;
              }
            }
            else
            {
              _menuBarreRect[BUT_MUSIQUE]->top =
                BUT_MOUSEOUT;
            }
            // Touches quand on est dans le main
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f), _menuBarre[BUT_TOUCHES]->getPosition()
                        , _menuBarre[BUT_TOUCHES]->getLocalBounds()))
            {
              _menuBarreRect[BUT_TOUCHES]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menu_prochain = MENU_TOUCHES;
                _buttonpressed[Mouse::Left] = true;
              }
            }
            else
            {
              _menuBarreRect[BUT_TOUCHES]->top =
                BUT_MOUSEOUT;
            }
            break;
          case MENU_MUSIQUE:
            // Retour au jeu imminent
            if (_key[Keyboard::Escape] &&
                !_keypressed[Keyboard::Escape])
            {
              _playerIsInGame = true;
              _playerIsInOption = false;
              _keypressed[Keyboard::Escape] = true;
            }
            // Retour au menu principal quand on est dans l'un des deux autres menus
            if (contact((Vector2f)Mouse::getPosition(_window), FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuBarre[BUT_RETOUR_MAIN]->getPosition(),
                        _menuBarre[BUT_RETOUR_MAIN]->getLocalBounds()))
            {
              _menuBarreRect[BUT_RETOUR_MAIN]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menu_prochain = MENU_MAIN;
                _buttonpressed[Mouse::Left] = true;
              }
            }
            else
            {
              _menuBarreRect[BUT_RETOUR_MAIN]->top =
                BUT_MOUSEOUT;
            }
            // Répétition
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuBarre[BUT_MUSIC_REPEAT]->getPosition(),
                        _menuBarre[BUT_MUSIC_REPEAT]->getLocalBounds()))
            {
              _menuBarreRect[BUT_MUSIC_REPEAT]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                if (_music_repeatfolder &&
                    !_music_randomfolder)
                {
                  _music_randomfolder = true;
                  _menuTxtBarre[BUT_MUSIC_REPEAT]->setString("Répétition : musique suivante au hasard");
                  _menuTxtBarre[BUT_MUSIC_REPEAT]->setPosition(_menuBarre[BUT_MUSIC_REPEAT]->getPosition().x + (_menuBarre[BUT_MUSIC_REPEAT]->getLocalBounds().width -
                      _menuTxtBarre[BUT_MUSIC_REPEAT]->getLocalBounds().width )/2,
                      _menuBarre[BUT_MUSIC_REPEAT]->getPosition().y +
                      (_menuBarre[BUT_MUSIC_REPEAT]->getLocalBounds().height-
                       _menuTxtBarre[BUT_MUSIC_REPEAT]->getLocalBounds().height)/4);
                }
                else if (_music_repeatfolder &&
                         _music_randomfolder)
                {
                  _music_repeatfolder = false;
                  _music_randomfolder = false;
                  _menuTxtBarre[BUT_MUSIC_REPEAT]->setString("Répétition : musique sélectionnée");
                  _menuTxtBarre[BUT_MUSIC_REPEAT]->setPosition(_menuBarre[BUT_MUSIC_REPEAT]->getPosition().x + (_menuBarre[BUT_MUSIC_REPEAT]->getLocalBounds().width -
                      _menuTxtBarre[BUT_MUSIC_REPEAT]->getLocalBounds().width )/2,
                      _menuBarre[BUT_MUSIC_REPEAT]->getPosition().y +
                      (_menuBarre[BUT_MUSIC_REPEAT]->getLocalBounds().height-
                       _menuTxtBarre[BUT_MUSIC_REPEAT]->getLocalBounds().height)/4);
                }
                else
                {
                  _music_repeatfolder = true;
                  _menuTxtBarre[BUT_MUSIC_REPEAT]->setString("Répétition : dossier entier");
                  _menuTxtBarre[BUT_MUSIC_REPEAT]->setPosition(_menuBarre[BUT_MUSIC_REPEAT]->getPosition().x + (_menuBarre[BUT_MUSIC_REPEAT]->getLocalBounds().width -
                      _menuTxtBarre[BUT_MUSIC_REPEAT]->getLocalBounds().width )/2,
                      _menuBarre[BUT_MUSIC_REPEAT]->getPosition().y +
                      (_menuBarre[BUT_MUSIC_REPEAT]->getLocalBounds().height-
                       _menuTxtBarre[BUT_MUSIC_REPEAT]->getLocalBounds().height)/4);
                }
                _buttonpressed[Mouse::Left] = true;
              }
            }
            else
            {
              _menuBarreRect[BUT_MUSIC_REPEAT]->top =
                BUT_MOUSEOUT;
            }
            // Jauge de son
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuJaugeFond[JAUGE_SON]->getPosition(),
                        _menuJaugeFond[JAUGE_SON]->getLocalBounds()))
            {
              _menuJaugeRect[JAUGE_SON]->top = BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menuJaugeCursor[JAUGE_SON]->setPosition(max(_menuJaugeFond[JAUGE_SON]->getPosition().x,
                    min((float)Mouse::getPosition(_window).x -
                        _menuJaugeCursor[JAUGE_SON]->getLocalBounds().width/2,
                        _menuJaugeFond[JAUGE_SON]->getPosition().x + 729.0f -
                        _menuJaugeCursor[JAUGE_SON]->getLocalBounds().width)),
                    _menuJaugeCursor[JAUGE_SON]->getPosition().y);
                _firehorn_volume =
                  100*(_menuJaugeCursor[JAUGE_SON]->getPosition().x -
                       _menuJaugeFond[JAUGE_SON]->getPosition().x)/(729.0f -
                           _menuJaugeCursor[JAUGE_SON]->getLocalBounds().width);
              }
            }
            else
            {
              _menuJaugeRect[JAUGE_SON]->top = BUT_MOUSEOUT;
            }
            // Jauge de musique
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuJaugeFond[JAUGE_MUSIQUE]->getPosition(),
                        _menuJaugeFond[JAUGE_MUSIQUE]->getLocalBounds()))
            {
              _menuJaugeRect[JAUGE_MUSIQUE]->top = BUT_MOUSEON;
              if (_button[Mouse::Left] && !_buttonpressed[Mouse::Left])
              {
                _menuJaugeCursor[JAUGE_MUSIQUE]->setPosition(max(_menuJaugeFond[JAUGE_MUSIQUE]->getPosition().x,
                    min((float)Mouse::getPosition(_window).x -
                        _menuJaugeCursor[JAUGE_MUSIQUE]->getLocalBounds().width/2,
                        _menuJaugeFond[JAUGE_MUSIQUE]->getPosition().x + 729.0f -
                        _menuJaugeCursor[JAUGE_MUSIQUE]->getLocalBounds().width)),
                    _menuJaugeCursor[JAUGE_MUSIQUE]->getPosition().y);
                _music_volume =
                  100*(_menuJaugeCursor[JAUGE_MUSIQUE]->getPosition().x -
                       _menuJaugeFond[JAUGE_SON]->getPosition().x)/(729.0f -
                           _menuJaugeCursor[JAUGE_MUSIQUE]->getLocalBounds().width);
              }
            }
            else
            {
              _menuJaugeRect[JAUGE_MUSIQUE]->top = BUT_MOUSEOUT;
            }
            // Liste des musiques : flèche du haut
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _listeFlecheHaut[LISTE_MUSIQUES]->getPosition(),
                        _listeFlecheHaut[LISTE_MUSIQUES]->getLocalBounds()))
            {
              _listeFlecheHaut[LISTE_MUSIQUES]->setColor(Color(191,191,191));
              if (_button[Mouse::Left] && !_buttonpressed[Mouse::Left])
              {
                *_listeCurseur[LISTE_MUSIQUES] = max((unsigned int)0, *_listeCurseur[LISTE_MUSIQUES] - 1);
                _buttonpressed[Mouse::Left] = true;
              }
            }
            else
            {
              _listeFlecheHaut[LISTE_MUSIQUES]->setColor(Color(255,255,255));
            }
            // Liste des musiques : flèche du bas
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _listeFlecheBas[LISTE_MUSIQUES]->getPosition(),
                        _listeFlecheHaut[LISTE_MUSIQUES]->getLocalBounds()))
            {
              _listeFlecheBas[LISTE_MUSIQUES]->setColor(Color(191,191,191));
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                *_listeCurseur[LISTE_MUSIQUES] = min(max((unsigned int)0, static_cast<unsigned int>(_listeChoixStr[LISTE_MUSIQUES].size()) - _listeContenance), *_listeCurseur[LISTE_MUSIQUES] + 1);
                _buttonpressed[Mouse::Left] = true;
              }
            }
            else
            {
              _listeFlecheBas[LISTE_MUSIQUES]->setColor(Color(255,255,255));
            }
            // Premier slot de la liste de la musique
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _listeChoix0[LISTE_MUSIQUES]->getPosition(),
                        _listeChoix0[LISTE_MUSIQUES]->getLocalBounds()))
            {
              if (*_listeSelected[LISTE_MUSIQUES] ==
                  *_listeCurseur[LISTE_MUSIQUES] + 0)
                _listeChoixTxt0[LISTE_MUSIQUES]->setFillColor(Color(127,255,127));
              else
                _listeChoixTxt0[LISTE_MUSIQUES]->setFillColor(Color(000,255,255));
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _music.stop();
                _music.openFromFile("assets/audio/musiques/" +
                                    _music_list.at(*_listeCurseur[LISTE_MUSIQUES] + 0));
                _music.play();
                *_listeSelected[LISTE_MUSIQUES] =
                  *_listeCurseur[LISTE_MUSIQUES] + 0;
              }
            }
            else
            {
              if (*_listeSelected[LISTE_MUSIQUES] ==
                  *_listeCurseur[LISTE_MUSIQUES] + 0)
                _listeChoixTxt0[LISTE_MUSIQUES]->setFillColor(Color(255,255,000));
              else
                _listeChoixTxt0[LISTE_MUSIQUES]->setFillColor(Color(255,255,255));
            }
            // Deuxième slot de la liste de la musique
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _listeChoix1[LISTE_MUSIQUES]->getPosition(),
                        _listeChoix1[LISTE_MUSIQUES]->getLocalBounds()))
            {
              if (*_listeSelected[LISTE_MUSIQUES] ==
                  *_listeCurseur[LISTE_MUSIQUES] + 1)
                _listeChoixTxt1[LISTE_MUSIQUES]->setFillColor(Color(127,255,127));
              else
                _listeChoixTxt1[LISTE_MUSIQUES]->setFillColor(Color(000,255,255));
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _music.stop();
                _music.openFromFile("assets/audio/musiques/" +
                                    _music_list.at(*_listeCurseur[LISTE_MUSIQUES] + 1));
                _music.play();
                *_listeSelected[LISTE_MUSIQUES] =
                  *_listeCurseur[LISTE_MUSIQUES] + 1;
              }
            }
            else
            {
              if (*_listeSelected[LISTE_MUSIQUES] ==
                  *_listeCurseur[LISTE_MUSIQUES] + 1)
                _listeChoixTxt1[LISTE_MUSIQUES]->setFillColor(Color(255,255,000));
              else
                _listeChoixTxt1[LISTE_MUSIQUES]->setFillColor(Color(255,255,255));
            }
            // Troisième slot de la liste de la musique
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _listeChoix2[LISTE_MUSIQUES]->getPosition(),
                        _listeChoix2[LISTE_MUSIQUES]->getLocalBounds()))
            {
              if (*_listeSelected[LISTE_MUSIQUES] ==
                  *_listeCurseur[LISTE_MUSIQUES] + 2)
                _listeChoixTxt2[LISTE_MUSIQUES]->setFillColor(Color(127,255,127));
              else
                _listeChoixTxt2[LISTE_MUSIQUES]->setFillColor(Color(000,255,255));
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _music.stop();
                _music.openFromFile("assets/audio/musiques/" +
                                    _music_list.at(*_listeCurseur[LISTE_MUSIQUES] + 2));
                _music.play();
                *_listeSelected[LISTE_MUSIQUES] =
                  *_listeCurseur[LISTE_MUSIQUES] + 2;
              }
            }
            else
            {
              if (*_listeSelected[LISTE_MUSIQUES] ==
                  *_listeCurseur[LISTE_MUSIQUES] + 2)
                _listeChoixTxt2[LISTE_MUSIQUES]->setFillColor(Color(255,255,000));
              else
                _listeChoixTxt2[LISTE_MUSIQUES]->setFillColor(Color(255,255,255));
            }
            break;
          case MENU_TOUCHES:
            // Retour au jeu imminent
            if (_key[Keyboard::Escape] &&
                !_keypressed[Keyboard::Escape])
            {
              _playerIsInGame = true;
              _playerIsInOption = false;
              _keypressed[Keyboard::Escape] = true;
            }
            // Retour au menu principal quand on est dans l'un des deux autres menus
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuBarre[BUT_RETOUR_MAIN]->getPosition(),
                        _menuBarre[BUT_RETOUR_MAIN]->getLocalBounds()))
            {
              _menuBarreRect[BUT_RETOUR_MAIN]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menu_prochain = MENU_MAIN;
                _buttonpressed[Mouse::Left] = true;
              }
            }
            else
            {
              _menuBarreRect[BUT_RETOUR_MAIN]->top =
                BUT_MOUSEOUT;
            }
            // La touche d'accélération
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuBarre[BUT_KEY_ACCERELATE]->getPosition(),
                        _menuBarre[BUT_KEY_ACCERELATE]->getLocalBounds()))
            {
              _menuBarreRect[BUT_KEY_ACCERELATE]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menuTxtBarre[BUT_KEY_ACCERELATE]->setFillColor(Color(255,255,0));
                _window.draw(*_menuTxtBarre[BUT_KEY_ACCERELATE]);
                _window.display();
                _menu_prochain = MENU_TOUCHES_ACCELERATE;
              }
            }
            else
            {
              _menuBarreRect[BUT_KEY_ACCERELATE]->top =
                BUT_MOUSEOUT;
            }
            // La touche pour aller à droite
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuBarre[BUT_KEY_FORWARD]->getPosition(),
                        _menuBarre[BUT_KEY_FORWARD]->getLocalBounds()))
            {
              _menuBarreRect[BUT_KEY_FORWARD]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menuTxtBarre[BUT_KEY_FORWARD]->setFillColor(Color(255,255,0));
                _window.draw(*_menuTxtBarre[BUT_KEY_FORWARD]);
                _window.display();
                _menu_prochain = MENU_TOUCHES_FORWARD;
              }
            }
            else
            {
              _menuBarreRect[BUT_KEY_FORWARD]->top =
                BUT_MOUSEOUT;
            }
            // La touche pour aller à gauche
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuBarre[BUT_KEY_BACKWARD]->getPosition(),
                        _menuBarre[BUT_KEY_BACKWARD]->getLocalBounds()))
            {
              _menuBarreRect[BUT_KEY_BACKWARD]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menuTxtBarre[BUT_KEY_BACKWARD]->setFillColor(Color(255,255,0));
                _window.draw(*_menuTxtBarre[BUT_KEY_BACKWARD]);
                _window.display();
                _menu_prochain = MENU_TOUCHES_BACKWARD;
              }
            }
            else
            {
              _menuBarreRect[BUT_KEY_BACKWARD]->top =
                BUT_MOUSEOUT;
            }
            // La touche pour aller en haut
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f), _menuBarre[BUT_KEY_GOUP]->getPosition()
                        , _menuBarre[BUT_KEY_GOUP]->getLocalBounds()))
            {
              _menuBarreRect[BUT_KEY_GOUP]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menuTxtBarre[BUT_KEY_GOUP]->setFillColor(Color(255,255,0));
                _window.draw(*_menuTxtBarre[BUT_KEY_GOUP]);
                _window.display();
                _menu_prochain = MENU_TOUCHES_GOUP;
              }
            }
            else
            {
              _menuBarreRect[BUT_KEY_GOUP]->top =
                BUT_MOUSEOUT;
            }
            // La touche pour aller en bas
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuBarre[BUT_KEY_GODOWN]->getPosition(),
                        _menuBarre[BUT_KEY_GODOWN]->getLocalBounds()))
            {
              _menuBarreRect[BUT_KEY_GODOWN]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menuTxtBarre[BUT_KEY_GODOWN]->setFillColor(Color(255,255,0));
                _window.draw(*_menuTxtBarre[BUT_KEY_GODOWN]);
                _window.display();
                _menu_prochain = MENU_TOUCHES_GODOWN;
              }
            }
            else
            {
              _menuBarreRect[BUT_KEY_GODOWN]->top =
                BUT_MOUSEOUT;
            }
            // La touche pour afficher les fps
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuBarre[BUT_KEY_FPSDISP]->getPosition(),
                        _menuBarre[BUT_KEY_FPSDISP]->getLocalBounds()))
            {
              _menuBarreRect[BUT_KEY_FPSDISP]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menuTxtBarre[BUT_KEY_FPSDISP]->setFillColor(Color(255,255,0));
                _window.draw(*_menuTxtBarre[BUT_KEY_FPSDISP]);
                _window.display();
                _menu_prochain = MENU_TOUCHES_FPSDISP;
              }
            }
            else
            {
              _menuBarreRect[BUT_KEY_FPSDISP]->top =
                BUT_MOUSEOUT;
            }
            // La touche pour tirer
            if (contact((Vector2f)Mouse::getPosition(_window)
                        , FloatRect(1.0f,1.0f,1.0f,1.0f),
                        _menuBarre[BUT_KEY_FIREBALL]->getPosition(),
                        _menuBarre[BUT_KEY_FIREBALL]->getLocalBounds()))
            {
              _menuBarreRect[BUT_KEY_FIREBALL]->top =
                BUT_MOUSEON;
              if (_button[Mouse::Left] &&
                  !_buttonpressed[Mouse::Left])
              {
                _menuTxtBarre[BUT_KEY_FIREBALL]->setFillColor(Color(255,255,0));
                _window.draw(*_menuTxtBarre[BUT_KEY_FIREBALL]);
                _window.display();
                _menu_prochain = MENU_TOUCHES_FIREBALL;
              }
            }
            else
            {
              _menuBarreRect[BUT_KEY_FIREBALL]->top =
                BUT_MOUSEOUT;
            }
            break;
          case MENU_TOUCHES_ACCELERATE:
            if (_event.type == Event::KeyPressed &&
                _event.key.code != Keyboard::Escape)
            {
              _keyid[KEYB_ACCELERATE] = _event.key.code;
              _menuTxtBarre[BUT_KEY_ACCERELATE]->setFillColor(Color(255,255,255));
              _window.draw(*_menuTxtBarre[BUT_KEY_ACCERELATE]);
              _window.display();
              _menu_prochain = MENU_TOUCHES;
            }
            break;
          case MENU_TOUCHES_FORWARD:
            if (_event.type == Event::KeyPressed &&
                _event.key.code != Keyboard::Escape)
            {
              _keyid[KEYB_FORWARD] = _event.key.code;
              _menuTxtBarre[BUT_KEY_FORWARD]->setFillColor(Color(255,255,255));
              _window.draw(*_menuTxtBarre[BUT_KEY_FORWARD]);
              _window.display();
              _menu_prochain = MENU_TOUCHES;
            }
            break;
          case MENU_TOUCHES_BACKWARD:
            if (_event.type == Event::KeyPressed &&
                _event.key.code != Keyboard::Escape)
            {
              _keyid[KEYB_BACKWARD] = _event.key.code;
              _menuTxtBarre[BUT_KEY_BACKWARD]->setFillColor(Color(255,255,255));
              _window.draw(*_menuTxtBarre[BUT_KEY_BACKWARD]);
              _window.display();
              _menu_prochain = MENU_TOUCHES;
            }
            break;
          case MENU_TOUCHES_GOUP:
            if (_event.type == Event::KeyPressed &&
                _event.key.code != Keyboard::Escape)
            {
              _keyid[KEYB_GOUP] = _event.key.code;
              _menuTxtBarre[BUT_KEY_GOUP]->setFillColor(Color(255,255,255));
              _window.draw(*_menuTxtBarre[BUT_KEY_GOUP]);
              _window.display();
              _menu_prochain = MENU_TOUCHES;
            }
            break;
          case MENU_TOUCHES_GODOWN:
            if (_event.type == Event::KeyPressed &&
                _event.key.code != Keyboard::Escape)
            {
              _keyid[KEYB_GODOWN] = _event.key.code;
              _menuTxtBarre[BUT_KEY_GODOWN]->setFillColor(Color(255,255,255));
              _window.draw(*_menuTxtBarre[BUT_KEY_GODOWN]);
              _window.display();
              _menu_prochain = MENU_TOUCHES;
            }
            break;
          case MENU_TOUCHES_FPSDISP:
            if (_event.type == Event::KeyPressed &&
                _event.key.code != Keyboard::Escape)
            {
              _keyid[KEYB_FPSDISP] = _event.key.code;
              _menuTxtBarre[BUT_KEY_FPSDISP]->setFillColor(Color(255,255,255));
              _window.draw(*_menuTxtBarre[BUT_KEY_FPSDISP]);
              _window.display();
              _menu_prochain = MENU_TOUCHES;
            }
            break;
          case MENU_TOUCHES_FIREBALL:
            if (_event.type == Event::KeyPressed &&
                _event.key.code != Keyboard::Escape)
            {
              _keyid[KEYB_FIREBALL] = _event.key.code;
              _menuTxtBarre[BUT_KEY_FIREBALL]->setFillColor(Color(255,255,255));
              _window.draw(*_menuTxtBarre[BUT_KEY_FIREBALL]);
              _window.display();
              _menu_prochain = MENU_TOUCHES;
            }
            break;
        }
        /// SOUND
        _music.setVolume(_music_volume);
        if (_music_repeatfolder)
        {
          if (_music.getDuration().asSeconds() <=
              _music.getPlayingOffset().asSeconds() + 0.5f)
          {
            *_listeSelected[LISTE_MUSIQUES] =
              (_music_randomfolder) ? rand() % _music_list.size() :
              (*_listeSelected[LISTE_MUSIQUES] + 1) % _music_list.size();
            _music.stop();
            _music.openFromFile("assets/audio/musiques/"+_music_list.at(*_listeSelected[LISTE_MUSIQUES]));
            _music.play();
          }
        }
        /// DRAWING
        for (unsigned i = 0 ; i < NBR_BUTTONS ; ++i)
        {
          _menuBarre[i]->setTextureRect(*_menuBarreRect[i]);
        }
        for (unsigned i = 0 ; i < NBR_JAUGES ; ++i)
        {
          _menuJaugeCursor[i]->setTextureRect(*_menuJaugeRect[i]);
        }
        _window.clear(Color(255,255,255));
        _window.draw(_background);
        _window.draw(_mysteryBoat);
        for (unsigned int i = 0 ; i < MAX_ROCKS ; ++i)
        {
          if (_backgroundRocks[i] != 0)
          {
            _window.draw(*_backgroundRocks[i]);
          }
        }
        _window.draw(_score);
        _window.draw(_credits);
        // Affichage selon les menus
        switch (_menu_actuel)
        {
          case MENU_MAIN:
            _window.draw(*_menuBarre[BUT_RETOUR_JEU]);
            _window.draw(*_menuTxtBarre[BUT_RETOUR_JEU]);
            _window.draw(*_menuBarre[BUT_MUSIQUE]);
            _window.draw(*_menuTxtBarre[BUT_MUSIQUE]);
            _window.draw(*_menuBarre[BUT_TOUCHES]);
            _window.draw(*_menuTxtBarre[BUT_TOUCHES]);
            break;
          case MENU_TOUCHES_ACCELERATE:
          case MENU_TOUCHES_FORWARD:
          case MENU_TOUCHES_BACKWARD:
          case MENU_TOUCHES_GOUP:
          case MENU_TOUCHES_GODOWN:
          case MENU_TOUCHES_FPSDISP:
          case MENU_TOUCHES_FIREBALL:
          case MENU_TOUCHES:
            _window.draw(*_menuBarre[BUT_RETOUR_MAIN]);
            _window.draw(*_menuTxtBarre[BUT_RETOUR_MAIN]);
            _menuTxtBarre[BUT_KEY_ACCERELATE]->setString("Accélérer : "+keystring(_keyid[KEYB_ACCELERATE]));
            _menuTxtBarre[BUT_KEY_ACCERELATE]->setPosition(_menuBarre[BUT_KEY_ACCERELATE]->getPosition().x +
                (_menuBarre[BUT_KEY_ACCERELATE]->getLocalBounds().width -
                 _menuTxtBarre[BUT_KEY_ACCERELATE]->getLocalBounds().width )/2,
                _menuBarre[BUT_KEY_ACCERELATE]->getPosition().y +
                (_menuBarre[BUT_KEY_ACCERELATE]->getLocalBounds().height-
                 _menuTxtBarre[BUT_KEY_ACCERELATE]->getLocalBounds().height)/4);
            _window.draw(*_menuBarre[BUT_KEY_ACCERELATE]);
            _window.draw(*_menuTxtBarre[BUT_KEY_ACCERELATE]);
            _menuTxtBarre[BUT_KEY_FORWARD]->setString("Aller vers la droite : "+keystring(_keyid[KEYB_FORWARD]));
            _menuTxtBarre[BUT_KEY_FORWARD]->setPosition(_menuBarre[BUT_KEY_FORWARD]->getPosition().x + (_menuBarre[BUT_KEY_FORWARD]->getLocalBounds().width -
                _menuTxtBarre[BUT_KEY_FORWARD]->getLocalBounds().width )/2,
                _menuBarre[BUT_KEY_FORWARD]->getPosition().y +
                (_menuBarre[BUT_KEY_FORWARD]->getLocalBounds().height-
                 _menuTxtBarre[BUT_KEY_FORWARD]->getLocalBounds().height)/4);
            _window.draw(*_menuBarre[BUT_KEY_FORWARD]);
            _window.draw(*_menuTxtBarre[BUT_KEY_FORWARD]);
            _menuTxtBarre[BUT_KEY_FPSDISP]->setString("Afficher les FPS : "+keystring(_keyid[KEYB_FPSDISP]));
            _menuTxtBarre[BUT_KEY_FPSDISP]->setPosition(_menuBarre[BUT_KEY_FPSDISP]->getPosition().x + (_menuBarre[BUT_KEY_FPSDISP]->getLocalBounds().width -
                _menuTxtBarre[BUT_KEY_FPSDISP]->getLocalBounds().width )/2,
                _menuBarre[BUT_KEY_FPSDISP]->getPosition().y +
                (_menuBarre[BUT_KEY_FPSDISP]->getLocalBounds().height-
                 _menuTxtBarre[BUT_KEY_FPSDISP]->getLocalBounds().height)/4);
            _window.draw(*_menuBarre[BUT_KEY_FPSDISP]);
            _window.draw(*_menuTxtBarre[BUT_KEY_FPSDISP]);
            _menuTxtBarre[BUT_KEY_FIREBALL]->setString("Tirer : "+keystring(_keyid[KEYB_FIREBALL]));
            _menuTxtBarre[BUT_KEY_FIREBALL]->setPosition(_menuBarre[BUT_KEY_FIREBALL]->getPosition().x + (_menuBarre[BUT_KEY_FIREBALL]->getLocalBounds().width -
                _menuTxtBarre[BUT_KEY_FIREBALL]->getLocalBounds().width )/2,
                _menuBarre[BUT_KEY_FIREBALL]->getPosition().y +
                (_menuBarre[BUT_KEY_FIREBALL]->getLocalBounds().height-
                 _menuTxtBarre[BUT_KEY_FIREBALL]->getLocalBounds().height)/4);
            _window.draw(*_menuBarre[BUT_KEY_FIREBALL]);
            _window.draw(*_menuTxtBarre[BUT_KEY_FIREBALL]);
            _menuTxtBarre[BUT_KEY_GOUP]->setString("Aller vers le haut : "+keystring(_keyid[KEYB_GOUP]));
            _menuTxtBarre[BUT_KEY_GOUP]->setPosition(_menuBarre[BUT_KEY_GOUP]->getPosition().x + (_menuBarre[BUT_KEY_GOUP]->getLocalBounds().width -
                _menuTxtBarre[BUT_KEY_GOUP]->getLocalBounds().width )/2,
                _menuBarre[BUT_KEY_GOUP]->getPosition().y +
                (_menuBarre[BUT_KEY_GOUP]->getLocalBounds().height-
                 _menuTxtBarre[BUT_KEY_GOUP]->getLocalBounds().height)/4);
            _window.draw(*_menuBarre[BUT_KEY_GOUP]);
            _window.draw(*_menuTxtBarre[BUT_KEY_GOUP]);
            _menuTxtBarre[BUT_KEY_BACKWARD]->setString("Aller vers la gauche : "+keystring(_keyid[KEYB_BACKWARD]));
            _menuTxtBarre[BUT_KEY_BACKWARD]->setPosition(_menuBarre[BUT_KEY_BACKWARD]->getPosition().x + (_menuBarre[BUT_KEY_BACKWARD]->getLocalBounds().width -
                _menuTxtBarre[BUT_KEY_BACKWARD]->getLocalBounds().width )/2,
                _menuBarre[BUT_KEY_BACKWARD]->getPosition().y +
                (_menuBarre[BUT_KEY_BACKWARD]->getLocalBounds().height-
                 _menuTxtBarre[BUT_KEY_BACKWARD]->getLocalBounds().height)/4);
            _window.draw(*_menuBarre[BUT_KEY_BACKWARD]);
            _window.draw(*_menuTxtBarre[BUT_KEY_BACKWARD]);
            _menuTxtBarre[BUT_KEY_GODOWN]->setString("Aller vers le bas : "+keystring(_keyid[KEYB_GODOWN]));
            _menuTxtBarre[BUT_KEY_GODOWN]->setPosition(_menuBarre[BUT_KEY_GODOWN]->getPosition().x + (_menuBarre[BUT_KEY_GODOWN]->getLocalBounds().width -
                _menuTxtBarre[BUT_KEY_GODOWN]->getLocalBounds().width )/2,
                _menuBarre[BUT_KEY_GODOWN]->getPosition().y +
                (_menuBarre[BUT_KEY_GODOWN]->getLocalBounds().height-
                 _menuTxtBarre[BUT_KEY_GODOWN]->getLocalBounds().height)/4);
            _window.draw(*_menuBarre[BUT_KEY_GODOWN]);
            _window.draw(*_menuTxtBarre[BUT_KEY_GODOWN]);
            break;
          case MENU_MUSIQUE:
            _window.draw(*_menuJaugeFond[JAUGE_SON]);
            _window.draw(*_menuJaugeCursor[JAUGE_SON]);
            _menuTxtJauge[JAUGE_SON]->setString("Volume du son : "+intToString(static_cast<int>(_firehorn_volume))+" %");
            _menuTxtJauge[JAUGE_SON]->setPosition(_menuJaugeFond[JAUGE_SON]->getPosition().x + (_menuJaugeFond[JAUGE_SON]->getLocalBounds().width -
                                                  _menuTxtJauge[JAUGE_SON]->getLocalBounds().width )/2,
                                                  _menuJaugeFond[JAUGE_SON]->getPosition().y +
                                                  (_menuJaugeFond[JAUGE_SON]->getLocalBounds().height-
                                                   _menuTxtJauge[JAUGE_SON]->getLocalBounds().height)/4);
            _window.draw(*_menuTxtJauge[JAUGE_SON]);
            _window.draw(*_menuJaugeFond[JAUGE_MUSIQUE]);
            _window.draw(*_menuJaugeCursor[JAUGE_MUSIQUE]);
            _menuTxtJauge[JAUGE_MUSIQUE]->setString("Volume de la musique : "+intToString(static_cast<int>(_music_volume))+" %");
            _menuTxtJauge[JAUGE_MUSIQUE]->setPosition(_menuJaugeFond[JAUGE_MUSIQUE]->getPosition().x + (_menuJaugeFond[JAUGE_MUSIQUE]->getLocalBounds().width -
                _menuTxtJauge[JAUGE_MUSIQUE]->getLocalBounds().width )/2,
                _menuJaugeFond[JAUGE_MUSIQUE]->getPosition().y +
                (_menuJaugeFond[JAUGE_MUSIQUE]->getLocalBounds().height-
                 _menuTxtJauge[JAUGE_MUSIQUE]->getLocalBounds().height)/4);
            _window.draw(*_menuTxtJauge[JAUGE_MUSIQUE]);
            _window.draw(*_menuBarre[BUT_RETOUR_MAIN]);
            _window.draw(*_menuTxtBarre[BUT_RETOUR_MAIN]);
            _window.draw(*_menuBarre[BUT_MUSIC_REPEAT]);
            _window.draw(*_menuTxtBarre[BUT_MUSIC_REPEAT]);
            _window.draw(*_listeNom[LISTE_MUSIQUES]);
            _window.draw(*_listeNomTxt[LISTE_MUSIQUES]);
            _window.draw(*_listeFlecheBas[LISTE_MUSIQUES]);
            _window.draw(*_listeFlecheHaut[LISTE_MUSIQUES]);
            _window.draw(*_listeChoix0[LISTE_MUSIQUES]);
            _window.draw(*_listeChoix1[LISTE_MUSIQUES]);
            _window.draw(*_listeChoix2[LISTE_MUSIQUES]);
            if (*_listeCurseur[LISTE_MUSIQUES] + 0 < _listeChoixStr[LISTE_MUSIQUES].size())
            {
              _listeChoixTxt0[LISTE_MUSIQUES]->setString(_listeChoixStr[LISTE_MUSIQUES].at(*_listeCurseur[LISTE_MUSIQUES] + 0));
            }
            else
              _listeChoixTxt0[LISTE_MUSIQUES]->setString("");
            if (*_listeCurseur[LISTE_MUSIQUES] + 1 < _listeChoixStr[LISTE_MUSIQUES].size())
            {
              _listeChoixTxt1[LISTE_MUSIQUES]->setString(_listeChoixStr[LISTE_MUSIQUES].at(*_listeCurseur[LISTE_MUSIQUES] + 1));
            }
            else
              _listeChoixTxt1[LISTE_MUSIQUES]->setString("");
            if (*_listeCurseur[LISTE_MUSIQUES] + 2 < _listeChoixStr[LISTE_MUSIQUES].size())
            {
              _listeChoixTxt2[LISTE_MUSIQUES]->setString(_listeChoixStr[LISTE_MUSIQUES].at(*_listeCurseur[LISTE_MUSIQUES] + 2));
            }
            else
              _listeChoixTxt2[LISTE_MUSIQUES]->setString("");
            _window.draw(*_listeChoixTxt0[LISTE_MUSIQUES]);
            _window.draw(*_listeChoixTxt1[LISTE_MUSIQUES]);
            _window.draw(*_listeChoixTxt2[LISTE_MUSIQUES]);
            break;
          default:
            _window.draw(*_menuBarre[BUT_RETOUR_MAIN]);
            _window.draw(*_menuTxtBarre[BUT_RETOUR_MAIN]);
            break;
        }
        _cursor.setPosition((Vector2f)Mouse::getPosition(_window));
        _window.draw(_cursor);
        _window.display();
        /// FPS Managing
        sleep (milliseconds(_t_fps -
                            _chrono.getElapsedTime().asMilliseconds()));
        _t_passedInOption +=
          _chrono.getElapsedTime().asMilliseconds();
      }
    }
    /// END SCOREBOARD LOOP
    if (_playerIsInMenu)
    {
      /// SCOREBOARD MANAGING
      if (_connected)
      {
        _window.clear(Color(255,255,255));
        _window.draw(_background);
        _window.draw(_mysteryBoat);
        for (unsigned int i = 0 ; i < MAX_ROCKS ; ++i)
        {
          if (_backgroundRocks[i] != 0)
          {
            _window.draw(*_backgroundRocks[i]);
          }
        }
        _window.draw(Text("Récupération des données en cours ...",
                          _policeFont, 40));
        _window.display();
        // Se connecter, récupérer le fichier
        _client.connect(_gtaddress, 21).getMessage();
        _client.login("297899_narwhal1411l",
                      "rt29xk").getMessage();
        _client.download("score", "", Ftp::Ascii).getMessage();
        // Récupérer les données
        _sb_amont.open("score");
        for (unsigned int i = 0 ; i < 5 ; i++)
        {
          _sb_amont >> _sb_scores[i];
          _sb_amont >> _sb_players[i];
        }
        _sb_amont.close();
        // Comparer les données et les réinscrire dans le fichier
        _sb_aval.open("score");
        for (unsigned int i = 0 ; i < 5 ; i++)
        {
          if (_sb_scores[i] < _dragonKill)
          {
            for (unsigned int j = 4 ; j > i ; j--)
            {
              _sb_scores[j] = _sb_scores[j-1];
              _sb_players[j] = _sb_players[j-1];
            }
            _sb_scores[i] = _dragonKill;
            _sb_players[i] = _pseudo;
            break;
          }
        }
        for (unsigned int i = 0 ; i < 5 ; ++i)
        {
          _sb_aval << _sb_scores[i] << " ";
          _sb_aval << _sb_players[i] << endl;
        }
        _sb_aval.close();
        // Envoyer le nouveau scoreboard au serveur et se barrer ma gueule
        _client.upload("score", "", Ftp::Ascii).getMessage();
        _client.disconnect().getMessage();
        // Afficher le scoreboard
        for (unsigned int i = 0 ; i < 5 ; i++)
        {
          sprintf_s(_sb_scores_chr[i], " : %d\n", _sb_scores[i]);
          _sb_scores_str[i] = _sb_scores_chr[i];
        }
        _sb.setString(string("------------------- SCOREBOARD--------------------\n")+ string("*")+_sb_players[0]+_sb_scores_str[0]+
                      string("*")+_sb_players[1]+_sb_scores_str[1]+
                      string("*")+_sb_players[2]+_sb_scores_str[2]+
                      string("*")+_sb_players[3]+_sb_scores_str[3]+
                      string("*")+_sb_players[4]+_sb_scores_str[4]+
                      string(" \n ")+string("Appuyez sur [Entrer] pour rejouer ou [Echap] pour quitter"));
        _sb.setPosition((WIN_WIDTH - _sb.getLocalBounds().width)
                        /2.00f, (WIN_HEIGHT-
                                 _sb.getLocalBounds().height)/2.00f);
        _window.clear(Color(255,255,255));
        _window.draw(_background);
        _window.draw(_mysteryBoat);
        for (unsigned int i = 0 ; i < MAX_ROCKS ; ++i)
        {
          if (_backgroundRocks[i] != 0)
          {
            _window.draw(*_backgroundRocks[i]);
          }
        }
        _window.draw(_sb);
        _window.display();
      }
      else
      {
        _window.clear(Color(255,255,255));
        _window.draw(_background);
        _window.draw(_mysteryBoat);
        for (unsigned int i = 0 ; i < MAX_ROCKS ; ++i)
        {
          if (_backgroundRocks[i] != 0)
          {
            _window.draw(*_backgroundRocks[i]);
          }
        }
        _window.draw(Text("Appuyez sur [Entrer] pour rejouer ou [Echap] pour quitter", _policeFont, 40));
        _window.display();
      }
      // Après le game over
      /// AfterGame Loop
      for (Clock _chrono ; _playerIsInMenu && _window.isOpen() ;)
      {
        /// ALWAYS REFRESH START TIME BEFORE ENTERING ANY MENU
        _chrono.restart();
        /// Event Managing AfterGame
        // Event Binding
        if (_window.pollEvent(_event))
        {
          if (_event.type == Event::Closed)
          {
            _window.close();
            _playerIsInMenu = false;
          } // Red Cross
          for (int i = 0 ; i < Keyboard::KeyCount ; ++i) //Keyboard
          {
            if (_event.type == Event::KeyPressed &&
                _event.key.code == i)
            {
              _key[i] = true;
            }
            if (_event.type == Event::KeyReleased &&
                _event.key.code == i)
            {
              _key[i] = false;
              _keypressed[i] = false;
            }
          }
        }
        // Key Checking
        if (_key[Keyboard::Escape] &&
            !_keypressed[Keyboard::Escape]) /// [Escape] -> Quit the game
        {
          _playerIsInMenu = false;
          _window.close();
          _keypressed[Keyboard::Escape] = true;
        }
        if (_key[Keyboard::Return] &&
            !_keypressed[Keyboard::Return]) /// [Enter] -> Restart game
        {
          _playerIsInMenu = false;
          _playerIsInGame = true;
          _backgroundRect.left = 0;
          _dragonKill = 0;
          _dragonIsAlive = true;
          _bonusCount = 0;
          _monsterTotal = 0;
          _monsterSpeed = MONSTER_INIT_SPEED;
          _monsterCount = 0;
          _epolarTotalCount = 0;
          _epolarSpeed = EPOLAR_INIT_SPEED;
          _epolarCount = 0;
          _gameHasToReboot = true;
          _t_passedInGame = 0;
          _t_passedInMenu = 0;
          _t_passedInOption = 0;
          _keypressed[Keyboard::Return] = true;
        }
        // Music
        if (_music_repeatfolder)
        {
          if (_music.getDuration().asSeconds() <=
              _music.getPlayingOffset().asSeconds() + 0.5f)
          {
            *_listeSelected[LISTE_MUSIQUES] =
              (_music_randomfolder) ? rand() % _music_list.size() :
              (*_listeSelected[LISTE_MUSIQUES] + 1) % _music_list.size();
            _music.stop();
            _music.openFromFile("assets/audio/musiques/"+_music_list.at(*_listeSelected[LISTE_MUSIQUES]));
            _music.play();
          }
        }
        // Drawing
        _window.clear(Color(255,255,255));
        _window.draw(_background);
        _window.draw(_mysteryBoat);
        for (unsigned int i = 0 ; i < MAX_ROCKS ; ++i)
        {
          if (_backgroundRocks[i] != 0)
          {
            _window.draw(*_backgroundRocks[i]);
          }
        }
        if (_connected)
          _window.draw(_sb);
        else _window.draw(Text("Appuyez sur [Entrer] pour rejouer ou [Echap] pour quitter", _policeFont, 40));
        _cursor.setPosition((Vector2f)Mouse::getPosition(_window));
        _window.draw(_cursor);
        _window.display();
        sleep(milliseconds(_t_fps -
                           _chrono.getElapsedTime().asMilliseconds()));
        _t_passedInMenu +=
          _chrono.getElapsedTime().asMilliseconds();
      }
    }
  }
  /// EXIT BEING SURE THAT EVERYTHING IS SAVED AND DELETED BEFORE IT LEAVES !!
  // Refresh option file
  _sb_aval.open("options");
  // Forward Key, Backward key, Godown Key, Goup Key, Fireball key, Accelerate Key, fpsshow Key, volume_music, volume_sound
  _sb_aval << _keyid[KEYB_FORWARD];
  _sb_aval << " ";
  _sb_aval << _keyid[KEYB_BACKWARD];
  _sb_aval << " ";
  _sb_aval << _keyid[KEYB_GODOWN];
  _sb_aval << " ";
  _sb_aval << _keyid[KEYB_GOUP];
  _sb_aval << " ";
  _sb_aval << _keyid[KEYB_FIREBALL];
  _sb_aval << " ";
  _sb_aval << _keyid[KEYB_ACCELERATE];
  _sb_aval << " ";
  _sb_aval << _keyid[KEYB_FPSDISP];
  _sb_aval << " ";
  _sb_aval << _music_volume;
  _sb_aval << " ";
  _sb_aval << _firehorn_volume;
  _sb_aval.close();
  // MENU (every item has been allocated so they all need to be deleted!)
  for (unsigned int i = 0 ; i < NBR_BUTTONS ; ++i)
  {
    delete _menuBarre[i];
    _menuBarre[i] = 0;
    delete _menuBarreRect[i];
    _menuBarreRect[i] = 0;
    delete _menuTxtBarre[i];
    _menuTxtBarre[i] = 0;
  }
  for (unsigned int i = 0 ; i < NBR_JAUGES ; ++i)
  {
    delete _menuJaugeCursor[i];
    _menuJaugeCursor[i] = 0;
    delete _menuJaugeRect[i];
    _menuJaugeRect[i] = 0;
    delete _menuJaugeFond[i];
    _menuJaugeFond[i] = 0;
  }
  for (unsigned int i = 0 ; i < NBR_LISTES ; ++i)
  {
    delete _listeChoixTxt0[i];
    _listeChoixTxt0[i] = 0;
    delete _listeChoixTxt1[i];
    _listeChoixTxt1[i] = 0;
    delete _listeChoixTxt2[i];
    _listeChoixTxt2[i] = 0;
    delete _listeChoix0[i];
    _listeChoix0[i] = 0;
    delete _listeChoix1[i];
    _listeChoix1[i] = 0;
    delete _listeChoix2[i];
    _listeChoix2[i] = 0;
    delete _listeCurseur[i];
    _listeCurseur[i] = 0;
    delete _listeFlecheBas[i];
    _listeFlecheBas[i] = 0;
    delete _listeFlecheHaut[i];
    _listeFlecheHaut[i] = 0;
    delete _listeNom[i];
    _listeNom[i] = 0;
    delete _listeNomTxt[i];
    _listeNomTxt[i] = 0;
    delete _listeRect[i];
    _listeRect[i] = 0;
    delete _listeSelected[i];
    _listeSelected[i] = 0;
  }
  // FIREBALLS
  for (unsigned int i = 0 ; i < MAX_FIREBALL ; ++i)
  {
    if (_fireball[i] != 0)
    {
      delete _fireball[i];
      _fireball[i] = 0;
    }
  }
  // MONSTER SPRITES
  for (unsigned int i = 0 ; i < MAX_MONSTER ; ++i)
  {
    if (_monster[i] != 0)
    {
      delete _monster[i];
      _monster[i] = 0;
    }
  }
  // SOUNDS
  for (unsigned int i = 0 ; i < MAX_SOUNDS ; ++i)
  {
    if (_firehorn[i] != 0)
    {
      delete _firehorn[i];
      _firehorn[i] = 0;
    }
  }
  // BONUS
  for (unsigned int i = 0 ; i < MAX_BONUS ; ++i)
  {
    if (_bonus[i] != 0)
    {
      delete _bonus[i];
      _bonus[i] = 0;
      delete _bonusLife[i];
      _bonusLife[i] = 0;
    }
  }
  // EPOLARS
  for (unsigned int i = 0 ; i < MAX_EPOLAR ; ++i)
  {
    if (_epolar[i] != 0)
    {
      delete _epolar[i];
      _epolar[i] = 0;
    }
    if (_epolarHealth[i] != 0)
    {
      delete _epolarHealth[i];
      _epolarHealth[i] = 0;
    }
    if (_epolarHealthMax[i] != 0)
    {
      delete _epolarHealthMax[i];
      _epolarHealthMax[i] = 0;
    }
    if (_epolarHealthBar[i] != 0)
    {
      delete _epolarHealthBar[i];
      _epolarHealthBar[i] = 0;
    }
    if (_epolarHealthBarDyn[i] != 0)
    {
      delete _epolarHealthBarDyn[i];
      _epolarHealthBarDyn[i] = 0;
    }
  }
  // ROCKS
  for (unsigned int i = 0 ; i < MAX_ROCKS ; ++i)
  {
    if (_backgroundRocks[i] != 0)
    {
      delete _backgroundRocks[i];
      _backgroundRocks[i] = 0;
    }
  }
  free(_key);
  free(_keypressed);
  free(_button);
  free(_buttonpressed);
  return 0;
}
