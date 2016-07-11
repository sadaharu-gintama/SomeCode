#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <assert.h>
#include <string>

struct date {
  unsigned short year;
  unsigned short month;
  unsigned short day;
};

date parse_date_time1(const std::string &s) {
  using boost::spirit::qi::_1;
  using boost::spirit::qi::ushort_;
  using boost::spirit::qi::char_;
  using boost::phoenix::ref;

  date res;
  const char* first = s.data();
  const char* const end = first + s.size();
  bool success = boost::spirit::qi::parse(first, end,
    ushort_[ref(res.year) = _1] >> char('-') >>
    ushort_[ref(res.month) = _1] >> char('-') >>
    ushort_[ref(res.day) = _1]);
  // we can also use lambda here
  // ushort_[[&res](unsigned short s) {res.year = s;}]
  if (!success || first != end) {
    throw std::logic_error("Parsing Failed");
  }
  return res;
}

// now modify the parser and take care of the digits count
date parse_date_time2(const std::string &s) {
  using boost::spirit::qi::_1;
  using boost::spirit::qi::uint_parser;
  using boost::spirit::qi::char_;
  using boost::phoenix::ref;

  uint_parser<unsigned short, 10, 2, 2> u2_;
  uint_parser<unsigned short, 10, 4, 4> u4_;

  date res;
  const char* first = s.data();
  const char* const end = first + s.size();
  bool success = boost::spirit::qi::parse(first, end,
    u4_[ref(res.year) = _1] >> char('-') >>
    u2_[ref(res.month) = _1] >> char('-') >>
    u2_[ref(res.day) = _1]);
  // we can also use lambda here
  // ushort_[[&res](unsigned short s) {res.year = s;}]
  if (!success || first != end) {
    throw std::logic_error("Parsing Failed");
  }
  return res;
}


int main() {
  date d = parse_date_time1("1988-03-25");
  assert(d.year == 1988);
  assert(d.day == 25);

  d = parse_date_time2("1988-03-25");
  assert(d.year == 1988);
  assert(d.day == 25);

  return 0;
}
