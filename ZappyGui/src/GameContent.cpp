/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameContent
*/

#include "GameContent.hpp"

namespace zappy {

GameContent::GameContent()
{
    this->_pointerToFunction["msz"] = std::bind(&GameContent::msz, this);
}

GameContent::~GameContent()
{
}

// void GameContent::addTeam(const std::string &teamName)
// {
//     if (teams_.find(teamName) == teams_.end()) {
//         teams_[teamName] = std::vector<Trantorian::trantorian>();
//     }
// }

// void GameContent::addTrantorian(const std::string &teamName, std::shared_ptr<ZappyGameObject> gameObject, int number)
// {
//     if (teams_.find(teamName) != teams_.end()) {
//         teams_[teamName].emplace_back(gameObject, teamName, number);
//     }
// }

// void GameContent::removeTrantorian(const std::string &teamName, std::shared_ptr<ZappyGameObject> gameObject)
// {
//     if (teams_.find(teamName) != teams_.end()) {
//         auto &trants = teams_[teamName];
//         trants.erase(std::remove_if(trants.begin(), trants.end(), [&gameObject](const Trantorian::trantorian &t) { return t.gameObject == gameObject; }), trants.end());
//         updateNumbers(teamName);
//     }
// }

// void GameContent::updateNumbers(const std::string &teamName)
// {
//     if (teams_.find(teamName) != teams_.end()) {
//         auto &trants = teams_[teamName];
//         for (size_t i = 0; i < trants.size(); ++i) {
//             trants[i].number = i + 1;
//         }
//     }
// }

/**
 * @brief Sets the pointer to the function.
 *
 * This function sets the pointer to the function for the gameContents.
 *
 * @param pointerToFunction The pointer to the function to set.
 */
void GameContent::setPointerToFunction(
    std::unordered_map<std::string, GameContent::FunctionPtr>
        &pointerToFunction)
{
    this->_pointerToFunction = pointerToFunction;
}

/**
 * @brief Gets the pointer to the function.
 *
 * @return The pointer to the function.
 */
std::unordered_map<std::string, GameContent::FunctionPtr> &
GameContent::getPointerToFunction()
{
    return this->_pointerToFunction;
}

/**
 * @brief Handles the msz command.
 *
 * This function handles the msz command received from the server.
 * The function parses the command and extracts the width and height of the
 * map. The function then prints the width and height of the map.
 */
void zappy::GameContent::msz() { std::cout << "msz" << std::endl; }

} // namespace zappy
