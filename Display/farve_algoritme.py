def rgb_rgb565(r, g, b):    
    r = (r >> 3);
    g = (g >> 2);
    b = (b >> 3);
    return(str(r) + ", " + str(g) + ", " + str(b))
    
print(rgb_rgb565(255, 255, 255))

print("unsigned char IntToRGB[256][3] = {")
for i in range(0,256):
    if (i<=32):
        r=0
        g=0
        b=127+(i*4)
    elif (i>32) and (i<=96):
        r=0
        g=(i-33)*4
        b=255
    elif (i>96) and (i<=160):
        r=(i-97)*4
        g=255
        b=255-((i-97)*4)
    elif (i>160) and (i<=223):
        r=255
        g=255-((i-161)*4)
        b=0
    else:
        r=255-((i-224)*4)
        g=0
        b=0
    r=int(r)
    g=int(g)
    b=int(b)
    print("\t\t\t{" + str(rgb_rgb565(r, g, b)) + "},")

print("};")