#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <string>
#include <sstream>

struct Photo {
  int id;
  char orient;
  std::unordered_set<std::string> tags;
};

struct Slide {
  int a;
  int b;
};

void read_photos(int N, std::vector<Photo>& photos) {
  Photo p;
  int curr_taglen;
  std::string curr_tag;
  for(int i = 0; i < N; ++i) {
    p.id = i;
    std::cin >> p.orient >> curr_taglen;
    p.tags.clear();
    while (curr_taglen > 0) {
      std::cin >> curr_tag;
      p.tags.insert(curr_tag);
      --curr_taglen;
    }
    photos.push_back(p);
  }
  return;
}

void read_slide(int& a, int& b, const std::vector<Photo>& photos) {
  std::cin >> a;
  if (photos[a].orient == 'V') {
    std::cin >> b;
  } else {
    b = -1;
  }
  return;
}

int tran_score(const Slide& lhs, const Slide& rhs, const std::vector<Photo>& photos) {
  std::set<std::string> lhs_tags;
  lhs_tags.insert(photos[lhs.a].tags.begin(), photos[lhs.a].tags.end());
  if (photos[lhs.a].orient == 'V') {
    lhs_tags.insert(photos[lhs.b].tags.begin(), photos[lhs.b].tags.end());
  }
  std::set<std::string> rhs_tags;
  rhs_tags.insert(photos[rhs.a].tags.begin(), photos[rhs.a].tags.end());
  if (photos[rhs.a].orient == 'V') {
    rhs_tags.insert(photos[rhs.b].tags.begin(), photos[rhs.b].tags.end());
  }

  int M = std::max(lhs_tags.size(), rhs_tags.size());

  std::vector<std::string> intersect(M);
  std::vector<std::string>::iterator it_inter;
  it_inter = std::set_intersection(lhs_tags.begin(), lhs_tags.end(),
                                   rhs_tags.begin(), rhs_tags.end(), intersect.begin());
  intersect.resize(it_inter - intersect.begin());

  std::vector<std::string> lhs_comp(M);
  std::vector<std::string>::iterator it_lcomp;
  it_lcomp = std::set_difference(lhs_tags.begin(), lhs_tags.end(),
                                 rhs_tags.begin(), rhs_tags.end(), lhs_comp.begin());
  lhs_comp.resize(it_lcomp - lhs_comp.begin());

  std::vector<std::string> rhs_comp(M);
  std::vector<std::string>::iterator it_rcomp;
  it_rcomp = std::set_difference(rhs_tags.begin(), rhs_tags.end(),
                                 lhs_tags.begin(), lhs_tags.end(), rhs_comp.begin());
  rhs_comp.resize(it_rcomp - rhs_comp.begin());

  return std::min(std::min(lhs_comp.size(), rhs_comp.size()),intersect.size());
}

int main() {
  int N; // num of photos
  std::cin >> N;
  std::vector<Photo> photos;
  read_photos(N, photos);
  std::vector<bool> avail(N, true);

  int S; // num of slides
  std::cin >> S;
  std::vector<Slide> slides(S);
  for(int i = 0; i < S; ++i) {
    read_slide(slides[i].a, slides[i].b, photos);
  }



  int curr_id;
  for(int i = 0; i < S; ++i) {
    curr_id = slides[i].a;
    if (photos[curr_id].orient == 'H') {
      if (slides[i].b != -1) {
        std::cerr << "slide " << i << " can only contain one H photo" << std::endl;
      }
    }
    if (photos[curr_id].orient == 'V') {
      if (slides[i].b == -1) {
        std::cerr << "slide " << i << " must contain two V photos" << std::endl;
      } else if (photos[slides[i].b].orient != 'V') {
        std::cerr << "slide " << i << " must contain two V photos" << std::endl;
      }
    }
  }



  for(int i = 0; i < S; ++i) {

    if (avail[slides[i].a]) {
      avail[slides[i].a] = false;

      if (slides[i].b != -1) {
        if (avail[slides[i].b]) {
          avail[slides[i].b] = false;
        } else {
          std::cerr << "slide " << i << " repeats a photo" << std::endl;
        }
      }

    } else {
      std::cerr << "slide " << i << " repeats a photo" << std::endl;
    }
  }



  if (S == 0) {
    std::cout << "error, 0 slides!" << std::endl;
  }
  int score = 0;
  for(int i = 0; i < S-1; ++i) {
    score += tran_score(slides[i], slides[i+1], photos);
  }
  std::cout << "valid solution! total score: " << score << std::endl;
  return 0;
}
