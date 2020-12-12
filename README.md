# mariecf2-meghasg2-alz3-ccj4
## Final Project
### Data
We used the [`airports.dat`](https://raw.githubusercontent.com/jpatokal/openflights/master/data/airports.dat) and [`routes.dat`](https://raw.githubusercontent.com/jpatokal/openflights/master/data/routes.dat) from the **OpenFlights** dataset. The full data files can also be found in the `/data` directory of this repository. More information about the format of the data, as well as sample data entries, can be found [here](https://openflights.org/data.html). 

We considered airport data entries to be invalid if they had null or empty values for name, city, country, IATA code, latitude, or longitude. Additionally, data entries with extraneous commas that hindered the parsing of data were also considered to be invalid. All invalid values were discarded and not included in the graph.

When parsing the routes data into edges, we excluded entries with null source or destination IATA codes, as well as cases where the source or destination vertices were not present in the graph.

### Overview

Our program parses the OpenFlights airport and route data into a graph structure where the airports are the nodes and the routes are the edges. We have implemented three algorithms that can be performed on the graph:

**Breadth-first search:** Performs a BFS traversal, returning a list of nodes in the order visited by the traversal.

**Dijkstra's algorithm:** Takes in two airport codes and attempts to find the shortest path (based on approximate distance) between them. Returns the path/distance or specifies that no path could be found.

**PageRank:** Performs the Google PageRank algorithm, returning the PageRank score of each vertex, and a sorted list of the most popular airports according to PageRank score.

### Instructions

If running on EWS, run `module load llvm/6.0.1` before proceeding.

To compile the demo executable, run `make`. To run, type `./finalproj`. This will start an interactive prompt that will be able to demonstrate all three algorithms.

To compile the test suite, run `make test`. To run the test suite, type `./test`.

Run `make clean` to clear any executable files and compilation byproducts.
