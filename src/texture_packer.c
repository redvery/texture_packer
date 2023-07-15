#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

int convert_dr(const char *diffuse_path, const char *roughness_path, const char *outpath)
{
    int diffuse_width, diffuse_height, diffuse_channels;
    unsigned char *diffuse = stbi_load(diffuse_path, &diffuse_width, &diffuse_height, &diffuse_channels, 0);

    if (!diffuse)
    {
        printf("Error: Failed to load %s\n", diffuse_path);
        return 1;
    }
    else if (diffuse_channels != 3)
    {
        printf("Error: %s needs to be a 3-channel RGB image.\n", diffuse_path);
        return 1;
    }

    int roughness_width, roughness_height, roughness_channels;
    unsigned char *roughness = stbi_load(roughness_path, &roughness_width, &roughness_height, &roughness_channels, 0);

    if (!roughness)
    {
        printf("Error: Failed to load %s\n", roughness_path);
        return 1;
    }
    else if (roughness_channels != 1)
    {
        printf("Error: %s needs to be a 1-channel grayscale image.\n", roughness_path);
        return 1;
    }

    if (diffuse_width != roughness_width || diffuse_height != roughness_height)
    {
        printf("Error: %s and %s need to have the same dimensions.\n", diffuse_path, roughness_path);
        return 1;
    }

    unsigned char *dr = (unsigned char *)calloc(diffuse_width * diffuse_height, 4);
    for (int i = 0; i < diffuse_width * diffuse_height; i++)
    {
        dr[i * 4 + 0] = diffuse[i * 3 + 0];
        dr[i * 4 + 1] = diffuse[i * 3 + 1];
        dr[i * 4 + 2] = diffuse[i * 3 + 2];
        dr[i * 4 + 3] = roughness[i];
    }
    stbi_image_free(diffuse);
    stbi_image_free(roughness);
    stbi_write_png(outpath, diffuse_width, diffuse_height, 4, dr, diffuse_width * 4);
    stbi_image_free(dr);
    return 0;
}

int convert_eno(const char *emission_path, const char *normal_path, const char *occlusion_path, const char *outpath)
{
    int emission_width, emission_height, emission_channels;
    unsigned char *emission = stbi_load(emission_path, &emission_width, &emission_height, &emission_channels, 0);

    if (!emission)
    {
        printf("Error: Failed to load %s\n", emission_path);
        return 1;
    }
    else if (emission_channels != 1)
    {
        printf("Error: %s needs to be a 1-channel grayscale image.\n", emission_path);
        return 1;
    }

    int normal_width, normal_height, normal_channels;
    unsigned char *normal = stbi_load(normal_path, &normal_width, &normal_height, &normal_channels, 0);

    if (!normal)
    {
        printf("Error: Failed to load %s\n", normal_path);
        return 1;
    }
    else if (normal_channels != 3)
    {
        printf("Error: %s needs to be a 3-channel RGB image.\n", normal_path);
        return 1;
    }
    else if (emission_width != normal_width || emission_height != normal_height)
    {
        printf("Error: %s and %s need to have the same dimensions.\n", emission_path, normal_path);
        return 1;
    }

    int occlusion_width, occlusion_height, occlusion_channels;
    unsigned char *occlusion = stbi_load(occlusion_path, &occlusion_width, &occlusion_height, &occlusion_channels, 0);

    if (!occlusion)
    {
        printf("Error: Failed to load %s\n", occlusion_path);
        return 1;
    }
    else if (occlusion_channels != 1)
    {
        printf("Error: %s needs to be a 1-channel grayscale image.\n", occlusion_path);
        return 1;
    }
    else if (emission_width != occlusion_width || emission_height != occlusion_height)
    {
        printf("Error: %s and %s need to have the same dimensions.\n", emission_path, occlusion_path);
        return 1;
    }


    unsigned char *eno = (unsigned char *)calloc(emission_width * emission_height, 4);
    for (int i = 0; i < emission_width * emission_height; i++)
    {
        eno[i * 4 + 0] = emission[i];
        eno[i * 4 + 1] = normal[i * 3 + 0];
        eno[i * 4 + 2] = occlusion[i];
        eno[i * 4 + 3] = normal[i * 3 + 1];
    }
    stbi_image_free(emission);
    stbi_image_free(normal);
    stbi_image_free(occlusion);
    stbi_write_png(outpath, emission_width, emission_height, 4, eno, emission_width * 4);
    stbi_image_free(eno);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 5 && memcmp(argv[1], "-dr", 3) == 0)
    {
        return convert_dr(argv[2], argv[3], argv[4]);
    }
    else if (argc == 6 && memcmp(argv[1], "-eno", 4) == 0)
    {
        return convert_eno(argv[2], argv[3], argv[4], argv[5]);
    }
    
    printf("Usage: texture_packer -dr image_diffuse.png image_roughness.png image_dr.png\n");
    printf("       texture_packer -eno image_emission.png image_normal.png image_occlusion.png image_eno.png\n");
    return 1;
}