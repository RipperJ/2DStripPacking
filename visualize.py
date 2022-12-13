from PIL import Image
import os
import sys
import random
import matplotlib.pyplot as plt
import matplotlib.patches as patches

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Wrong format!")
        exit(1)
    elif not os.path.exists(sys.argv[1]):
        print("Verbose log file does not exist!")
        exit(1)
    with open(sys.argv[1], "r") as log_f:
        W, H = [int(_) for _ in log_f.readline().split()]
        log = log_f.readlines()
        print("Optimal Height Found: {}".format(H))

    # img = Image.new("RGB", (W, H), (0, 0, 0))
    # draw = ImageDraw.Draw(img)
    fig = plt.figure(dpi=300)
    ax = fig.add_subplot(111)
    area = 0
    for line in log:
        l = line.split()
        x = int(l[2])
        y = int(l[3])
        w = int(l[4])
        h = int(l[5])
        area += w * h
        r = int(l[6])
        # xx = x + w if r == 0 else x + h
        # yy = y + h if r == 0 else y + w
        w, h = (w, h) if r == 0 else (h, w)
        ax.add_patch(
            patches.Rectangle((x, y), w, h, facecolor="#"+hex(random.randint(0, 16777215))[2:].rjust(6, '0'), linewidth=0.5, edgecolor="#000000", alpha=0.5)
        )
        # draw.rectangle((x, y, xx, yy), fill=(random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)))
    # img.save("./floorplan.jpg", quality=95)
    ax.set_xlim([0, W])
    ax.set_ylim([0, H])
    plt.xlim([0, W])
    plt.ylim([0, H])
    plt.title("Floorplan for {} blocks".format(len(log)))
    fig.savefig("floorplan.png")
    print("Area util = {:.4f}".format(area / W / H))