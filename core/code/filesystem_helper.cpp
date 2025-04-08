#include "filesystem_helper.h"

#include "../../utils/format_stream.h"
#include "../app/errors.h"

void fs::FilesystemHelper::SetPath(const std::string& path) {
    current_path_ = path;
    MakeDir();
}

void fs::FilesystemHelper::MakeDir() {
    if (!std::filesystem::exists(current_path_)) {
        if (!std::filesystem::create_directory(current_path_)) {
            throw errors::FilesystemError{FormatStream() << "Can't create a directory \""
                                                         << current_path_ << "\""};
        }
    }
}

void fs::FilesystemHelper::RestorePath(const std::filesystem::path& path) {
    current_path_ = path;
}

fs::WithDirectory fs::FilesystemHelper::WithDir(const std::string& new_dir) {
    auto prev_path = current_path_;
    current_path_ /= new_dir;
    MakeDir();
    return fs::WithDirectory(*this, prev_path);
}

std::filesystem::path fs::FilesystemHelper::GetPath() const {
    return current_path_;
}

std::ofstream fs::FilesystemHelper::OpenFile(const std::string& filename) {
    auto file_path = current_path_ / filename;
    std::ofstream out(file_path);
    if (!out.good() || !out) {
        throw errors::FilesystemError{FormatStream()
                                      << "Can't create a file \"" << file_path << "\""};
    }
    return std::move(out);
}

fs::WithDirectory::WithDirectory(FilesystemHelper& helper, const std::filesystem::path& prev_path)
    : helper_(helper), prev_path_(prev_path) {
}

fs::WithDirectory::~WithDirectory() {
    helper_.RestorePath(prev_path_);
}
