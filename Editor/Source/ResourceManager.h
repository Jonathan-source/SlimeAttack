#pragma once

class ResourceManager
{
public:
    static ResourceManager& Get() {
        static ResourceManager instance;
        return instance;
    }

    void SetResourcePath(const std::string& path);

    Texture& GetTexture(const std::string& name);

    Font& GetFont(const std::string& name);

    Sound& GetSound(const std::string& name);

private:
    ResourceManager() = default;
    virtual ~ResourceManager();

    std::string m_resourcePath;

    typedef std::unordered_map<std::string, Texture> TextureMap;
    TextureMap m_textures;

    typedef std::unordered_map<std::string, Font> FontMap;
    FontMap m_fonts;

    typedef std::unordered_map<std::string, Sound> SoundMap;
    SoundMap m_sounds;
};