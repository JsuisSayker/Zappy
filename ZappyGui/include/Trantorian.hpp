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
    #include "GameContent.hpp"
    #include "Map.hpp"
    #include "GameObject.hpp"
    #include "ErrorHandling.hpp"

namespace zappy {
    class Trantorian {
        public:
        Trantorian(std::shared_ptr<ZappyGameObject> trantorianObject, std::shared_ptr<ZappyGameObject> pointLightObject, std::string team, int playerNumber, std::shared_ptr<ZappyGui> zappyGui);
        ~Trantorian() = default;

        // void updateTrantorian(const Trantorian &trant);

        // void updateTrantorianTranslation(const Trantorian &trant, glm::vec3 translation);

        // void updateTrantorianRotation(const Trantorian &trant, glm::vec3 rotation);

        // void updateTrantorianLevel(const std::string &team, ZappyGameObject gameObject);

        // void updateInventory(const std::string &team, ZappyGameObject gameObject, const std::string &item, bool operator_);

        // std::unordered_map<std::string, int> getInventory(const std::string &team, ZappyGameObject gameObject) const;      

        // ZappyGameObject::id_t getTrantorianGameObjectId(const std::string &team, ZappyGameObject gameObject) const;
        
        // const std::vector<Trantorian> &getTrantorians() const { return Trantorians_; }

        private:
            ZappyGameObject::id_t trantorianObject;
            ZappyGameObject::id_t pointLightObject;
            resources inventory;
            std::string team;
            std::shared_ptr<ZappyGui> zappyGui;
            int playerNumber;
            int level;
    };
}
