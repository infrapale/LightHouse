import time
from neopixel import Neopixel
from machine import Pin, Timer
import random

rows   = 3
pix_per_round = 26
numpix = pix_per_round * rows
strip = Neopixel(numpix, 0, 14, "RGBW")

red = (255, 0, 0, 0)
orange = (255, 165, 0, 0)
yellow = (255, 150, 0, 0)
green = (0, 255, 0, 0 )
blue = (0, 0, 255, 0)
indigo = (75, 0, 130, 0)
violet = (138, 43, 226, 0)
white = (0, 0, 0, 255)

color_fi = (
    (55,  0,  0,  0),
    (155,  0,  0,  0),
    (255,  0,  0,  0),
    (255,  0,  0,  0),
    (255,  0,  0,  0),
    (255,  0,  0,  0),
    (255,  0,  0,  0),
    (255,  0,  0,  0),
    (255,255,  0,  0),
    (  0,255,  0,  0),
    (  0,255,  0,  0),
    (  0,255,  0,  0),
    (  0,255,  0,  0),
    (  0,255,  0,  0),
    (  0,255,  0,  0),
    (  0,255,  0,  0),
    ( 55, 55, 55,255),
    (255,255,255,255),
    (255,255,255,255),
    (255,255,255,255),
    (255,255,255,255),
    (255,255,255,255),
    (255,255,255,255),
    (255,255,255,255),
    (255,255,255,255),
    (255, 55, 55, 55)
)
# print(color_fi[1])
      
white_100 = (255,255,255,255)
# white_75 = list(map(operator.mul, white_100, [0.75]*4))
white_75 = [ int(x * 25 / 100) for x in white_100]
white_50 = [ int(x * 10 / 100) for x in white_100]
white_25 = [ int(x * 5 / 100) for x in white_100]

#print (white_75)

black = (0, 0, 0, 0)
position = 0
#colors_rgbw = (red, orange, white_100, black, yellow, green, blue, indigo, violet, white)
#color_pos = (white_100, white_100, white_75, white_50, white_25)
color_pos = [0,0,0,0]*5 
# same colors as normaln rgb, just 0 added at the end
#colors_rgbw = [color+tuple([0]) for color in colors_rgb]
#colors_rgbw.append((0, 0, 0, 255))

# uncomment colors_rgb if you have RGB strip
# colors = colors_rgb
# colors = colors_rgbw

run_scheme = 0
run_time   = 0
seconds    = 0

rotate_tim = Timer()
schedule_tim  = Timer()


def rotate_tick(timer):
    global position
    position = position + 1
    if position >= pix_per_round:
        position = 0
    # print(position)

def schedule_tick(timer):
    global run_scheme
    global run_time
    global seconds
    seconds = seconds + 1
    if seconds > 300:
        seconds = 0
    if run_scheme == 0:    
        if seconds == 30:
            run_scheme = 1
            run_time = 10
        elif seconds == 60:
            run_scheme = 2
            run_time = 5
    else:
        if run_time > 0:
            run_time = run_time -1
        else:
            run_scheme = 0
            
        
rotate_tim.init(freq=10.0, mode=Timer.PERIODIC, callback=rotate_tick)
schedule_tim.init(freq=1.0, mode=Timer.PERIODIC, callback=schedule_tick)



print(color_fi[10])

strip.brightness(120)
iterations = 20


def light_color_shades():
    for pix in range(numpix):
        # print(random.randint(0,255))
        c = (random.randint(0,255),random.randint(0,255),random.randint(0,128), 0)
        strip.set_pixel(pix, c)
    strip.show()
    time.sleep(0.5)

def light_random_scatter():
    for pix in range(numpix):
        # print(random.randint(0,255))
        n=random.randint(0,255)
        if (n > 128):
            c = (0,0,0,255)
        elif n > 120:
            c = (0,255,0,0)
        elif n > 100:
            c = (0,255,0,0)
        elif n > 80:
            c = (0,0,255,0)
        else:
            c = (0,0,0,0)
        
            
        # c = (random.randint(0,255),random.randint(0,255),random.randint(0,128), 0)
        strip.set_pixel(pix, c)
    strip.show()
    time.sleep(0.05)


def light_house_normal():
    for sector in range(pix_per_round):
        # print(sector)
        color_x = color_fi[sector]
        color_pos[0] = color_x
        color_pos[1] = [ int(x * 66 / 100) for x in color_x]
        color_pos[2] = [ int(x * 33 / 100) for x in color_x]
        color_pos[3] = [ int(x * 10 / 100) for x in color_x]
        color_pos[4] = [ int(x * 5 / 100) for x in color_x]
        # print(color_pos)
        dist1 = abs(sector -position)
        dist2 = abs(sector-position+pix_per_round)
        dist  = min(dist1,dist2)
        if dist < 5:
            color = color_pos[dist]
            #print(color)
        else:
            color = black
        for row in range(rows):    
            strip.set_pixel(row*pix_per_round + sector, color)
    strip.show()
 

while True:
    while run_scheme == 0:
        light_house_normal()
    while run_scheme == 1:
        light_color_shades()
    while run_scheme == 2:
        light_random_scatter()


while True:
    for color in colors:
        for sector in range(pix_per_round):
            for row in range(rows):
                for x in range(pix_per_round):
                    pix_indx = row*pix_per_round + x
                    # print(row,x,pix_indx)
                    if abs(sector-x) <2:
                        strip.set_pixel((row)*pix_per_round + x , color)
                    else:
                        strip.set_pixel((row)*pix_per_round + x , black)
                #time.sleep(0.1)
                strip.show()
                


