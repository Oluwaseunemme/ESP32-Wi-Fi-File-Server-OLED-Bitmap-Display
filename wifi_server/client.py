import requests
from wifi_server.Bitmap import bitConverter
url = "http://10.168.248.110/"


#i open file and pass it via the files parameter
def upload(uploadFileName):
    with open(uploadFileName, "rb") as f:
        files = {"file": (uploadFileName, f, "application/octet-stream")}
        response = requests.post(f"{url}upload", files=files)#handle file posting to the server.
    print(f"Status Code: {response.status_code}")
    print(f"Response: {response.text}")

def download(viewfilename):
    param = {"name": viewfilename}
    response = requests.get(f"{url}view", params=param)
    if response.status_code == 200:
        with open(f"downloaded_{viewfilename}", "wb") as f:
            f.write(response.content)
        print("File successfully retrieved!")
    else:
        print(f"Error {response.status_code}: {response.text}")

def delete(fileRoute):#deleted function that takes route of file to be deleted
    import requests

    headers = {
        "Content-Type": "text/plain"
    }
    response = requests.post(f"{url}delete", data=fileRoute, headers=headers)
    print(f"Response:{response.status_code}, {response.text}")

def createBitmap():#this method helps locally convert img into bimap txt file for server processing
    converter=bitConverter("start_image.jpg", 128, 64, "star_hex")
    converter.convertImg()

upload("star_hex.txt")#parse in name of your file on the server 
delete("NotoSansShavian-Regular.bin")
download("Quantum_computer.jpeg")
