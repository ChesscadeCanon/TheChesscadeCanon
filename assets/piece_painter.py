from PIL import Image
from sys import argv
from os import listdir, chdir, mkdir, path

if len(argv) < 4:
    print("usage:")
    print("python piece_painter.py [input dir] [output dir] [hex color]")
    exit()

work_dir = argv[1]
out_dir = argv[2]
hex_color = argv[3]
rgb_color = tuple(int(hex_color[i:i+2], 16) for i in (0, 2, 4))
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
    from_color = (0, 0, 0, 255) if name[0] == 'w' else (255, 255, 255, 255)
    img = Image.open(file)
    img = img.convert("RGBA")
    datas = img.getdata()

    new_image_data = []
    for item in datas:
        if item == from_color:
            new_image_data.append(rgb_color)
        else:
            new_image_data.append(item)
        
    img.putdata(new_image_data)
    img.save('../' + out_dir + '/' + name)

