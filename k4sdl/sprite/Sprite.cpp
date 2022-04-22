#include "Sprite.hpp"

namespace k4sdl {
    void Frame::position(int *l_x, int *l_y) {
        if (l_x != NULL)
            *l_x = x;
        if (l_y != NULL)
            *l_y = y;
    }

    void Frame::size(int *l_w, int *l_h) {
        if (l_w != NULL)
            *l_w = w;
        if (l_h != NULL)
            *l_h = h;
    }

    void Sprite::loadSprite(SDL_Texture* l_texture, std::vector<Frame> l_frameInfo,
                            std::vector<Tag> l_tagInfo) {
        frameInfo = l_frameInfo;
        tagInfo = l_tagInfo;
        activeFrame = -1;
        activeTag = -1;

        setFrame(0);
        setTexture(l_texture);
    }

    void Sprite::setFrame(int num) {
        if (num < frameInfo.size() && num >= 0 && activeFrame != num) {
            activeFrame = num;
            width = frameInfo[activeFrame].w;
            height = frameInfo[activeFrame].h;
            predictRealSize();
        }
    }

    void Sprite::setTag(std::string name) {
        int i = 0;
        for (auto tag : tagInfo) {
            if (tag.name.compare(name) == 0)
            {
                setTagByNum(i);
                return;
            }
            i++;
        }
    }

    void Sprite::setTagByNum(int num) {
        if (num < tagInfo.size() && num >= 0 && activeTag != num) {
            activeTag = num;
            tagTime = 0.0f;
            tagFrame = 0;
            if (tagInfo[activeTag].frames.size() > 0) {
                setFrame(tagInfo[activeTag].frames[0]);
            }
        }
    }

    void Sprite::animate(float dt) {
        if (activeTag == -1)
            return;
        if (tagInfo[activeTag].frames.size() == 0)
            return;
        tagTime += dt;
        float duration = tagInfo[activeTag].frameDurations[tagFrame];
        while (tagTime > duration && duration > 0) {
            tagTime -= duration;
            tagFrame++;
            tagFrame %= tagInfo[activeTag].frames.size();
            duration = tagInfo[activeTag].frameDurations[tagFrame];
        }
        setFrame(tagInfo[activeTag].frames[tagFrame]);
    }

    void Sprite::predictRealSize() {
        float newWidth = (float)width * scale.x;
        float newHeight = (float)height * scale.y;

        float rotation2 = std::fmod(rotation2, 180.0f);
        if (std::fabs(rotation2) > 0.0000001f) {
            if (rotation2 >= 90.0f) {
                // Swap width and height if rotation >= 90
                float tmp = newWidth;
                newWidth = newHeight;
                newHeight = tmp;
                rotation2 -= 90.0f;
            }

            // Convert to radians
            rotation2 *= M_PI;
            rotation2 /= 180.0f;
            float newWidth2 = std::ceil(newWidth * std::cos(rotation2) + newHeight * std::sin(rotation2));
            float newHeight2 = std::ceil(newWidth * std::sin(rotation2) + newHeight * std::cos(rotation2));
            newWidth = newWidth2;
            newHeight = newHeight2;
        }

        realWidth = (int)std::ceil(newWidth);
        realHeight = (int)std::ceil(newHeight);
    }

    void Sprite::setSize(int l_width, int l_height, bool conserveRatio, int ratioType) {
        if (width == 0 || height == 0)
            return;
        if (!conserveRatio) {
            scale.x = (float)l_width / (float)width;
            scale.y = (float)l_height / (float)height;
        }
        else {
            bool scaleAxisX = width > height;
            if (ratioType == Sprite::SNAP_MIN)
                scaleAxisX = !scaleAxisX;
            if (scaleAxisX) {
                scale.x = (float)l_width / (float)width;
                scale.y = scale.x;
            }
            else {
                scale.y = (float)l_height / (float)height;
                scale.x = scale.y;
            }
        }
        predictRealSize();
    }

    float Sprite::getRotation() {
        return rotation;
    }

    void Sprite::setRotation(float l_rotation) {
        if (rotation == l_rotation)
            return;
        rotation = l_rotation;
        rotation = std::fmod(rotation, 360.0f);
        predictRealSize();
    }

    void Sprite::rotate(float rotationDiff) {
        if (rotationDiff == 0)
            return;
        setRotation(rotation + rotationDiff);
    }

    Vector2<float> Sprite::getScale() {
        return scale;
    }

    void Sprite::setScale(Vector2<float> l_scale) {
        if (l_scale == scale)
            return;
        scale = l_scale;
    }

    SDL_Texture* Sprite::getTexture() {
        return texture;
    }

    void Sprite::setTexture(SDL_Texture* l_texture, bool free) {
        if (l_texture == texture)
            return;
        if (free) {
            if (texture != NULL)
                SDL_DestroyTexture(texture);
            texture = NULL;
        }
        texture = l_texture;
        width = 0; height = 0;
        if (texture) {
            SDL_SetTextureAlphaMod(texture, alpha);
            SDL_QueryTexture(texture, NULL, NULL, &width, &height);
            predictRealSize();
        }
    }

    void Sprite::getFlipped(bool *l_flippedX, bool *l_flippedY) {
        if (l_flippedX != NULL)
            *l_flippedX = flippedX;
        if (l_flippedY != NULL)
            *l_flippedY = flippedY;
    }

    void Sprite::setFlipped(bool l_flippedX, bool l_flippedY) {
        flippedX = l_flippedX;
        flippedY = l_flippedY;
    }

    void Sprite::flip(bool flipX, bool flipY) {
        if (flipX)
            flippedX = !flippedX;
        if (flipY)
            flippedY = !flippedY;
    }

    int Sprite::getAlpha() {
        return alpha;
    }

    void Sprite::setAlpha(int l_alpha) {
        if (l_alpha == alpha)
            return;
        alpha = l_alpha;
        SDL_SetTextureAlphaMod(texture, alpha);
    }

    void Sprite::setLoader(std::shared_ptr<SpriteLoader> l_loader) {
        loader = l_loader;
    }

    void Sprite::draw(Camera cam) {
        Renderable::draw(cam);
        if (texture == NULL) {
            return;
        }
        SDL_Rect clip;
        if (activeFrame != -1) {
            clip.x = frameInfo[activeFrame].x;
            clip.y = frameInfo[activeFrame].y;
        }
        else {
            clip.x = 0;
            clip.y = 0;
        }
        clip.w = width;
        clip.h = height;
        SDL_Rect position;
        position.x = (int)(screenPosition.x - (float)width * center.x);
        position.y = (int)(screenPosition.y - (float)height * center.y);
        position.w = width;
        position.h = height;
        SDL_RendererFlip flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL * flippedX + SDL_FLIP_VERTICAL * flippedY);
        if (visible) {
            SDL_RenderCopyEx(cam.getRenderer(), texture, &clip, &position, rotation, NULL, flip);
        }
    }

    SDL_Rect Sprite::getScreenRect(Camera cam, SDL_Rect* clip) {
        positionToScreen(cam);
        int w = (int)((float)realWidth * cam.zoom.x);
        int h = (int)((float)realHeight * cam.zoom.y);
        SDL_Rect rect;
        rect.x = (int)(screenPosition.x - (float)realWidth * center.x);
        rect.y = (int)(screenPosition.y - (float)realHeight * center.y);
        rect.w = w;
        rect.h = h;
        cam.clipRect(rect);
        return rect;
    }

    SDL_Rect Sprite::getWorldRect() {
        SDL_Rect result;
        result.x = (int)(position.x - (float)realWidth * center.x);
        result.y = (int)(position.y - (float)realHeight * center.y);
        result.w = realWidth;
        result.h = realHeight;
        return result;
    }

    Sprite::~Sprite() {
        std::cout << "Sprite destructor called" << std::endl;
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = NULL;
        }
    }
}