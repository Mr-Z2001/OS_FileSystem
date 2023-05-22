#include <cassert>
#include <fstream>

int main()
{
  std::ifstream f;
  f.open("userdata.csv");
  assert(f.is_open());
  f.close();
  return 0;
}