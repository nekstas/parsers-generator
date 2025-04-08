#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

namespace fs {

class FilesystemHelper;

class WithDirectory {
private:
    friend class FilesystemHelper;

    WithDirectory(FilesystemHelper& helper, const std::filesystem::path& prev_path);

public:
    ~WithDirectory();

private:
    FilesystemHelper& helper_;
    std::filesystem::path prev_path_;
};

class FilesystemHelper {
public:
    FilesystemHelper() = default;

    void SetPath(const std::string& path);
    WithDirectory WithDir(const std::string& path);
    void RestorePath(const std::filesystem::path& path);

    std::ofstream OpenFile(const std::string& filename);

    std::filesystem::path GetPath() const;

private:
    void MakeDir();

private:
    std::filesystem::path current_path_;
};

}  // namespace fs
