//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#define bind0(X) std::bind(&X, this)
#define bind1(X) std::bind(&X, this, std::placeholders::_1)
#define bind2(X) std::bind(&X, this, std::placeholders::_1, std::placeholders::_2)