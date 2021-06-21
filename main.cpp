

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <list>
#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>

class PatternManager
{
    public:
        void insert_pattern(int id, std::string name, std::string relative_path, bool should_call);
        void insert_pattern_from_file(const std::string filename);
        const std::list<struct MyTuple>::iterator& get_pattern_with_id(const int id);
        const std::vector<std::list<struct MyTuple>::iterator>& get_pattern_with_name(const std::string name);
        const std::vector<std::list<struct MyTuple>::iterator>& get_pattern_with_path(const std::string path);
        const std::vector<std::list<struct MyTuple>::iterator>& get_pattern_with_false();
        const std::vector<std::list<struct MyTuple>::iterator>& get_pattern_with_true();
        const std::list<struct MyTuple>::iterator& safe_get_pattern_with_id(const int id);
        const std::vector<std::list<struct MyTuple>::iterator>& safe_get_pattern_with_name(const std::string name);
        const std::vector<std::list<struct MyTuple>::iterator>& safe_get_pattern_with_path(const std::string path);
        const std::vector<std::list<struct MyTuple>::iterator>& safe_get_pattern_with_false();
        const std::vector<std::list<struct MyTuple>::iterator>& safe_get_pattern_with_true();
    private:
        std::unordered_map<int, std::list<struct MyTuple>::iterator> m_id_mapping;
        std::unordered_map<std::string, std::vector<std::list<struct MyTuple>::iterator> > m_name_mapping;
        std::unordered_map<std::string, std::vector<std::list<struct MyTuple>::iterator> > m_path_mapping;
        std::unordered_map<std::string, std::vector<std::list<struct MyTuple>::iterator> > m_call_mapping;
        std::list<struct MyTuple> m_patterns;
};

struct MyTuple
{
    const int id;
    const std::string name;
    const std::string relative_path;
    const bool should_call;
    MyTuple (const int id, const std::string& name, const std::string& relative_path, const bool should_call):
    id(id), name(name), relative_path(relative_path), should_call(should_call)
    {}
};

void print_pattern(const struct MyTuple& t);
void write_pattern_to_file(const struct MyTuple& t, const std::string& filename, bool append = false);

int main(int argc, char **argv)
{
    PatternManager p;
    // read the pattern calls from file
    p.insert_pattern_from_file("patterns/Functional.pat");
    // try to create an ID duplicate
    p.insert_pattern(42, "test_duplicate", "../test.pat", false);
    p.insert_pattern(120, "test", "../test.pat", false);
    // get the pattern call with the specified id
    auto patternID = p.safe_get_pattern_with_id(33);
    // write the retrieved pattern call in a seperate file
    write_pattern_to_file(*patternID, "patterns/Id.pat", true);
    // get the pattern calls with the specified name
    auto patternsName = p.safe_get_pattern_with_name("myPattern3");
    // write the retrieved pattern calls in a seperate file
    for (auto &p: patternsName)
        write_pattern_to_file(*p, "patterns/Name.pat", true);
    // get the pattern calls with the specified path   
    auto patternsPath = p.safe_get_pattern_with_path("src/patterns/Functional.pat");
    // write the retrieved pattern calls in a seperate file
    for (auto &p: patternsPath)
        write_pattern_to_file(*p, "patterns/Path.pat", true);
    // get the pattern calls with call flags which are true   
    auto patternsTrue = p.safe_get_pattern_with_true();
    // write the retrieved pattern calls in a seperate file
    for (auto &p: patternsTrue)
        write_pattern_to_file(*p, "patterns/True.pat", true);
    // get the pattern calls with call flags which are true   
    auto patternsFalse = p.safe_get_pattern_with_false();
    // write the retrieved pattern calls in a seperate file
    for (auto &p: patternsFalse)
        write_pattern_to_file(*p, "patterns/False.pat", true);

    std::cout << "Done!" << std::endl;
    return EXIT_SUCCESS;
}

/**
 * Prints the contents of a pattern call to the console.
 *
 * @param t The pattern call whose contents are to be printed.
 */
void print_pattern (const struct MyTuple& t)
{
    std::cout << "ID: " << t.id << "\n"
              << "name: " << t.name << "\n"
              << "relative path: " << t.relative_path << "\n"
              << "should call: " << t.should_call << "\n";
}

/**
 * Prints the contents of a pattern call as a CSV to a file.
 *
 * @param t The pattern call whose contents are to be printed.
 * @param filename The relative path of the file where the CSV is printed.
 * @param append Default value is false. If set to true multiple pattern calls are written in the file line by line.
 * @throws std::runtime_error If the file could not be closed.
 */
void write_pattern_to_file(const struct MyTuple& t, const std::string& filename, bool append)
{
    std::ofstream file;
    file.open(filename, (append) ? std::ios_base::app : std::ios_base::out);
    file << t.id << ",\"" << t.name << "\"," << t.relative_path << "," << ((t.should_call) ? "true" : "false") << std::endl;
    file.close();
    if (file.is_open())
        throw std::runtime_error("Close operation on file [" + filename + "] failed!");
}

/**
 * Gets the pattern call with the specified id. 
 * Should be used when it is known that
 * a pattern call with the specified id exists in the map.
 *
 * @param id the specified identifier.
 * @return iterator to the pattern call with the specified id.
 */
const std::list<struct MyTuple>::iterator& PatternManager::get_pattern_with_id (const int id)
{
    return this->m_id_mapping[id];
}

/**
 * Gets the pattern call with the specified id. 
 *
 * @param id the specified identifier.
 * @return iterator to the pattern call with the specified id.
 * @throws std::runtime_error If the specified id doesn't exist in the map.
 */
const std::list<struct MyTuple>::iterator& PatternManager::safe_get_pattern_with_id (const int id)
{   
    if (this->m_id_mapping.find(id) == this->m_id_mapping.end())
    {
        throw std::runtime_error("A pattern with id [" + std::to_string(id) + "] doesn't exist!");
    }
    return this->m_id_mapping[id];
}

/**
 * Gets the pattern calls with the specified name. 
 * Should be used when it is known that
 * a pattern call with the specified name exists in the map.
 *
 * @param name the specified name.
 * @return vector of iterators to the pattern calls with the specified name.
 */
const std::vector<std::list<struct MyTuple>::iterator>& PatternManager::get_pattern_with_name (const std::string name)
{
    return this->m_name_mapping[name];
}

/**
 * Gets the pattern calls with the specified name. 
 *
 * @param name the specified name.
 * @return vector of iterators to the pattern calls with the specified name.
 * @throws std::runtime_error If the specified name doesn't exist in the map.
 */
const std::vector<std::list<struct MyTuple>::iterator>& PatternManager::safe_get_pattern_with_name (const std::string name)
{
    if (this->m_name_mapping.find(name) == this->m_name_mapping.end())
    {
        throw std::runtime_error("A pattern with name [" + name + "] doesn't exist!");
    }
    return this->m_name_mapping[name];
}

/**
 * Gets the pattern calls with the specified path. 
 * Should be used when it is known that
 * a pattern call with the specified path exists in the map.
 *
 * @param path the specified path.
 * @return vector of iterators to the pattern calls with the specified path.
 */
const std::vector<std::list<struct MyTuple>::iterator>& PatternManager::get_pattern_with_path (const std::string path)
{
    return this->m_path_mapping[path];
}

/**
 * Gets the pattern calls with the specified path. 
 *
 * @param path the specified path.
 * @return vector of iterators to the pattern calls with the specified path.
 * @throws std::runtime_error If the specified path doesn't exist in the map.
 */
const std::vector<std::list<struct MyTuple>::iterator>& PatternManager::safe_get_pattern_with_path (const std::string path)
{
    if (this->m_path_mapping.find(path) == this->m_path_mapping.end())
    {
        throw std::runtime_error("A pattern with path [" + path + "] doesn't exist!");
    }
    return this->m_path_mapping[path];
}

/**
 * Gets the pattern calls with false call flags. 
 * Should be used when it is known that
 * a pattern call with false call flag exists in the map.
 *
 * @return vector of iterators to the pattern calls with with false call flags.
 */
const std::vector<std::list<struct MyTuple>::iterator>& PatternManager::get_pattern_with_false ()
{
    return this->m_call_mapping["false"];
}

/**
 * Gets the pattern calls with false call flags. 
 *
 * @return vector of iterators to the pattern calls with with false call flags.
 * @throws std::runtime_error If there is no call pattern whose flag is false in the map.
 */
const std::vector<std::list<struct MyTuple>::iterator>& PatternManager::safe_get_pattern_with_false ()
{
    if (this->m_call_mapping.find("false") == this->m_path_mapping.end())
    {
        throw std::runtime_error("A pattern with call flag [false] doesn't exist!");
    }
    return this->m_call_mapping["false"];
}

/**
 * Gets the pattern calls with true call flags. 
 * Should be used when it is known that
 * a pattern call with true call flag exists in the map.
 *
 * @return vector of iterators to the pattern calls with with true call flags.
 */
const std::vector<std::list<struct MyTuple>::iterator>& PatternManager::get_pattern_with_true ()
{
    return this->m_call_mapping["true"];
}

/**
 * Gets the pattern calls with true call flags. 
 *
 * @return vector of iterators to the pattern calls with with true call flags.
 * @throws std::runtime_error If there is no call pattern whose flag is true in the map.
 */
const std::vector<std::list<struct MyTuple>::iterator>& PatternManager::safe_get_pattern_with_true ()
{
    if (this->m_call_mapping.find("true") == this->m_path_mapping.end())
    {
        throw std::runtime_error("A pattern with call flag [true] doesn't exist!");
    }
    return this->m_call_mapping["true"];
}

/**
 * Creates a pattern call and its mappings.
 *
 * @param id The pattern call identification.
 * @param id The pattern call name.
 * @param id The pattern call relative path.
 * @param id The pattern call flag.
 */
void PatternManager::insert_pattern (const int id, const std::string name, const std::string relative_path, const bool should_call)
{
    struct MyTuple t (id, name, relative_path, should_call);
    this->m_patterns.push_front(t);
    if (this->m_id_mapping.find(id) == this->m_id_mapping.end())
    {
        this->m_id_mapping[id] = this->m_patterns.begin();
    } else {
        return;
    }
    this->m_name_mapping[name].emplace_back(this->m_patterns.begin());
    this->m_path_mapping[relative_path].emplace_back(this->m_patterns.begin());
    this->m_call_mapping[(should_call) ? "true" : "false"].emplace_back(this->m_patterns.begin());
}

/**
 * Reads CSV from the specified file then creates the pattern calls and their mappings.
 *
 * @param filename the relative path of the file which has the pattern call information written as CSV.
 * @throws std::runtime_error If the file could not be opened.
 */
void PatternManager::insert_pattern_from_file(const std::string filename)
{
    std::ifstream stream(filename);
    if(!stream.is_open())
        throw std::runtime_error("Failed to open file [" + filename + "]!");
    std::vector<std::string> pattern;
    std::string row, item;
    while (std::getline(stream, row))
    {
        pattern.clear();
        row.erase(remove(row.begin(), row.end(), '\"'), row.end());
        std::stringstream linestream(row);
        while (std::getline(linestream, item, ','))
        {
            pattern.emplace_back(item);
        }
        this->insert_pattern(std::stoi(pattern[0]), pattern[1], pattern[2], ((pattern[3] == "true") ? true : false));
    }
    stream.close();
}
