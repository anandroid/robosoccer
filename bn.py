# Starting with defining the network structure
from pgmpy.models import BayesianModel
from pgmpy.factors.discrete import TabularCPD
from pgmpy.sampling.Sampling import BayesianModelSampling
from pgmpy.factors.discrete import State
from pgmpy.sampling import GibbsSampling
import sys

# Define the model structure (also see the instructions)
table=[[0.5, 0.7, 0.8, 0.9, 0.6, 0.75, 0.9, 0.95, 0.6, 0.75, 0.9, 0.95, 0.7, 0.8, 0.9, 1, 0.4, 0.6, 0.7, 0.8, 0.5, 0.6, 0.8, 0.95, 0.5, 0.6, 0.7, 0.9, 0.6, 0.7, 0.85, 0.95, 0.2, 0.3, 0.5, 0.7, 0.2, 0.3, 0.5, 0.8, 0.3, 0.3, 0.5, 0.7, 0.2, 0.3, 0.5, 0.8, 0.3, 0.2, 0.2, 0.1, 0.2, 0.3, 0.2, 0.1, 0.2, 0.3, 0.2, 0.1, 0.2, 0.2, 0.1, 0.0, 0.3, 0.3, 0.1, 0.1, 0.3, 0.2, 0.1, 0.05, 0.3, 0.15, 0.1, 0.05, 0.3, 0.1, 0.05, 0.0], [0.25, 0.05, 0.05, 0.0, 0.2, 0.05, 0.0, 0.0, 0.2, 0.05, 0.0, 0.0, 0.1, 0.1, 0.0, 0.0, 0.3, 0.2, 0.1, 0.05, 0.25, 0.2, 0.05, 0.0, 0.25, 0.2, 0.1, 0.0, 0.2, 0.15, 0.05, 0.0, 0.2, 0.2, 0.2, 0.1, 0.2, 0.2, 0.2, 0.1, 0.1, 0.2, 0.2, 0.1, 0.2, 0.2, 0.2, 0.0, 0.3, 0.5, 0.5, 0.7, 0.4, 0.5, 0.5, 0.8, 0.4, 0.5, 0.5, 0.8, 0.5, 0.6, 0.7, 0.9, 0.5, 0.5, 0.7, 0.8, 0.5, 0.6, 0.7, 0.8, 0.5, 0.7, 0.8, 0.8, 0.5, 0.7, 0.8, 0.9], [0.25, 0.25000000000000006, 0.14999999999999997, 0.09999999999999998, 0.2, 0.2, 0.09999999999999998, 0.050000000000000044, 0.2, 0.2, 0.09999999999999998, 0.050000000000000044, 0.20000000000000004, 0.09999999999999995, 0.09999999999999998, 0.0, 0.3, 0.2, 0.20000000000000004, 0.14999999999999997, 0.25, 0.2, 0.14999999999999997, 0.050000000000000044, 0.25, 0.2, 0.20000000000000004, 0.09999999999999998, 0.2, 0.15000000000000005, 0.10000000000000002, 0.050000000000000044, 0.6000000000000001, 0.49999999999999994, 0.3, 0.20000000000000004, 0.6000000000000001, 0.49999999999999994, 0.3, 0.09999999999999995, 0.6, 0.49999999999999994, 0.3, 0.20000000000000004, 0.6000000000000001, 0.49999999999999994, 0.3, 0.19999999999999996, 0.39999999999999997, 0.30000000000000004, 0.30000000000000004, 0.20000000000000007, 0.4, 0.19999999999999996, 0.30000000000000004, 0.09999999999999998, 0.4, 0.19999999999999996, 0.30000000000000004, 0.09999999999999998, 0.30000000000000004, 0.20000000000000007, 0.20000000000000007, 0.09999999999999998, 0.19999999999999996, 0.19999999999999996, 0.20000000000000007, 0.09999999999999998, 0.19999999999999996, 0.20000000000000007, 0.20000000000000007, 0.1499999999999999, 0.19999999999999996, 0.15000000000000002, 0.09999999999999998, 0.1499999999999999, 0.19999999999999996, 0.20000000000000007, 0.1499999999999999, 0.09999999999999998]]
soccer_model = BayesianModel([('Goad_diff', 'Mode'), 
                              ('Time', 'Mode'),
                              ('Avg_Team_pos', 'Mode')])

# now defining the parameters.
cpd_goal_diff = TabularCPD(variable='Goad_diff', variable_card=5,
                      values=[[0.2], [0.2],[0.3],[0.2],[0.1]])
cpd_time = TabularCPD(variable='Time', variable_card=4,
                       values=[[0.25], [0.25],[0.25],[0.25]])
cpd_team_pos = TabularCPD(variable='Avg_Team_pos', variable_card=4,
                       values=[[0.3], [0.3],[0.2],[0.2]])
cpd_mode = TabularCPD(variable='Mode', variable_card=3,
                        values=table,
                        evidence=['Goad_diff', 'Avg_Team_pos','Time'],
                        evidence_card=[5, 4, 4])

# Associating the parameters with the model structure.
soccer_model.add_cpds(cpd_goal_diff, cpd_time, cpd_team_pos, cpd_mode)

# Checking if the cpds are valid for the model.
print soccer_model.check_model()

# # Check d-separations. This is only meant for those interested. You do not need to understand this to do the project.
# print cancer_model.is_active_trail('Pollution', 'Smoker')
# print cancer_model.is_active_trail('Pollution', 'Smoker', observed=['Cancer'])
# print cancer_model.local_independencies('Xray')
# print cancer_model.get_independencies()

# # Print model information
# print cancer_model.edges()
# print cancer_model.nodes()
# print cancer_model.get_cpds()

# Doing exact inference using Variable Elimination
from pgmpy.inference import VariableElimination
mode_infer = VariableElimination(soccer_model)

# Query
gd=int(sys.argv[1])
if gd<-1:
  gd=0
elif gd==-1:
  gd=1
elif gd==0:
  gd=2
elif gd==1:
  gd=3
elif gd>1:
  gd=4
ti=float(sys.argv[2])
ti=int(ti)
if ti<100:
  ti=0
elif ti<300:
  ti=1
elif ti<450:
  ti=2
else:
  ti=3
print ti
a=mode_infer.query(variables=['Mode'], evidence={'Goad_diff': gd, 'Time':ti})['Mode']

max=a.values[0]
g=0
for x in range(3):
   if max<a.values[x]:
      max=a.values[x]
      g=x
print a.values[g]
print g
if g==1:
  g=0
elif g==2:
  g=1
elif g==0:
  g=2

file_name="mode.txt"
f=open(file_name,"w+")
f.write(str(g))
f.close()
