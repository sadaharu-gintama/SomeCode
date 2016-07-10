#include <boost/variant.hpp>
#include <vector>
#include <iostream>
#include <string>

typedef boost::variant<int, float, std::string> cell_t;
typedef std::vector<cell_t> db_row_t;

db_row_t get_row(const char *query) {
  db_row_t row;
  row.push_back(10);
  row.push_back(10.1f);
  row.push_back(std::string("Hello again"));
  return row;
}

struct db_sum_visitor: public boost::static_visitor<double> {
  double operator() (int value) const {
    return value;
  }
  double operator() (float value) const {
    return value;
  }
  double operator() (const std::string &value) const {
    return 0.0;
  }
};

int main() {
  db_row_t row = get_row("Query: give me some row, please:");
  double res = 0.0;
  db_row_t::const_iterator it = row.begin();
  for (; it != row.end(); ++it) {
    res += boost::apply_visitor(db_sum_visitor(), *it);
  }

  std::cout << "Summary is: " << res << std::endl;
  return 0;
}
