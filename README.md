# JSON++

A lightweight and easey-to-use JSON parser/manipulator implemented in C++ new standards

## Motivation

[JavaScript Object Notation (JSON)][json] is a widely-using text format for structured data presentation and exchanging. Although there are many third libraries support JSON parsing, most of them are too heavy or not so easy to use. New standards of C++ and STL make it possible to manipulate JSON in a easier or more elegant way in C++. Here my project 'JSON++' is just a try on this direction.

[json]: https://en.wikipedia.org/wiki/JSON

## Quick Start

[JSON++][json++] is published as [single C++ header file][source], and it is easy to fetch/download and embed into your projects.

[json++]: https://github.com/yanlinlin82/jsonxx
[source]: https://raw.githubusercontent.com/yanlinlin82/jsonxx/master/json.hpp

After include the header:

```{c++}
#include "json.hpp"
```

You can manipulate JSON format easily in C++, as following examples:

```{c++}
Json j;
j["x"] = 123;
j["y"] = "hello";
j["z"][2] = true;
std::cout << j << std::endl; // output: {"x":123,"y":"hello","z":[null,null,true]}
```

## Known Limitations

* Float numbers are not precise enough, because they are converted and stored in string format.

    ```cpp
    std::cerr << json(1.23456789) << std::endl;
    // output: 1.234568
    ```

* When using C++ syntax to construct, only the same type values are allowed to declare in an array.

    ```cpp
    json j1({123, 45});      // OK
    json j2({"abc", "xyz"}); // OK
    json j3({123, "xyz"});   // syntex error
    ```
