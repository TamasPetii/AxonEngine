#include <string>
#include <filesystem>

namespace Axn {
    namespace Utils {
        inline std::string GetAssetPath(const std::string& relativePath) {
            #ifdef AXN_PROJECT_DIR
                        return std::string(AXN_PROJECT_DIR) + relativePath;
            #else
                        return relativePath; 
            #endif
        }
    }
}