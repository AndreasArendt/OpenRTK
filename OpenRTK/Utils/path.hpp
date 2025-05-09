#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

bool hasExtension(const fs::path& path, const std::vector<std::string>& extensions) 
{
    std::string ext = path.extension().string();
    for (const auto& e : extensions) 
    {
        if (ext == e) 
            return true;
    }
    return false;
}

bool hasExtension(const fs::path& path, const std::string& extension)
{
    std::string ext = path.extension().string();    
    if (ext == extension)
        return true;
    
    return false;
}