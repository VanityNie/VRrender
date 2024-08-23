#include"pch.h"





struct TextureSpecification
{
    bool          IsUsageSampled = true;
    bool          IsUsageStorage = false;
    bool          IsUsageTransfert = true;
    bool          PerformTransition = true;
    bool          IsCubemap = false;
    uint32_t      Width = 0;
    uint32_t      Height = 0;
    uint32_t      BytePerPixel = 4;
    uint32_t      LayerCount = 1;


};


class Image
{

private:
	VkImage handle;
};

class Texture
{


	


};


class Texture2D : public Texture
{


};


class CubMap
{

};