/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Font
*/

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Glyph.hpp"

#include <map>
#include <memory>
#include <string>

class TextureAtlas;

struct TextDimensions {
    int width;
    int height;
};

namespace lve {
class Font {
  public:
    Font::Font(const FT_Face &face, std::shared_ptr<TextureAtlas> ta);
    ~Font();

    Glyph getGlyphs();

  protected:
    FT_Face m_face;
    std::shared_ptr<TextureAtlas> m_textureAtlas;
    std::map<uint16_t, std::shared_ptr<Glyph>> m_glyphs;
};
} // namespace lve
