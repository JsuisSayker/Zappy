/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Trantorian
*/

#include "Trantorian.hpp"

namespace zappy {

Trantorian::Trantorian(std::shared_ptr<ZappyGameObject> gameObject, std::string team, int number)
{
    trantorian newTrantorian{gameObject, team, number};
    trantorians_.push_back(newTrantorian);
}

// void Trantorian::addTrantorian(const trantorian& trant)
// {
//     trantorians_.push_back(trant);
// }

// void Trantorian::removeTrantorian(const std::string& team, std::shared_ptr<ZappyGameObject> gameObject)
// {
//     auto iterator = std::remove_if(trantorians_.begin(), trantorians_.end(), [&team, gameObject](const trantorian &t) { return t.team == team && t.gameObject == gameObject; }); // Reorders the elements in the range so that the elements to be removed appear at the end of the range.

//     if (iterator != trantorians_.end()) {
//         trantorians_.erase(iterator, trantorians_.end());
//     }
// }

void Trantorian::updateTrantorian(const trantorian &trant) // Updates an existing trantorian in the trantorians_ vector with new information
{
    for (auto &t : trantorians_) { // Iterates through the trantorians_ vector
        if (t.team == trant.team && t.gameObject == trant.gameObject) { // if match found, replace the trantorian with the one with the updated info
            t = trant;
            break;
        }
    }
}

void Trantorian::updateTrantorianTranslation(const trantorian& trant, glm::vec3 translation)
{
    if (trant.gameObject) {
        trant.gameObject->transform.translation = translation;
    }
}

void Trantorian::updateTrantorianRotation(const trantorian& trant, glm::vec3 rotation)
{
    if (trant.gameObject) {
        trant.gameObject->transform.rotation = rotation;
    }
}

void Trantorian::updateTrantorianLevel(const std::string& team, std::shared_ptr<ZappyGameObject> gameObject)
{
    for (auto& trant : trantorians_) {
        if (trant.team == team && trant.gameObject == gameObject) {
            trant.level += 1;
            break;
        }
    }
}

void Trantorian::updateInventory(const std::string& team, std::shared_ptr<ZappyGameObject> gameObject, const std::string &item, bool operator_) {
    for (auto& trant : trantorians_) {
        if (trant.team == team && trant.gameObject == gameObject) {
            if (item == "food") {
                if (!operator_)
                    if (trant.food > 0)
                        trant.food--;
                else
                    trant.food++;
            } else if (item == "linemate") {
                if (!operator_)
                    if (trant.linemate > 0)
                        trant.linemate--;
                else
                    trant.linemate++;
            } else if (item == "deraumere") {
                if (!operator_)
                    if (trant.deraumere > 0)
                        trant.deraumere--;
                else
                    trant.deraumere++;
            } else if (item == "sibur") {
                if (!operator_)
                    if (trant.sibur > 0)
                        trant.sibur--;
                else
                    trant.sibur++;
            } else if (item == "mendiane") {
                if (!operator_)
                    if (trant.mendiane > 0)
                        trant.mendiane--;
                else
                    trant.mendiane++;
            } else if (item == "phiras") {
                if (!operator_)
                    if (trant.phiras > 0)
                        trant.phiras--;
                else
                    trant.phiras++;
            } else if (item == "thystame") {
                if (!operator_)
                    if (trant.thystame > 0)
                        trant.thystame--;
                else
                    trant.thystame++;
            }
            break;
        }
    }
}

ZappyGameObject::id_t Trantorian::getTrantorianGameObjectId(const std::string& team, std::shared_ptr<ZappyGameObject> gameObject) const
{
    for (const auto& trant : trantorians_) {
        if (trant.team == team && trant.gameObject == gameObject) {
            return trant.gameObject->getId();
        }
    }
    return 0;
}

} // namespace zappy