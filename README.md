# Texture Packer
Combine channels of multiple images into one.

## Examples:

#### Albedo/Roughness Map
```
texpack 4 r@image_albedo.png g@image_albedo.png b@image_albedo.png r@image_roughness.png image_ar.png
```

#### Normal/Occlusion Map
```
texpack 4 0 r@image_normal.png r@image_occlusion.png g@image_normal.png image_no.png
```

#### Metallic/Normal/Occlusion Map
```
texpack 4 r@image_metallic.png r@image_normal.png r@image_occlusion.png g@image_normal.png image_mno.png
```

#### Paint/Material/Dirt/Glow Map
```
texpack 4 r@image_paint.png r@image_material.png r@image_dirt.png r@image_glow.png image_pmdg.png
```

#### Add alpha channel to RGB image
```
texpack 4 r@image.png g@image.png b@image.png 255 image_with_alpha.png
```

#### Remove alpha channel from RGBA image
```
texpack 3 r@image.png g@image.png b@image.png image_without_alpha.png
```

#### Set green channel to 128 in RGB image
```
texpack 3 r@image.png 128 b@image.png image2.png
```