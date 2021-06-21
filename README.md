# Pattern_Call_Query
#### by Alp

## Features

- Read pattern calls in CSV format from file
- Query options for the pattern call id, name, relative path and call flag
- O(1) query complexity in average
- Export the retrieved pattern calls to files in CSV format

The program reads pattern call information from "patterns/Functional.pat" file, performs the queries and writes the results in seperate files.

#### Building for source

In main.cpp directory:

```sh
g++ main.cpp -o main -std=c++17
```

To run:

```sh
./main
```


#### Benefits of this design

- O(1) query complexity in average
- Interfaces return read only values
- Large objects are never copied

#### Trade-offs

- Memory consumption is higher than a naive implementation such as a vector of tuples

#### Complexity

- O(1) complexity in average

