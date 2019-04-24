import fileinput
import string
import random
import os
import sys

g=sys.argv[1]
current_pos=sys.argv[2].replace('R','').replace('C','').split()
count=int(sys.argv[3])
s=open("hyp_X_soccer_problem.pddl").read()
a=string.replace(s,'Rs? Cs?','R'+current_pos[0]+' C'+current_pos[1])
a=string.replace(a,'Rg? Cg?',g)

if count%2==0:
	a=string.replace(a,'?move','ver')
else:
	a=string.replace(a,'?move','hoz')
count =count +1
f=open("hyp_1_soccer_problem.pddl","w+")
f.write(a)
f.close()
cmd_line = 'cp hyp_1_soccer_problem.pddl ~/Desktop/Shared/Parallelsfolder/fastdownward ; cd ~/Desktop/Shared/Parallelsfolder/fastdownward ; python fast-downward.py soccer_domain.pddl hyp_1_soccer_problem.pddl --search "astar(lmcut())" > hyp_1_soccer_problem.soln ; cp hyp_1_soccer_problem.soln ~/Desktop/Shared/Parallelsfolder/utaustinvilla3d ; cd ~/Desktop/Shared/Parallelsfolder/utaustinvilla3d'
os.system(cmd_line)
s=open("hyp_1_soccer_problem.soln","r")
counter=-1
for x in s:
	if counter==1:
		counter = counter +1
		plan = x
	if counter==0:
		counter = counter +1
	if "Solution found!" in x:
		counter=counter+1

print plan