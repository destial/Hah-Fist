#include "InputEvent.hpp"

std::vector<std::function<void(const InputEvent&)>> InputEvent::Listeners;