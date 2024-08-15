#include "ShaderReader.h"

ShaderType ShaderReader::GetShaderType(const std::filesystem::path& path)
{
    std::string extension = path.extension().string();

    if (extension == ".vert") return ShaderType::Vertex;
    if (extension == ".frag") return ShaderType::Fragment;
    if (extension == ".geom") return ShaderType::Geometry;
    if (extension == ".tesc") return ShaderType::TessControl;
    if (extension == ".tese") return ShaderType::TessEvaluation;
    if (extension == ".comp") return ShaderType::Compute;
    if (extension == ".rgen") return ShaderType::Raygen;
    if (extension == ".zhit") return ShaderType::AnyHit;
    if (extension == ".chit") return ShaderType::ClosestHit;
    if (extension == ".miss") return ShaderType::Miss;
    if (extension == ".isct") return ShaderType::Intersection;
    if (extension == ".call") return ShaderType::Callable;

    // 如果没有匹配的扩展名，返回Unknown
    return ShaderType::Unknown;
}

std::vector<uint32_t> ShaderReader::ReadAsBinary(std::string_view filename)
{
    
    std::ifstream file_stream = {};
    file_stream.open(filename, std::ifstream::binary | std::ifstream::ate);
    if (!file_stream.is_open())
    {

        LOG_ERROR("====== Shader file : {} cannot be opened ======", filename.data());

    }

    size_t                buffer_size = static_cast<size_t>(file_stream.tellg());
    std::vector<uint32_t> buffer(buffer_size / 4);
    file_stream.seekg(std::ifstream::beg);
    file_stream.read(reinterpret_cast<char*>(buffer.data()), buffer_size);
    file_stream.close();

    return buffer;
}


