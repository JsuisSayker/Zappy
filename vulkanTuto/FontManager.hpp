/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** FontManager
*/

#pragma once

#include <memory>
#include <stdexcept>
#include <memory>  
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.hpp"

namespace lve {
class FontManager {
  public:
    FontManager();
    ~FontManager();

    std::shared_ptr<Font> GetFont(
        const std::string &fontPath, int fontSize);

    FT_Library m_library;
      

  private:
};
} // namespace lv