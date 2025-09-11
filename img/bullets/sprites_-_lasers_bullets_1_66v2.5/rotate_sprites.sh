#!/bin/bash

# Dossier contenant les images PNG d'origine
input_dir="$1"
# Dossier de sortie pour les images tournées et recadrées
output_dir="$2"

# Créer le dossier de sortie s'il n'existe pas
mkdir -p "$output_dir"

# Rotation et recadrage des images
for img in "$input_dir"/*.png; do
    # Récupérer le nom de fichier sans l'extension
    cp "$img" "$output_dir"
    filename=$(basename "$img" .png)
    echo "$img"
    
    # Récupérer les dimensions de l'image d'origine
    width=$(identify -format "%w" "$img")
    height=$(identify -format "%h" "$img")
    
    # Rotation et crop centré
    convert "$img" -background none -rotate 30 -gravity center -extent ${width}x${height} "$output_dir/${filename}_htr.png"
    convert "$img" -background none -rotate -30 -gravity center -extent ${width}x${height} "$output_dir/${filename}_btr.png"
    convert "$img" -background none -rotate 150 -gravity center -extent ${width}x${height} "$output_dir/${filename}_ihtr.png"
    convert "$img" -background none -rotate -150 -gravity center -extent ${width}x${height} "$output_dir/${filename}_ibtr.png"
    convert "$img" -background none -rotate 180 -gravity center -extent ${width}x${height} "$output_dir/${filename}_i.png"
done

echo "Rotation et recadrage centrés terminés. Les images sont dans le dossier $output_dir"

