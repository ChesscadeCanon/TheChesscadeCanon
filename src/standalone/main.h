#pragma once
#define MILLISECONDS_DIFFERENCE(__from_time__, __to_time__) ((Time)(1000.0 * ((__to_time__).time - (__from_time__).time) + ((__to_time__).millitm - (__from_time__).millitm)))
