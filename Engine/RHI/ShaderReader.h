#include"pch.h"
#include"VulkanContext.h"

enum class ShaderType {
    Vertex,
    Fragment,
    Geometry,
    TessControl,
    TessEvaluation,
    Compute,
    Raygen,
    AnyHit,
    ClosestHit,
    Miss,
    Intersection,
    Callable,
    Unknown
};

class ShaderReader
{

private:
    std::mutex m_lock;
    std::ifstream m_filestream;
public:
	ShaderReader() {
        
	}
    ShaderType GetShaderType(const std::filesystem::path& path);

    std::vector<uint32_t>ReadAsBinary(std::string_view filename);
    
    //todo 
    std::future<VkResult>ReadAsync(std::string_view filename);

    
};