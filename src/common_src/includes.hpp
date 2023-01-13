#pragma once

#include <v8.h>
#include <libplatform/libplatform.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <X11/Xlib.h>
#include <math.h>
#include <zmq.hpp>
#include <filesystem>
#include <nlohmann/json.hpp>
// #include "json.hpp"
#include <queue>
#include <map>
// #include "json.h pp"
#include "v8helpers.h"
#include "ScriptManager.h"

using namespace sf;