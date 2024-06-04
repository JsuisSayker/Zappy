/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameContent
*/

#include "GameContent.hpp"

namespace zappy {

void GameContent::addTeam(const std::string &teamName)
{
    if (teams_.find(teamName) == teams_.end()) {
        teams_[teamName] = std::vector<Trantorian::trantorian>();
    }
}

void GameContent::addTrantorian(const std::string &teamName, std::shared_ptr<ZappyGameObject> gameObject, int number)
{
    if (teams_.find(teamName) != teams_.end()) {
        teams_[teamName].emplace_back(gameObject, teamName, number);
    }
}

void GameContent::removeTrantorian(const std::string &teamName, std::shared_ptr<ZappyGameObject> gameObject)
{
    if (teams_.find(teamName) != teams_.end()) {
        auto &trants = teams_[teamName];
        trants.erase(std::remove_if(trants.begin(), trants.end(), [&gameObject](const Trantorian::trantorian &t) { return t.gameObject == gameObject; }), trants.end());
        updateNumbers(teamName);
    }
}

void GameContent::updateNumbers(const std::string &teamName)
{
    if (teams_.find(teamName) != teams_.end()) {
        auto &trants = teams_[teamName];
        for (size_t i = 0; i < trants.size(); ++i) {
            trants[i].number = i + 1;
        }
    }
}

} // namespace zappy
