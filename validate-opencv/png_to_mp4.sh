for i in {1..24}
do
    ffmpeg -framerate 1 -loop 1 -i "8k_image_with_white_grid_at_${i}.png" -c:v libx264 -pix_fmt yuv420p -t 5 "white_grid_at_${i}.mp4"
done