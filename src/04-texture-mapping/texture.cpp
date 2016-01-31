#include <SOIL.h>
#include <GL/glew.h>
#include <iostream>
#include "texture.hpp"

GLuint InitTexture(const unsigned char *imageData, int width, int height, GLenum format)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);      // ???

    // Specify target texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);

    // Clamp texture coordinates in [0, 1] range
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set linear magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Choose mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureId;
}

void UpdateTexture(const unsigned char *imageData, int width, int height, GLenum format)
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, imageData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

GLuint LoadImageToTexture(const char *filePath, int *w, int *h)
{
    int width, height, channels;
    unsigned char *data = SOIL_load_image(filePath, &width, &height, &channels, SOIL_LOAD_RGBA);
    if (!data) {
        std::cerr << "Failed to load image: " << filePath << std::endl;
        return 0;
    }
    std::cout << "Loaded image: " << filePath << std::endl;
    std::cout << "    " << width << " x " << height << " - " << channels << " channels" << std::endl;

    GLuint textureId = InitTexture(data, width, height, GL_RGBA);
    SOIL_free_image_data(data);

    *w = width;
    *h = height;
    return textureId;
}