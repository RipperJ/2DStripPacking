from PIL import Image
import os
import sys
import random
import matplotlib.pyplot as plt
import matplotlib.patches as patches

if __name__ == "__main__":
    LOG_DIR = "./log/"
    IMG_DIR = "./img/"
    files = [_ for _ in os.listdir(LOG_DIR) if _.endswith(".log") and _.find("verbose") == -1]
    
    for log_file in [LOG_DIR + _ for _ in files]:
        with open(log_file, "r") as log_f:
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
        # plt.title("Floorplan for {} blocks".format(len(log)))
        plt.title(log_file[6:-4] + ": " + "H = {}".format(H))
        fig.savefig(IMG_DIR+"{}.png".format(log_file[6:-4]))
        print("Area util = {:.4f}".format(area / W / H))
        plt.close()