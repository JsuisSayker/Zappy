/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Glyph
*/

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <memory>
#include <stdexcept>

class ITexture;
class TextureAtlas;

namespace lve {
class Glyph {
  public:
    Glyph(FT_Face face, FT_UInt ix);
    ~Glyph();

    int GetLeft();
    int GetTop();
    int GetWidth();
    int GetHeight();
    int GetAdvance();

    void CreateTextureFromBitmap(std::shared_ptr<TextureAtlas> &ta);

  protected:
    FT_Face m_face;
    FT_UInt m_glyphIndex;
    int m_left;
    int m_top;
    int m_width;
    int m_height;
    int m_advance;
    std::shared_ptr<ITexture> m_texture;
  private:
};
} // namespace lve