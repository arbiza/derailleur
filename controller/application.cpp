
/**
 *  File: application.cpp
 *
 *  Copyright (c) 2014 Lucas Arbiza <lucas.arbiza@gmail.com>
 * 
 *  This file is an implementation of the class(es) defined in
 *  application.hpp header file.
 *
 *  See the header file for documentation and license information.
 *
 **/

#include <memory>
#include <map>

#include "application.hpp"
#include "switch.hpp"


derailleur::Application::Application() {

    switches_rack_ = std::make_shared<std::map<int, derailleur::Switch>>;
}









