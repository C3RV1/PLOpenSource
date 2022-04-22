#include "Text.hpp"

namespace k4sdl {
    void Text::setFont(std::unique_ptr<Font> l_font) {
        font = std::move(l_font);
        renderNeeded = true;
    }

    void Text::setText(std::string l_text) {
        if (text.compare(l_text) == 0)
            return;
        text = l_text;
        renderNeeded = true;
    }

    void Text::setBgColor(std::unique_ptr<SDL_Color> l_bgColor) {
        bgColor = std::move(l_bgColor);
        renderNeeded = true;
    }

    SDL_Color Text::getBgColor() {
        return *bgColor;
    }

    void Text::setLineSpacing(int l_lineSpacing) {
        if (lineSpacing == l_lineSpacing)
            return;
        lineSpacing = l_lineSpacing;
        renderNeeded = true;
    }

    int Text::getLineSpacing() {
        return lineSpacing;
    }

    void Text::setAlignment(float l_alignment) {
        if (alignment == l_alignment)
            return;
        alignment = l_alignment;
        renderNeeded = true;
    }

    float Text::getAlignment() {
        return alignment;
    }

    void Text::draw(Camera cam) {
        if (font == nullptr)
            return;
        render();
        Sprite::draw(cam);
    }

    void Text::render() {
        if (!renderNeeded)
            return;
        renderNeeded = false;
        setTexture(font->render(text, color, bgColor.get(), lineSpacing, alignment));
    }
}