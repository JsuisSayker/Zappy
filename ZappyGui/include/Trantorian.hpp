/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Trantorian
*/

#pragma once

    #include <string>
    #include <vector>
    #include <memory>
    #include <algorithm>
    #include <glm/vec3.hpp>
    #include "Device.hpp"
    #include "Map.hpp"
    #include "GameObject.hpp"
    #include "ErrorHandling.hpp"

namespace zappy {
    class Trantorian {
        public:
        Trantorian(std::shared_ptr<ZappyGameObject> gameObject, std::string team, int number);
        ~Trantorian() = default;

        // void addTrantorian(const trantorian &trant);

        // void removeTrantorian(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject);

        // void updateTrantorian(const trantorian &trant);

        // void updateTrantorianTranslation(const trantorian &trant, glm::vec3 translation);

        // void updateTrantorianRotation(const trantorian &trant, glm::vec3 rotation);

        // void updateTrantorianLevel(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject);

        // void updateInventory(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject, const std::string &item, bool operator_);

        // std::unordered_map<std::string, int> getInventory(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject) const;      

        // ZappyGameObject::id_t getTrantorianGameObjectId(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject) const;
        
        // const std::vector<trantorian> &getTrantorians() const { return trantorians_; }

        // std::unordered_map<std::string, int> getInventory() const
        // {
        //     return { {"food", food}, {"linemate", linemate}, {"deraumere", deraumere}, {"sibur", sibur}, {"mendiane", mendiane}, {"phiras", phiras}, {"thystame", thystame} };
        // }
        private:
            ZappyGameObject trantorianObject;
            ZappyGameObject pointLightObject;
            ressources inventory;
            std::string team;
            int playerNumber;
            int level;
    };
}
