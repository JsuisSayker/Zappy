/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Egg
*/

#pragma once

    #include <memory>
    #include <algorithm>
    #include <vector>
    #include <string>
    #include <glm/vec3.hpp>
    #include "Device.hpp"
    #include "ErrorHandling.hpp"
    #include "GameObject.hpp"
    #include "Map.hpp"

namespace zappy {

    class Egg {
        public:
            Egg(ZappyGameObject::id_t eggObjectId, int eggNumber, int playerNumber, const glm::vec3 &position);
            ~Egg() = default;

        public:
            int eggNumber;
            int playerNumber;
            glm::vec3 position;
            ZappyGameObject::id_t eggObjectId;
    };

} // namespace zappy
