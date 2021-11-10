from PIL import Image
import sys


def message_to_byte(numbers):
    result = []
    message_size = bin(len(numbers))
    message_size = message_size[2:len(message_size)]
    result.append(message_size)
    for i in range(0, len(numbers)):
        res = bin(int(numbers[i]))
        res = res[2:len(res)]
        result.append(res)
    return result


def hide_message(img, nb_bytes=None):
    m = img.height // 2  # milieu de l'image
    pixels = img.load()  # tableau des pixels
    tab = message_to_byte(nb_bytes)
    for x in range(0, len(tab)):
        for j in range(0, len(tab[x])):
            r, g, b, _ = pixels[x, j]
            if m % 2 == 0:
                r = (r & 0b11111110) | int(tab[x][j])
            else:
                b = (b & 0b11111110) | int(tab[x][j])
            pixels[x, m] = r, g, b

# def get_message(img):
#     m = img.height // 2  # milieu de l'image
#     pixels = img.load()  # tableau des pixels
#     for x in range(0, len(tab)):
#
#         for j in range(0, len(tab[x])):
#             r, g, b, _ = pixels[x, j]
#             if m % 2 == 0:
#                 r = (r & 0b11111110) | int(tab[x][j])
#             else:
#                 b = (b & 0b11111110) | int(tab[x][j])
#             pixels[x, m] = r, g, b


def question_1(file_name, output):
    student_number = "123456798"
    img = Image.open(file_name)
    hide_message(img, student_number)
    img.save(output)


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("usage: {} image output".format(sys.argv[0]))
        sys.exit(1)
    question_1(f'images/{sys.argv[1]}.png', f'{sys.argv[1]}_modifié.png')
