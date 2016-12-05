
A = 208
do_print = True

infile = open('/Users/erezcohen/Desktop/DensityFluctuations/massi/pb208-ico0-skin-10k.dat','r')
outfile = open('/Users/erezcohen/Desktop/DensityFluctuations/massi/pb208-ico0-skin-10k.csv','w')

lines = infile.readlines()

header = 'nucleus,nucleon,x,y,z,Sz,Iz'

if do_print: print header
outfile.write( header + '\n')
nuclei_ctr , nucleons_ctr = 1 , 1

with open('/Users/erezcohen/Desktop/DensityFluctuations/massi/pb208-ico0-skin-10k.dat') as f:
    
    for line in f:
        

        data = line.split()
        outcsv_string = "%d,%d,%f,%f,%f,%d,%d"%(nuclei_ctr,nucleons_ctr,float(data[0]),float(data[1]),float(data[2]),float(data[3]),float(data[4]))
        outfile.write( outcsv_string + '\n')
        
        if nucleons_ctr%A==0:
            if do_print: print outcsv_string
            nuclei_ctr = nuclei_ctr+1
            if (nuclei_ctr%1==0): print 'starting nucleous',nuclei_ctr
            nucleons_ctr = 1
        nucleons_ctr = nucleons_ctr+1


infile.close()
outfile.close()

print 'done.'