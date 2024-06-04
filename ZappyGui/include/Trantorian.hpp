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
    #include "GameObject.hpp"
    #include "ErrorHandling.hpp"

namespace zappy {
    class Trantorian {
        public:

        struct trantorian {
            std::shared_ptr<ZappyGameObject> gameObject;
            std::string team;
            int number;
            int level;
            int food;
            int linemate;
            int deraumere;
            int sibur;
            int mendiane;
            int phiras;
            int thystame;

            trantorian(std::shared_ptr<ZappyGameObject> gameObject, const std::string &team, int number)
                : gameObject(gameObject), team(team), number(number), level(0), food(food), linemate(0), deraumere(0), sibur(0), mendiane(0), phiras(0), thystame(0) {}
            
            std::unordered_map<std::string, int> getInventory() const
            {
                return { {"food", food}, {"linemate", linemate}, {"deraumere", deraumere}, {"sibur", sibur}, {"mendiane", mendiane}, {"phiras", phiras}, {"thystame", thystame} };
            }
        };

        Trantorian(std::shared_ptr<ZappyGameObject> gameObject, std::string team, int number);
        ~Trantorian() = default;

        // void addTrantorian(const trantorian &trant);

        // void removeTrantorian(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject);

        void updateTrantorian(const trantorian &trant);

        void updateTrantorianTranslation(const trantorian &trant, glm::vec3 translation);

        void updateTrantorianRotation(const trantorian &trant, glm::vec3 rotation);

        void updateTrantorianLevel(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject);

        void updateInventory(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject, const std::string &item, bool operator_);

        std::unordered_map<std::string, int> getInventory(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject) const;      

        ZappyGameObject::id_t getTrantorianGameObjectId(const std::string &team, std::shared_ptr<ZappyGameObject> gameObject) const;
        
        const std::vector<trantorian> &getTrantorians() const { return trantorians_; }

        private:
            std::vector<trantorian> trantorians_;
    };
}
