#include "MongoCxxInit.hpp"

namespace DataBase {

MongoCxxInit& MongoCxxInit::Instance() {
  static MongoCxxInit init;
  return init;
}

MongoCxxInit::MongoCxxInit() : inst{} {}

}  // DataBase