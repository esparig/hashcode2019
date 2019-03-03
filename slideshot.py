#!/bin/python3

class Photo:
    def __init__(self, orientation, tags):
        self.orientation = orientation
        self.tags = tags

if __name__ == '__main__':
    num_photos = int(input())
    photos = []
    for i in range(num_photos):
        inp = input().split()
        photos.append(Photo(inp[0], set(inp[2:])))
        
    for p in photos:
        print(p.orientation, p.tags)
