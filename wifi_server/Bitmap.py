from PIL import Image
from getBytes import BitByte

class bitConverter:
    def __init__(self, fileName, imgWidth, imgHeight, hexFileName):
        self.fileName=fileName
        self.width=imgWidth
        self.height=imgHeight
        self.textName=hexFileName

    def convertImg(self):
        # Load image and convert to grayscale
        img = Image.open(self.fileName).convert("L")

        img = img.resize((self.width, self.height))#valid for multiples of 8s

        w, h = img.size
        threshold = w/2  #Choose a threshold: pixels <= threshold -> 1 else 0 (can also be inverted), i chose half
        binary = []

        for y in range(h):
            row = []
            for x in range(w):
                pixel = img.getpixel((x, y))  # 0..255
                bit = 0 if pixel <= threshold else 1 #this means any color int value above 128 is 1 while below is 0
                row.append(bit)
            binary.append(row)

        for row in binary[:20]:
            print("".join(str(b) for b in row))

        with open("output_bitmap.txt", "w") as f:
            for row in binary:
                f.write("".join(str(b) for b in row) + "\n")

        byteData= BitByte()
        result =byteData.convert(binary); # helps convert bitmap into Hexadecimal bytesmap for compactibility and scaling

        with open(f"{self.textName}.txt", "w") as f:#helps convert hexmap into txt
            counter=1
            for y in result:
                f.write(str(y)+("," if counter != (w/8) else "\n"))
                counter= counter+1 if counter !=(w/8) else 1
        print("Done converting....")


