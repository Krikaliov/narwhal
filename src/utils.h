#ifndef UTILS_H
#define UTILS_H 1

#include "main.h"

/// @brief Passes an image grayscale
/// @param src Original image
/// @return The grayscaled image
sf::Image grayscale(sf::Image src);

/// @brief Browse any audio file in OGG, 3GP or WAV format
/// from the directory assets/audio/musiques using system CLI.
/// @return All music found.
std::vector<std::string> browseMusic();

/// @brief Return the name of the given keycode
/// @param k the keycode
/// @return The name of this keycode.
std::string keystring(unsigned int k);

/// @brief Compute the digit length of an integer counting the negative sign.
/// @param n any integer.
/// @return its digit length.
int numberLength(int n);

/// @brief Compute the date (in ms since app launching) of the next monster spawn
/// @param numberSpawned number of monsters that have already spawned
/// @return the date of the next monster spawn.
sf::Int32 nextMonsterSpawnDate(sf::Int32 numberSpawned);

/// @brief Compute the date (in ms since app launching) of the next bonus spawn
/// @param numberSpawned number of bonuses that have already spawned
/// @return the date of the next bonus spawn.
sf::Int32 nextBonusSpawnDate(sf::Int32 numberSpawned);

/// @brief Compute the date (in ms since app launching) of the next epolar spawn
/// @param numberSpawned number of epolar that have already spawned
/// @return the date of the next epolar spawn.
sf::Int32 nextEpolarSpawnDate(sf::Int32 numberSpawned);

/// @brief Compute the y coordinate of the point crossed by the line passing through two other points at some vertical axis.
/// @param posA Point of the line.
/// @param posB Point of the line.
/// @param x vertical axis x=x.
/// @return y coordinate of the meeting point between the line and this vertical axis.
float objectif_y(sf::Vector2f posA, sf::Vector2f posB, float x);

/// @brief Collision test between two lined rectangles (not SAT!)
/// @param posA Position of the rectangle A
/// @param boundsA Bounds of the rectangle A
/// @param posB Position of the rectangle B
/// @param boundsB Bounds of the rectangle B
/// @param margin (can be negative)
/// @return the collision assertion between A and B.
bool contact(sf::Vector2f posA, sf::FloatRect boundsA, sf::Vector2f posB, sf::FloatRect boundsB, float margin = 0.0f);

#endif // UTILS_H
