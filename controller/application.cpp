
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

#include "application.hpp"
#include "switch.hpp"


derailleur::Application::Application(const char* address,
				     const int port,
				     const int n_workers,
				     const bool secure) {
    // this->controller_ =
    // 	new derailleur::Controller(address,
    // 				   port,
    // 				   n_workers,
    // 				   secure,
    // 				   std::make_unique<Application>(this));

    derailleur::Controller controller_(
	address,
	port,
	n_workers,
	secure,
	std::make_unique<derailleur::Application>(this) );
}



derailleur::Application::~Application() {
    //delete this->controller_;
}




















