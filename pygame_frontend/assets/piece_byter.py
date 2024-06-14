from PIL import Image
from sys import argv
from os import listdir, chdir, mkdir, path

if len(argv) < 2:
    print("usage:")
    print("python piece_painter.py [input dir]")
    exit()

work_dir = argv[1]
chdir(work_dir)
files = listdir('.')
for file in files:
    if not file.endswith(".png"):
        continue
    name = file.split('/')[-1]
    if name[0] != 'b':
        continue
    print(file)
    img = Image.open(file)
    img = img.convert("RGBA")
    datas = img.getdata()
    your_color = (0, 0, 0, 255) if name[0] == 'w' else (255, 255, 255, 255)
    my_color = (0, 0, 0, 255) if name[0] == 'b' else (255, 255, 255, 255)
    raw = ''.join(['0' if sum(p[:3]) // 3 < 128 else '1' for p in datas])
    def lineify(bits):
        ret = ''
        start = 0
        while start < 256:
            ret += '  ' if start % 64 else '0b'
            ret += bits[start : start + 16]
            start += 16
            ret += '\\\n' if start % 64 else ',\n'
        return ret
    out = lineify(raw)
    print(out)

