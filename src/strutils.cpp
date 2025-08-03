#include "strutils.h"

using namespace std;

namespace N
{
  vector<char> stringToCharList(string s)
  {
    const char * chaine = s.c_str();
    unsigned int i = 0;
    vector<char> retour;
    while (chaine[i] != 0)
    {
      retour.push_back(chaine[i]);
      i++;
    }
    retour.push_back(0);
    return retour;
  }

  string charListToString(vector<char> v)
  {
    char *chaine = (char*) malloc (sizeof(char) * v.size() + 1);
    for (unsigned int i = 0 ; i < v.size() ; i++)
    chaine[i] = v[i];
    chaine[v.size()] = '\0';
    string retour = chaine;
    free(chaine);
    return retour;
  }

  string intToString(int i)
  {
    char c[] = "";
    sprintf(c, "%d", i);
    string r = c;
    return r;
  }
}
