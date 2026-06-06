#include <iostream>
#include <ostream>
#include <unordered_map>
#include <fstream>
#include "commands.hpp"

namespace petrov
{
  void runExecute(std::istream&, CommandExecuter&);
}

int main(int argc, char const *argv[])
{
  if (argc < 2)
  {
    std::cerr << "invalid argument count\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Couldnt open file\n";
    return 1;
  }

  petrov::pvec_t data;
  readPolygon(file, data);

  std::unordered_map< std::string, petrov::cmd_t > cmds;
  cmds["AREA"] = petrov::area;
  cmds["MAX"] = petrov::max;
  cmds["MIN"] = petrov::min;
  cmds["COUNT"] = petrov::count;
  cmds["INTERSECTIONS"] = petrov::intersection;
  cmds["SAME"] = petrov::same;
  petrov::CommandExecuter ce{cmds, std::cout, data};
  petrov::runExecute(std::cin, ce);
}

void petrov::runExecute(std::istream& in, petrov::CommandExecuter& ce)
{
  if (!(in >> ce)) {
    return;
  }
  runExecute(in, ce);
}

