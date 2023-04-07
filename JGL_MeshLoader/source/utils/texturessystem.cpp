#include "texturessystem.h"

std::string TextureSystem::getPathRelativeToExecutable(const std::string& path)
{
    char buf[1024];
    int len = sizeof(buf) - 1;
#ifdef _WIN32
    int n = GetModuleFileNameA(NULL, buf, len);
#else
    int n = readlink("/proc/self/exe", buf, len);
    if (n <= 0) return "";
    if (n >= len) n = len - 1;
    buf[n] = '\0';
#endif
    while (n > 0 && buf[n] != '\\' && buf[n] != '/') --n;
    return std::string(buf, buf + n + 1) + path;
}

unsigned int TextureSystem::getTextureId(const char* path, GLint tex_wrapping)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(getPathRelativeToExecutable(path).c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex_wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex_wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}