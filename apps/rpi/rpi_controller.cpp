#include "rpi_controller.h"

namespace Rpi {

RpiController::RpiController(Responder * parentResponder) :
  ViewController(nullptr),
  //ViewController(parentResponder),
  m_rpiView()
{
}

View * RpiController::view() {
  return &m_rpiView;
}

}
