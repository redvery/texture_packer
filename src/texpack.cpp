#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <map>
#include "stb_image.h"
#include "stb_image_write.h"

enum SourceType
{
    Undefined,
    Value,
    Image
};

struct Source
{
    Source(std::string input)
    {
        if (input.find_first_not_of("0123456789") == std::string::npos)
        {
            value = std::stoi(input);
            if (value >= 0 && value <= 255)
            {
                type = SourceType::Value;
                return;
            }
        }
        else if (input.find("r@") == 0) channel = 0;
        else if (input.find("g@") == 0) channel = 1;
        else if (input.find("b@") == 0) channel = 2;
        else if (input.find("a@") == 0) channel = 3;

        if (channel != -1)
        {
            data = stbi_load(input.substr(2).c_str(), &width, &height, &channels, 0);
            if (data)
            {
                type = SourceType::Image;
                return;
            }
        }

        std::cout << "Error: Invalid parameter: " << input << std::endl;
    }
    ~Source()
    {
        std::cout << "Freeing " << (type == SourceType::Image ? "image" : "value") << std::endl;
        if (type == SourceType::Image)
        {
            stbi_image_free(data);
        }
    }

    SourceType type = SourceType::Undefined;
    int value = 0;
    int width = 0;
    int height = 0;
    int channels = 0;
    int channel = -1;
    unsigned char *data = NULL;
};


int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        int channels = std::stoi(argv[1]);
        if ((channels == 1 || channels == 3 || channels == 4) && argc == channels + 3)
        {
            std::cout << "Creating a " << channels << "-channel image." << std::endl;
            int width = 0;
            int height = 0;
            //create as many sources as channels given
            std::vector<Source*> sources;
            for (int i = 0; i < channels; i++)
            {
                Source *s = new Source(argv[i + 2]);
                sources.push_back(s);
                if (s->type == SourceType::Image)
                {
                    if (width == 0 || height == 0)
                    {
                        width = s->width;
                        height = s->height;
                        std::cout << "Dimensions: " << width << "x" << height << std::endl;
                    }
                    else if (width != s->width || height != s->height)
                    {
                        std::cout << "Error: Images need to have the same dimensions." << std::endl;
                        return 1;
                    }
                }
            }
            if (width > 0 && height > 0)
            {
                unsigned char *out = (unsigned char *)calloc(width * height, channels);
                for (int i = 0; i < width * height; i++)
                {
                    for (int j = 0; j < channels; j++)
                    {
                        if (sources[j]->type == SourceType::Value)
                        {
                            out[i * channels + j] = sources[j]->value;
                        }
                        else if (sources[j]->type == SourceType::Image)
                        {
                            out[i * channels + j] = sources[j]->data[i * sources[j]->channels + sources[j]->channel];
                        }
                    }
                }
                stbi_write_png("out.png", width, height, channels, out, width * channels);
                stbi_image_free(out);
                return 0;
            }
            else
            {
                std::cout << "Error: At least one input needs to be an image." << std::endl;
            }
        }
    }

    std::cout << "Usage: texpack <channels> <source1> <soource2> <sourceN> <out.png>" << std::endl;
    std::cout << "       texpack 4 r@image_albedo.png g@image_albedo.png b@image_albedo.png r@image_roughness.png image_ar.png" << std::endl;
    std::cout << "       texpack 4 0 r@image_normal.png r@image_occlusion.png g@image_normal.png image_no.png" << std::endl;
    std::cout << "       texpack 4 r@image_metallic.png r@image_normal.png r@image_occlusion.png g@image_normal.png image_mno.png" << std::endl;
    std::cout << "       texpack 4 r@image_paint.png r@image_material.png r@image_dirt.png r@image_glow.png image_pmdg.png" << std::endl;
    return 1;
}