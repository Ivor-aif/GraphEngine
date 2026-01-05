#include <charconv>
#include <filesystem>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "headers/Graph.h"
#include "headers/algorithm.h"
#include "headers/generator.h"
#include "headers/stats.h"

enum class StrOpt {
    import,
    newg,
    edit,
    check,
    exportg,
    output,
    nodecount,
    size,
    dim,
    degree,
    connectrate,
    isregular,
    category,
    name,
    index,
    rename,
    remove,
    swap,
    wssw,
    an,
    ans,
    ae,
    ee,
    rn,
    re,
    filename,
    unknown
};

StrOpt s2e(std::string_view);
void printHelp();
void printHelpImport();
void printHelpNewg();
void printHelpEdit();
void printHelpCheck();
void printHelpExportg();
void printHelpOutput();

int main(const int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
    }
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> options;
    std::vector<std::string> commands;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        std::ranges::transform(arg, arg.begin(), ::tolower);
        if (arg.starts_with('=')) {
            std::cout << "Cannot find option for value-like argument: " << arg << "." << std::endl;
            continue;
        }
        if (!arg.starts_with('-')) {
            commands.push_back(arg + "_" + std::to_string(i));
            continue;
        }
        if (commands.empty()) {
            std::cerr << "Command is empty." << std::endl;
            return 1;
        }
        const size_t pos = arg.find_first_not_of('-');
        if (pos == std::string::npos) {
            std::cout << "Empty argument for command " << commands.back().substr(0, commands.back().find_last_of('_')) << " will be ignored." << std::endl;
            continue;
        }
        std::string option = arg.substr(pos);
        std::string val;
        if (i+1 < argc && argv[i+1][0] == '=') {
            val = argv[++i];
            const size_t pos1 = val.find_first_not_of('=');
            if (pos1 == std::string::npos) {
                std::cout << "Empty value for option " << option << " of the command " << commands.back().substr(0, commands.back().find_last_of('_')) << " will be ignored." << std::endl;
                continue;
            }
            val = val.substr(pos1);
        }
        options[commands.back()].emplace_back(option, val);
    }
    std::vector<std::pair<std::string, Graph>> graphs;
    for (const std::string& command: commands) {
        const std::string cmd = command.substr(0, command.find_last_of('_'));
        if (cmd.empty() || ((cmd == "help" || cmd == "?" || cmd == "h") && !options.contains(command))) {
            printHelp();
            continue;
        }
        if ((cmd == "help" || cmd == "?" || cmd == "h") && options.contains(command)) {
            for (const std::pair<std::string, std::string>& option: options[command]) {
                switch (s2e(option.first)) {
                    case StrOpt::import:
                        printHelpImport();
                        break;
                    case StrOpt::newg:
                        printHelpNewg();
                        break;
                    case StrOpt::edit:
                        printHelpEdit();
                        break;
                    case StrOpt::check:
                        printHelpCheck();
                        break;
                    case StrOpt::exportg:
                        printHelpExportg();
                        break;
                    case StrOpt::output:
                        printHelpOutput();
                        break;
                    default:
                        std::cout << "Unknown option: " << option.first << "." << std::endl;
                }
            }
            continue;
        }
        if (cmd == "import") {
            if (!options[command].empty()) {
                for (const std::pair<std::string, std::string>& option: options[command]) {
                    std::ifstream file(option.first);
                    if (!file.is_open()) {
                        std::cout << "File " << option.first << " does not exist." << std::endl;
                        continue;
                    }
                    int v1, v2, lines;
                    std::string direct, line;
                    bool isDirect = false;
                    double weight;
                    char comma;
                    std::getline(file, line);
                    std::stringstream ss(line);
                    ss >> v1 >> comma >> v2 >> comma >> direct;
                    lines = v2;
                    std::ranges::transform(direct, direct.begin(), ::tolower);
                    if (direct == "true") {
                        isDirect = true;
                    }
                    Graph graph(v1, isDirect);
                    for (int i = 0; i < lines; i++) {
                        std::getline(file, line);
                        ss >> v1 >> comma >> v2 >> comma >> weight;
                        graph.addEdge({v1, v2, weight});
                    }
                    std::string name = option.first.substr(0, option.first.find_last_of('.'));
                    if (!option.second.empty()) {
                        name = option.second;
                    }
                    graphs.emplace_back(name, graph);
                    std::cout << "[Log]: " << name << " has imported successful." << std::endl;
                }
            } else {
                std::cout << "Command " << command << " need `csv` file(s), use help -import to know more." << std::endl;
            }
        }
        if (cmd == "newg") {
            if (!options[command].empty()) {
                int nodeCount = -1, size = -1, dim = -1, degree = -1;
                double connectRate = -1.;
                bool isRegular = false;
                std::string category = "-1", name;
                for (std::pair<std::string, std::string> option: options[command]) {
                    std::string type = option.first;
                    std::from_chars_result pe{};
                    switch (s2e(type)) {
                        case StrOpt::nodecount:
                            if (~nodeCount) {
                                std::cout << "Node count is not empty, " << nodeCount << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), nodeCount);
                            if (pe.ec != std::errc()) {
                                nodeCount = -1;
                                std::cout << "Invalid node count: " << option.second << "." << std::endl;
                            }
                            break;
                        case StrOpt::size:
                            if (~size) {
                                std::cout << "Size is not empty, " << size << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), size);
                            if (pe.ec != std::errc()) {
                                size = -1;
                                std::cout << "Invalid size: " << option.second << "." << std::endl;
                            }
                            break;
                        case StrOpt::dim:
                            if (~dim) {
                                std::cout << "Dimension is not empty, " << dim << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), dim);
                            if (pe.ec != std::errc()) {
                                dim = -1;
                                std::cout << "Invalid dimension: " << option.second << "." << std::endl;
                            }
                            break;
                        case StrOpt::degree:
                            if (~degree) {
                                std::cout << "Degree is not empty, " << degree << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), degree);
                            if (pe.ec != std::errc()) {
                                degree = -1;
                                std::cout << "Invalid degree: " << option.second << "." << std::endl;
                            }
                            break;
                        case StrOpt::connectrate:
                            if (fabs(connectRate + 1.) > 1.e-12) {
                                std::cout << "Connect rate is not empty, " << connectRate << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), connectRate);
                            if (pe.ec != std::errc()) {
                                connectRate = -1.;
                                std::cout << "Invalid connectrate: " << option.second << "." << std::endl;
                            }
                            break;
                        case StrOpt::isregular:
                            if (isRegular) {
                                std::cout << "Regular has been set into TRUE at " << cmd << " -" << type << ", it will be overwritten." << std::endl;
                            }
                            isRegular = (option.second == "true");
                            break;
                        case StrOpt::category:
                            if (category != "-1") {
                                std::cout << "Category is not empty, " << category << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            category = option.second;
                            break;
                        case StrOpt::name:
                            if (!name.empty()) {
                                std::cout << "Name is not empty, " << name << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            name = option.second;
                            break;
                        default:
                            std::cout << "Unknown option: " << option.first << "." << std::endl;
                    }
                }
                if (category == "-1") {
                    std::cout << "Create new graph failed. To create a specified graph, category is needed. Use help -newg to know more." << std::endl;
                } else if (category == "fc") {
                    if (!~nodeCount) {
                        std::cout << "Create new graph failed. To create a full connected graph, node count is needed. Use help -newg to know more." << std::endl;
                    } else {
                        graphs.emplace_back(name, fullConnect(nodeCount));
                        std::cout << "[Log]: Full connected graph with " << nodeCount << " node(s) and named " << name << " created successful." << std::endl;
                    }
                } else if (category == "lattice") {
                    if (!~size) {
                        std::cout << "Create new graph failed. To create a lattice graph, size is needed. Use help -newg to know more." << std::endl;
                    } else {
                        if (!~dim) {
                            graphs.emplace_back(name, lattice(size));
                            std::cout << "[Log]: Lattice graph with " << size << " size(s), default dimension(2) and named " << name << " created successful." << std::endl;
                        } else {
                            graphs.emplace_back(name, lattice(size, dim));
                            std::cout << "[Log]: Lattice graph with " << size << " size(s), " << dim << " dimension and named " << name << " created successful." << std::endl;
                        }
                    }
                } else if (category == "ring") {
                    if (!~nodeCount) {
                        std::cout << "Create new graph failed. To create a ring graph, node count is needed. Use help -newg to know more." << std::endl;
                    } else {
                        if (~degree) {
                            graphs.emplace_back(name, ring(nodeCount));
                            std::cout << "[Log]: Ring graph with " << nodeCount << " node(s), default degree(4) and named " << name << " created successful." << std::endl;
                        } else {
                            graphs.emplace_back(name, ring(nodeCount, degree));
                            std::cout << "[Log]: Ring graph with " << nodeCount << " node(s), " << degree << " degree(s) and named " << name << " created successful." << std::endl;
                        }
                    }
                }  else if (category == "or") {
                    if (!~nodeCount) {
                        std::cout << "Create new graph failed. To create a odd regular graph, node count is needed. Use help -newg to know more." << std::endl;
                    } else {
                        if (~degree) {
                            graphs.emplace_back(name, oddRegular(nodeCount));
                            std::cout << "[Log]: Odd regular graph with " << nodeCount << " node(s), default degree(3) and named " << name << " created successful." << std::endl;
                        } else {
                            graphs.emplace_back(name, oddRegular(nodeCount, degree));
                            std::cout << "[Log]: Odd regular graph with " << nodeCount << " node(s), " << degree << " degree(s) and named " << name << " created successful." << std::endl;
                        }
                    }
                } else if (category == "er") {
                    if (!~nodeCount) {
                        std::cout << "Create new graph failed. To create an E-R random graph, node count is needed. Use help -newg to know more." << std::endl;
                        continue;
                    }
                    if (!~degree) {
                        std::cout << "Create new graph failed. To create an E-R random graph, (average) degree is needed. Use help -newg to know more." << std::endl;
                        continue;
                    }
                    if (fabs(connectRate + 1.) > 1.e-12) {
                        std::cout << "Create new graph failed. To create an E-R random graph, connect rate is needed. Use help -newg to know more." << std::endl;
                        continue;
                    }
                    graphs.emplace_back(name, erRandom(nodeCount, degree, connectRate, isRegular));
                    std::cout << "[Log]: E-R random graph with " << nodeCount << " node(s), " << degree << " (average) degree(s), " << connectRate << " connect rate, " << (isRegular ? "regularity" : "irregularity") << " and named " << name << " created successful." << std::endl;
                } else if (category == "basf") {
                    if (!~nodeCount) {
                        std::cout << "Create new graph failed. To create an B-A scale-free graph, node count is needed. Use help -newg to know more." << std::endl;
                        continue;
                    }
                    if (!~degree) {
                        std::cout << "Create new graph failed. To create an B-A scale-free graph, (average) degree is needed. Use help -newg to know more." << std::endl;
                        continue;
                    }
                    graphs.emplace_back(name, baScaleFree(nodeCount, degree));
                    std::cout << "[Log]: B-A scale-free graph with " << nodeCount << " node(s), " << degree << " (average) degree(s) and named " << name << " created successful." << std::endl;
                } else {
                    std::cout << "Create new graph failed. Invalid category " << category << ". Use help -newg to know more or use help -import to import an arbitrary graph." << std::endl;
                }
            } else {
                graphs.emplace_back("", Graph());
                std::cout << "[Log]: Default empty graph without name created successful." << std::endl;
            }
            continue;
        }
        if (cmd == "edit") {
            if (!options[command].empty()) {
                int index = -1;
                int v1 = -1, v2 = -1;
                double d1 = -1.;
                std::string s1;
                StrOpt edit = StrOpt::unknown;
                for (const std::pair<std::string, std::string>& option: options[command]) {
                    std::string type = option.first;
                    std::from_chars_result pe{};
                    switch (s2e(type)) {
                        case StrOpt::index:
                            if (~index) {
                                std::cout << "Index is not empty, " << index << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), index);
                            if (pe.ec != std::errc()) {
                                index = -1;
                                std::cout << "Invalid index: " << option.second << "." << std::endl;
                            }
                            break;
                        case StrOpt::name: {
                            if (~index) {
                                std::cout << "Index is not empty, index for name " << option.second << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            auto it = std::ranges::find_if(graphs, [&option](const std::pair<std::string, Graph>& grh) {
                                return grh.first == option.second;
                            });
                            if (it == graphs.end()) {
                                index = -1;
                                std::cout << "Cannot find graph name: " << option.second << ", index reset." << std::endl;
                            } else {
                                index = static_cast<int>(distance(graphs.begin(), it));
                            }
                            break;
                        }
                        case StrOpt::rename:
                            if (!s1.empty()) {
                                std::cout << "New name is not empty, " << s1 << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            s1 = option.second;
                            if (edit != StrOpt::unknown) {
                                std::cout << "Edit item is not empty and it will be overwritten, only one item can be modified in once edit." << std::endl;
                            }
                            edit = StrOpt::rename;
                            break;
                        case StrOpt::remove:
                            if (edit != StrOpt::unknown) {
                                std::cout << "Edit item is not empty and it will be overwritten, only one item can be modified in once edit." << std::endl;
                            }
                            edit = StrOpt::remove;
                            break;
                        case StrOpt::swap:
                            if (~v1) {
                                std::cout << "Swap object is not empty, " << v1 << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), v1);
                            if (pe.ec != std::errc()) {
                                v1 = -1;
                                std::cout << "Invalid objective index: " << option.second << "." << std::endl;
                            }
                            if (edit != StrOpt::unknown) {
                                std::cout << "Edit item is not empty and it will be overwritten, only one item can be modified in once edit." << std::endl;
                            }
                            edit = StrOpt::swap;
                            break;
                        case StrOpt::wssw:
                            if (fabs(d1 + 1.) > 1.e-12) {
                                std::cout << "W-S small world structure's connect rate is not empty, " << d1 << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), d1);
                            if (pe.ec != std::errc()) {
                                d1 = -1.;
                                std::cout << "Invalid connectrate: " << option.second << "." << std::endl;
                            }
                            if (edit != StrOpt::unknown) {
                                std::cout << "Edit item is not empty and it will be overwritten, only one item can be modified in once edit." << std::endl;
                            }
                            edit = StrOpt::wssw;
                            break;
                        case StrOpt::an:
                            if (edit != StrOpt::unknown) {
                                std::cout << "Edit item is not empty and it will be overwritten, only one item can be modified in once edit." << std::endl;
                            }
                            edit = StrOpt::an;
                            break;
                        case StrOpt::ans:
                            if (~v1) {
                                std::cout << "Add node count is not empty, " << v1 << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), v1);
                            if (pe.ec != std::errc()) {
                                v1 = -1;
                                std::cout << "Invalid node count: " << option.second << "." << std::endl;
                            }
                            if (edit != StrOpt::unknown) {
                                std::cout << "Edit item is not empty and it will be overwritten, only one item can be modified in once edit." << std::endl;
                            }
                            edit = StrOpt::ans;
                            break;
                        case StrOpt::ae:
                            if (~v1 || ~v2 || fabs(d1 + 1.) > 1.e-12) {
                                std::cout << "Add edge is not empty, {" << v1 << ", " << v2 << ", " << d1 << "} at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), v1);
                            if (pe.ec != std::errc()) {
                                v1 = -1;
                                std::cout << "Invalid node index: " << option.second << "." << std::endl;
                            }
                            pe = std::from_chars(++pe.ptr, option.second.data() + option.second.size(), v2);
                            if (pe.ec != std::errc()) {
                                v2 = -1;
                                std::cout << "Invalid node index: " << option.second << "." << std::endl;
                            }
                            pe = std::from_chars(++pe.ptr, option.second.data() + option.second.size(), d1);
                            if (pe.ec != std::errc()) {
                                d1 = -1.;
                                std::cout << "Invalid node edge weight: " << option.second << "." << std::endl;
                            }
                            if (edit != StrOpt::unknown) {
                                std::cout << "Edit item is not empty and it will be overwritten, only one item can be modified in once edit." << std::endl;
                            }
                            edit = StrOpt::ae;
                            break;
                        case StrOpt::ee:
                            if (~v1 || ~v2 || fabs(d1 + 1.) > 1.e-12) {
                                std::cout << "Edit edge is not empty, {" << v1 << ", " << v2 << ", " << d1 << "} at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), v1);
                            if (pe.ec != std::errc()) {
                                v1 = -1;
                                std::cout << "Invalid node index: " << option.second << "." << std::endl;
                            }
                            pe = std::from_chars(++pe.ptr, option.second.data() + option.second.size(), v2);
                            if (pe.ec != std::errc()) {
                                v2 = -1;
                                std::cout << "Invalid node index: " << option.second << "." << std::endl;
                            }
                            pe = std::from_chars(++pe.ptr, option.second.data() + option.second.size(), d1);
                            if (pe.ec != std::errc()) {
                                d1 = -1.;
                                std::cout << "Invalid node edge weight: " << option.second << "." << std::endl;
                            }
                            if (edit != StrOpt::unknown) {
                                std::cout << "Edit item is not empty and it will be overwritten, only one item can be modified in once edit." << std::endl;
                            }
                            edit = StrOpt::ee;
                            break;
                        case StrOpt::rn:
                            if (~v1) {
                                std::cout << "Remove node is not empty, " << v1 << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), v1);
                            if (pe.ec != std::errc()) {
                                v1 = -1;
                                std::cout << "Invalid node count: " << option.second << "." << std::endl;
                            }
                            if (edit != StrOpt::unknown) {
                                std::cout << "Edit item is not empty and it will be overwritten, only one item can be modified in once edit." << std::endl;
                            }
                            edit = StrOpt::rn;
                            break;
                        case StrOpt::re:
                            if (~v1 || ~v2) {
                                std::cout << "Remove edge is not empty, {" << v1 << ", " << v2 << "} at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), v1);
                            if (pe.ec != std::errc()) {
                                v1 = -1;
                                std::cout << "Invalid node index: " << option.second << "." << std::endl;
                            }
                            pe = std::from_chars(++pe.ptr, option.second.data() + option.second.size(), v2);
                            if (pe.ec != std::errc()) {
                                v2 = -1;
                                std::cout << "Invalid node index: " << option.second << "." << std::endl;
                            }
                            if (edit != StrOpt::unknown) {
                                std::cout << "Edit item is not empty and it will be overwritten, only one item can be modified in once edit." << std::endl;
                            }
                            edit = StrOpt::re;
                            break;
                        default:
                            std::cout << "Unknown option " << type << "." << std::endl;
                    }
                }
                if (!~index || index >= static_cast<int>(graphs.size())) {
                    std::cout << "Invalid index " << index << " in current graph list." << std::endl;
                    continue;
                }
                if (edit == StrOpt::rename) {
                    if (s1.empty()) {
                        std::cout << "Edit failed. To rename a graph, new name is needed. Use help -edit to know more." << std::endl;
                        continue;
                    }
                    std::string oldName = graphs[index].first;
                    graphs[index].first = s1;
                    std::cout << "[Log]: The " << index << "th graph named " << oldName << " has been renamed into " << s1 << "." << std::endl;
                } else if (edit == StrOpt::remove) {
                    graphs.erase(graphs.begin() + index);
                    std::cout << "[Log]: The " << index << "th graph removed successful." << std::endl;
                } else if (edit == StrOpt::swap) {
                    if (!~v1 || v1 >= static_cast<int>(graphs.size())) {
                        std::cout << "Edit failed. Invalid swap objective index " << v1 << " in current graph list." << std::endl;
                        continue;
                    }
                    std::swap(graphs[index], graphs[v1]);
                    std::cout << "[Log]: The " << index << "th graph have swapped with the " << v1 << "th successful." << std::endl;
                } else if (edit == StrOpt::wssw) {
                    wsSmallWorld(graphs[index].second, d1);
                    std::cout << "[Log]: The " << index << "th graph named " << graphs[index].first << " has applied W-S small-world with " << d1 << " reconnect rate successful." << std::endl;
                } else if (edit == StrOpt::an) {
                    graphs[index].second.addNode();
                    std::cout << "[Log]: The " << index << "th graph named " << graphs[index].first << " has added a node successful." << std::endl;
                } else if (edit == StrOpt::ans) {
                    if (!~v1) {
                        std::cout << "Edit failed. To add some nodes on a graph, new node count is needed. Use help -edit to know more." << std::endl;
                        continue;
                    }
                    graphs[index].second.addNodes(v1);
                    std::cout << "[Log]: The " << index << "th graph named " << graphs[index].first << " has added " << v1 << " node(s) successful." << std::endl;
                } else if (edit == StrOpt::ae) {
                    if (!~v1 || !~v2) {
                        std::cout << "Edit failed. To add an edge on graph, start node and end node is needed. Use help -edit to know more." << std::endl;
                        continue;
                    }
                    graphs[index].second.addEdge({v1, v2, d1});
                    std::cout << "[Log]: The " << index << "th graph named " << graphs[index].first << "\'s edge between " << v1 << " and " << v2 << " weighted by " << d1 << " added successful." << std::endl;
                } else if (edit == StrOpt::ee) {
                    if (!~v1 || ~v2) {
                        std::cout << "Edit failed. To edit an edge's weight on graph, start node and end node is needed. Use help -edit to know more." << std::endl;
                        continue;
                    }
                    graphs[index].second.updateEdgeWeight({v1, v2, d1});
                    std::cout << "[Log]: The " << index << "th graph named " << graphs[index].first << "\'s edge between " << v1 << " and " << v2 << " has been updated into weight " << d1 << " successful." << std::endl;
                } else if (edit == StrOpt::rn) {
                    if (!~v1) {
                        std::cout << "Edit failed. To remove a nodes on a graph, node index is needed. Use help -edit to know more." << std::endl;
                        continue;
                    }
                    graphs[index].second.removeNode(v1);
                    std::cout << "[Log]: The " << index << "th graph named " << graphs[index].first << " has removed " << v1 << "th node successful." << std::endl;
                } else if (edit == StrOpt::re) {
                    if (!~v1 || ~v2) {
                        std::cout << "Edit failed. To remove an edge's weight on graph, start node and end node is needed. Use help -edit to know more." << std::endl;
                        continue;
                    }
                    graphs[index].second.removeEdge({v1, v2});
                    std::cout << "[Log]: The " << index << "th graph named " << graphs[index].first << "\'s edge between " << v1 << " and " << v2 << " has been removed successful." << std::endl;
                } else {
                    std::cout << "Edit failed, unknown edit option. Use help -edit to know more." << std::endl;
                }
            } else {
                std::cout << "Edit graph failed. Object not specified." << std::endl;
            }
            continue;
        }
        if (cmd == "check") {
            if (!options[command].empty()) {
                int index = -1;
                if (options[command][0].first == "index") {
                    std::from_chars_result pe = std::from_chars(options[command][0].second.data(), options[command][0].second.data() + options[command][0].second.size(), index);
                    if (pe.ec != std::errc()) {
                        index = -1;
                        std::cout << "Invalid index: " << options[command][0].second << "." << std::endl;
                    }
                } else if (options[command][0].first == "name") {
                    auto it = std::ranges::find_if(graphs, [&options, &command](const std::pair<std::string, Graph>& grh) {
                        return grh.first == options[command][0].second;
                    });
                    if (it == graphs.end()) {
                        index = -1;
                        std::cout << "Cannot find graph name: " << options[command][0].second << ", index reset." << std::endl;
                    } else {
                        index = static_cast<int>(distance(graphs.begin(), it));
                    }
                }
                if (!~index || index >= static_cast<int>(graphs.size())) {
                    std::cout << "Invalid index " << index << " in current graph list." << std::endl;
                    continue;
                }
                std::cout << "[Log]: Connectivity check finished, " << index << "th graph named " << graphs[index].first << " is" << (dfs(graphs[index].second) ? " " : " not ") << "well connected." << std::endl;
            } else {
                std::cout << "[Log]: Connectivity check finished." << std::endl;
                int index = 0;
                for (const auto& [name, graph]: graphs) {
                    std::cout << index << "th graph named " << name << " is" << (dfs(graph) ? " " : " not ") << "well connected." << std::endl;
                    index++;
                }
            }
            continue;
        }
        if (cmd == "exportg") {
            int index = -1;
            std::string fn = "-1";
            if (!options[command].empty()) {
                for (const std::pair<std::string, std::string>& option: options[command]) {
                    std::string type = option.first;
                    std::from_chars_result pe{};
                    switch (s2e(type)) {
                        case StrOpt::index:
                            if (~index) {
                                std::cout << "Index is not empty, " << index << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            pe = std::from_chars(option.second.data(), option.second.data() + option.second.size(), index);
                            if (pe.ec != std::errc()) {
                                index = -1;
                                std::cout << "Invalid index: " << option.second << "." << std::endl;
                            }
                            break;
                        case StrOpt::name: {
                            if (~index) {
                                std::cout << "Index is not empty, index for name " << option.second << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            auto it = std::ranges::find_if(graphs, [&option](const std::pair<std::string, Graph>& grh) {
                                return grh.first == option.second;
                            });
                            if (it == graphs.end()) {
                                index = -1;
                                std::cout << "Cannot find graph name: " << option.second << ", index reset." << std::endl;
                            } else {
                                index = static_cast<int>(distance(graphs.begin(), it));
                            }
                            break;
                        }
                        case StrOpt::filename:
                            if (fn.ends_with(".csv")) {
                                std::cout << "Filename is not empty, " << fn << " at " << cmd << " -" << type << " will be overwritten." << std::endl;
                            }
                            if (option.second.ends_with(".csv")) {
                                fn = option.second;
                            } else {
                                fn = option.second + ".csv";
                            }
                            break;
                        default:
                            std::cout << "Unknown option " << type << "." << std::endl;
                    }
                }
            } else {
                index = 0;
                fn = "exportg.csv";
            }
            if (!~index || index >= static_cast<int>(graphs.size())) {
                std::cout << "Invalid index " << index << " in current graph list." << std::endl;
                continue;
            }
            if (fn == "-1") {
                fn = "exportg.csv";
            }
            std::string nfn = "nodes_" + fn, efn = "edges_" + fn;
            std::filesystem::path pathN = nfn, pathE = efn;
            int suffix = 0;
            while (std::filesystem::exists(nfn) || std::filesystem::exists(efn)) {
                nfn = pathN.stem().string() + std::to_string(suffix) + pathN.extension().string();
                efn = pathE.stem().string() + std::to_string(suffix) + pathE.extension().string();
                suffix++;
            }
            std::ofstream ofs(nfn);
            ofs << "nodeIndex,reachNodeCount,degreeDistribution";
            for (int i = 0; i < graphs[index].second.getNodeCount(); i++) {
                ofs << ",distanceTo" << i << "thNode";
            }
            ofs << std::endl;
            std::pair<std::pair<double, double>, std::vector<double>> dgrDist = degreeDistribution(graphs[index].second);
            std::vector<int> reach(graphs[index].second.getNodeCount(), 0);
            for (int i = 0; i < graphs[index].second.getNodeCount(); i++) {
                for (int j = 0; j < graphs[index].second.getNodeCount(); j++) {
                    if (i == j) {
                        continue;
                    }
                    reach[i] += bfs(graphs[index].second, {i, j});
                }
            }
            std::vector<std::vector<double>> dist = floyd(graphs[index].second);
            for (int i = 0; i < graphs[index].second.getNodeCount(); i++) {
                ofs << i << "," << reach[i] << "," << dgrDist.second[i];
                for (int j = 0; j < graphs[index].second.getNodeCount(); j++) {
                    ofs << "," << dist[i][j];
                }
                ofs << std::endl;
            }
            ofs.close();
            ofs.open(efn);
            int ec = graphs[index].second.getEdgeCount();
            ofs << graphs[index].second.getNodeCount() << "," << ec << (graphs[index].second.isDirected() ? ",True" : ",False") << std::endl;
            for (int i = 0; i < graphs[index].second.getNodeCount(); i++) {
                int lmt = graphs[index].second.isDirected() ? graphs[index].second.getNodeCount() : i;
                for (const auto& [nbr, wei]: graphs[index].second.getNeighbors(i)) {
                    if (nbr >= lmt) {
                        break;
                    }
                    ofs << i << "," << nbr << "," << wei << std::endl;
                }
            }
            ofs.close();
            std::cout << "[Log]: Exported graph index " << index << "th with " << graphs[index].first << " name successful. Node information in " << nfn << " file, and edges in " << efn << " file." << std::endl;
            continue;
        }
        if (cmd == "output") {
            std::string fn = "-1";
            if (!options[command].empty() && s2e(options[command][0].first) == StrOpt::filename) {
                if (options[command][0].second.ends_with(".csv")) {
                    fn = options[command][0].second;
                } else {
                    fn = options[command][0].second + ".csv";
                }
            } else {
                fn = "output.csv";
            }
            std::filesystem::path path = fn;
            int suffix = 0;
            while (std::filesystem::exists(fn)) {
                fn = path.stem().string() + std::to_string(suffix++) + path.extension().string();
            }
            std::ofstream ofs(fn);
            ofs << "index,name,isDirect,nodeCount,edgeCount,averageDegree,varianceDegree,clusterCoefficient,minimumDistance,averageDistance,maximumDistance" << std::endl;
            for (int i = 0; i < graphs.size(); ++i) {
                ofs << i << ',' << graphs[i].first << ',' << (graphs[i].second.isDirected() ? "True," : "False,") << graphs[i].second.getNodeCount() << ',' << graphs[i].second.getEdgeCount() << ',';
                auto [ds, dd] = degreeDistribution(graphs[i].second);
                ofs << ds.first << ',' << ds.second << ',' << clusterCoefficient(graphs[i].second) << ',';
                auto [min, ave, max] = distance(graphs[i].second);
                ofs << min << ',' << ave << ',' << max << std::endl;
            }
            ofs.close();
            std::cout << "[Log]: Export all the graph(s) successful at " << fn << "." << std::endl;
            continue;
        }
        std::cout << "Unknown command: " << cmd << "." << std::endl;
    }
    return 0;
}

void printHelp() {
    std::cout << "----------------------------------------------------------------" << std::endl;
    std::cout << "-= Welcome to use Graph Engine! =-" << std::endl << std::endl;
    std::cout << "Use `help`/`h`/`?` or EMPTY to show this help-list." << std::endl;
    std::cout << "Format: GraphEngine <command> -<option> =<value>, option and value may empty." << std::endl;
    std::cout << "A whole run can include a lot of continuously commands, graph will form a list with creation order by default. When all command finished, everything will be release. In other words, there is no relation or remember between running, you must key-in all your operates at once." << std::endl;
    std::cout << "Command: `import` to read \"csv\" files as edge sets, each file will be read as an single graph. Option: file name (must end with \".csv\"). Value: graph name(may empty), default for file name." << std::endl;
    std::cout << "\tUse help -import to know more." << std::endl;
    std::cout << "Command: `newg` to create a specific new graph. Option list: {name, category, nodecount, size, dim, degree, connectrate, isregular, category}. Value: name refers to graph name(may empty), default for empty; category is essential, others are optional." << std::endl;
    std::cout << "\tUse help -newg to know more." << std::endl;
    std::cout << "Command: `edit` to edit existed graph\'s attributes. Option list: {index, name, rename, remove, swap, wssw, an, ans, ae, ee, rn, re}. `Index` and `name` are to locate exact graph, `rename` just as its name to rename an exist graph, others are for modify graph's structure." << std::endl;
    std::cout << "\tUse help -edit to know more." << std::endl;
    std::cout << "Command: `check` to check the connectivity of the specific graph, i.e. whether an entity can reach every node starts form zeroth node or not. Option(optional): index or name, only one option effective and it must be the former." << std::endl;
    std::cout << "\tUse help -check to know more." << std::endl;
    std::cout << "Command: `exportg` to export specific graph\'s all node information and all the edges. Option list: {index, name, filename}, all of options are optional. `Index` and `name` are to locate exact graph, `filename` is to specify the export file name, default export the zeroth graph into nodes_exportg.csv and edges_exportg.csv." << std::endl;
    std::cout << "\tUse help -exportg to know more." << std::endl;
    std::cout << "Command: `output` to export all the graphs\' abstract in list. Option(optional): filename, only the first optional will effective. Value: the output file name." << std::endl;
    std::cout << "\tUse help -output to know more." << std::endl;
    std::cout << "Other commands will be implicitly ignored." << std::endl;
    std::cout << "If an error occurs at the theoretical level, the backend interface will directly report an error. For example, if a graph with no base node degree exists, attempting to create it forcibly will result in an error." << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
}

void printHelpImport() {
    std::cout << "Command: `import` to read `csv` files as edge sets, each file will be read as an single graph. Option: file name (must end with \".csv\"). Value: graph name(may empty), default for file name." << std::endl;
    std::cout << "For example, GraphEngine import -../myGraph.csv =g1 -./myNewGraph.csv." << std::endl;
    std::cout << "It will import two graphs append into graph list, first one will be named with \"g1\", and the second one will be use default name\"./myNewGraph\"." << std::endl;
    std::cout << "Data in `csv` file must have a head line: \"nodeCount,edgeCount,isDirected\\n\", and then it must have `edgeCount` lines remain, each line with format: \"startNode,endNode,edgeWeight\\n\". `startNode` and `endNode` must be an integer and smaller than `nodeCount`. `isDirected` must use TRUE to active, otherwise will be undirected." << std::endl;
    std::cout << "Make sure that graph name NOT repeated, it will cause conflict. If so, use index." << std::endl;
}

void printHelpNewg() {
    std::cout << "Command: `newg` to create a specific new graph. Option list: {name, category, nodecount, size, dim, degree, connectrate, isregular, category}. Value: name refers to graph name(may empty), default for empty; category is essential, others are optional." << std::endl;
    std::cout << "Option name refers to graph name, default for empty. Make sure that graph name NOT repeated, it will cause conflict. If so, use index." << std::endl;
    std::cout << "Category(essential) refers to graph structure, only \"fc\", \"lattice\", \"ring\", \"or\", \"er\" and \"basf\" are support. Others may use import to create, see help -import. Different `category` need different optional option(s)." << std::endl;
    std::cout << "\tCategory fc: full connect structure, `nodecount` is essential for node count." << std::endl;
    std::cout << "\tCategory lattice: lattice structure, `size` is essential for graph size in every dimension, `dim` is optional for dimension(default value is 2)." << std::endl;
    std::cout << "\tCategory ring: ring structure, `nodecount` and `degree` are essential for node count and degree, respectively." << std::endl;
    std::cout << "\tCategory or: odd-degree regular structure, `nodecount` and `degree` are essential for node count and degree, respectively." << std::endl;
    std::cout << "\tCategory er: E-R random structure, structure, `nodecount`, `degree` and `connectrate` are essential for node count (average) degree and node-pair connect rate, respectively, `isregular` is optional for whether the network is regular or not(default value is FALSE, only \"True\" can activate it)." << std::endl;
    std::cout << "\tCategory basf: B-A scale-free structure, `nodecount` and `degree` are essential for node count and average degree, respectively." << std::endl;
    std::cout << "Each option need at most one value for once `newg` command, twice or more use will overwrite." << std::endl;
    std::cout << "For example, GraphEngine newg -category =basf -name =myGraph1 -nodecount =1000 -degree =4." << std::endl;
    std::cout << "It will create a B-A scale-free structured graph with 1000 nodes and 4 average degree, and will be named by \"myGraph1\"." << std::endl;
    std::cout << "If some option(s) or value(s) are invalid, overwrite is support. If persist in use invalids, it would cause theoretical error with default value." << std::endl;
}

void printHelpEdit() {
    std::cout << "Command: `edit` to edit existed graph\'s attributes. Option list: {index, name, rename, remove, swap, wssw, an, ans, ae, ee, rn, re}. `Index` and `name` are to locate exact graph, `rename` just as its name to rename an exist graph, others are for modify graph's structure." << std::endl;
    std::cout << "Option `index` is to locate the `index`\'s \"value\"th graph, and `name` is to locate the first graph with `name`\'s \"value\". Located graph will be edited at this command, once command only edit one item." << std::endl;
    std::cout << "Option `rename` is to rename the located graph. It need a non-empty string value as new name. Make sure that graph name NOT repeated, it will cause conflict. If so, use index." << std::endl;
    std::cout << "Option `remove` is to remove the located graph. It does not need any value. Notice that after remove, the index located at the back will change." << std::endl;
    std::cout << "Option `swap` is to swap the located graph and objective graph in graph list. It need an integer value to specify the objective. Only index here, name is invalid." << std::endl;
    std::cout << "Option `wssw` is to make the located graph change into W-S small-world structure, it need a float value for reconnect rate." << std::endl;
    std::cout << "Option `an` is to add an node into the located graph. It does not need any value." << std::endl;
    std::cout << "Option `ans` is to add some nodes into the located graph. It need an integer value for new node count." << std::endl;
    std::cout << "Option `ae` is to add an edge into the located graph. It need an value format as \"2,3,1.5\", refers to start node, end node and weight, respectively. Split char can be vary." << std::endl;
    std::cout << "Option `ee` is to edit an edge\'s weight into the located graph. It need an value format as \"2,3,5.1\", refers to start node, end node and new weight, respectively. Split char can be vary." << std::endl;
    std::cout << "option `rn` is to remove a node into the located graph. It need an integer value for node index. Notice that after remove, the node index located at the back will change." << std::endl;
    std::cout << "Option `re` is to remove an edge into the located graph. It need an value format as \"2,3\", refers to start node and end node for prepare removed edge, respectively. Split char can be vary." << std::endl;
    std::cout << "For example, GraphEngine edit -index =2 -wssw =0.3 edit -index =2 -rename =WS edit -name =WS -ae =300,301,1.0." << std::endl;
    std::cout << "It will continuously edit the 2th graph 3 times(If neither the 0th nor the 1st graph\'s name are not \"WS\"). They are in order make it turn into W-S small-world structure with 0.3 reconnect rate, rename into\"WS\", and add edge between \"300\"th and \"301\"th with \"1.0\" weight." << std::endl;
    std::cout << "If some option(s) or value(s) are invalid, overwrite is support. If persist in use invalids, it would cause theoretical error with default value." << std::endl;
}

void printHelpCheck() {
    std::cout << "Command: `check` to check the connectivity of the specific graph, i.e. whether an entity can reach every node starts form zeroth node or not. Option(optional): index or name, only one option effective and it must be the former." << std::endl;
    std::cout << "Option `index` is to locate the `index`\'s \"value\"th graph, and `name` is to locate the first graph with `name`\'s \"value\". Located graph will be checked at this command." << std::endl;
    std::cout << "If there is no option, every graph will be checked in order." << std::endl;
    std::cout << "For example, GraphEngine check -index =0 check -name =WS." << std::endl;
    std::cout << "It will check the connectivity of 0th(if exist) and the first named \"WS\"\'s graph." << std::endl;
}

void printHelpExportg() {
    std::cout << "Command: `exportg` to export specific graph\'s all node information and all the edges. Option list: {index, name, filename}, all of options are optional. `Index` and `name` are to locate exact graph, `filename` is to specify the export file name, default export the zeroth graph into nodes_exportg.csv and edges_exportg.csv." << std::endl;
    std::cout << "Option `index` is to locate the `index`\'s \"value\"th graph, and `name` is to locate the first graph with `name`\'s \"value\". Located graph will be export at this command, default export zeroth graph." << std::endl;
    std::cout << "Option `filename` is to specify a new file to output, default `filename` is \"exportg\". This command will not edit an existed file, please check \"nodes_`filename`XXX.csv\" and \"edges_`filename`XXX.csv\" when finished." << std::endl;
    std::cout << "Nodes information includes node index, reachable node count, degree distribution and minimum distance to any other nodes." << std::endl;
    std::cout << "Edges information are all of the edges, i.e. start node, end node and weight. Especially, the first line is node count, edge count and is directed, so you can just use `import` to reprocess this graph(see help -import)." << std::endl;
    std::cout << "For example, GraphEngine exportg -index =1 -filename =exportA exportg -name =WS -filename =exportB." << std::endl;
    std::cout << "Then you will see four new \".csv\" files, just open it and read." << std::endl;
}

void printHelpOutput() {
    std::cout << "Command: `output` to export all the graphs\' abstract in list. Option(optional): filename, only the first optional will effective. Value: the output file name." << std::endl;
    std::cout << "Option `filename` is to specify a new file to output, default `filename` is \"output\". This command will not edit an existed file, please check \"`filename`XXX.csv\" when finished." << std::endl;
    std::cout << "Graph abstract includes graph\'s index, name, whether direct or not, node count, edge count, average degree, degree variance, cluster coefficient, minimum distance between two nodes, average distance between two nodes and maximum distance between two nodes." << std::endl;
    std::cout << "Output will according to list order." << std::endl;
    std::cout << "For example, GraphEngine output -filename =outA -filename =outB." << std::endl;
    std::cout << "Then you will see a new \"outAXXX.csv\" file, just open it and read." << std::endl;
}

StrOpt s2e(const std::string_view str) {
    if (str == "import") return StrOpt::import;
    if (str == "newg") return StrOpt::newg;
    if (str == "edit") return StrOpt::edit;
    if (str == "check") return StrOpt::check;
    if (str == "exportg") return StrOpt::exportg;
    if (str == "output") return StrOpt::output;
    if (str == "nodecount") return StrOpt::nodecount;
    if (str == "size") return StrOpt::size;
    if (str == "dim") return StrOpt::dim;
    if (str == "degree") return StrOpt::degree;
    if (str == "connectrate") return StrOpt::connectrate;
    if (str == "isregular") return StrOpt::isregular;
    if (str == "category") return StrOpt::category;
    if (str == "name") return StrOpt::name;
    if (str == "index") return StrOpt::index;
    if (str == "rename") return StrOpt::rename;
    if (str == "remove") return StrOpt::remove;
    if (str == "swap") return StrOpt::swap;
    if (str == "wssw") return StrOpt::wssw;
    if (str == "an") return StrOpt::an;
    if (str == "ans") return StrOpt::ans;
    if (str == "ae") return StrOpt::ae;
    if (str == "ee") return StrOpt::ee;
    if (str == "rn") return StrOpt::rn;
    if (str == "re") return StrOpt::re;
    if (str == "filename") return StrOpt::filename;
    return StrOpt::unknown;
}
