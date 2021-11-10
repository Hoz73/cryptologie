from PIL import Image


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


if __name__ == '__main__':
    end = False
    while not end:
        print('1 - hide a student number within an image \n'
              '2 - show the hidden number within a chosen image\n'
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
        elif choice == '9':
            end = True
        else:
            print('############# not a valid choice, retry again #############')

