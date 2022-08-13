#include "pch.h"
#include "ResourceManager.h"

void ResourceManager::SetResourcePath(const std::string& path)
{
    m_resourcePath = path;
}

Texture& ResourceManager::GetTexture(const std::string& name)
{
    TextureMap::iterator it = m_textures.find(name);
    if (it == m_textures.end())
    {
        Texture& texture = m_textures[name];
        texture = LoadTexture(std::string(m_resourcePath + "\\Textures\\" + name).c_str());
        return texture;
    }
    return it->second;
}

Font& ResourceManager::GetFont(const std::string& name)
{
    FontMap::iterator it = m_fonts.find(name);
    if (it == m_fonts.end())
    {
        Font& font = m_fonts[name];
        font = LoadFont(std::string(m_resourcePath + "\\Fonts\\" + name).c_str());
        return font;
    }
    return it->second;
}

Sound& ResourceManager::GetSound(const std::string& name)
{
    SoundMap::iterator it = m_sounds.find(name);
    if (it == m_sounds.end())
    {
        Sound& sound = m_sounds[name];
        sound = LoadSound(std::string(m_resourcePath + "\\Sounds\\" + name).c_str());
        return sound;
    }
    return it->second;
}

ResourceManager::~ResourceManager()
{
    for (auto& it : m_textures)
        UnloadTexture(it.second);

    for (auto& it : m_fonts)
        UnloadFont(it.second);

    for (auto& it : m_sounds)
        UnloadSound(it.second);
}
