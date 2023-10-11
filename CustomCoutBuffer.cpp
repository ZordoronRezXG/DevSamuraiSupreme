/**
 * MIT License
 *
 * Copyright (c) 2023 Zordoron Rez
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
/**
 * MIT License
 *
 * Copyright (c) 2023 Zordoron Rez
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * Contributions and Acknowledgments
 * -------------------------------
 *
 * We would appreciate and are grateful for any acknowledgments or shout-outs if you
 * use this software in your projects. While not a formal license requirement, we kindly
 * request that you consider giving credit to the original author, "Zordoron Rez",
 * when using or referencing this software.
 *
 * We believe in open-source collaboration and appreciate your support!
 */
#include "version.h"
#include <chrono>
#include <ctime>
#include <string>
#include "CustomCoutBuffer.h"

CustomCoutBuffer::CustomCoutBuffer() {
}

bool doOnce = true;

void flush() {
    std::cout << std::flush;
}

int CustomCoutBuffer::sync(int flag, bool showT) {
    // Get the current time
    auto now = std::chrono::system_clock::now();

    // Convert to a time_t
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Create a struct tm to hold the time
    struct tm timeinfo;

    // Use localtime_s to convert the time to a struct tm
    if (localtime_s(&timeinfo, &now_time) == 0) {
        // Convert to a string
        char dt[26];
        if (strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S: ", &timeinfo)) {
            // Print the current date and time
            if (showT) {
                std::cout << "ZNT " << dt;
            }
        }
        else {
            std::cout << "ZNT...: ";
        }
    }
    else {
        std::cout << "ZNT...: ";
    }
    
    if (str().empty()) {
        if (doOnce) {
            str("");
            std::cout << std::endl;
            std::cout << std::string(4 * 2, ' ') << "(●'◡'●)" << " " << PROJECT_NAME << " " << PROJECT_VERSION << std::endl << std::endl << std::flush;
            std::cout << std::string(4 * 2, ' ') << "Initialized znt log for "  << PROJECT_NAME << std::endl << std::endl << std::flush;

            doOnce = false;
        }
        str("");
        return 0;
    }

    if (flag == 0) {
        std::cout << std::endl;
        std::cout << std::string(4 * 2, ' ') << "@_" << str() << std::endl << std::endl << std::flush;
    }
    else if (flag == 1) {
        std::cout << std::endl;
        std::cout << std::string(4 * 2, ' ') << "@_" << str() << std::endl << std::string(4 * 2, ' ') << " loading..."  << std::endl << std::endl << std::flush;
    }
    str("");
    return 0;
}
