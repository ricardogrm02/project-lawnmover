///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum disk_color { DISK_LIGHT, DISK_DARK };

class disk_state {
private:
  std::vector<disk_color> _colors;

public:
  disk_state(size_t light_count) : _colors(light_count * 2, DISK_LIGHT) {

    assert(light_count > 0);

    for (size_t i = 1; i < _colors.size(); i += 2) {
      _colors[i] = DISK_DARK;
    }
  }

  bool operator==(const disk_state &rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const { return _colors.size(); }

  size_t light_count() const { return total_count() / 2; }

  size_t dark_count() const { return light_count(); }

  bool is_index(size_t i) const { return (i < total_count()); }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_initialized() const {
    for (size_t i = 0; i < total_count();
         i++) {         // check each position before function
      if (i % 2 == 0) { // check even position --> should be light
        if (_colors[i] == DISK_DARK) {
          return false;
        }
      } else { // check odd position --> should be dark
        if (_colors[i] == DISK_LIGHT) {
          return false;
        }
      }
    }

    return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks on
  // the left (low indices) and all dark disks on the right (high indices).
  bool is_sorted() const {
    // Iterate through the vector containing disk_state
    for (int i = 0; i < total_count(); i++) {
      // Check if we have dark disk in the lower indices, return false if we do
      if (get(i) == DISK_DARK && i < total_count() / 2) {
        return false;
      } // Check if we have dark disk in the lower indices, return false if we
        // do
      else if (get(i == DISK_LIGHT && i > total_count() / 2)) {
        return false;
      }
    }
    // We didn't find any disk out of place, return true
    return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:
  sorted_disks(const disk_state &after, unsigned swap_count)
      : _after(after), _swap_count(swap_count) {}

  sorted_disks(disk_state &&after, unsigned swap_count)
      : _after(after), _swap_count(swap_count) {}

  const disk_state &after() const { return _after; }

  unsigned swap_count() const { return _swap_count; }
};

// Algorithm that sorts disks using the alternate algorithm.
sorted_disks sort_alternate(const disk_state &before) {
  int numOfSwap = 0;
  disk_state disks = before;
  // Run loop for every disk in the vector
  for (int i = 0; i < disks.total_count() - 1; i++) {
    // if i is even, then we iterate through every even pair
    if (i % 2 == 0) {
      // Iterate through each even pair
      for (int j = 0; j < disks.total_count() - 1; j += 2) {
        // if left disk is dark and right disk is light swap, increment #ofSwaps
        if (disks.get(j) == DISK_DARK && disks.get(j + 1) == DISK_LIGHT) {
          disks.swap(j);
          numOfSwap++;
        }
      }
    } // if i is odd iterate through the odd pairs
    else if (i % 2 == 1) {
      // iterate through the odd pairs
      for (int k = 1; k < disks.total_count() - 1; k += 2) {
        // if left  disk is dark and right disk is light swap, increment
        // #ofSwaps
        if (disks.get(k) == DISK_DARK && disks.get(k + 1) == DISK_LIGHT) {
          disks.swap(k);
          numOfSwap++;
        }
      }
    }
  }

  // record # of step swap
  return sorted_disks(disk_state(disks), numOfSwap);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state &before) {
  int numOfSwap = 0;
  disk_state disks = before;
  // iterate through the vector n/2 times (light_count = n/2)
  for (int i = 0; i < disks.light_count();
       i++) { // lightcount is just totalcount/2

    // Iterate through the vector from left to right
    for (int j = 0; j < disks.total_count() - 1; j++) {
      // if the disk on the left is dark and the disk on the right is light,
      // swap them, increment num of swaps
      if (disks.get(j) == DISK_DARK && disks.get(j + 1) == DISK_LIGHT) {
        disks.swap(j);
        numOfSwap++;
      }
    }
    // Iterate through the vector from right to left
    for (int k = disks.total_count() - 1; k > 0; k--) {
      // if the disk on the right is a light one, and the one on the left is
      // dark, swap them, increment num of swap
      if (disks.get(k) == DISK_LIGHT && disks.get(k - 1) == DISK_DARK) {
        disks.swap(k - 1);
        numOfSwap++;
      }
    }
  }

  return sorted_disks(disk_state(disks), numOfSwap);
}
