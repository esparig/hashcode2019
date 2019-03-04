#!/bin/python3
from random import shuffle
from sys import stderr

class Slide:
    def __init__(self, is_H, ids, tags):
        self.is_H = is_H
        self.ids = ids
        self.tags = tags

def get_ordered_slides_random(photosH, photosV):
    slides = []
    #print(len(photosH) + len(photosV)//2)
    x = [i for i in range(len(photosH))]
    shuffle(x)
    for elem in x:
        #print(photosH[elem].id)
        slides.append(Slide(True, [photosH[elem].id], photosH[elem].tags))
    y = [(photosV[i], photosV[i+1]) for i in range(0,len(photosV),2)]
    shuffle(y)
    for y1, y2 in y:
        #print(y1.id, y2.id)
        slides.append(Slide(False, [y1.id, y2.id], y1.tags.union(y2.tags)))
    return slides

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

def print_solution(result):
    print(len(result))
    for s in result:
        if s.is_H:
            print (s.ids[0])
        else:
            print (str(s.ids[0])+" "+str(s.ids[1]))

def print_solution_slides(result):
    print(len(result))
    for slide in result:
        for i in slide.ids:
            print(i, end=" ")
        print()

def get_t1minust2(r):
    lenr = len(r)
    M = [[0 for x in range(lenr)] for y in range(lenr)]
    for i in range(lenr):
        for j in range(lenr):
            if i != j:
                M[i][j] = len(r[i].tags.difference(r[j].tags))
    return M

def get_t1intersectiont2(r):
    lenr = len(r)
    M = [[0 for x in range(lenr)] for y in range(lenr)]
    for i in range(lenr):
        for j in range(lenr):
            if i != j:
                M[i][j] = len(r[i].tags.intersection(r[j].tags))
    return M

def get_minscore(M1, M2):
    lenm = len(M1[0])
    M = [[0 for x in range(lenm)] for y in range(lenm)]
    for i in range(lenm):
        for j in range(lenm):
            M[i][j] = min(M1[i][j], M2[i][j], M1[j][i])
    return M

def get_max_score(M, r, c):
    lenm = len(M[0])
    max_score, ri, rj = 0, None, None
    #print(r, c)
    for i in r:
        for j in c:
            if M[i][j] >= max_score:
                max_score = M[i][j]
                ri = i
                rj = j
    return (ri, rj)

def get_max_score_from(M, from_slide, c):
    lenm = len(M[0])
    max_score, ri, rj = 0, None, None
    #print(r, c)
    for j in c:
        if M[from_slide][j] >= max_score:
            max_score = M[from_slide][j]
            rj = j
    return (from_slide, rj)

def get_greedy_order(random_slides, minM):
    result = []
    row = set([i for i in range(0, len(minM[0]))])
    column = set([i for i in range(0, len(minM[0]))])

    (from_slide, to_slide) = get_max_score(minM, row, column)
    result.append(random_slides[from_slide])
    result.append(random_slides[to_slide])
    row.remove(from_slide)
    column.remove(from_slide)

    while (row and column):
        (from_slide, to_slide) = get_max_score_from(minM, to_slide, column)
        if from_slide != to_slide:
            result.append(random_slides[to_slide])
            row.remove(from_slide)
            column.remove(from_slide)
        else:
            break

    return result


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
    print("Hello!", file=stderr)
    random_slides = get_ordered_slides_random(photosH, photosV)
    print("random slides computed", file=stderr)
    M1 = get_t1minust2(random_slides)
    print("t1minust2 computed", file=stderr)
    M2 = get_t1intersectiont2(random_slides)
    print("t1intersectiont2 computed", file=stderr)
    minM = get_minscore(M1, M2)
    print("score matrix computed", file=stderr)

    #print(M1, M2, minM)

    result = get_greedy_order(random_slides, minM)
    print_solution_slides(result)
