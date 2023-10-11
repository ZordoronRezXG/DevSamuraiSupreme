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


#include "LogMaker.h"
#include "Coutzr.h"
Coutzr ncout;
LogMaker::LogMaker() {
}

void LogMaker::zlog(int flag, const std::string str) {
    ncout << flag << str;
    ncout.buffer.sync();
}