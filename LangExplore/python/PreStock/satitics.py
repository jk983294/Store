import re

f = open('log_whole_sim_window7_optimize.txt','r')
text = f.read()
lines = re.split(r'\r*\n*',text)
rsts = []
i=0
while i < len(lines):   
    if lines[i].startswith('paras'):
        nums = lines[i+1].split('\t')
        payoffs = re.search(r'(.*)\s+(.*)\s+(.*)\s+(.*)',lines[i+2])
        nums[len(nums)-1] = payoffs.group(3)
        #payoffs = lines[i+2].split(' ')
        
        for j in range(len(nums)):
            nums[j] = float(nums[j])   
        print nums
        rsts.append(nums)
        i += 3
    else:
        i+=1
#获得每一行结果后开始统计
total = 0
for i in range(len(rsts)):
    total += rsts[i][len(nums) -1]
total = total / len(rsts)
print total
f.close()
