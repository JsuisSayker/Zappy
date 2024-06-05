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
    this->map_ = std::make_unique<Map>();
    this->_pointerToFunction["msz"] =
        std::bind(&GameContent::msz, this, std::placeholders::_1);
    this->_pointerToFunction["bct"] =
        std::bind(&GameContent::bct, this, std::placeholders::_1);
    this->_pointerToFunction["mct"] =
        std::bind(&GameContent::mct, this, std::placeholders::_1);
    this->_pointerToFunction["tna"] =
        std::bind(&GameContent::tna, this, std::placeholders::_1);
    this->_pointerToFunction["ppo"] =
        std::bind(&GameContent::ppo, this, std::placeholders::_1);
    this->_pointerToFunction["plv"] =
        std::bind(&GameContent::plv, this, std::placeholders::_1);
    this->_pointerToFunction["pin"] =
        std::bind(&GameContent::pin, this, std::placeholders::_1);
    this->_pointerToFunction["sgt"] =
        std::bind(&GameContent::sgt, this, std::placeholders::_1);
    this->_pointerToFunction["sst"] =
        std::bind(&GameContent::sst, this, std::placeholders::_1);
    this->_pointerToFunction["pex"] =
        std::bind(&GameContent::pex, this, std::placeholders::_1);
    this->_pointerToFunction["pbc"] =
        std::bind(&GameContent::pbc, this, std::placeholders::_1);
    this->_pointerToFunction["pic"] =
        std::bind(&GameContent::pic, this, std::placeholders::_1);
    this->_pointerToFunction["pie"] =
        std::bind(&GameContent::pie, this, std::placeholders::_1);
    this->_pointerToFunction["pfk"] =
        std::bind(&GameContent::pfk, this, std::placeholders::_1);
    this->_pointerToFunction["pdr"] =
        std::bind(&GameContent::pdr, this, std::placeholders::_1);
    this->_pointerToFunction["pgt"] =
        std::bind(&GameContent::pgt, this, std::placeholders::_1);
    this->_pointerToFunction["pdi"] =
        std::bind(&GameContent::pdi, this, std::placeholders::_1);
    this->_pointerToFunction["enw"] =
        std::bind(&GameContent::enw, this, std::placeholders::_1);
    this->_pointerToFunction["eht"] =
        std::bind(&GameContent::eht, this, std::placeholders::_1);
    this->_pointerToFunction["ebo"] =
        std::bind(&GameContent::ebo, this, std::placeholders::_1);
    this->_pointerToFunction["edi"] =
        std::bind(&GameContent::edi, this, std::placeholders::_1);
    this->_pointerToFunction["WELCOME"] =
        std::bind(&GameContent::welcome, this, std::placeholders::_1);
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

void GameContent::msz(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 3) {
        std::cerr << "msz: invalid number of arguments" << std::endl;
        return;
    }

    int width = std::stoi(actualCommand[1]);
    int height = std::stoi(actualCommand[2]);

    this->map_.get()->setHeight(height);
    this->map_.get()->setWidth(width);
}

void GameContent::bct(std::vector<std::string> actualCommand)
{
    std::cout << "bct" << std::endl;
}

void GameContent::mct(std::vector<std::string> actualCommand)
{
    std::cout << "mct" << std::endl;
}

void GameContent::tna(std::vector<std::string> actualCommand)
{
    std::cout << "tna" << std::endl;
}

void GameContent::ppo(std::vector<std::string> actualCommand)
{
    std::cout << "ppo" << std::endl;
}

void GameContent::plv(std::vector<std::string> actualCommand)
{
    std::cout << "plv" << std::endl;
}

void GameContent::pin(std::vector<std::string> actualCommand)
{
    std::cout << "pin" << std::endl;
}

void GameContent::sgt(std::vector<std::string> actualCommand)
{
    std::cout << "sgt" << std::endl;
}

void GameContent::sst(std::vector<std::string> actualCommand)
{
    std::cout << "sst" << std::endl;
}

void GameContent::pex(std::vector<std::string> actualCommand)
{
    std::cout << "pex" << std::endl;
}

void GameContent::pbc(std::vector<std::string> actualCommand)
{
    std::cout << "pbc" << std::endl;
}

void GameContent::pic(std::vector<std::string> actualCommand)
{
    std::cout << "pic" << std::endl;
}

void GameContent::pie(std::vector<std::string> actualCommand)
{
    std::cout << "pie" << std::endl;
}

void GameContent::pfk(std::vector<std::string> actualCommand)
{
    std::cout << "pfk" << std::endl;
}

void GameContent::pdr(std::vector<std::string> actualCommand)
{
    std::cout << "pdr" << std::endl;
}

void GameContent::pgt(std::vector<std::string> actualCommand)
{
    std::cout << "pgt" << std::endl;
}

void GameContent::pdi(std::vector<std::string> actualCommand)
{
    std::cout << "pdi" << std::endl;
}

void GameContent::enw(std::vector<std::string> actualCommand)
{
    std::cout << "enw" << std::endl;
}

void GameContent::eht(std::vector<std::string> actualCommand)
{
    std::cout << "eht" << std::endl;
}

void GameContent::ebo(std::vector<std::string> actualCommand)
{
    std::cout << "ebo" << std::endl;
}

void GameContent::edi(std::vector<std::string> actualCommand)
{
    std::cout << "edi" << std::endl;
}

void GameContent::welcome(std::vector<std::string> actualCommand)
{
    std::cout << "WELCOME" << std::endl;
    dprintf(this->client.get()->getSocketFd(), "GRAPHIC\n");
}

void GameContent::bufferToSplitedBuffer(std::string buffer)
{
    // split the bufer by \n
    std::vector<std::string> bufferSplited;
    std::string token;
    std::istringstream tokenStream(buffer);
    while (std::getline(tokenStream, token, '\n')) {
        bufferSplited.push_back(token);
    }
    // split each line by space
    for (auto &i : bufferSplited) {
        std::vector<std::string> line;
        std::string token;
        std::istringstream tokenStream(i);
        while (std::getline(tokenStream, token, ' ')) {
            line.push_back(token);
        }
        this->splitedBuffer_.push_back(line);
    }
}

std::vector<std::vector<std::string>> &GameContent::getSplitedBuffer()
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
