#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib>

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

std::unordered_set<std::string> get_tags(const Slide& lhs, const std::vector<Photo>& photos) {
  std::unordered_set<std::string> lhs_tags;
  lhs_tags.insert(photos[lhs.a].tags.begin(), photos[lhs.a].tags.end());
  if (photos[lhs.a].orient == 'V') {
    lhs_tags.insert(photos[lhs.b].tags.begin(), photos[lhs.b].tags.end());
  }
  return lhs_tags;
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

bool get_slide(const std::vector<Photo>& photos,
               std::vector<bool>& avail,
               int& LAST_USED,
               Slide& s) {

  for(int i = LAST_USED; i < avail.size(); ++i) {
    if (avail[i]) {
      if (photos[i].orient == 'H') {
        s.a = i;
        s.b = -1;
        avail[i] = false;
        LAST_USED = i;
        return true;
      } else {
        for(int j = i+1; j < avail.size(); ++j) {
          if (avail[j] && photos[j].orient == 'V') {
            s.a = i;
            s.b = j;
            avail[i] = false;
            avail[j] = false;
            LAST_USED = i;
            return true;
          }
        }
      }
    }
  }
  return false;
}

std::vector<Photo>* photos_ptr;


bool photo_comparator(const int& lhs, const int& rhs) {



  std::vector<Photo>& photos = *photos_ptr;
  return photos[lhs].tags.size() <= photos[rhs].tags.size();
}

bool get_available(const std::vector<Photo>& photos,
               std::vector<bool>& avail,
               std::vector<int>& c,
               Slide& s) {

  std::random_shuffle(c.begin(), c.end());

  for(int i = 0; i < c.size(); ++i) {
    if (avail[c[i]]) {
      if (photos[c[i]].orient == 'H') {
        s.a = c[i];
        s.b = -1;
        avail[c[i]] = false;
        return true;
      } else {

        for(int j = c[i]+1; j < avail.size(); ++j) {
          if (avail[j] && photos[j].orient == 'V') {
            s.a = c[i];
            s.b = j;
            avail[c[i]] = false;
            avail[j] = false;
            return true;
          }
        }
      }
    }
  }
  return false;
}

int main() {
  int N; // num of photos
  std::cin >> N;
  std::vector<Photo> photos;
  read_photos(N, photos);
  std::vector<bool> avail(N, true);
  photos_ptr = &photos;

  std::unordered_set<std::string>::iterator itp;
  std::unordered_map<std::string, std::vector<int> >::iterator itr;
  std::unordered_map<std::string, std::vector<int> > ridx;
  for(int i = 0; i < N; ++i) {
    for(itp = photos[i].tags.begin();
        itp != photos[i].tags.end(); ++itp) {

      itr = ridx.find(*itp);
      if (itr == ridx.end()) {
        ridx[*itp] = std::vector<int>();
        itr = ridx.find(*itp);
      }

      itr->second.push_back(i);
    }
  }

  int LAST_USED = 0;
  std::vector<Slide> res;
  Slide curr;
  get_slide(photos, avail, LAST_USED, curr);
  res.push_back(curr);

  std::unordered_set<std::string> curr_tags;
  std::unordered_set<std::string>::iterator itct;
  bool found = true;

  while(found) {
    curr_tags = get_tags(curr, photos);
    found = false;
    for(itct = curr_tags.begin(); itct != curr_tags.end() && !found; ++itct) {
      std::vector<int>& c = ridx[*itct];
      if (get_available(photos, avail, c, curr)) {
        found = true;
      }
    }
    if (!found) {
      if (get_slide(photos, avail, LAST_USED, curr)) {
        res.push_back(curr);
        found = true;
      } else {
        found = false;
      }
    } else {
      res.push_back(curr);
    }
  }

  std::cout << res.size() <<  std::endl;
  for(int i = 0; i < res.size(); ++i) {
    std::cout << res[i].a;
    if (res[i].b != -1) {
      std::cout << ' ' << res[i].b;
    }
    std::cout << std::endl;
  }
  return 0;
}
