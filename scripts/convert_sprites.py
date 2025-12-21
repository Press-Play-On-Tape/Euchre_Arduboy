from PIL import Image
import os
from pathlib import Path

def get_shade(rgba, shades, shade):
    if rgba[0] == 252 and rgba[1] == 111 and rgba[2] == 207 and rgba[3] == 255: 
        return 0

    w = (254 + shades) // shades
    b = (shade + 1) * w
    return 1 if rgba[0] >= b else 0

def get_mask(rgba):

    if rgba[0] == 252 and rgba[1] == 111 and rgba[2] == 207 and rgba[3] == 255: 
        return 0
    return 1 if rgba[3] >= 128 else 0

def convert(fname, shades, sw = None, sh = None, num = None, maskImage = False):

    if not (shades >= 2 and shades <= 4):
        print('shades argument must be 2, 3, or 4')
        return None

    im = Image.open(fname).convert('RGBA')
    pixels = list(im.getdata())
    
    masked = maskImage
    q = 0
    for i in pixels:
        q = q + 1
        # print(i[0])
        # print(i[1])
        # print(i[2])
        # print(i[3])
        if i[0] == 252 and i[1] == 111 and i[2] == 207 and i[3] == 255: 
            masked = True
            break
        if i[3] < 255:
            # print('masked!!! ')
            # print(q)
            masked = True
            # exit()
            break

    print('{}, shades {}, masked {}'.format(fname, shades, masked))


    w = im.width
    h = im.height
    if sw is None: sw = w
    if sh is None: sh = h
    nw = w // sw
    nh = h // sh
    if num is None: num = nw * nh
    sp = (sh + 7) // 8
    
    if nw * nh <= 0:
        print('%s: Invalid sprite dimensions' % fname)
        return None
        
    bytes = bytearray([sw, sh])
    
    for n in range(num):
        bx = (n % nw) * sw
        by = (n // nw) * sh
        for shade in range(shades - 1):
            for p in range(sp):
                for ix in range(sw):
                    x = bx + ix
                    byte = 0
                    mask = 0
                    for iy in range(8):
                        y = p * 8 + iy
                        if y >= sh: break
                        y += by
                        i = y * w + x
                        rgba = pixels[i]
                        byte |= (get_shade(rgba, shades, shade) << iy)
                        mask |= (get_mask(rgba) << iy)
                    bytes += bytearray([byte])
                    if masked:
                        bytes += bytearray([mask])
    
    return bytes
    
def convert_header(fname, fout, sym, shades, sw = None, sh = None, num = None, maskImage = False):
    bytes = convert(fname, shades, sw, sh, num, maskImage)
    if bytes is None: return
    with open(fout, 'a') as f:
        # f.write('#pragma once\n\n#include <stdint.h>\n#include <avr/pgmspace.h>\n\n')
        # f.write('constexpr uint8_t %s[] PROGMEM =\n{\n' % sym)
        f.write('uint8_t %s[] = {\n  ' % sym)
        for n in range(len(bytes)):
            if n % 16 == 2:
                f.write('\n  ')
            f.write('%3d,' % bytes[n])
            # f.write(' ' if n % 16 != 15 else '\n')
            # f.write(' ' if n % 18 != 2 else '\n')
            f.write(' ')
        if len(bytes) % 16 != 2:
            f.write('\n')
        f.write('};\n\n')

def deleteFile(filename):
    if os.path.isfile(filename):
        os.remove(filename)

BASE = './images/'
IMAGES = '../images/'

deleteFile(BASE + 'Images.hpp')

convert_header(IMAGES + 'Cards/Cards_Bottom.png',                                              BASE + 'Images.hpp', 'Cards_Bottom', 4, 18, 24)
convert_header(IMAGES + 'Cards/Cards_Bottom_Grey.png',                                         BASE + 'Images.hpp', 'Cards_Bottom_Grey', 4, 18, 24)
convert_header(IMAGES + 'Cards/Cards_Left.png',                                                BASE + 'Images.hpp', 'Cards_Left', 4, 23, 24)
convert_header(IMAGES + 'Cards/Cards_Right.png',                                               BASE + 'Images.hpp', 'Cards_Right', 4, 23, 24)

convert_header(IMAGES + 'Cards/Hand_Left.png',                                                 BASE + 'Images.hpp', 'Hand_Left', 4, 5, 40)
convert_header(IMAGES + 'Cards/Hand_Right.png',                                                BASE + 'Images.hpp', 'Hand_Right', 4, 5, 40)
convert_header(IMAGES + 'Cards/Hand_Top.png',                                                  BASE + 'Images.hpp', 'Hand_Top', 4, 43, 8)
convert_header(IMAGES + 'Cards/Suit.png',                                                      BASE + 'Images.hpp', 'Suit', 4, 14, 16)

convert_header(IMAGES + 'HUD/HUD_Top.png',                                                     BASE + 'Images.hpp', 'HUD_Top', 4, 23, 32)
convert_header(IMAGES + 'HUD/HUD_Bottom.png',                                                  BASE + 'Images.hpp', 'HUD_Bottom', 4, 23, 32)
convert_header(IMAGES + 'HUD/Numbers_3x5.png',                                                 BASE + 'Images.hpp', 'Numbers_3x5', 4, 13, 8)
convert_header(IMAGES + 'HUD/HUD_Trick_Score.png',                                             BASE + 'Images.hpp', 'HUD_Trick_Score', 4, 7, 8)
convert_header(IMAGES + 'HUD/HUD_Game_Score.png',                                              BASE + 'Images.hpp', 'HUD_Game_Score', 4, 14, 8)

convert_header(IMAGES + 'Cards/Hand_Full.png',                                                 BASE + 'Images.hpp', 'Hand_Full', 4)
convert_header(IMAGES + 'Cards/Hand_Full_Grey.png',                                            BASE + 'Images.hpp', 'Hand_Full_Grey', 4)
convert_header(IMAGES + 'Cards/Rotate/Rotate.png',                                             BASE + 'Images.hpp', 'Rotate', 4, 30, 32)
convert_header(IMAGES + 'Cards/Flip/Flip.png',                                                 BASE + 'Images.hpp', 'Flip', 4, 16, 24)

convert_header(IMAGES + 'Bid/Bid_Round1.png',                                                  BASE + 'Images.hpp', 'Bid_Round1', 4, 35, 24)
convert_header(IMAGES + 'Bid/Bid_Round2.png',                                                  BASE + 'Images.hpp', 'Bid_Round2', 4, 48, 24)
convert_header(IMAGES + 'Bid/Bid_Suits.png',                                                   BASE + 'Images.hpp', 'Bid_Suits', 4, 9, 8)
convert_header(IMAGES + 'Bid/Bid_Numbers.png',                                                 BASE + 'Images.hpp', 'Bid_Numbers', 4, 7, 8)

convert_header(IMAGES + 'Bid/Bid_Top.png',                                                     BASE + 'Images.hpp', 'Bid_Top', 4, 23, 8)
convert_header(IMAGES + 'Bid/Bid_Bottom.png',                                                  BASE + 'Images.hpp', 'Bid_Bottom', 4, 23, 8)
convert_header(IMAGES + 'Bid/Bid_Left.png',                                                    BASE + 'Images.hpp', 'Bid_Left', 4, 7, 24)
convert_header(IMAGES + 'Bid/Bid_Right.png',                                                   BASE + 'Images.hpp', 'Bid_Right', 4, 7, 24)
convert_header(IMAGES + 'Bid/Bid_Top_Discard.png',                                             BASE + 'Images.hpp', 'Bid_Top_Discard', 4, 27, 8)
convert_header(IMAGES + 'Bid/Bid_Bottom_Discard.png',                                          BASE + 'Images.hpp', 'Bid_Bottom_Discard', 4, 27, 8)
convert_header(IMAGES + 'Bid/Bid_Left_Discard.png',                                            BASE + 'Images.hpp', 'Bid_Left_Discard', 4, 7, 32)
convert_header(IMAGES + 'Bid/Bid_Right_Discard.png',                                           BASE + 'Images.hpp', 'Bid_Right_Discard', 4, 7, 32)
convert_header(IMAGES + 'Bid/Bid_Result.png',                                                  BASE + 'Images.hpp', 'Bid_Result', 4, 40, 8)
convert_header(IMAGES + 'Titles/TitleEuchre.png',                                              BASE + 'Images.hpp', 'TitleEuchre', 4, 128, 64)
convert_header(IMAGES + 'Titles/TitleEuchre_Assist.png',                                       BASE + 'Images.hpp', 'TitleEuchre_Assist', 4)
convert_header(IMAGES + 'Dealer.png',                                                          BASE + 'Images.hpp', 'Dealer', 4)
convert_header(IMAGES + 'EveryonePassed.png',                                                  BASE + 'Images.hpp', 'EveryonePassed', 4)
convert_header(IMAGES + 'KittyInstructions.png',                                               BASE + 'Images.hpp', 'KittyInstructions', 4)
convert_header(IMAGES + 'ChooseSuit.png',                                                      BASE + 'Images.hpp', 'ChooseSuit', 4, 49, 24)

convert_header(IMAGES + 'Titles/PPOT.png',                                                     BASE + 'Images.hpp', 'PPOT', 4, 128, 64)
convert_header(IMAGES + 'Titles/You_Won.png',                                                  BASE + 'Images.hpp', 'You_Won', 4, 75, 48)
convert_header(IMAGES + 'Titles/You_Lost.png',                                                 BASE + 'Images.hpp', 'You_Lost', 4, 75, 48)
