# Texture Packer
Combines channels of 5 PNG images into 2

#### Diffuse/Roughness Map
```
texture_packer -ar image_albedo.png image_roughness.png image_ar.png
```

#### Normal/Occlusion Map
```
texture_packer -no image_normal.png image_occlusion.png image_no.png
```

#### Metallic/Normal/Occlusion Map
```
texture_packer -mno image_metallic.png image_normal.png image_occlusion.png image_mno.png
```
