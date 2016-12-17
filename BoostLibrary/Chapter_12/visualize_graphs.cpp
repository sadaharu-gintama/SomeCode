// 1. let's write the std::otream operator for graph_type
#include <boost/graph/graphviz.hpp>

std::ostream &operator << (std::ostream &out, const graph_type &g) {
  detail::vertex_write<graph_type> vw(g);
  boost::write_graphviz(out, g, vw);
  return out;

}

// 2. detail::vertex_writer structure
namespace detail {
  template <class GraphT> class vertex_write {
    const GraphT &g_;
  public:
    explicit vertex_write(const GraphT &g) : g_(g) {};
    template <class VertexDescriptorT>
    void operator() (std::ostream &out, const VertexDescriptorT &d) const {
      out << "[label =\""
          << boost::get(boost::vertex_bundle, g_)[d]
          << "\"]";
    }
  };
}
