/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Trantorian
*/

#include "Trantorian.hpp"

namespace zappy {

Trantorian::Trantorian(std::shared_ptr<ZappyGameObject> trantObject, std::shared_ptr<ZappyGameObject> lightObject, std::string team, int playerNumber)
    : trantorianObject(std::move(trantObject)), pointLightObject(std::move(lightObject)), team(team), playerNumber(playerNumber), level(0)
{
    this->inventory.food = 0;
    this->inventory.linemate = 0;
    this->inventory.deraumere = 0;
    this->inventory.sibur = 0;
    this->inventory.mendiane = 0;
    this->inventory.phiras = 0;
    this->inventory.thystame = 0;
}

// void Trantorian::updateTrantorian(const Trantorian &trant) // Updates an existing trantorian in the trantorians_ vector with new information
// {
//     for (auto &t : trantorians_) { // Iterates through the trantorians_ vector
//         if (t.team == trant.team && t.gameObject == trant.gameObject) { // if match found, replace the trantorian with the one with the updated info
//             t = trant;
//             break;
//         }
//     }
// }

// void Trantorian::updateTrantorianTranslation(const trantorian& trant, glm::vec3 translation)
// {
//     if (trant.gameObject) {
//         trant.gameObject->transform.translation = translation;
//     }
// }

// void Trantorian::updateTrantorianRotation(const trantorian& trant, glm::vec3 rotation)
// {
//     if (trant.gameObject) {
//         trant.gameObject->transform.rotation = rotation;
//     }
// }

// void Trantorian::updateTrantorianLevel(const std::string& team, std::shared_ptr<ZappyGameObject> gameObject)
// {
//     for (auto& trant : trantorians_) {
//         if (trant.team == team && trant.gameObject == gameObject) {
//             trant.level += 1;
//             return;
//         }
//     }
//     throw TratorianNotFoundException();
// }

// void Trantorian::updateInventory(const std::string& team, std::shared_ptr<ZappyGameObject> gameObject, const std::string &item, bool operator_) {
//     for (auto& trant : trantorians_) {
//         if (trant.team == team && trant.gameObject == gameObject) {
//             if (item == "food") {
//                 if (!operator_)
//                     if (trant.food > 0)
//                         trant.food--;
//                 else
//                     trant.food++;
//             } else if (item == "linemate") {
//                 if (!operator_)
//                     if (trant.linemate > 0)
//                         trant.linemate--;
//                 else
//                     trant.linemate++;
//             } else if (item == "deraumere") {
//                 if (!operator_)
//                     if (trant.deraumere > 0)
//                         trant.deraumere--;
//                 else
//                     trant.deraumere++;
//             } else if (item == "sibur") {
//                 if (!operator_)
//                     if (trant.sibur > 0)
//                         trant.sibur--;
//                 else
//                     trant.sibur++;
//             } else if (item == "mendiane") {
//                 if (!operator_)
//                     if (trant.mendiane > 0)
//                         trant.mendiane--;
//                 else
//                     trant.mendiane++;
//             } else if (item == "phiras") {
//                 if (!operator_)
//                     if (trant.phiras > 0)
//                         trant.phiras--;
//                 else
//                     trant.phiras++;
//             } else if (item == "thystame") {
//                 if (!operator_)
//                     if (trant.thystame > 0)
//                         trant.thystame--;
//                 else
//                     trant.thystame++;
//             }
//             return;
//         }
//     }
//     throw TratorianNotFoundException();
// }

// std::unordered_map<std::string, int> Trantorian::getInventory(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject) const
// {
//     for (const auto &trant : trantorians_) {
//         if (trant.team == team && trant.gameObject == gameObject) {
//             return trant.getInventory();
//         }
//     }
//     throw TratorianNotFoundException();
// }

// ZappyGameObject::id_t Trantorian::getTrantorianGameObjectId(const std::string& team, std::shared_ptr<ZappyGameObject> gameObject) const
// {
//     for (const auto& trant : trantorians_) {
//         if (trant.team == team && trant.gameObject == gameObject) {
//             return trant.gameObject->getId();
//         }
//     }
//     throw TratorianNotFoundException();
// }

} // namespace zappy
