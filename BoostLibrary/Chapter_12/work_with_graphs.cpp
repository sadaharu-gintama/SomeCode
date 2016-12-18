// 1. describing the graph type
#include <boost/graph/adjacency_list.hop>
#include <string>

typedef std::string vertex_t;
typedef boost::adjacency_list<
  boost::vecS,
  boost:vecS,
  boost::bidirectionalS,
  vertex_t> graph_type;

// 2. now we constructed it
graph_type graph;

// 3. let's use a non portable trick that speeds up graph construction
static const std::size_t vertex_count = 5;
graph.m_vertices.reserve(vertex_count);

// 4. add vertexes to the graph
typedef boost::graph_traits<graph_type>::vertex_descriptor descriptor_t;

descriptor_t cpp = boost::add_vertex(vertex_t("C++"), graph);
descriptor_t stl = boost::add_vertex(vertex_t("STL"), graph);
descriptor_t boost = boost::add_vertex(vertex_t("Boost"), graph);
descriptor_t guru = boost::add_vertex(vertex_t("C++ guru"), graph);
descriptor_t ansic = boost::add_vertex(vertex_t("C"), graph);

// 5. it's time to connect vertices with edges
boost::add_edge(cpp, stl, graph);
boost::add_edge(stl, boost, graph);
boost::add_edge(boost, guru, graph);
boost::add_edge(ansic, guru, graph);

// 6. we make a function that searches for a vertex
template <class GraphT>
void find_and_print(const GraphT &g, boost::string_ref name) {
  // 7. now we write code that gets iterators to all vertices
  typedef typename boost::graph_traits<graph_type>::vertex_iterator vert_it_t;
  vert_it_t it, end;
  boost::tie(it, end) = boost::vertices(g);

  // 6. it's time to run a search for the required vertex
  typedef boost::graph_traits<graph_type>::vertex_descriptor desc_t;
  for (; it != end; ++it) {
    desc_t desc = *it;
    if (boost::get(boost::vertex_bundle, g)[desc] == name.data()) {
      break;
    }
  }
  assert(it != end);
  std::cout << name << '\n';
}