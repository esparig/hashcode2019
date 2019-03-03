#!/bin/python3
from random import shuffle


def get_ordered_slides_random(photosH, photosV):
    print(len(photosH) + len(photosV)//2)
    x = [i for i in range(len(photosH))]
    shuffle(x)
    for elem in x:
        print(photosH[elem].id)
    y = [(photosV[i], photosV[i+1]) for i in range(0,len(photosV),2)]
    shuffle(y)
    for y1, y2 in y:
        print(y1.id, y2.id)

def get_ordered_slides(photosH, photosV):
    print(len(photosH) + len(photosV)//2)
    for p in photosH:
        print(p.id)
    for i in range(0, len(photosV), 2):
        print(photosV[i].id, photosV[i+1].id)

class Photo:
    def __init__(self, id, orientation, tags):
        self.id = id
        self.orientation = orientation
        self.tags = tags

if __name__ == '__main__':
    num_photos = int(input())
    photosH = []
    photosV = []
    for i in range(num_photos):
        inp = input().split()
        if inp[0] == 'H':
            photosH.append(Photo(i, inp[0], set(inp[2:])))
        else:
            photosV.append(Photo(i, inp[0], set(inp[2:])))
    """
    for p in photosH:
        print(p.orientation, p.tags)
    for p in photosV:
        print(p.orientation, p.tags)
    """
    get_ordered_slides_random(photosH, photosV)
