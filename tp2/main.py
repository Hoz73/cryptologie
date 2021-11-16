from PIL import Image
from Crypto.PublicKey import RSA
from Crypto.Hash import SHA256
from Crypto.Signature import PKCS1_v1_5


def message_to_byte(numbers):
    result = []
    message_size = bin(len(numbers))
    message_size = message_size[2:len(message_size)]
    while len(message_size) != 4:
        message_size = "0" + message_size
    message_size = "1" + message_size
    result.append(message_size)
    for i in range(0, len(numbers)):
        res = bin(int(numbers[i]))
        res = res[2:len(res)]
        nb_boucle = 4 - len(res)
        for j in range(0, nb_boucle):
            res = "0" + res
        res = "1" + res
        result.append(res)
    return result


def hide_message(img, nb_bytes=None):
    pixels = img.load()  # tableau des pixels
    tab = message_to_byte(nb_bytes)
    for y in range(0, len(tab)):
        for x in range(0, len(tab[y])):
            r, g, b, _ = pixels[x, y]
            r = (r & 0b11111110) | int(tab[y][x])
            pixels[x, y] = r, g, b


def get_message(img):
    pixels = img.load()
    res = ''
    for x in range(1, 5):
        r, _, _, _ = pixels[x, 0]
        res = res + bin(r)[-1]
    message_size = int(str(res), 2) + 1
    student_number = ''
    for y in range(1, message_size):
        number = ''
        for x in range(1, 5):
            r, _, _, _ = pixels[x, y]
            number = number + bin(r)[-1]
        student_number = student_number + str(int(str(number), 2))
    return student_number


def generate_public_and_private_key():
    key = RSA.generate(2048)
    private_key = key.export_key()
    file_out = open("private.pem", "wb")
    file_out.write(private_key)
    file_out.close()

    public_key = key.publickey().export_key()
    file_out = open("public.pem", "wb")
    file_out.write(public_key)
    file_out.close()


def signe_data(file_name, signature_name):
    f = open(f'data/{file_name}.txt', "r")
    message = f.read()
    f.close()

    myfile = open("private.pem", "r")
    private_key = RSA.importKey(myfile.read())
    f.close()

    signer = PKCS1_v1_5.new(private_key)
    digest = SHA256.new()
    digest.update(bytes(message,'utf-8'))
    sig = signer.sign(digest)

    f = open(f'signatures/{signature_name}.bin', "wb")
    f.write(sig)
    f.close()


def verfy_signed_message(data, sign):
    f = open(f'data/{data}.txt', "r")
    message = f.read()
    f.close()
    digest = SHA256.new()
    digest.update(bytes(message, 'utf-8'))

    f = open(f'signatures/{sign}.bin', "rb")
    sign = f.read()
    f.close()
    myfile = open("public.pem", "r")
    public_key = RSA.importKey(myfile.read())
    verifier = PKCS1_v1_5.new(public_key)
    return verifier.verify(digest, sign)




if __name__ == '__main__':
    end = False
    while not end:
        print('1 - hide a student number within an image \n'
              '2 - show the hidden number within a chosen image\n'
              '3 - generate a public and private key\n'
              '4 - signe data with the private key\n'
              '5 - verify data with public key\n'
              '9 - end the programme ')
        choice = input('chose your action :')
        if choice == '1':
            image_name = input('write the image name : ')
            student_number = input('write the student number : ')
            img = Image.open(f'images/{image_name}.png')
            hide_message(img, student_number)
            img.save(f'{image_name}_modifié.png')
            print('############# hiding the number within the chosen image has been done successfully ###########')
        elif choice == '2':
            image_name = input('write the image name : ')
            img = Image.open(f'{image_name}.png')
            print('############# here you are the hidden student number withen the chosen image : ###########')
            print(get_message(img))
        elif choice == '3':
            generate_public_and_private_key()
            print('############# a public and a private key have been generated : ###########')
        elif choice == '4':
            data_file = input('write the name of your data file : ')
            sing_file = input('write the name of your signature file : ')
            signe_data(data_file, sing_file)
        elif choice == '5':
            data_file = input('write the name of your data file : ')
            sing_file = input('write the name of your signature file : ')
            if verfy_signed_message(data_file, sing_file):
                print('the signature is valid')
            else:
                print('the signature is not valid')
        elif choice == '9':
            end = True
        else:
            print('############# not a valid choice, retry again #############')
