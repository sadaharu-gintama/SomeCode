// 1. start with including the header files
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <string>

// 2. now we need to define the image types that we wish to work with
typedef boost::mpl::vector<
  boost::gil::gray8_image_t,
  boost::gil::gray16_image_t,
  boost::gil::rgb8_image_t,
  boost::gil::rgb16_image_t
  > img_types;

// 3. open an existing png image can be implemented like this
std::string file_name(argv[1]);
boost::gil::any_image<img_types> source;
boost::gil::png_read_image(file_name, source);

// 4. we need to apply the operation to the picture as follows:
boost::gil::apply_operation(view(source), negate());

// 5. the following code line will help you to write an image
boost::gil::png_write_view("negate_" + file_name, const_view(source));

// 6. take a look at the modifying operation
struct negate {
  typedef void result_type;

  template <class View>
  void operator () (const View &source) const {
    // ...
  }
};

// 7. the body of operator() consists of getting a channel type
typedef typename View::value_type value_type;
typedef typename boost::gil::channel_type<value_type>::type channel_t;

// 8. it also iterates through pixels
const std::size_t channels = boost::gil::num_channels<View>::value;
const channel_t max_val = (std::numeric_limits<channel_t>::max) ();

for (unsigned int y = 0; y < source.height(); ++y) {
  for (unsigned int x = 0; x < source.width(); ++x) {
    for (unsigned int c = 0; c < channels; ++c) {
      source(x, y)[c] = max_val - source(x, y)[c];
    }
  }
 }
