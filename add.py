import subprocess
import re

# 你提供的 .gitmodules 内容
gitmodules_content = """
[submodule "submodules/glfw"]
    path = submodules/glfw
    url = https://github.com/glfw/glfw
[submodule "submodules/glm"]
    path = submodules/glm
    url = https://github.com/g-truc/glm
[submodule "submodules/imgui"]
    path = submodules/imgui
    url = https://github.com/MomoDeve/imgui
[submodule "submodules/VulkanMemoryAllocator"]
    path = submodules/VulkanMemoryAllocator
    url = https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
[submodule "submodules/glslang"]
    path = submodules/glslang
    url = https://github.com/KhronosGroup/glslang
[submodule "submodules/tinyobjloader"]
    path = submodules/tinyobjloader
    url = https://github.com/tinyobjloader/tinyobjloader
[submodule "submodules/span"]
    path = submodules/span
    url = https://github.com/tcbrindle/span
[submodule "submodules/spirv-reflect"]
    path = submodules/spirv-reflect
    url = https://github.com/KhronosGroup/SPIRV-Reflect
[submodule "submodules/stb"]
    path = submodules/stb
    url = https://github.com/nothings/stb
[submodule "submodules/tinygltf"]
    path = submodules/tinygltf
    url = https://github.com/syoyo/tinygltf
[submodule "submodules/tinyddsloader"]
    path = submodules/tinyddsloader
    url = https://github.com/benikabocha/tinyddsloader
"""

# 解析 .gitmodules 内容
submodules = re.findall(r'\[submodule "(.+)"\]\s+path = (.+)\s+url = (.+)', gitmodules_content)

# 添加每个子模块
for name, path, url in submodules:
    print(f"Adding submodule: {name}")
    try:
        subprocess.run(['git', 'submodule', 'add', url.strip(), path.strip()], check=True)
        print(f"Successfully added {name}")
    except subprocess.CalledProcessError as e:
        print(f"Error adding {name}: {e}")

print("All submodules have been processed.")