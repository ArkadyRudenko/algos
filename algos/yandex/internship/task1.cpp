#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector months = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int y1, y2, m1, m2, d1, d2, h1, h2, min1, min2, sec1, sec2;
  std::cin >> y1 >> m1 >> d1 >> h1 >> min1 >> sec1 >> y2 >> m2 >> d2 >> h2 >>
      min2 >> sec2;

  int days{0}, secs{0};

  if (y2 - y1 > 1) {
    days += (y2 - y1 - 1) * 365;
  }

  if (y1 != y2) {
    for (int m = m1 + 1; m <= 12; ++m) {
      days += months[m];
    }
    for (int m = 1; m < m2; ++m) {
      days += months[m];
    }
  } else {  // years are equals
    if (m1 <= m2) {
      int full_months = m2 - m1 - 1;
      for (int i = 0; i < full_months; ++i) {
        days += months[m1 + 1 + i];
      }
    }
  }

  if (m1 != m2 || y1 != y2) {
    days += months[m1] == d1 ? 0 : months[m1] - d1;
    days += d2 - 1;
  }
  if (m1 == m2 && y1 == y2) {
    days += d2 == d1 ? 0 : d2 - d1;
  }

  if (y1 == y2 && m1 == m2 && d1 == d2) {
    secs += h2 - h1 > 1 ? (h2 - h1 - 1) * 60 * 60 : 0;
    if (h1 != h2) {
      secs += min2 * 60;
      secs += sec2;
      secs += (60 - min1) * 60;
      secs += 60 - sec1;
    } else {
      if (min2 - min1 > 1) {
        secs += (min2 - min1) * 60;
      }
      if (min1 == min2) {
        secs += sec2 - sec1;
      } else {
        secs += sec2;
        secs += 60 - sec1;
      }
    }
  } else {
    secs += h2 * 60 * 60;
    secs += min2 * 60;
    secs += sec2;

    secs += h1 == 0 ? 23 * 60 * 60 : (24 - h1 - 1) * 60 * 60;
    secs += min1 == 0 ? 59 * 60 : (60 - min1 - 1) * 60;
    secs += 60 - sec1;

    int full_day = 24 * 60 * 60;
    if (secs >= full_day) {
      days += 1;
      secs -= full_day;
    }

    // TODO: add full day
  }

  std::cout << days << " " << secs;

  return 0;
}

/*
980 2 12 10 30 1
980 2 12 10 31 37

 */