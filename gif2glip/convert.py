#!/usr/bin/python

import Image, ImageEnhance
import os, glob, sys, tempfile, re

class createAnimation:
    def __init__(self,filename):
        self.pimage = 0

        self.taillex = 0
        self.tailley = 0

        self.tmp = 0

        self.pal = [0, 0, 0]

        # Temporary folder
        self.path = tempfile.mkdtemp();

        self.filename = filename

    def genMap(self) :
        # Web color Map
        color = [0,0,0]

        for i in range(215):
            if (color[2] == 0xFF):
                if (color[1] == 0xFF):
                    color[0] = color[0] + 0x33
                    color[1] = 0x00
                else:
                    color[1] = color[1] + 0x33

                color[2] = 0x00
            else:
                color[2] = color[2] + 0x33

            self.pal.append (color[0])
            self.pal.append (color[1])
            self.pal.append (color[2])

        self.pimage = Image.new("P", (1, 1), 0)
        self.pimage.putpalette(self.pal)


    def main(self):
        x = re.match("^.*-(\d+)-(\d+)-(\d+)\.gif", self.filename)
        if x:
            delay, nbr_lignes, nbr_colonnes = int(x.group(1)), int(x.group(2)), int(x.group(3))
        else:
            sys.stderr.write("Filename: fichier-delay-lignes-colonnes.gif\n")
            sys.exit(1)

        self.taillex = nbr_colonnes
        self.tailley = nbr_lignes
        print "Size : %dx%d (%dpx x %dpx)"%(nbr_colonnes, nbr_lignes, self.taillex, self.tailley)

        # Temp folder
        print "Temp : %s" % (self.path)

        # Split the animated gif
        nom_frame = self.path + "/frame%03d.gif"
        cmd = "convert %s -scene 1 +adjoin +repage %s" % (self.filename,nom_frame)
        if os.system(cmd) == 0:
            print "Decomposition de l'image temporaire"

        liste = glob.glob(self.path+"/frame*")
        nbr_images = (len(liste))

        # Generate the color map
        self.genMap()

        goodfilename = (os.path.basename(self.filename)).split('.')[0]
        goodfilename = goodfilename.replace('-','')
        goodfilename = goodfilename.replace('_','')
        animinc = open("../dataAnim/"+goodfilename+".h","w")

        # Agregation des DATA
        animinc.write("static const unsigned short data_"+goodfilename+"[] = {")

        # Lecture de chaque frame
        # Creation des fichiers temporaires DATA
        outfiles = [open(self.path+"/DATA%d" % num, "w") for num in range(self.taillex*self.tailley)]
        liste.sort()
        for img in liste:
          # Ouverture
          im = Image.open(img)
          # Redimensionnement de l'image
          self.tmp = im.resize((8*self.taillex, 8*self.tailley))
          # Mise a jour de la couleur
          self.setColor()
          # Ecriture de l'image
          print "%s"%(img)
          # Sauvegarde de l'image
          self.tmp.save("%s"%(img))
          #Separation par bloc et ecriture dans le fichier data correspondant
          self.writeBloc(outfiles)

        # On concatene les fichiers DATA dans animinc
        for num in range(self.taillex*self.tailley):
          outfiles[num].close()
          outfiles[num] = open(self.path+"/DATA%d" % num,"r")
          animinc.write(outfiles[num].read())
          outfiles[num].close()

        animinc.write("};")

        # Structure definissant l'animation
        stopmap = "stopmap" in goodfilename and 1 or 0
        desync = "desync" in goodfilename and 1 or 0
        animinc.write(("\n\nstatic const AnimationDescriptor "+goodfilename+" = {\""+goodfilename[:20]+"\",\n%d,\n%d,\n%d,\n%d,\ndata_"+goodfilename+",%d,%d\n};") % (nbr_images,10*delay,self.tailley,self.taillex,stopmap,desync))
        animinc.close()

        return nbr_images

    def setColor(self):
        # Passage en RGBA
        self.tmp = self.tmp.convert("RGBA")

        # Mise de la couleur de transparence au noir
        plist = list(self.tmp.getdata())
        for j in range(len(plist)):
            if plist[j][3] == 0:
                plist[j] = (0,0,0,255)

        self.tmp.putdata(plist)

        # Passage en RGB
        self.tmp = self.tmp.convert("RGB")

        # Assignation de la nouvelle table de couleurs
        self.tmp = self.tmp.quantize(palette=self.pimage)

    def writeImage(self):
        data = list (self.tmp.getdata())
        for d in range(len(data)):
            if ((d%(8*self.taillex))==0):
                print ""

            print "%003d"%(data[d]),

    def writeBloc(self,outfiles):
        lut = self.tmp.resize((256, 1))
        lut.putdata(range(256))
        lut = list(lut.convert("RGB").getdata())

        #ecriture du delai de l'image
        # !!!! demander micka comment on les recupere !!!! (82)
        #for i in range (self.taillex*self.tailley):
        #    outfiles[i].write("bufferIMG = {")#delaysArr[%d]=0x0A;\n" % num)
        #on recupere les valeurs des pixels dans data
        data = list (self.tmp.getdata())
        #on ecrit d'abord les 4 premiers blocs
        cpt = 0
        ligne_bloc = 0
        pixel = 0
        while ligne_bloc < self.tailley:
            ligne = 0
            while ligne < 8:
                for num_bloc in range (self.taillex):
                    for j in range (8):
                        color = ((lut[data[pixel]][0] & 0x0F)<<8)
                        color = color | ((lut[data[pixel]][1] & 0x0F)<<4)
                        color = color | (lut[data[pixel]][2] & 0x0F)
                        outfiles[num_bloc+ligne_bloc*(self.taillex)].write("0x%03X," %(color))
                        pixel = pixel+1
                ligne = ligne + 1
            ligne_bloc = ligne_bloc + 1
