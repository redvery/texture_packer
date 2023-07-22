#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

int convert_ar(const char *albedo_path, const char *roughness_path, const char *outpath)
{
    int albedo_width, albedo_height, albedo_channels;
    unsigned char *albedo = stbi_load(albedo_path, &albedo_width, &albedo_height, &albedo_channels, 0);

    if (!albedo)
    {
        printf("Error: Failed to load %s\n", albedo_path);
        return 1;
    }
    else if (albedo_channels != 3)
    {
        printf("Error: %s needs to be a 3-channel RGB image.\n", albedo_path);
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

    if (albedo_width != roughness_width || albedo_height != roughness_height)
    {
        printf("Error: %s and %s need to have the same dimensions.\n", albedo_path, roughness_path);
        return 1;
    }

    unsigned char *ar = (unsigned char *)calloc(albedo_width * albedo_height, 4);
    for (int i = 0; i < albedo_width * albedo_height; i++)
    {
        ar[i * 4 + 0] = albedo[i * 3 + 0];
        ar[i * 4 + 1] = albedo[i * 3 + 1];
        ar[i * 4 + 2] = albedo[i * 3 + 2];
        ar[i * 4 + 3] = roughness[i];
    }
    stbi_image_free(albedo);
    stbi_image_free(roughness);
    stbi_write_png(outpath, albedo_width, albedo_height, 4, ar, albedo_width * 4);
    stbi_image_free(ar);
    return 0;
}

int convert_no(const char *normal_path, const char *occlusion_path, const char *outpath)
{
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
    else if (normal_width != occlusion_width || normal_height != occlusion_height)
    {
        printf("Error: %s and %s need to have the same dimensions.\n", normal_path, occlusion_path);
        return 1;
    }


    unsigned char *eno = (unsigned char *)calloc(normal_width * normal_height, 4);
    for (int i = 0; i < normal_width * normal_height; i++)
    {
        eno[i * 4 + 0] = 0;
        eno[i * 4 + 1] = normal[i * 3 + 0];
        eno[i * 4 + 2] = occlusion[i];
        eno[i * 4 + 3] = normal[i * 3 + 1];
    }
    stbi_image_free(normal);
    stbi_image_free(occlusion);
    stbi_write_png(outpath, normal_width, normal_height, 4, eno, normal_width * 4);
    stbi_image_free(eno);
    return 0;
}

int convert_mno(const char *metallic_path, const char *normal_path, const char *occlusion_path, const char *outpath)
{
    int metallic_width, metallic_height, metallic_channels;
    unsigned char *metallic = stbi_load(metallic_path, &metallic_width, &metallic_height, &metallic_channels, 0);

    if (!metallic)
    {
        printf("Error: Failed to load %s\n", metallic_path);
        return 1;
    }
    else if (metallic_channels != 1)
    {
        printf("Error: %s needs to be a 1-channel grayscale image.\n", metallic_path);
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
    else if (metallic_width != normal_width || metallic_height != normal_height)
    {
        printf("Error: %s and %s need to have the same dimensions.\n", metallic_path, normal_path);
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
    else if (metallic_width != occlusion_width || metallic_height != occlusion_height)
    {
        printf("Error: %s and %s need to have the same dimensions.\n", metallic_path, occlusion_path);
        return 1;
    }


    unsigned char *eno = (unsigned char *)calloc(metallic_width * metallic_height, 4);
    for (int i = 0; i < metallic_width * metallic_height; i++)
    {
        eno[i * 4 + 0] = metallic[i];
        eno[i * 4 + 1] = normal[i * 3 + 0];
        eno[i * 4 + 2] = occlusion[i];
        eno[i * 4 + 3] = normal[i * 3 + 1];
    }
    stbi_image_free(metallic);
    stbi_image_free(normal);
    stbi_image_free(occlusion);
    stbi_write_png(outpath, metallic_width, metallic_height, 4, eno, metallic_width * 4);
    stbi_image_free(eno);
    return 0;
}



int main(int argc, char *argv[])
{
    if (argc == 5 && memcmp(argv[1], "-ar", 3) == 0)
    {
        return convert_ar(argv[2], argv[3], argv[4]);
    }
    else if (argc == 5 && memcmp(argv[1], "-no", 4) == 0)
    {
        return convert_no(argv[2], argv[3], argv[4]);
    }
    else if (argc == 6 && memcmp(argv[1], "-eno", 4) == 0)
    {
        return convert_mno(argv[2], argv[3], argv[4], argv[5]);
    }
    
    printf("Usage: texture_packer -ar image_albedo.png image_roughness.png image_ar.png\n");
    printf("       texture_packer -no image_normal.png image_occlusion.png image_no.png\n");
    printf("       texture_packer -mno image_metallic.png image_normal.png image_occlusion.png image_mno.png\n");
    return 1;
}