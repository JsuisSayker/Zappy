/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** FontManager
*/

#include "FontManager.hpp"

lve::FontManager::FontManager() {}

lve::FontManager::~FontManager() {}

std::shared_ptr<lve::Font> lve::FontManager::GetFont(const std::string &fontname, int pt)
{
    FT_Face face;
    auto error = FT_New_Face(m_library, fontname.c_str(), 0, &face);
    if (!error) {
        error = FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(pt));
        if (error) {
            throw std::runtime_error("couldn't set pixel sizes");
        }
        return std::make_shared<Font>(face, m_textureAtlas);
    }

    throw std::runtime_error("couldn't load font");
    }
