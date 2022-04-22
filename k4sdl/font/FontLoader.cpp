#include "FontLoader.hpp"

namespace k4sdl {
    void FontLoaderOS::load(std::string path, int size, Text &text) {
        std::string realPath = basePath + path;
        std::ifstream f(realPath.c_str(), std::ios::binary);
        if (!f.good()) {
            std::cout << "Error loading os font. Couldn't open font " << path << std::endl;
            if (path != fallBack && fallBack.compare("") != 0) {
                FontLoaderOS::load(fallBack, size, text);
            }
            return;
        }
        f.close();

        std::string extension = realPath.substr(realPath.find_last_of("."));
        if (extension.compare(".ttf") == 0) {
            TTF_Font* font = TTF_OpenFont(realPath.c_str(), size);
            std::unique_ptr<TTFFont> loadedFont = std::make_unique<TTFFont>(font);
            text.setFont(std::move(loadedFont));
            return;
        }
        
        std::cout << "Extension " << extension << " not supported" << std::endl;
        return;
    }
}