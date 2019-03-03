#!/bin/python3

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
    get_ordered_slides(photosH, photosV)
