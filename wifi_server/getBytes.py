class BitByte:
   
   def _getbyte(self, valueStr):#method helps convert string bits to hex
        n_bits = len(valueStr)
        width = (n_bits + 3) // 4  # number of hex digits
        hex_str = f"0x{int(valueStr, 2):0{width}X}"
        return hex_str
   
   def convert(self, listName):#helps convert 8 bits to string 8bit
       byteList=[]
       for x in listName:
         bytee=""
         for y in x:
            bytee = bytee+str(y)
            if len(bytee)==8:
               byteList.append(self._getbyte(bytee))
               bytee=""
       return byteList
            

