/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Font
*/

#include "Font.hpp"

lve::Font::Font(const FT_Face &face, std::shared_ptr<TextureAtlas> ta)
{
    m_face = face;
    m_textureAtlas = std::move(ta);
}

lve::Font::~Font() {}
