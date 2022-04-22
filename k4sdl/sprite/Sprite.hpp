#ifndef K4SDL_SPRITE
#define K4SDL_SPRITE

namespace k4sdl {
    class Sprite;
}

#include <SDL.h>
#include <string>
#include <vector>
#include "../Renderable.hpp"
#include "../Vector2/Vector2.hpp"
#include "SpriteLoader.hpp"
#include <cmath>
#include <memory>

namespace k4sdl {
    class Frame {
    public:
        uint16_t x, y, w, h;
        SDL_Rect rect();
        void position(int *l_x, int *l_y);
        void size(int *l_w, int *l_h);
    };

    class Tag {
    public:
        int16_t childXOff, childYOff;
        uint16_t childTagNum;
        std::string name;
        std::vector<uint8_t> frames;
        std::vector<float> frameDurations;
    };

    class Sprite : public Renderable {
    private:
        SDL_Texture* texture = NULL;
        Vector2<float> scale;
        bool flippedX = false, flippedY = false;
        int width, height;
        int realWidth, realHeight;
        float rotation = 0.0f;
        std::vector<Frame> frameInfo;
        int activeFrame = -1;
        std::vector<Tag> tagInfo;
        int activeTag = -1;
        float tagTime = 0.0f;
        int tagFrame = 0;
        int alpha = 255;
        std::shared_ptr<SpriteLoader> loader = nullptr;

    public:
        Sprite() : scale(1) {}

        constexpr static int SNAP_MAX = 0;
        constexpr static int SNAP_MIN = 1;

        void loadSprite(SDL_Texture* l_texture, std::vector<Frame> l_frameInfo,
                        std::vector<Tag> l_tagInfo);
        
        void setFrame(int num);
        void setTag(std::string name);
        void setTagByNum(int num);
        void animate(float dt);
        
        float getRotation();
        void setRotation(float l_rotation);
        void rotate(float rotationDiff);
        
        void predictRealSize();
        
        Vector2<float> getScale();
        void setScale(Vector2<float> l_scale);
        
        void setSize(int l_width, int l_height, bool conserveRatio = false, int ratioType = Sprite::SNAP_MAX);

        SDL_Texture* getTexture();
        void setTexture(SDL_Texture* l_texture, bool free = true);

        void getFlipped(bool *l_flippedX, bool *l_flippedY);
        void setFlipped(bool l_flippedX, bool l_flippedY);
        void flip(bool flipX, bool flipY);

        int getAlpha();
        void setAlpha(int l_alpha);

        virtual void draw(Camera cam);
        virtual SDL_Rect getScreenRect(Camera cam, SDL_Rect* clip);
        virtual SDL_Rect getWorldRect();

        void setLoader(std::shared_ptr<SpriteLoader> l_loader);

        virtual ~Sprite();
    };
}

#endif
