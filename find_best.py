import os
import math

if __name__ == "__main__":
    LOG_DIR = "./log/"
    files = [_ for _ in os.listdir(LOG_DIR) if _.endswith(".log") and _.find("verbose") == -1]
    
    for testcase in ["10b", "50b", "100b", "200b", "500b"]:
        print("\nTestcase: {}".format(testcase))
        # Algorithm 1: shelf
        min_h = math.inf
        min_name = ""
        shelf_logs = sorted([_ for _ in files if _.find("guillotine") == -1 and _.find("shelf") != -1 and _.find(testcase) != -1])
        # print(len(shelf_logs))
        for log in shelf_logs:
            with open(LOG_DIR + log) as f:
                W, H = f.readline().split()
                H = int(H)
                # print(H, log)
                if H < min_h:
                    min_h = H
                    min_name = log
        print("1: {} {}".format(min_name, min_h))
        
        # Algorithm 2: shelf-guillotine
        min_h = math.inf
        min_name = ""
        guillotine_logs = sorted([_ for _ in files if _.find("guillotine") != -1 and _.find("shelf") != -1 and _.find(testcase) != -1])
        # print(len(guillotine_logs))
        for log in guillotine_logs:
            with open(LOG_DIR + log) as f:
                W, H = f.readline().split()
                H = int(H)
                # print(H, log)
                if H < min_h:
                    min_h = H
                    min_name = log
        print("2: {} {}".format(min_name, min_h))
        # exit(1)
        
        # Algorithm 3: skyline
        min_h = math.inf
        min_name = ""
        skyline_logs = sorted([_ for _ in files if _.find("guillotine") == -1 and _.find("skyline") != -1 and _.find(testcase) != -1])
        # print(len(skyline_logs))
        for log in skyline_logs:
            with open(LOG_DIR + log) as f:
                W, H = f.readline().split()
                H = int(H)
                # print(H, log)
                if H < min_h:
                    min_h = H
                    min_name = log
        print("3: {} {}".format(min_name, min_h))
        
        # Algorithm 4: skyline-guillotine
        min_h = math.inf
        min_name = ""
        skyline_guillotine_logs = sorted([_ for _ in files if _.find("guillotine") != -1 and _.find("skyline") != -1 and _.find(testcase) != -1])
        # print(len(skyline_guillotine_logs))
        for log in skyline_guillotine_logs:
            with open(LOG_DIR + log) as f:
                W, H = f.readline().split()
                H = int(H)
                # print(H, log)
                if H < min_h:
                    min_h = H
                    min_name = log
        print("4: {} {}".format(min_name, min_h))