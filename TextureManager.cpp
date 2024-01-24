#include "TextureManager.h"

TextureManager::TextureManager() { //Constructor
    glGenTextures(1, &textureID);
}

TextureManager::~TextureManager() { //Destructor
    glDeleteTextures(1, &textureID);
}

GLuint TextureManager::loadTexture(const std::string& filePath) {
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Load image using SDL_image
    SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
    if (!tempSurface) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
        return false;
    }

    SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(tempSurface, SDL_PIXELFORMAT_ABGR8888, 0);
    if (!convertedSurface) {
        std::cerr << "Failed to convert surface format" << std::endl;
        SDL_FreeSurface(tempSurface);
        return false;
    }

    // Setting texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Uploading texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedSurface->w, convertedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, convertedSurface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clean up
    SDL_FreeSurface(convertedSurface);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID; //textureID
}

void TextureManager::bindTexture(GLuint textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);// Activate the specified texture unit
    glBindTexture(GL_TEXTURE_2D, textureID); // Bind the texture to the active texture unit
}