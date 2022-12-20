import os

cpp_executable = "./floorplan"

testcase_home = "./testcase/"
testcases = ["10.txt", "50.txt", "100.txt", "200.txt", "500.txt"]

# algorithm:
## 0: MILP
## 1: shelf w | w/o guillotine
## 2: skyline w | w/o guillotine

algorithm = 0
os.system(" ".join([cpp_executable, testcase_home+"10.txt", str(algorithm), str(10)]))
#! Since other MILP cases leads to timeout (>2 hours), please run them separately using command `./floorplan ./testcase/<testcase_file> 0 <you_own_time_out(seconds)>`

algorithm = 1
sub_alg_types = range(16)
sort_types = range(6)
for testcase in testcases:
    for sub_type in sub_alg_types:
        for sort_type in sort_types:
            os.system(" ".join([cpp_executable, testcase_home+testcase, str(algorithm), str(sub_type), str(sort_type)]))
            
algorithm = 2
sub_alg_types = range(4)
sort_types = range(6)
for testcase in testcases:
    for sub_type in sub_alg_types:
        for sort_type in sort_types:
            os.system(" ".join([cpp_executable, testcase_home+testcase, str(algorithm), str(sub_type), str(sort_type)]))
            
