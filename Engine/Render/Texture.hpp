#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
    public:
        static Texture* MakeTexture(const std::string& file);

        void Use(int unit);

    private:
        unsigned int texture;
};

#endif
