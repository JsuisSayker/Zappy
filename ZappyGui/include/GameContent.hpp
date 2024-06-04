/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameContent
*/

#pragma once

    #include <string>
    #include <vector>
    #include <memory>
    #include "GameObject.hpp"
    #include "Map.hpp"
    #include "Trantorian.hpp"

namespace zappy {
    class GameContent {
        public:
            void addTeam(const std::string &teamName);
            
            void addTrantorian(const std::string &teamName, std::shared_ptr<ZappyGameObject> gameObject, int number);
            
            void removeTrantorian(const std::string &teamName, std::shared_ptr<ZappyGameObject> gameObject);
            
            void updateNumbers(const std::string &teamName);

            const std::unordered_map<std::string, std::vector<Trantorian::trantorian>>& getTeams() const { return teams_; }

        private:
            std::unique_ptr<Map> map_;
            ZappyGameObject::Map ressources_;
            std::vector<Trantorian> trantorians_;
            std::unordered_map<std::string, glm::vec3> teamsColors_;
    };
}
