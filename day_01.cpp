#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "util.hpp"

const map<std::string, int> numbers = {
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}};

int main()
{
  using namespace std;
  vector<string> lines;
  for (const auto &line : fiterator("input/day1.txt", '\n'))
    lines.push_back(line);

  int64_t sum = 0;

  for (const auto &line : lines)
  {
    int64_t prev = 0, last = 0;
    for (char c : line)
      if (c >= '0' && c <= '9')
        if (prev)
          last = c;
        else
          prev = c;
    if (!last)
      last = prev;
    if (prev)
      sum += (prev - '0') * 10 + last - '0';
  }

  cout << "Day1:" << sum << endl;

  int64_t sum2 = 0;

  for (const auto &line : lines)
  {
    int64_t prev = 0, last = 0;
    for (int i = line.size() - 1; i >= 0; i--)
    {
      char c = line[i];
      if (!(c >= '0' && c <= '9'))
      {
        c = 0;
        for (auto const &x : numbers)
        {
          auto num = line.substr(i, x.first.size());
          if (num == x.first)
          {
            c = x.second + '0';
            break;
          }
        }
      }
      if (c)
      {
        if (!last)
          last = c;
        else
          prev = c;
      }
    }
    if (!prev)
      prev = last;
    sum2 += (prev - '0') * 10 + last - '0';
  }
  cout << "Day1_2:" << sum2 << endl;
}