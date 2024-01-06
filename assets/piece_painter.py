from PIL import Image
from sys import argv
from os import listdir, chdir, mkdir, path

if len(argv) < 4:
    print("usage:")
    print("python piece_painter.py [input dir] [output dir] [hex color]")
    exit()

work_dir = argv[1]
out_dir = argv[2]
hex_color_1 = argv[3] if int(argv[3], 16) >= 0 else ""
hex_color_2 = argv[4] if len(argv) > 4 and int(argv[4], 16) >= 0 else ""
rgb_color_1 = tuple(int(hex_color_1[i:i+2], 16) for i in (0, 2, 4)) if hex_color_1 else ()
rgb_color_2 = tuple(int(hex_color_1[i:i+2], 16) for i in (0, 2, 4)) if hex_color_2 else ()
if not path.exists(out_dir):
    mkdir(out_dir)
chdir(work_dir)
files = listdir('.')
for file in files:
    if not file.endswith(".png"):
        continue
    name = file.split('/')[-1]
    if name[0] not in ('w', 'b'):
        continue
    img = Image.open(file)
    img = img.convert("RGBA")
    datas = img.getdata()
    your_color = (0, 0, 0, 255) if name[0] == 'w' else (255, 255, 255, 255)
    my_color = (0, 0, 0, 255) if name[0] == 'b' else (255, 255, 255, 255)
    new_image_data = []
    for item in datas:
        if rgb_color_1 and item == your_color:
            new_image_data.append(rgb_color_1)
        elif rgb_color_2 and item == my_color:
            new_image_data.append(rgb_color_2)
        else:
            new_image_data.append(item)
    img.putdata(new_image_data)
    img.save('../' + out_dir + '/' + name)

