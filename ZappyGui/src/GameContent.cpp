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
    this->client = std::make_shared<Client>();
    this->_pointerToFunction["msz"] = std::bind(&GameContent::msz, this);
    this->_pointerToFunction["bct"] = std::bind(&GameContent::bct, this);
    this->_pointerToFunction["mct"] = std::bind(&GameContent::mct, this);
    this->_pointerToFunction["tna"] = std::bind(&GameContent::tna, this);
    this->_pointerToFunction["ppo"] = std::bind(&GameContent::ppo, this);
    this->_pointerToFunction["plv"] = std::bind(&GameContent::plv, this);
    this->_pointerToFunction["pin"] = std::bind(&GameContent::pin, this);
    this->_pointerToFunction["sgt"] = std::bind(&GameContent::sgt, this);
    this->_pointerToFunction["sst"] = std::bind(&GameContent::sst, this);
    this->_pointerToFunction["pex"] = std::bind(&GameContent::pex, this);
    this->_pointerToFunction["pbc"] = std::bind(&GameContent::pbc, this);
    this->_pointerToFunction["pic"] = std::bind(&GameContent::pic, this);
    this->_pointerToFunction["pie"] = std::bind(&GameContent::pie, this);
    this->_pointerToFunction["pfk"] = std::bind(&GameContent::pfk, this);
    this->_pointerToFunction["pdr"] = std::bind(&GameContent::pdr, this);
    this->_pointerToFunction["pgt"] = std::bind(&GameContent::pgt, this);
    this->_pointerToFunction["pdi"] = std::bind(&GameContent::pdi, this);
    this->_pointerToFunction["enw"] = std::bind(&GameContent::enw, this);
    this->_pointerToFunction["eht"] = std::bind(&GameContent::eht, this);
    this->_pointerToFunction["ebo"] = std::bind(&GameContent::ebo, this);
    this->_pointerToFunction["edi"] = std::bind(&GameContent::edi, this);
    this->_pointerToFunction["WELCOME"] =
        std::bind(&GameContent::welcome, this);
}

GameContent::~GameContent() {}

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

void GameContent::msz() { std::cout << "msz" << std::endl; }

void GameContent::bct() { std::cout << "bct" << std::endl; }

void GameContent::mct() { std::cout << "mct" << std::endl; }

void GameContent::tna() { std::cout << "tna" << std::endl; }

void GameContent::ppo() { std::cout << "ppo" << std::endl; }

void GameContent::plv() { std::cout << "plv" << std::endl; }

void GameContent::pin() { std::cout << "pin" << std::endl; }

void GameContent::sgt() { std::cout << "sgt" << std::endl; }

void GameContent::sst() { std::cout << "sst" << std::endl; }

void GameContent::pex() { std::cout << "pex" << std::endl; }

void GameContent::pbc() { std::cout << "pbc" << std::endl; }

void GameContent::pic() { std::cout << "pic" << std::endl; }

void GameContent::pie() { std::cout << "pie" << std::endl; }

void GameContent::pfk() { std::cout << "pfk" << std::endl; }

void GameContent::pdr() { std::cout << "pdr" << std::endl; }

void GameContent::pgt() { std::cout << "pgt" << std::endl; }

void GameContent::pdi() { std::cout << "pdi" << std::endl; }

void GameContent::enw() { std::cout << "enw" << std::endl; }

void GameContent::eht() { std::cout << "eht" << std::endl; }

void GameContent::ebo() { std::cout << "ebo" << std::endl; }

void GameContent::edi() { std::cout << "edi" << std::endl; }

void GameContent::welcome()
{
    std::cout << "WELCOME" << std::endl;
    dprintf(this->client.get()->getSocketFd(), "GRAPHIC\n");
}

void GameContent::setSplitedBuffer(std::string splitedBuffer)
{
    std::istringstream iss(splitedBuffer);
    std::string word;

    while (iss >> word) {
        this->splitedBuffer_.push_back(word);
    }
}

std::vector<std::string> &GameContent::getSplitedBuffer()
{
    return this->splitedBuffer_;
}

void GameContent::createTeam(std::shared_ptr<ZappyModel> lveModel, const std::string &teamName, const glm::vec3 &position)
{
    auto object = std::make_shared<zappy::ZappyGameObject>(zappy::ZappyGameObject::createGameObject());
    object->model = lveModel;
    object->transform.translation = position;
    object->transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.emplace(object->getId(), std::move(*object));

    auto pointLight = std::make_shared<zappy::ZappyGameObject>(zappy::ZappyGameObject::makePointLight(0.2f));
    pointLight->color = {1.0f, 1.0f, 1.0f}; //////////////////////////////////////////////////////////// dont forget to change to generate random color
    gameObjects.emplace(pointLight->getId(), std::move(*pointLight));

    this->teamsColors_[teamName] = pointLight->color;

    Trantorian newTrantorian(object, pointLight, teamName, 0); /////////// change number
    trantorians_.emplace_back(newTrantorian);
}

void GameContent::addTrantorian(std::shared_ptr<ZappyModel> lveModel, const std::string &teamName, const glm::vec3 &position)
{
    auto object = std::make_shared<zappy::ZappyGameObject>(zappy::ZappyGameObject::createGameObject());
    object->model = lveModel;
    object->transform.translation = position;
    object->transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.emplace(object->getId(), std::move(*object));

    auto pointLight = std::make_shared<zappy::ZappyGameObject>(zappy::ZappyGameObject::makePointLight(0.2f));
    pointLight->color = this->teamsColors_[teamName];
    gameObjects.emplace(pointLight->getId(), std::move(*pointLight));

    Trantorian newTrantorian(object, pointLight, teamName, 0); /////////// change number
    trantorians_.emplace_back(newTrantorian);
}

} // namespace zappy
