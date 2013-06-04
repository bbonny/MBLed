1#!/usr/bin/python

import convert
import sys,os

def main():
  # Checking arguments
  if (len(sys.argv) < 2):
    print "Usage: ./gif2glip.py <fichier_gif> [<fichier_gif> ...]"
    sys.exit()

  for i in range(1,len(sys.argv)):
    if not(os.path.isfile(sys.argv[i])) or (os.path.splitext(sys.argv[i]))[1] != ".gif":
      print "A gif file is needed"
      sys.exit()

  # Write the global include file
  globalinc = open("../dataAnim/catalogue.h","w")

  # Compute all the gif files
  max_images = 0
  for i in range(1,len(sys.argv)):
    objet = convert.createAnimation(sys.argv[i])
    nbr_images = objet.main()
    if(nbr_images > max_images):
      max_images = nbr_images
    goodfilename = (os.path.basename(sys.argv[i])).split('.')[0]
    goodfilename = goodfilename.replace('-','')
    goodfilename = goodfilename.replace('_','')
    globalinc.write("#include \""+goodfilename+".h\"\n")

  globalinc.write("\n#define MAX_IMAGES %d\n" % max_images)

  # Create the catalogue
  globalinc.write("\nconst AnimationDescriptor * animationList[] = {")
  for i in range(1,len(sys.argv)):
    goodfilename = (os.path.basename(sys.argv[i])).split('.')[0]
    goodfilename = goodfilename.replace('-','')
    goodfilename = goodfilename.replace('_','')
    globalinc.write("&"+goodfilename+", ")

  globalinc.write("0};")
  globalinc.close()

main()
