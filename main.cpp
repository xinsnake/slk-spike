#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <tsk/auto/tsk_auto.h>

const char * images = "images/128SystemGolden3005.dd";
std::string outputPath = "output/";

class CdotTskAuto : public TskAuto {
public:
    TSK_RETVAL_ENUM processFile(TSK_FS_FILE * fs_file, const char *path) override {
        auto fileName = fs_file->name->name;
        std::cout << "Processing file: " << path << fileName << std::endl;
        std::cout << "Creating directory: " << outputPath + path << std::endl;
        createDirectories(outputPath + path);
        if (fs_file->meta->type == TSK_FS_META_TYPE_REG) {
            std::cout << "Writing file: " << outputPath + path + fileName << std::endl;
            auto buffer = new char[fs_file->meta->size];
            auto bytesRead = tsk_fs_file_read(fs_file, 0, buffer, fs_file->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
            auto destFilePath = outputPath + path + fileName;
            std::ofstream outFile(destFilePath, std::ios::binary);
            outFile.write(buffer, bytesRead);
            outFile.close();
            delete[] buffer;
        }
        return TSK_OK;
    }

    static void createDirectories(const std::string& path) {
        size_t pos = 0;
        std::string dir;
        while ((pos = path.find_first_of('/', pos)) != std::string::npos) {
            dir = path.substr(0, pos++);
            if (!dir.empty() && mkdir(dir.c_str(), 0755) && errno != EEXIST) {
                throw std::runtime_error("Failed to create directory: " + dir + " Error: " + std::strerror(errno));
            }
        }
        if (!path.empty() && mkdir(path.c_str(), 0755) && errno != EEXIST) {
            throw std::runtime_error("Failed to create directory: " + path + " Error: " + std::strerror(errno));
        }
    }
};

int main() {
    CdotTskAuto tskAuto;
    tskAuto.openImage(1, &images, TSK_IMG_TYPE_ENUM::TSK_IMG_TYPE_DETECT, 0);
    tskAuto.findFilesInImg();

    return 0;
}
