import pandas as pd, numpy as np
from math import sqrt

A = 208
do_print = False

infile = open('/Users/erezcohen/Desktop/DensityFluctuations/massi/pb208-ico0-skin-10k.dat','r')
outfile = open('/Users/erezcohen/Desktop/DensityFluctuations/massi/pb208-ico0-skin-10k.csv','w')

lines = infile.readlines()

#
#nucleon_columns = ['nucleus_ctr','nucleon_ctr','x','y','z','R','Sz','Iz']
#nucleus_columns = ['nucleus_ctr','nucleon','R','nn_seperation','pp_seperation','pn_seperation']
#nuclei_columns  = ['nucleus']
#

header = 'nucleus,nucleon,x,y,z,R,Sz,Iz'

if do_print: print header
outfile.write( header + '\n')
nuclei_ctr , nucleons_ctr = 0 , 0

#df_nuclei = []


with open('/Users/erezcohen/Desktop/DensityFluctuations/massi/pb208-ico0-skin-10k.dat') as f:
    
    for line in f:
        # read the line form the input file
        data = line.split()
        x,y,z,Sz,Iz = float(data[0]),float(data[1]),float(data[2]),float(data[3]),float(data[4])
        R = sqrt( x*x + y*y + z*z )
        outcsv_string = "%d,%d,%f,%f,%f,%f,%d,%d"%(nuclei_ctr,nucleons_ctr,x,y,z,R,Sz,Iz)
        
#             generate a new data frame for each nucleus
#            df_nucleus = pd.DataFrame( columns = nucleus_columns )

        if do_print: print outcsv_string



#        df_nucleon = pd.DataFrame({'nucleus':nuclei_ctr
#                                  ,'nucleon':nucleons_ctr
#                                  ,'x':x,'y':y,'z':z,'R':R
#                                  ,'Sz':Sz,'Iz':Iz}
#                                  ,index=[nucleons_ctr])
#                                  
#        df_nucleus = df_nucleus.append( df_nucleon )

        outfile.write( outcsv_string + '\n')
        nucleons_ctr = nucleons_ctr+1
        
        if nucleons_ctr%A==0:
        
            nucleons_ctr = 0
            nuclei_ctr = nuclei_ctr + 1
            if nuclei_ctr%1000==0: print 'reading nucleus ',nuclei_ctr


#        if nucleons_ctr%A==0:
#
#            nn_seperation , pp_seperation , pn_seperation = [] , [] , []
#            
#            for nucleon_1 in df_nucleus:
#
#                for nucleon_2 in df_nucleus and nucleon_1!=nucleon_2:
#                    
#                    
#                    r = sqrt(  (nucleon_1.x-nucleon_2.x)*(nucleon_1.x-nucleon_2.x)
#                             + (nucleon_1.y-nucleon_2.y)*(nucleon_1.y-nucleon_2.y)
#                             + (nucleon_1.z-nucleon_2.z)*(nucleon_1.z-nucleon_2.z) )
#
#                    if nucleon_1.Iz==0 and nucleon_2.Iz==0:
#                        nn_seperation.append(r)
#                    
#                    if nucleon_1.Iz==1 and nucleon_2.Iz==1:
#                        pp_seperation.append(r)
#                    
#                    if nucleon_1.Iz != nucleon_2.Iz:
#                        pn_seperation.append(r)
#
#            df_nucleon.append( 'nn_seperation':nn_seperation )
#            df_nucleon.append( 'pp_seperation':pp_seperation )
#            df_nucleon.append( 'pn_seperation':pn_seperation )
#    








infile.close()
outfile.close()

print 'done running over all nuclei'