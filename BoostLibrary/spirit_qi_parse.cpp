#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <iostream>
#include <string>

struct datetime {
  enum zone_offset_t {
    OFFSET_NOT_SET,
    OFFSET_Z,
    OFFSET_UTC_PLUS,
    OFFSET_UTC_MINUS
  };

  unsigned short year_;
  unsigned short month_;
  unsigned short day_;
  unsigned short hours_;
  unsigned short minutes_;
  unsigned short seconds_;
  zone_offset_t zone_offset_type;
  unsigned int zone_offset_in_min_;
  void set_zone_offset_type(zone_offset_t t) {zone_offset_type = t;};
private:
  static void dt_assert(bool v, const char *msg) {
    if (!v) {
      throw std::logic_error("Assertion failed: " + std::string(msg));
    }
  }

public:
  datetime() : year_(0), month_(0), day_(0), hours_(0), minutes_(0),
               seconds_(0), zone_offset_type(OFFSET_NOT_SET),
               zone_offset_in_min_(0) {};
};

void set_zone_offset(datetime &dt, char sign,
                     unsigned short hours, unsigned short minutes) {
  dt.zone_offset_type = (sign == '+') ?
    datetime::OFFSET_UTC_PLUS : datetime::OFFSET_UTC_MINUS;
  dt.zone_offset_in_min_ = hours * 60 + minutes;
}

datetime parse_datetime(const std::string &s) {
  using boost::spirit::qi::_1;
  using boost::spirit::qi::_2;
  using boost::spirit::qi::_3;
  using boost::spirit::qi::uint_parser;
  using boost::spirit::qi::char_;
  using boost::phoenix::bind;
  using boost::phoenix::ref;

  datetime ret;
  uint_parser<unsigned short, 10, 2, 2> u2_;
  uint_parser<unsigned short, 10, 4, 4> u4_;

  boost::spirit::qi::rule<const char*, void()> timezone_parser = -(
    char_('Z')[bind(&datetime::set_zone_offset_type, &ret, datetime::OFFSET_Z)]|
    ((char_('+')|char_('-')) >> u2_ >> ':' >> u2_)[
      bind(&set_zone_offset, ref(ret), _1, _2, _3)]
    );

  const char* first = s.begin();
  const char* end = first + s.size();

  // parse whatever
  return ret;
}
